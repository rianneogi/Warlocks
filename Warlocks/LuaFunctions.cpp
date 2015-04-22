#include "LuaFunctions.h"

static int setMessageString(lua_State* L)
{
	world.currentMessage.addValue(lua_tostring(L,1),lua_tostring(L,2));
	return 0;
}

static int setMessageInt(lua_State* L)
{
	world.currentMessage.addValue(lua_tostring(L,1),std::to_string(lua_tointeger(L,2)));
	return 0;
}

static int setUnitLabelString(lua_State* L)
{
	world.units.at(lua_tointeger(L,1)).addLabel(lua_tostring(L,2),lua_tostring(L,3));
	return 0;
}

static int setUnitLabelInt(lua_State* L)
{
	world.units.at(lua_tointeger(L,1)).addLabel(lua_tostring(L,2),std::to_string(lua_tointeger(L,3)));
	std::cout << "unit label set to " << lua_tointeger(L,3) << "\n";
	return 0;
}

static int changeTileType(lua_State* L)
{
	//world.tiles[lua_tointeger(L,1)][lua_tointeger(L,2)].setType(lua_tointeger(L,3));

	Message msg("tiletypechange");
	msg.addValue("x",lua_tointeger(L,1));
	msg.addValue("y",lua_tointeger(L,2));
	msg.addValue("tiletype",lua_tointeger(L,3));
	world.MsgMngr.sendMessage(msg);

	return 0;
}

static int dealDamage(lua_State* L)
{
	int src = lua_tointeger(L,1);
	int target = lua_tointeger(L,2);
	int amount = lua_tointeger(L,3);
	int damagetype = lua_tointeger(L,4);
	if(!world.unitExists(target))
		return 0;

	Message msg("unitdamage");
	msg.addValue("source",std::to_string(src));
	msg.addValue("target",std::to_string(target));
	msg.addValue("amount",std::to_string(amount));
	msg.addValue("damagetype",std::to_string(damagetype));
	//world.dispatchMessage(msg);
	world.MsgMngr.sendMessage(msg);

	//world.units.at(unit).handleMessage("damage",lua_tointeger(L,2),0,0,0);
	//world.deadUnitCheck();
	return 0;
}

static int dealDamageArea(lua_State* L) //deprecated
{
	int src = lua_tointeger(L,1);
	int targetx = lua_tointeger(L,2);
	int targety = lua_tointeger(L,3);
	int range = lua_tointeger(L,4);
	int amount = lua_tointeger(L,5);
	int damagetype = lua_tointeger(L,6);
	int targettype = lua_tointeger(L,7);

	Message msg("unitdamagearea");
	msg.addValue("source",std::to_string(src));
	msg.addValue("targetx",std::to_string(targetx));
	msg.addValue("targety",std::to_string(targety));
	msg.addValue("range",std::to_string(range));
	msg.addValue("amount",std::to_string(amount));
	msg.addValue("damagetype",std::to_string(damagetype));
	msg.addValue("targetflags",std::to_string(targettype));
	world.MsgMngr.sendMessage(msg);

	return 0;
}

static int healUnit(lua_State* L)
{
	int src = lua_tointeger(L,1);
	int target = lua_tointeger(L,2);
	int amount = lua_tointeger(L,3);
	if(!world.unitExists(target))
		return 0;

	Message msg("unitheal");
	msg.addValue("source",std::to_string(src));
	msg.addValue("target",std::to_string(target));
	msg.addValue("amount",std::to_string(amount));
	//world.dispatchMessage(msg);
	world.MsgMngr.sendMessage(msg);

	//world.units.at(unit).handleMessage("damage",lua_tointeger(L,2),0,0,0);
	//world.deadUnitCheck();
	return 0;
}

static int summonUnit(lua_State* L)
{
	std::string name = lua_tostring(L,1);
	int x = lua_tointeger(L,2);
	int y = lua_tointeger(L,3);
	int owner = lua_tointeger(L,4);
	//world.summonUnit(name,x,y,owner);
	Message msg("unitsummon");
	msg.addValue("unittype",name);
	msg.addValue("x",std::to_string(x));
	msg.addValue("y",std::to_string(y));
	msg.addValue("owner",std::to_string(owner));
	msg.addValue("unitid",world.nextValidUnitId);
	world.MsgMngr.sendMessage(msg);
	std::cout << "unit summoned : " << name << " id: " << world.nextValidUnitId <<  "\n";
	return 0;
}

static int destroyUnit(lua_State* L)
{
	int id = lua_tointeger(L,1);
	
	Message msg("unitdestroy");
	msg.addValue("unit",id);
	world.MsgMngr.sendMessage(msg);

	return 0;
}

