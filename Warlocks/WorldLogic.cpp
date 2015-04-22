#include "World.h"

int World::handleMessage(Message& msg)
{
	std::string type = msg.getString("msgtype");
	if(type=="startturn")
	{
		int plyr = msg.getInt("player");
		ManaCap[plyr] += 1;
		Mana[plyr] = ManaCap[plyr];
		for(int i = 0;i<VerticalTileCount;i++)
		{
			for(int j = 0;j<HorizontalTileCount;j++)
			{
				int id = getUnitAt(j,i);
				if(id!=-1)
				{
					if(plyr==units.at(id).Owner)
					{
						for(int k = 0;k<units.at(id).abilityCount;k++) //lower cooldowns
						{
							if(units.at(id).abilities.at(k).currCooldown>0)
							{
								units.at(id).abilities.at(k).currCooldown -= 1;
							}
						}
						if(getUnitCanReady(id))
						{
							Message msg("unitready");
							msg.addValue("unit",std::to_string(id));
							MsgMngr.sendMessage(msg);
						}
					}
				}
			}
		}
	}
	else if(type=="endturn")
	{
		int plyr = msg.getInt("player");
		for(int i = 0;i<VerticalTileCount;i++)
		{
			for(int j = 0;j<HorizontalTileCount;j++)
			{
				int id = getUnitAt(j,i);
				if(id!=-1)
				{
					for(int i = 0;i<units.at(id).buffs.size();i++)
					{
						if(units.at(id).buffs.at(i).Duration!=-1)
						{
							units.at(id).buffs.at(i).Duration--; //reduce buff duration
							if(units.at(id).buffs.at(i).Duration==0)
							{
								units.at(id).buffs.erase(units.at(id).buffs.begin()+i);
								i--;
							}
						}
					}
					if(units.at(id).timedLife>0)
					{
						units.at(id).timedLife--;
					}
				}
			}
		}
		deadUnitCheck();
	}
	else if(type=="unitcast")
	{
		castSpell(msg.getInt("caster"),msg.getInt("spell"),msg.getInt("targetx"),msg.getInt("targety"));
	}
	else if(type=="dodeadunitcheck")
	{
		deadUnitCheck();
	}
	else if(type=="unitsummon")
	{
		summonUnit(msg.getString("unittype"),msg.getInt("x"),msg.getInt("y"),msg.getInt("owner"));
	}
	else if(type=="unitmove")
	{
		int uid = msg.getInt("unit");
		int j = msg.getInt("targetx");
		int i = msg.getInt("targety");
		units.at(uid).MovePoints -= getUnitTileEnterCost(uid,units.at(uid).x,units.at(uid).y,j,i)
			+ getUnitTileExitCost(uid,units.at(uid).x,units.at(uid).y,j,i);
		moveUnit(uid,j,i);
	}
	else if(type=="unitattack")
	{
		int uid = msg.getInt("unit");
		int target = msg.getInt("target");

		int attack = getUnitAttack(uid);
		if(attack>0)
		{
			Message msg("unitdamage");
			msg.addValue("source",uid);
			msg.addValue("target",target);
			msg.addValue("amount",attack);
			msg.addValue("damagetype",DAMAGETYPE_ATTACK);
			MsgMngr.sendMessage(msg);
		}

		units.at(uid).hasAttacked = 1;
		units.at(uid).MovePoints -= getUnitAttackCost(uid);

		Animation a("Attack1","Attack",getSpritePos(units.at(target).x,units.at(target).y),1);
		ActiveAnimations.push_back(a);
		ActiveAnimations.at(ActiveAnimations.size()-1).play();
	}
	else if(type=="unitdamage")
	{
		int src = msg.getInt("source");
		int target = msg.getInt("target");
		int amt = msg.getInt("amount");
		damageUnit(src,target,amt);
		deadUnitCheck();
	}
	else if(type=="unitdamagearea")
	{
		int src = msg.getInt("source");
		int tx = msg.getInt("targetx");
		int ty = msg.getInt("targety");
		int range = msg.getInt("range");
		int amt = msg.getInt("amount");
		int dmgtype = msg.getInt("damagetype");
		int flags = msg.getInt("targetflags");
		damageUnitArea(src,tx,ty,range,amt,dmgtype,flags);
		deadUnitCheck();
	}
	else if(type=="unitheal")
	{
		healUnit(msg.getInt("source"),msg.getInt("target"),msg.getInt("amount"));
		deadUnitCheck();
	}
	else if(type=="unithealarea")
	{
		int src = msg.getInt("source");
		int tx = msg.getInt("targetx");
		int ty = msg.getInt("targety");
		int range = msg.getInt("range");
		int amt = msg.getInt("amount");
		int flags = msg.getInt("targetflags");
		healUnitArea(src,tx,ty,range,amt,flags);
		deadUnitCheck();
	}
	else if(type=="unitdestroy")
	{
		units.at(msg.getInt("unit")).Life = 0;
		deadUnitCheck();
	}
	else if(type=="unitteleport")
	{
		moveUnit(msg.getInt("unit"),msg.getInt("targetx"),msg.getInt("targety"));
	}
	else if(type=="unitdeath")
	{
		int uid = msg.getInt("unit");
		tiles[units.at(uid).x][units.at(uid).y].isOccupied = false;
		//units.erase(uid);
		units.at(uid).UniqueId = -1;
		if(SelectionId == uid)
		{
			SelectionId = -1;
		}
	}
	else if(type=="unitready")
	{
		units.at(msg.getInt("unit")).hasAttacked = 0; //refresh attack status
		units.at(msg.getInt("unit")).MovePoints = 6;
	}
	else if(type=="unitsetlife")
	{
		units.at(msg.getInt("unit")).Life = msg.getInt("amount");
	}
	else if(type=="unitsetmaxlife")
	{
		units.at(msg.getInt("unit")).maxLife = msg.getInt("amount");
	}
	else if(type=="unitsetbaseattack")
	{
		units.at(msg.getInt("unit")).Attack = msg.getInt("amount");
	}
	else if(type=="unitsetmovepoints")
	{
		units.at(msg.getInt("unit")).MovePoints = msg.getInt("amount");
	}
	else if(type=="unitchangeowner")
	{
		units.at(msg.getInt("unit")).Owner = msg.getInt("owner");
	}
	else if(type=="unitsettimedlife")
	{
		units.at(msg.getInt("unit")).timedLife = msg.getInt("amount");
	}
	else if(type=="buffcreate")
	{
		Buff b(msg.getInt("bufftype"),msg.getInt("target"),msg.getInt("duration"),msg.getInt("owner"),msg.getInt("ispermanent"),0,0);
		units.at(msg.getInt("target")).buffs.push_back(b);
	}
	else if(type=="buffremove")
	{
		int uid = msg.getInt("target");
		int bid = msg.getInt("bufftype");
		for(int i = 0;i<world.units.at(uid).buffs.size();i++)
		{
			if(world.units.at(uid).buffs.at(i).BuffType==bid)
			{
				units.at(uid).buffs.erase(world.units.at(uid).buffs.begin()+i);
				i--;
			}
		}
	}
	else if(type=="buffdispel")
	{
		dispelUnit(msg.getInt("unit"),msg.getInt("targetflags"));
	}
	else if(type=="buffsetduration")
	{
		units.at(msg.getInt("unit")).buffs.at(msg.getInt("buff")).Duration = msg.getInt("duration");
	}
	else if(type=="spellsetcurrentcooldown")
	{
		units.at(msg.getInt("unit")).abilities.at(msg.getInt("spell")).currCooldown = msg.getInt("cooldown");
	}
	else if(type=="tiletypechange")
	{
		world.tiles[msg.getInt("x")][msg.getInt("y")].setType(msg.getInt("tiletype"));
	}
	else if(type=="playanimation")
	{
		Animation a(msg.getString("animation"),msg.getString("sound"),getSpritePos(msg.getInt("x"),msg.getInt("y")),atof(msg.getString("scalingfactor").c_str()));
		ActiveAnimations.push_back(a);
		ActiveAnimations.at(ActiveAnimations.size()-1).play();
	}
	else if(type=="playsound")
	{
		ActiveAnimations.push_back(Animation("Attack1",msg.getString("sound"),0,0,1.0,1));
		ActiveAnimations.at(ActiveAnimations.size()-1).play();
	}
	return 0;
}


