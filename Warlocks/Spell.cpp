#include "Spell.h"

lua_State* LuaSpells;
lua_State* LuaUnits;

std::deque<std::string> SpellNames;
std::deque<sf::Texture> SpellTextures;

std::deque<int> isSpellTypeSummon;

int getSpellTypeFromName(std::string name)
{
	for(int i = 0;i<SpellNames.size();i++)
	{
		if(SpellNames.at(i)==name)
		{
			return i;
		}
	}
	return -1;
}

static int loadspell(lua_State* L)
{
	std::string s = lua_tostring(L,1);
	SpellNames.push_back(s);
	std::cout << "Loaded Spell : " << s << "\n";
	sf::Texture t;
	s = "Graphics\\Spells\\" + s + ".png";
	t.loadFromFile(s);
	SpellTextures.push_back(t);
	isSpellTypeSummon.push_back(0);
	return 0;
}

static int loadsummon(lua_State* L)
{
	std::string s = lua_tostring(L,1);
	SpellNames.push_back(s);
	std::cout << "	Loaded Spell : " << s << "\n";
	sf::Texture t;
	s = "Graphics\\Units\\" + s + ".png";
	t.loadFromFile(s);
	SpellTextures.push_back(t);
	isSpellTypeSummon.push_back(1);
	return 0;
}

int initSpells()
{
	std::cout << "Loading Spells...\n";
	LuaSpells = luaL_newstate(); //create new lua state
	luaL_openlibs(LuaSpells);

	lua_register(LuaSpells,"loadspell",loadspell); //register loadspell
	lua_register(LuaSpells,"loadsummon",loadsummon); //register loadsummon
	registerLua(LuaSpells); //register functions

	if(luaL_loadfile(LuaSpells, "Lua\\Spells.lua") || lua_pcall(LuaSpells, 0, 0, 0)) 
	{
        std::cout<<"Error: failed to load Spells.lua"<<std::endl;
		std::cout << lua_tostring(LuaSpells,-1) << "\n";
		getch();
		return -1;
    }

	lua_getglobal(LuaSpells,"loadSpells");
	lua_pcall(LuaSpells,0,0,0); //execute once to load units
	lua_pop(LuaSpells,1);
	
	return 0;
}

void cleanupSpells()
{
	lua_close(LuaSpells);
}

Spell::Spell()
{
	SpellType = -1;
	Range = 0;
	MoveCost = 0;
	ManaCost = 0;
	Cooldown = 0;
	currCooldown = 0;
}

Spell::Spell(int id,int x,int y)
{
	SpellType = id;
	sprite = sf::Sprite(SpellTextures.at(id));
	sprite.setPosition(x,y);
	sprite.setScale(0.6f,0.6f);
	cdtext = sf::Text("0",Font,10);
	cdtext.setColor(sf::Color::White);
	cdtext.setPosition(x+4,y+2);
	cdbox = sf::RectangleShape(sf::Vector2f(10,10));
	cdbox.setFillColor(sf::Color::Black);
	cdbox.setPosition(x+3,y+3);

	lua_State* L = luaOpen("Lua\\Spells.lua");

	lua_getglobal(L,SpellNames.at(SpellType).c_str());
	lua_getfield(L,1,"range");
	Range = lua_tointeger(L,-1);
	luaClean(L);

	lua_getglobal(L,SpellNames.at(SpellType).c_str());
	lua_getfield(L,1,"manaCost");
	ManaCost = lua_tointeger(L,-1);
	luaClean(L);

	lua_getglobal(L,SpellNames.at(SpellType).c_str());
	lua_getfield(L,1,"moveCost");
	MoveCost = lua_tointeger(L,-1);
	luaClean(L);

	lua_getglobal(L,SpellNames.at(SpellType).c_str());
	lua_getfield(L,1,"cooldown");
	Cooldown = lua_tointeger(L,-1);
	luaClean(L);

	if(isSpellTypeSummon.at(SpellType)==1)
	{
		lua_getglobal(LuaUnits,SpellNames.at(SpellType).c_str());
		lua_getfield(LuaUnits,-1,"text");
		Text = lua_tostring(LuaUnits,-1);
		lua_pop(LuaUnits,1);

		lua_getfield(LuaUnits,-1,"name");
		Name = lua_tostring(LuaUnits,-1);
		lua_pop(LuaUnits,2);
	}
	else
	{
		lua_getglobal(L,SpellNames.at(SpellType).c_str());
		lua_getfield(L,1,"text");
		Text = lua_tostring(L,-1);
		luaClean(L);

		lua_getglobal(L,SpellNames.at(SpellType).c_str());
		lua_getfield(L,1,"name");
		Name = lua_tostring(L,-1);
		luaClean(L);
	}

	currCooldown = 0;

	lua_close(L);
}

Spell::Spell(int id,int x,int y,lua_State* L) //custom lua state
{
	SpellType = id;
	sprite = sf::Sprite(SpellTextures.at(id));
	sprite.setPosition(x,y);
	sprite.setScale(0.6f,0.6f);
	cdtext = sf::Text("0",Font,10);
	cdtext.setColor(sf::Color::White);
	cdtext.setPosition(x+4,y+2);
	cdbox = sf::RectangleShape(sf::Vector2f(10,10));
	cdbox.setFillColor(sf::Color::Black);
	cdbox.setPosition(x+3,y+3);

	currCooldown = 0;

	lua_getfield(L,-1,"range");
	Range = lua_tointeger(L,-1);
	//std::cout << "range : " << Range << "\n";
	//lua_pop(L,-1);
	lua_getfield(L,-2,"manaCost");
	ManaCost = lua_tointeger(L,-1);

	lua_getfield(L,-3,"moveCost");
	MoveCost = lua_tointeger(L,-1);
	//std::cout << "movecost : " << MoveCost << "\n";

	lua_getfield(L,-4,"cooldown");
	Cooldown = lua_tointeger(L,-1);
	//std::cout << "cooldown : " << Cooldown << "\n";

	lua_getfield(L,-5,"text");
	Text = lua_tostring(L,-1);

	lua_getfield(L,-6,"name");
	Name = lua_tostring(L,-1);
}

Spell::~Spell()
{
}

//void Spell::OnCast(int x,int y,const World& world) //deprecated
//{
//	std::cout << "spell cast: " << SpellNames.at(SpellType) << "\n";
//
//	std::string s = SpellNames.at(SpellType);
//	lua_getglobal(LuaSpells,s.c_str());
//	lua_getfield(LuaSpells,-1,"OnCast");
//	lua_pushinteger(LuaSpells,x);
//	lua_pushinteger(LuaSpells,y);
//	lua_pcall(LuaSpells,2,0,0);
//	luaClean(LuaSpells);
//
//	currCooldown = Cooldown;
//}

int Spell::getRange()
{
	return Range;
}

void Spell::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	if(currCooldown>0)
	{
		cdtext.setString(std::to_string(currCooldown));
		window.draw(cdbox);
		window.draw(cdtext);
	}
}

bool Spell::checkCollision(int x,int y)
{
	sf::FloatRect b = sprite.getGlobalBounds();
	if(x>=b.left && x<=b.left+b.width && y>=b.top && y<=b.top+b.height)
	{
		return true;
	}
	return false;
}