static int teleportUnit(lua_State* L)
{
	int unit = lua_tointeger(L,1);
	int x = lua_tointeger(L,2);
	int y = lua_tointeger(L,3);
	if(!world.unitExists(unit))
		return 0;

	Message msg("unitteleport");
	msg.addValue("unit",unit);
	msg.addValue("targetx",x);
	msg.addValue("targety",y);
	world.MsgMngr.sendMessage(msg);

	return 0;
}


static int readyUnit(lua_State* L)
{
	int unit = lua_tointeger(L,1);
	if(!world.unitExists(unit))
		return 0;

	Message msg("unitready");
	msg.addValue("unit",unit);
	world.MsgMngr.sendMessage(msg);

	return 0;
}

static int addBuff(lua_State* L)
{
	int uid = lua_tointeger(L,2);
	if(!world.unitExists(uid))
	{
		return 0;
	}
	int bid = getBuffTypeFromName(lua_tostring(L,1));
	int dur = lua_tointeger(L,3);
	int owner = lua_tointeger(L,4);
	int isper = lua_tointeger(L,5);

	Message msg("buffcreate");
	msg.addValue("bufftype",bid);
	msg.addValue("target",uid);
	msg.addValue("duration",dur);
	msg.addValue("owner",owner);
	msg.addValue("ispermanent",isper);
	world.MsgMngr.sendMessage(msg);
	/*Buff b(bid,uid,dur,owner,isper,0,0);
	world.units.at(uid).buffs.push_back(b);*/
	return 0;
}

static int removeBuff(lua_State* L)
{
	int uid = lua_tointeger(L,1);
	int bid = getBuffTypeFromName(lua_tostring(L,2));
	
	Message msg("buffremove");
	msg.addValue("target",uid);
	msg.addValue("bufftype",bid);
	world.MsgMngr.sendMessage(msg);

	return 0;
}

static int dispelBuffs(lua_State* L)
{
	int uid = lua_tointeger(L,1);
	int targetflags = lua_tointeger(L,2);

	Message msg("buffdispel");
	msg.addValue("unit",uid);
	msg.addValue("targetflags",targetflags);
	world.MsgMngr.sendMessage(msg);

	return 0;
}

static int setBuffDuration(lua_State* L)
{
	int uid = lua_tointeger(L,1);
	int bid = lua_tointeger(L,2);
	int dur = lua_tointeger(L,3);

	Message msg("buffsetduration");
	msg.addValue("unit",uid);
	msg.addValue("buff",bid);
	msg.addValue("duration",dur);
	world.MsgMngr.sendMessage(msg);

	return 0;
}

static int setSpellCurrentCooldown(lua_State* L)
{
	int uid = lua_tointeger(L,1);
	int sid = lua_tointeger(L,2);
	int cd = lua_tointeger(L,3);

	Message msg("spellsetcurrentcooldown");
	msg.addValue("unit",uid);
	msg.addValue("spell",sid);
	msg.addValue("cooldown",cd);
	world.MsgMngr.sendMessage(msg);

	return 0;
}

static int setUnitLife(lua_State* L)
{
	int uid = lua_tointeger(L,1);
	int amnt = lua_tointeger(L,2);

	Message msg("unitsetlife");
	msg.addValue("unit",uid);
	msg.addValue("amount",amnt);
	world.MsgMngr.sendMessage(msg);

	return 0;
}

static int setUnitMaxLife(lua_State* L)
{
	int uid = lua_tointeger(L,1);
	int amnt = lua_tointeger(L,2);

	Message msg("unitsetmaxlife");
	msg.addValue("unit",uid);
	msg.addValue("amount",amnt);
	world.MsgMngr.sendMessage(msg);

	return 0;
}

static int setUnitBaseAttack(lua_State* L)
{
	int uid = lua_tointeger(L,1);
	int amnt = lua_tointeger(L,2);

	Message msg("unitsetbaseattack");
	msg.addValue("unit",uid);
	msg.addValue("amount",amnt);
	world.MsgMngr.sendMessage(msg);

	return 0;
}

static int setUnitMovePoints(lua_State* L)
{
	int uid = lua_tointeger(L,1);
	int mp = lua_tointeger(L,2);

	Message msg("unitsetmovepoints");
	msg.addValue("unit",uid);
	msg.addValue("amount",mp);
	world.MsgMngr.sendMessage(msg);

	return 0;
}

static int changeUnitOwner(lua_State* L)
{
	int uid = lua_tointeger(L,1);
	int owner = lua_tointeger(L,2);

	Message msg("unitchangeowner");
	msg.addValue("unit",uid);
	msg.addValue("owner",owner);
	world.MsgMngr.sendMessage(msg);

	return 0;
}