void World::update()
{
	bool worldchanged = false;
	while(MsgMngr.hasMoreMessages())
	{
		Message msg = MsgMngr.peekMessage();
		MsgMngr.dispatch();
		std::cout << "dispatching message: ";
		for(std::map<std::string,std::string>::iterator i = msg.map.begin();i!=msg.map.end();i++)
		{
			std::cout << i->first << " " << i->second << " ";
		}
		std::cout << "\n";
		dispatchMessage(msg);
		worldchanged = true;
	}
	if(worldchanged)
	{
		updateInfobox();
	}
}

void World::dispatchMessage(Message& msg)
{
	std::string type = msg.getString("msgtype");
	std::map<int,Unit>::iterator i;

	currentMessage = msg;

	currentMessage.addValue("msgtype","mod "+type);
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(msg);
	}

	//std::cout << "  mod\n";
	if(currentMessage.getInt("msgContinue")==0) //do we continue?
		return;

	currentMessage.addValue("msgtype","pre "+type);
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(msg);
	}

	//std::cout << "  pre\n";
	if(currentMessage.getInt("msgContinue")==0)
		return;

	currentMessage.addValue("msgtype",type);
	handleMessage(currentMessage);
	//std::cout << "  in\n";

	currentMessage.addValue("msgtype","post "+type);
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(currentMessage);
	}

	//std::cout << "  post\n";
}

