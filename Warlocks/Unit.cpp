#include "Unit.h"

std::deque<std::string> UnitNames;
std::deque<sf::Texture> UnitTextures;
//std::deque<std::string> AbilityNames;
//std::deque<sf::Texture> AbilityTextures;

int getUnitTypeFromName(std::string name)
{
	for(int i = 0;i<UnitNames.size();i++)
	{
		if(UnitNames.at(i)==name)
		{
			return i;
		}
	}
	return -1;
}

//int getAbilityIdFromName(std::string name)
//{
//	for(int i = 0;i<SpellNames.size();i++)
//	{
//		if(SpellNames.at(i)==name)
//		{
//			return i;
//		}
//	}
//	return -1;
//}

static int loadunit(lua_State* L)
{
	std::string s = lua_tostring(L,1);
	UnitNames.push_back(s);
	std::cout << "	Loaded Unit : " << s << "\n";
	sf::Texture t;
	std::string spath = "Graphics\\Units\\" + s + ".png";
	t.loadFromFile(spath);
	UnitTextures.push_back(t);
	lua_State* tmpL = luaL_newstate();
	luaL_openlibs(tmpL);
	//lua_register(tmpL,"loadunit",loadunit); //register loadunit
	registerLua(tmpL);
	if (luaL_loadfile(tmpL, "Lua\\Units.lua") || lua_pcall(tmpL, 0, 0, 0)) 
	{
        std::cout<<"Error: failed to load Units.lua"<<std::endl;
		std::cout << lua_tostring(tmpL,-1) << "\n";
		return 0;
    }

	lua_getglobal(tmpL,s.c_str());
	lua_getfield(tmpL,-1,"abilityCount");
	int abilityCount = lua_tointeger(tmpL,-1);
	luaClean(tmpL);

	for(int i = 0;i<abilityCount;i++)
	{
		std::string s2 = "ability" + std::to_string(i+1);
		lua_getglobal(tmpL,s.c_str());
		lua_getfield(tmpL,-1,s2.c_str());
		lua_getfield(tmpL,-1,"id");
		std::string s3 = lua_tostring(tmpL,-1);
		luaClean(tmpL);

		SpellNames.push_back(s3);
		std::cout << "	  Loaded Ability: " << s3 << "\n"; 
		s3 = "Graphics\\Abilities\\" + s3 + ".png";
		sf::Texture t2;
		t2.loadFromFile(s3);
		SpellTextures.push_back(t2);
	}
	lua_close(tmpL);
	return 0;
}

int initUnits()
{
	std::cout << "Loading Units...\n";

	LuaUnits = luaL_newstate(); //create new lua state
	luaL_openlibs(LuaUnits);

	lua_register(LuaUnits,"loadunit",loadunit); //register loadspell
	registerLua(LuaUnits); //register functions
	if (luaL_loadfile(LuaUnits, "Lua\\Units.lua") || lua_pcall(LuaUnits, 0, 0, 0)) 
	{
        std::cout<<"Error: failed to load Units.lua"<<std::endl;
		std::cout << lua_tostring(LuaUnits,-1) << "\n";
		getch();
		return -1;
    }
	lua_getglobal(LuaUnits,"loadUnits");
	lua_pcall(LuaUnits,0,0,0); //execute once to load units
	luaClean(LuaUnits);
	
	return 0;
}

sf::Vector2f getSpritePos(int x,int y)
{
	sf::Vector2f v;
	v.x = x*42;
	v.y = 50+y*42;
	return v;
}

Unit::Unit()
{
	UniqueId = -1;
	UnitType = -1;
	x=0;
	y=0;
	Life = 0;
	MovePoints = 0;
	Attack = 0;
	AttackRange = 1;
	name = "";
	text = "";
	timedLife = -1;
	Owner = 0;
	hasAttacked = 0;
}

Unit::Unit(int unitid,int tilex,int tiley,int owner,int uniqueid)
{
	if(unitid==-1)
		std::cout << "ERROR: UnitId = " << unitid << "\n";
	UnitType = unitid;
	UniqueId = uniqueid;
	x=tilex;
	y=tiley;
	sprite = sf::Sprite(UnitTextures.at(unitid));
	sf::Vector2f v = getSpritePos(tilex,tiley);
	sprite.setPosition(v.x+5,v.y+5);
	sprite.setScale(0.5f,0.5f);

	MovePoints = 0;
	timedLife = -1;
	Owner = owner;

	//lua_State* luaS = luaOpen("Lua\\Units.lua");
	
	lua_getglobal(LuaUnits,UnitNames.at(UnitType).c_str());
	lua_getfield(LuaUnits,-1,"life");
	Life = lua_tointeger(LuaUnits,-1);
	maxLife = Life;
	//luaClean(luaS);
	lua_pop(LuaUnits,1);

	lua_getglobal(LuaUnits,UnitNames.at(UnitType).c_str());
	lua_getfield(LuaUnits,-1,"attack");
	Attack = lua_tointeger(LuaUnits,-1);
	lua_pop(LuaUnits,1);

	lua_getglobal(LuaUnits,UnitNames.at(UnitType).c_str());
	lua_getfield(LuaUnits,-1,"attackRange");
	AttackRange = lua_tointeger(LuaUnits,-1);
	lua_pop(LuaUnits,1);
	
	//lua_getglobal(luaS,UnitNames.at(UnitType).c_str());
	lua_getfield(LuaUnits,-1,"name");
	name = lua_tostring(LuaUnits,-1);
	//luaClean(luaS);
	lua_pop(LuaUnits,1);

	//lua_getglobal(luaS,UnitNames.at(UnitType).c_str());
	lua_getfield(LuaUnits,-1,"text");
	text = lua_tostring(LuaUnits,-1);
	//luaClean(luaS);
	lua_pop(LuaUnits,1);

	//lua_getglobal(luaS,UnitNames.at(UnitType).c_str());
	lua_getfield(LuaUnits,-1,"abilityCount");
	abilityCount = lua_tointeger(LuaUnits,-1);
	//luaClean(luaS);
	lua_pop(LuaUnits,1);
	
	abilities = std::deque<Spell>(0);
	for(int i = 0;i<abilityCount;i++)
	{
		//lua_getglobal(LuaUnits,UnitNames.at(UnitType).c_str());
		lua_getfield(LuaUnits,-1,("ability"+std::to_string(i+1)).c_str());
		lua_getfield(LuaUnits,-1,"id");
		std::string abilid = lua_tostring(LuaUnits,-1);
		//luaClean(LuaUnits);
		lua_pop(LuaUnits,2);
		
		lua_State* L = luaL_newstate();
		luaL_openlibs(L);
		registerLua(L);
		if (luaL_loadfile(L, "Lua\\Units.lua") || lua_pcall(L, 0, 0, 0)) 
		{
			std::cout<<"Error: failed to load Units.lua"<<std::endl;
			std::cout << lua_tostring(L,-1) << "\n";
		}
		lua_getglobal(L,UnitNames.at(UnitType).c_str());
		lua_getfield(L,1,("ability"+std::to_string(i+1)).c_str());
		
		abilities.push_back(Spell(getSpellTypeFromName(abilid),HorizontalTileCount*42 + 110 +i*42,165,L));
		luaClean(L);
		lua_close(L);
	}
	lua_pop(LuaUnits,1);

	hasAttacked = 0;

	//lua_close(luaS);
}