static int setUnitTimedLife(lua_State* L)
{
	int uid = lua_tointeger(L,1);
	int amt = lua_tointeger(L,2);

	Message msg("unitsettimedlife");
	msg.addValue("unit",uid);
	msg.addValue("amount",amt);
	world.MsgMngr.sendMessage(msg);
	
	return 0;
}

static int setPlayerMana(lua_State* L)
{
	world.Mana[lua_tointeger(L,1)] = lua_tointeger(L,2);
	return 0;
}

static int setPlayerManaCap(lua_State* L)
{
	world.ManaCap[lua_tointeger(L,1)] = lua_tointeger(L,2);
	return 0;
}

static int doDeadUnitCheck(lua_State* L)
{
	//world.deadUnitCheck();
	Message msg("dodeadunitcheck");
	world.MsgMngr.sendMessage(msg);
	return 0;
}

static int playAnimation(lua_State* L)
{
	Message msg("playanimation");
	msg.addValue("animation",lua_tostring(L,1));
	msg.addValue("sound",lua_tostring(L,2));
	msg.addValue("x",lua_tointeger(L,3));
	msg.addValue("y",lua_tointeger(L,4));
	msg.addValue("scalingfactor",lua_tonumber(L,5));
	world.MsgMngr.sendMessage(msg);
	return 0;
}

static int playSound(lua_State* L)
{
	Message msg("playsound");
	msg.addValue("sound",lua_tostring(L,1));
	msg.addValue("volume",lua_tonumber(L,2));
	world.MsgMngr.sendMessage(msg);
	return 0;
}

static int getMessageString(lua_State* L)
{
	lua_pushstring(L,world.currentMessage.getString(lua_tostring(L,1)).c_str());
	return 1;
}

static int getMessageInt(lua_State* L)
{
	lua_pushinteger(L,world.currentMessage.getInt(lua_tostring(L,1)));
	return 1;
}

static int getUnitLabelString(lua_State* L)
{
	lua_pushstring(L,world.units.at(lua_tointeger(L,1)).getLabel(lua_tostring(L,2)).c_str());
	return 1;
}

static int getUnitLabelInt(lua_State* L)
{
	lua_pushinteger(L,world.units.at(lua_tointeger(L,1)).getLabelAsInt(lua_tostring(L,2)));
	return 1;
}

static int getUnitPos(lua_State* L)
{
	int id = lua_tointeger(L,1);
	lua_pushinteger(L,world.units.at(id).x);
	lua_pushinteger(L,world.units.at(id).y);
	return 2;
}

static int getUnitX(lua_State* L)
{
	int id = lua_tointeger(L,1);
	lua_pushinteger(L,world.units.at(id).x);
	return 1;
}

static int getUnitY(lua_State* L)
{
	int id = lua_tointeger(L,1);
	lua_pushinteger(L,world.units.at(id).y);
	return 1;
}

static int getUnitAt(lua_State* L)
{
	int id = world.getUnitAt(lua_tointeger(L,1),lua_tointeger(L,2));
	lua_pushinteger(L,id);
	return 1;
}

static int getUnitLife(lua_State* L)
{
	int id = lua_tointeger(L,1);
	lua_pushinteger(L,world.units.at(id).Life);
	return 1;
}

static int getUnitMaxLife(lua_State* L)
{
	int id = lua_tointeger(L,1);
	lua_pushinteger(L,world.units.at(id).maxLife);
	return 1;
}

static int getUnitAttack(lua_State* L)
{
	int id = lua_tointeger(L,1);
	lua_pushinteger(L,world.getUnitAttack(id));
	return 1;
}

static int getUnitBaseAttack(lua_State* L)
{
	int id = lua_tointeger(L,1);
	lua_pushinteger(L,world.units.at(id).Attack);
	return 1;
}

static int getUnitAttackRange(lua_State* L)
{
	int id = lua_tointeger(L,1);
	lua_pushinteger(L,world.getUnitAttackRange(id));
	return 1;
}

static int getUnitBaseAttackRange(lua_State* L)
{
	int id = lua_tointeger(L,1);
	lua_pushinteger(L,world.units.at(id).AttackRange);
	return 1;
}

static int getUnitAbilityCount(lua_State* L)
{
	int id = lua_tointeger(L,1);
	lua_pushinteger(L,world.units.at(id).abilityCount);
	return 1;
}

static int getUnitMovePoints(lua_State* L)
{
	int id = lua_tointeger(L,1);
	lua_pushinteger(L,world.units.at(id).MovePoints);
	return 1;
}