int World::deadUnitCheck()
{
	std::map<int,Unit>::iterator i;
	std::vector<int> markedForDeath;
	for(i = units.begin();i!=units.end();++i)
	{
		if(unitExists(i->first))
		{
			if(i->second.Life <= 0)
			{
				markedForDeath.push_back(i->first);
				std::cout << "marked " << i->first << "\n";
			}
			else if(i->second.timedLife == 0)
			{
				markedForDeath.push_back(i->first);
				std::cout << "marked " << i->first << "\n";
			}
			else if(i->second.Life > i->second.maxLife) //check if Life is greater than Max Life
			{
				i->second.Life = i->second.maxLife;
			}
		}
	}
	for(int j = 0;j<markedForDeath.size();j++)
	{
		Message msg("unitdeath");
		msg.addValue("unit",markedForDeath.at(j));
		MsgMngr.sendMessage(msg);
	}
	return 0;
}

void World::endTurn()
{
	CastingUnitId = -1;
	CastingSpellId = -1;
	clearHighlightedTiles();

	//end previous turn
	Message msg("endturn");
	msg.addValue("player",turn);
	MsgMngr.sendMessage(msg);

	turn = (turn+1)%2;

	//start next turn
	Message msg2("startturn");
	msg2.addValue("player",turn);
	MsgMngr.sendMessage(msg2);
}

int World::getUnitAt(int x,int y)
{
	std::map<int,Unit>::iterator i;
	for(i = units.begin();i!=units.end();++i)
	{
		if(unitExists(i->first))
		{
			if(i->second.x == x && i->second.y == y)
			{
				return i->second.UniqueId;
			}
		}
	}
	return -1;
}

int World::getUnitTileEnterCost(int uid,int x1,int y1,int x2,int y2)
{
	Message oldmsg = currentMessage;
	currentMessage = Message("get unittileentercost");
	currentMessage.addValue("entercost",TileEnterCost[tiles[x2][y2].type]);
	currentMessage.addValue("unit",uid);
	currentMessage.addValue("fromx",x1);
	currentMessage.addValue("fromy",y1);
	currentMessage.addValue("tox",x2);
	currentMessage.addValue("toy",y2);

	std::map<int,Unit>::iterator i;
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(currentMessage);
	}
	int c = currentMessage.getInt("entercost");
	currentMessage = oldmsg;
	return c;
}

int World::getUnitTileExitCost(int uid,int x1,int y1,int x2,int y2)
{
	Message oldmsg = currentMessage;
	currentMessage = Message("get unittileexitcost");
	currentMessage.addValue("exitcost",TileExitCost[tiles[x1][y1].type]);
	currentMessage.addValue("unit",uid);
	currentMessage.addValue("fromx",x1);
	currentMessage.addValue("fromy",y1);
	currentMessage.addValue("tox",x2);
	currentMessage.addValue("toy",y2);

	std::map<int,Unit>::iterator i;
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(currentMessage);
	}
	int c = currentMessage.getInt("exitcost");
	currentMessage = oldmsg;
	return c;
}

int World::getUnitAttack(int uid)
{
	Message oldmsg = currentMessage;
	currentMessage = Message("get unitattack");
	currentMessage.addValue("attack",units.at(uid).Attack);
	currentMessage.addValue("unit",uid);

	std::map<int,Unit>::iterator i;
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(currentMessage);
	}
	int c = currentMessage.getInt("attack");
	currentMessage = oldmsg;
	return c;
}