Unit::Unit(std::string idname,int x_,int y_,int owner,int uniqueid)
{
	*this = Unit(getUnitTypeFromName(idname),x_,y_,owner,uniqueid);
}

Unit::~Unit()
{
}

//bool Unit::isSilenced()
//{
//	return (StateFlags&UNITSTATE_SILENCED);
//}
//
//bool Unit::isParalyzed()
//{
//	return (StateFlags&UNITSTATE_PARALYZED);
//}

void Unit::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void Unit::addLabel(std::string key,std::string value)
{
	Labels[key] = value;
}

void Unit::addLabel(std::string key,int value)
{
	Labels[key] = std::to_string(value);
}

std::string Unit::getLabel(std::string key)
{
	return Labels[key];
}

int Unit::getLabelAsInt(std::string key)
{
	return atoi(Labels[key].c_str());
}

void Unit::removeLabel(std::string key)
{
	Labels.erase(key);
}

int Unit::sendMessageToBuffs(Message& msg)
{
	for(int i = 0;i<buffs.size();i++)
	{
		lua_getglobal(LuaBuffs,BuffNames.at(buffs.at(i).BuffType).c_str());
		lua_getfield(LuaBuffs,-1,"HandleMessage");
		lua_pushinteger(LuaBuffs,UniqueId);
		lua_pcall(LuaBuffs,1,0,0);
		lua_pop(LuaBuffs,1);
		//luaClean(LuaBuffs);
	}
	return 0;
}

int Unit::handleMessage(Message& msg)
{
	lua_getglobal(LuaUnits,UnitNames.at(UnitType).c_str());
	lua_getfield(LuaUnits,-1,"HandleMessage");
	lua_pushinteger(LuaUnits,UniqueId);
	lua_pcall(LuaUnits,1,0,0);
	sendMessageToBuffs(msg);
	lua_pop(LuaUnits,1);
	return 0;
}

//int Unit::sendMessageToBuffs(const std::string& message,int data1,int data2,int data3,int data4)
//{
//	for(int i = 0;i<buffs.size();i++)
//	{
//		lua_getglobal(LuaBuffs,BuffNames.at(buffs.at(i).BuffType).c_str());
//		lua_getfield(LuaBuffs,-1,"HandleMessage");
//		lua_pushstring(LuaBuffs,message.c_str());
//		lua_pushinteger(LuaBuffs,UniqueId);
//		lua_pushinteger(LuaBuffs,data1);
//		lua_pushinteger(LuaBuffs,data2);
//		lua_pushinteger(LuaBuffs,data3);
//		lua_pushinteger(LuaBuffs,data4);
//		lua_pcall(LuaBuffs,6,1,0);
//		luaClean(LuaBuffs);
//	}
//	return 1;
//}

//int Unit::handleMessage(const std::string& message,int data1,int data2,int data3,int data4)
//{
//	sendMessageToBuffs(message,data1,data2,data3,data4);
//	if(message=="endturn")
//	{
//		if(data1==UniqueId)
//		{
//			for(int i = 0;i<buffs.size();i++)
//			{
//				if(buffs.at(i).Duration!=-1)
//				{
//					buffs.at(i).Duration--; //reduce buff duration
//					if(buffs.at(i).Duration==0)
//					{
//						buffs.erase(buffs.begin()+i);
//					}
//				}
//			}
//			if(timedLife>0) //lower timed life
//			{
//				timedLife--;
//			}
//		}
//	}
//	else if(message=="startturn")
//	{
//		if(data1==UniqueId)
//		{
//			for(int k = 0;k<abilityCount;k++) //lower cooldowns
//			{
//				if(abilities.at(k).currCooldown>0)
//				{
//					abilities.at(k).currCooldown -= 1;
//				}
//			}
//		}
//	}
//	else if(message=="ready")
//	{
//		if(data1==UniqueId)
//			MovePoints = 6;
//		return 1;
//	}
//	else if(message=="destroy")
//	{
//		return 1;
//	}
//	else if(message=="damage")
//	{
//		Life = Life-data1;
//		return 1;
//	}
//	return -1;
//}