static int getUnitOwner(lua_State* L)
{
	int id = lua_tointeger(L,1);
	lua_pushinteger(L,world.units.at(id).Owner);
	return 1;
}

static int getUnitTimedLife(lua_State* L)
{
	int id = lua_tointeger(L,1);
	lua_pushinteger(L,world.units.at(id).timedLife);
	return 1;
}

static int getUnitBuffCount(lua_State* L)
{
	lua_pushinteger(L,world.units.at(lua_tointeger(L,1)).buffs.size());
	return 1;
}

static int isUnitAvatar(lua_State* L)
{
	lua_pushinteger(L,world.isUnitAvatar(lua_tointeger(L,1)));
	return 1;
}

static int unitHasBuff(lua_State* L)
{
	int uid = lua_tointeger(L,1);
	std::string bid = lua_tostring(L,2);
	int flag = 0;
	for(int i = 0;i<world.units.at(uid).buffs.size();i++)
	{
		if(BuffNames.at(world.units.at(uid).buffs.at(i).BuffType)==bid)
		{
			flag = 1;
			break;
		}
	}
	if(flag==0)
	{
		lua_pushinteger(L,0); //buff doesn't exist
	}
	else
	{
		lua_pushinteger(L,1); //buff exists
	}
	return 1;
}

static int canUnitAttack(lua_State* L)
{
	int uid = lua_tointeger(L,1);
	lua_pushinteger(L,world.getUnitCanAttack(uid));
	return 1;
}

static int canUnitMove(lua_State* L)
{
	int uid = lua_tointeger(L,1);
	lua_pushinteger(L,world.getUnitCanMove(uid));
	return 1;
}

static int canUnitCast(lua_State* L)
{
	int uid = lua_tointeger(L,1);
	lua_pushinteger(L,world.getUnitCanCast(uid));
	return 1;
}

static int canUnitReady(lua_State* L)
{
	int uid = lua_tointeger(L,1);
	lua_pushinteger(L,world.getUnitCanReady(uid));
	return 1;
}

static int getBuffDuration(lua_State* L)
{
	int uid = lua_tointeger(L,1);
	std::string bid = lua_tostring(L,2);
	int flag = 0;
	for(int i = 0;i<world.units.at(uid).buffs.size();i++)
	{
		if(BuffNames.at(world.units.at(uid).buffs.at(i).BuffType)==bid)
		{
			lua_pushinteger(L,world.units.at(uid).buffs.at(i).Duration);
			flag = 1;
			break;
		}
	}
	if(flag==0)
	{
		lua_pushinteger(L,-2); //buff doesn't exist
	}
	return 1;
}

static int buffIsDispellable(lua_State* L)
{
	int uid = lua_tointeger(L,1);
	std::string bid = lua_tostring(L,2);
	int flag = 0;
	for(int i = 0;i<world.units.at(uid).buffs.size();i++)
	{
		if(BuffNames.at(world.units.at(uid).buffs.at(i).BuffType)==bid)
		{
			if(world.units.at(uid).buffs.at(i).Flags&BUFF_NONDISPELLABLE)
			{
				lua_pushinteger(L,0);
			}
			else
			{
				lua_pushinteger(L,1);
			}
			flag = 1;
			break;
		}
	}
	if(flag==0)
	{
		lua_pushinteger(L,-2); //buff doesn't exist
	}
	return 1;
}

static int getSpellCurrentCooldown(lua_State* L)
{
	lua_pushinteger(L,world.units.at(lua_tointeger(L,1)).abilities.at(lua_tointeger(L,2)).currCooldown);
	return 1;
}

static int getTileType(lua_State* L)
{
	lua_pushinteger(L,world.tiles[lua_tointeger(L,1)][lua_tointeger(L,2)].type);
	return 1;
}

static int tileIsOccupied(lua_State* L)
{
	if(world.tiles[lua_tointeger(L,1)][lua_tointeger(L,2)].isOccupied == true)
		lua_pushinteger(L,1);
	else
		lua_pushinteger(L,0);
	return 1;
}

static int getPlayerMana(lua_State* L)
{
	lua_pushinteger(L,world.Mana[lua_tointeger(L,1)]);
	return 1;
}

static int getPlayerManaCap(lua_State* L)
{
	lua_pushinteger(L,world.ManaCap[lua_tointeger(L,1)]);
	return 1;
}

static int getTurn(lua_State* L)
{
	lua_pushinteger(L,world.turn);
	return 1;
}

static int getCastingSpellId(lua_State* L)
{
	lua_pushinteger(L,world.CastingSpellId);
	return 1;
}