int World::getUnitAttackRange(int uid)
{
	Message oldmsg = currentMessage;
	currentMessage = Message("get unitattackrange");
	currentMessage.addValue("attackrange",units.at(uid).AttackRange);
	currentMessage.addValue("unit",uid);

	std::map<int,Unit>::iterator i;
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(currentMessage);
	}
	int c = currentMessage.getInt("attackrange");
	currentMessage = oldmsg;
	return c;
}

int World::getUnitAttackCost(int uid)
{
	Message oldmsg = currentMessage;
	currentMessage = Message("get unitattackcost");
	currentMessage.addValue("attackcost",2);
	currentMessage.addValue("unit",uid);

	std::map<int,Unit>::iterator i;
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(currentMessage);
	}
	int c = currentMessage.getInt("attackcost");
	currentMessage = oldmsg;
	return c;
}

int World::getUnitCanAttack(int uid)
{
	Message oldmsg = currentMessage;
	currentMessage = Message("get unitcanattack");
	currentMessage.addValue("canattack",1);
	currentMessage.addValue("unit",uid);

	std::map<int,Unit>::iterator i;
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(currentMessage);
	}
	int c = currentMessage.getInt("canattack");
	currentMessage = oldmsg;
	return c;
}

int World::getUnitCanCast(int uid)
{
	Message oldmsg = currentMessage;
	currentMessage = Message("get unitcancast");
	currentMessage.addValue("cancast",1);
	currentMessage.addValue("unit",uid);

	std::map<int,Unit>::iterator i;
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(currentMessage);
	}
	int c = currentMessage.getInt("cancast");
	currentMessage = oldmsg;
	return c;
}

int World::getUnitCanMove(int uid)
{
	Message oldmsg = currentMessage;
	currentMessage = Message("get unitcanmove");
	currentMessage.addValue("canmove",1);
	currentMessage.addValue("unit",uid);

	std::map<int,Unit>::iterator i;
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(currentMessage);
	}
	int c = currentMessage.getInt("canmove");
	currentMessage = oldmsg;
	return c;
}

int World::getUnitCanReady(int uid)
{
	Message oldmsg = currentMessage;
	currentMessage = Message("get unitcanready");
	currentMessage.addValue("canready",1);
	currentMessage.addValue("unit",uid);

	std::map<int,Unit>::iterator i;
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(currentMessage);
	}
	int c = currentMessage.getInt("canready");
	currentMessage = oldmsg;
	return c;
}

void World::summonUnit(int type,int x,int y,int owner)
{
	Unit u(type,x,y,owner,nextValidUnitId);
	tiles[x][y].isOccupied = true;
	units.insert(std::pair<int,Unit>(nextValidUnitId,u));

	/*lua_getglobal(LuaUnits,UnitNames.at(type).c_str());
	lua_getfield(LuaUnits,-1,"OnSummon");
	lua_pushinteger(LuaUnits,nextValidUnitId);
	lua_pcall(LuaUnits,1,0,0);*/

	nextValidUnitId++;
}

void World::damageUnit(int src,int target,int amount)
{
	if(amount>0)
		units.at(target).Life = units.at(target).Life-amount;
}

void World::damageUnitArea(int src,int x,int y,int range,int amount,int dmgtype,int flags)
{
	int myowner = units.at(src).Owner;
	for(int i = x-range;i<=x+range;i++)
	{
		for(int j = y-range;j<=y+range;j++)
		{
			int uid = getUnitAt(i,j);
			if(uid!=-1 && (uid!=src || !(flags&TARGET_NONSELF)))
			{
				if(isUnitAvatar(uid)==0 || !(flags&TARGET_NONAVATAR))
				{
					int owner = units.at(uid).Owner;
					if(flags&TARGET_ALLY && !(flags&TARGET_ENEMY) && owner==myowner)  //damage only allies
						damageUnit(src,uid,amount);
					else if(!(flags&TARGET_ALLY) && flags&TARGET_ENEMY && owner!=myowner)  //damage only enemies
						damageUnit(src,uid,amount);
					else if(!(flags&TARGET_ALLY) && !(flags&TARGET_ENEMY))  //damage both
						damageUnit(src,uid,amount);
					else if(flags&TARGET_ALLY && flags&TARGET_ENEMY)  //damage both
						damageUnit(src,uid,amount);
				}
			}
		}
	}
}

void World::healUnit(int src,int target,int amt)
{
	if(amt > 0)
		units.at(target).Life = units.at(target).Life+amt;
}