static int getCastingUnitId(lua_State* L)
{
	lua_pushinteger(L,world.CastingUnitId);
	return 1;
}

static int printstr(lua_State* L)
{
	std::cout << lua_tostring(L,1);
	return 0;
}

static int printint(lua_State* L)
{
	std::cout << lua_tointeger(L,1);
	return 0;
}

void registerLua(lua_State* L)
{
	lua_register(L,"printstr",printstr);
	lua_register(L,"printint",printint);

	//actions
	lua_register(L,"setMessageString",setMessageString);
	lua_register(L,"setMessageInt",setMessageInt);
	lua_register(L,"setUnitLabelString",setUnitLabelString);
	lua_register(L,"setUnitLabelInt",setUnitLabelInt);
	lua_register(L,"changeTileType",changeTileType);
	lua_register(L,"dealDamage",dealDamage);
	//lua_register(L,"dealDamageArea",dealDamageArea);
	lua_register(L,"healUnit",healUnit);
	lua_register(L,"summonUnit",summonUnit);
	lua_register(L,"destroyUnit",destroyUnit);
	lua_register(L,"teleportUnit",teleportUnit);
	lua_register(L,"readyUnit",readyUnit);
	lua_register(L,"addBuff",addBuff);
	lua_register(L,"removeBuff",removeBuff);
	lua_register(L,"dispelBuffs",dispelBuffs);
	lua_register(L,"setBuffDuration",setBuffDuration);
	lua_register(L,"setSpellCurrentCooldown",setSpellCurrentCooldown);
	lua_register(L,"setUnitLife",setUnitLife);
	lua_register(L,"setUnitMaxLife",setUnitMaxLife);
	lua_register(L,"setUnitBaseAttack",setUnitBaseAttack);
	lua_register(L,"setUnitMovePoints",setUnitMovePoints);
	lua_register(L,"setUnitOwner",getUnitOwner);
	lua_register(L,"setUnitTimedLife",setUnitTimedLife);
	lua_register(L,"setPlayerMana",setPlayerMana);
	lua_register(L,"setPlayerManaCap",setPlayerManaCap);
	lua_register(L,"doDeadUnitCheck",doDeadUnitCheck);
	lua_register(L,"playAnimation",playAnimation);
	lua_register(L,"playSound",playSound);

	//gets
	lua_register(L,"getMessageString",getMessageString);
	lua_register(L,"getMessageInt",getMessageInt);
	lua_register(L,"getUnitLabelString",getUnitLabelString);
	lua_register(L,"getUnitLabelInt",getUnitLabelInt);
	lua_register(L,"getUnitAt",getUnitAt);
	lua_register(L,"getUnitPos",getUnitPos);
	lua_register(L,"getUnitX",getUnitX);
	lua_register(L,"getUnitY",getUnitY);
	lua_register(L,"getUnitLife",getUnitLife);
	lua_register(L,"getUnitMaxLife",getUnitMaxLife);
	lua_register(L,"getUnitAttack",getUnitAttack);
	lua_register(L,"getUnitBaseAttack",getUnitBaseAttack);
	lua_register(L,"getUnitAttackRange",getUnitAttackRange);
	lua_register(L,"getUnitBaseAttackRange",getUnitBaseAttackRange);
	lua_register(L,"getUnitAbilityCount",getUnitAbilityCount);
	lua_register(L,"getUnitMovePoints",getUnitMovePoints);
	lua_register(L,"getUnitOwner",getUnitOwner);
	lua_register(L,"getUnitTimedLife",setUnitTimedLife);
	lua_register(L,"getUnitBuffCount",getUnitBuffCount);
	lua_register(L,"isUnitAvatar",isUnitAvatar);
	lua_register(L,"unitHasBuff",unitHasBuff);
	lua_register(L,"canUnitAttack",canUnitAttack);
	lua_register(L,"canUnitMove",canUnitMove);
	lua_register(L,"canUnitCast",canUnitCast);
	lua_register(L,"canUnitReady",canUnitReady);
	lua_register(L,"getBuffDuration",getBuffDuration);
	lua_register(L,"buffIsDispellable",buffIsDispellable);
	lua_register(L,"getSpellCurrentCooldown",getSpellCurrentCooldown);
	lua_register(L,"getTileType",getTileType);
	lua_register(L,"tileIsOccupied",tileIsOccupied);
	lua_register(L,"getPlayerMana",getPlayerMana);
	lua_register(L,"getPlayerManaCap",getPlayerManaCap);
	lua_register(L,"getTurn",getTurn);
	lua_register(L,"getCastingSpellId",getCastingSpellId);
	lua_register(L,"getCastingUnitId",getCastingUnitId);
}