void World::healUnitArea(int src,int x,int y,int range,int amount,int flags)
{
	int myowner = units.at(src).Owner;
	for(int i = x-range;i<=x+range;i++)
	{
		for(int j = y-range;j<=y+range;j++)
		{
			int uid = getUnitAt(i,j);
			if(uid!=-1 && (uid!=src || !(flags&TARGET_NONSELF)))
			{
				if(isUnitAvatar(uid)==0 || !(flags&TARGET_NONAVATAR))
				{
					int owner = units.at(uid).Owner;
					if(flags&TARGET_ALLY && !(flags&TARGET_ENEMY) && owner==myowner)  //damage only allies
						healUnit(src,uid,amount);
					else if(!(flags&TARGET_ALLY) && flags&TARGET_ENEMY && owner!=myowner)  //damage only enemies
						healUnit(src,uid,amount);
					else if(!(flags&TARGET_ALLY) && !(flags&TARGET_ENEMY))  //damage both
						healUnit(src,uid,amount);
					else if(flags&TARGET_ALLY && flags&TARGET_ENEMY)  //damage both
						healUnit(src,uid,amount);
				}
			}
		}
	}
}

void World::summonUnit(std::string name,int x,int y,int owner)
{
	summonUnit(getUnitTypeFromName(name),x,y,owner);
}

void World::moveUnit(int id,int x,int y)
{
	tiles[units.at(id).x][units.at(id).y].isOccupied = false;
	units.at(id).x = x;
	units.at(id).y = y;
	tiles[x][y].isOccupied = true;
	units.at(id).sprite.setPosition(getSpritePos(x,y).x+5,getSpritePos(x,y).y+5);
}

void World::dispelUnit(int id,int flags)
{
	for(int i = 0;i<world.units.at(id).buffs.size();i++)
	{
		if(flags&TARGET_ALLY) //destroy allied buffs
		{
			if(world.units.at(id).buffs.at(i).Owner==world.units.at(id).Owner && !(world.units.at(id).buffs.at(i).Flags&BUFF_NONDISPELLABLE))
			{
				units.at(id).buffs.erase(world.units.at(id).buffs.begin()+i);
				i--;
			}
		}
		if(flags&TARGET_ENEMY) //destroy enemy buffs
		{
			if(world.units.at(id).buffs.at(i).Owner!=world.units.at(id).Owner && !(world.units.at(id).buffs.at(i).Flags&BUFF_NONDISPELLABLE))
			{
				units.at(id).buffs.erase(world.units.at(id).buffs.begin()+i);
				i--;
			}
		}
		if(!(flags&TARGET_ENEMY && flags&TARGET_ALLY)) //destroy all buffs
		{
			if(!(world.units.at(id).buffs.at(i).Flags&BUFF_NONDISPELLABLE))
			{
				units.at(id).buffs.erase(world.units.at(id).buffs.begin()+i);
				i--;
			}
		}
	}
}

int World::unitExists(int uid)
{
	if(units.at(uid).UniqueId==-1)
	{
		//units.erase(uid);
		return 0;
	}
	return 1;
}

int World::isUnitAvatar(int uid)
{
	if(units.at(uid).UnitType==0)
		return 1;
	return 0;
}

void World::castSpell(int castingunit,int castingspell,int targetx,int targety)
{
	lua_State* L;
	if(isUnitAvatar(castingunit)==0) //unit is not an avatar
	{
		L = LuaUnits;
		lua_getglobal(L,UnitNames.at(units.at(castingunit).UnitType).c_str());
		lua_getfield(L,-1,("ability"+std::to_string(castingspell+1)).c_str());
	}
	else
	{
		L = LuaSpells;
		lua_getglobal(L,SpellNames.at(units.at(castingunit).abilities.at(castingspell).SpellType).c_str());
	}
	//lua_getglobal(LuaSpells,SpellNames.at(CastingSpellId).c_str());
	lua_getfield(L,-1,"OnCast");
	lua_pushinteger(L,castingunit);
	lua_pushinteger(L,castingspell);
	lua_pushinteger(L,targetx);
	lua_pushinteger(L,targety);
	lua_pcall(L,4,0,0);
	lua_pop(L,2);
	Mana[units.at(castingunit).Owner] -= units.at(castingunit).abilities.at(castingspell).ManaCost;
	units.at(castingunit).abilities.at(castingspell).currCooldown = units.at(castingunit).abilities.at(castingspell).Cooldown;
	units.at(castingunit).MovePoints -= units.at(castingunit).abilities.at(castingspell).MoveCost;
	//CastingSpellId = -1;
	//CastingUnitId = -1;
	//clearHighlightedTiles();
}