package.path = package.path .. ';./?.lua;'
require("Common")

loadUnits = function()
	loadunit("Avatar")

	--Defenders Of Ankharia
	loadunit("IronmanMilitia")
	loadunit("DragonwindWarrior")
	loadunit("StarlightCrusader")
	loadunit("SilverhornCavalier")
	loadunit("Faithbringer")
	loadunit("Lifebringer")

	--Defilers Of Ankharia
	loadunit("AbyssZombie")
	loadunit("CavernZombie")
	loadunit("ToxicSkeleton")
	loadunit("RenegadeTemplar")
	loadunit("RavenclawAssassin")
	loadunit("HarvestorOfSouls")
	loadunit("DarkWanderer")
	loadunit("Devourer")

	--Machinima
	loadunit("IronSoldier")
	loadunit("SuicideDrone")
	loadunit("LandMine")

	--Infidels Of Hell
	loadunit("Deathbringer")
	loadunit("Warbringer")
	loadunit("SpectralLich")

	--Magicka
	loadunit("FlarecloakMage")
	loadunit("ClericOfTheMist")
	loadunit("ShadowMystic")
	loadunit("Arcanist")
	loadunit("FluxCapacitor")

	loadunit("RockGiant")
	loadunit("MoltenGiant")
	loadunit("MinotaurTerraformer")
	loadunit("LightningBeast")
	loadunit("LightningSpirit")
end

Avatar = {
	name = "Avatar",
	text = "",

	life = 20,
	attack = 0,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)   --Avatar cannot attack
		m_cantAttack(unitid)
	end
}

IronmanMilitia = {
	name = "Ironman Militia",
	text = "Armor 1 (All incoming damage is reduced by 1)",

	life = 2,
	attack = 2,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)
		m_Armor(unitid,1)
	end
}

DragonwindWarrior = {
	name = "Dragonwind Warrior",
	text = "Armor 1 (All incoming damage is reduced by 1)\nCharging Attack (This unit gets +1 attack until the end of the turn after moving)",

	life = 3,
	attack = 2,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)
		m_Armor(unitid,1)
		if(getMessageString("msgtype")=="post unitmove") then
			if(getMessageInt("unit")==unitid) then
				addBuff("Strength",unitid,1,unitid,false)
			end
		end
	end
}

SilverhornCavalier = {
	name = "Silverhorn Cavalier",
	text = "Armor 1 (All incoming damage is reduced by 1)",

	life = 5,
	attack = 2,
	attackRange = 1,

	abilityCount = 1,

	ability1 = {
		id = "Armor",
		name = "Armor Up",
		text = "Target unit gets +1 Armor for 4 turns.",

		range = 1,
		moveCost = 2,
		targetCount = 1,
		cooldown = 2,

		OnCast = function(casterid,spellid,x,y)
			addBuff("Armor",getUnitAt(x,y),4,casterid,false)
		end,

		CheckTarget = function(casterid,spellid,x,y)
			if(tileIsOccupied(x,y)==1) then
				return 1
			else
				return 0
			end
		end
	},

	HandleMessage = function(unitid)
		m_Armor(unitid,1)
	end
}

StarlightCrusader = {
	name = "Starlight Crusader",
	text = "Armor 1 (All incoming damage is reduced by 1)",

	life = 4,
	attack = 4,
	attackRange = 1,

	abilityCount = 1,

	ability1 = {
		id = "Strength",
		name = "Crusader's Spirit",
		text = "Bolsters the target's spirit, making it deal 2 more damage until the end of the turn.",

		range = 1,
		moveCost = 2,
		targetCount = 1,
		cooldown = 2,

		OnCast = function(casterid,spellid,x,y)
			addBuff("Strength 2",getUnitAt(x,y),1,casterid,0)
		end,

		CheckTarget = function(casterid,spellid,x,y)
			if(tileIsOccupied(x,y)==1) then
				return 1
			else
				return 0
			end
		end
	},

	HandleMessage = function(unitid)
		m_Armor(unitid,1)
	end
}

Faithbringer = {
	name = "Faithbringer",
	text = "",

	life = 7,
	attack = 4,
	attackRange = 1,

	abilityCount = 2,

	ability1 = {
		id = "HolyLight",
		name = "Holy Light",
		text = "Heals target by 3.",

		range = 2,
		moveCost = 2,
		targetCount = 1,
		cooldown = 1,

		OnCast = function(casterid,spellid,x,y)
			unitid = getUnitAt(x,y)
			healUnit(casterid,unitid,3)
			playAnimation("Heal3","Heal",x,y,1)
		end,

		CheckTarget = function(casterid,spellid,x,y)
			if(tileIsOccupied(x,y)==1) then
				return 1
			else
				return 0
			end
		end
	},

	ability2 = {
		id = "Faith",
		name = "Faith",
		text = "Dispels all buffs from target unit.",

		range = 2,
		moveCost = 2,
		targetCount = 1,
		cooldown = 1,

		OnCast = function(casterid,spellid,x,y)
			unitid = getUnitAt(x,y)
			dispelBuffs(unitid)
			playAnimation("Heal4","Dispel",x,y,1)
		end,

		CheckTarget = function(casterid,spellid,x,y)
			if(tileIsOccupied(x,y)==1) then
				return 1
			else
				return 0
			end
		end
	},

	HandleMessage = function(unitid)
	end
}

Lifebringer = {
	name = "Lifebringer",
	text = "Battlecry - Heals adjacent allied units by 4.",

	life = 7,
	attack = 4,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)
		func = function(unitid)
			func2 = function(unit,x,y,range,caster,targetflags)
				healUnit(caster,unit,4)
			end
			cx = getUnitX(unitid)
			cy = getUnitY(unitid)
			f_doUnitArea(cx,cy,1,unitid,TARGET_ALLY+TARGET_NONSELF,func2)
			playAnimation("Heal3","Heal",cx-1,cy-1,3)
		end
		m_Battlecry(unitid,func)
	end
}

AbyssZombie = {
	name = "Abyss Zombie",
	text = "",

	life = 1,
	attack = 1,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)
	end
}

CavernZombie = {
	name = "Cavern Zombie",
	text = "Poisonous Body (When this unit receives damage, the damager is poisoned for 4 turns.)",

	life = 3,
	attack = 1,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)
		if(getMessageString("msgtype")=="pre unitdamage") then
			if(getMessageInt("target")==unitid) then
				addBuff("Poison",getMessageInt("source"),4,unitid,false)
			end
		end
	end
}

ToxicSkeleton = {
	name = "Toxic Skeleton",
	text = "Poisonous Attack (When this unit deals damage, the target is poisoned for 4 turn)",

	life = 2,
	attack = 2,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)
		if(getMessageString("msgtype")=="post unitdamage") then
			if(getMessageInt("source")==unitid) then
				if(getMessageInt("damagetype")==DAMAGETYPE_ATTACK) then
					addBuff("Poison",getMessageInt("target"),4,unitid,false)
				end
			end
		end
	end
}

RenegadeTemplar = {
	name = "Renegade Templar",
	text = "This unit deals +2 physical damage to units that have less life than this unit.",

	life = 6,
	attack = 4,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)
		if(getMessageString("msgtype")=="mod unitdamage") then
			if(getMessageInt("source")==unitid) then
				target = getMessageInt("target")
				if(getUnitLife(target) < getUnitLife(unitid)) then
					if(getMessageInt("damagetype")==DAMAGETYPE_ATTACK) then
						setMessageInt("amount",getMessageInt("amount")+2)
					end
				end
			end
		end
	end
}

RavenclawAssassin = {
	name = "Ravenclaw Assassin",
	text = "This unit deals +2 physical damage against injured units.",

	life = 4,
	attack = 2,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)
		if(getMessageString("msgtype")=="mod unitdamage") then
			if(getMessageInt("source")==unitid) then
				target = getMessageInt("target")
				if(getUnitLife(target) < getUnitMaxLife(target)) then
					if(getMessageInt("damagetype")==DAMAGETYPE_ATTACK) then
						setMessageInt("amount",getMessageInt("amount")+2)
					end
				end
			end
		end
	end
}

IronSoldier = {
	name = "Iron Soldier",
	text = "",

	life = 3,
	attack = 4,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)
	end
}

SuicideDrone = {
	name = "Suicide Drone",
	text = "w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w w ww w w ww www www w ww",

	life = 2,
	attack = 0,
	attackRange = 1,

	abilityCount = 2,

	ability1 = {
		id = "SuicideBomb",
		name = "Suicide Bomb",
		text = "The Suicide Drone blows itself up dealing 2 damage to nearby enemy units.",

		range = 1,
		moveCost = 2,
		targetCount = 0,
		cooldown = 1,

		OnCast = function(casterid,spellid,x,y)
			destroyUnit(unitid)
		end,

		CheckTarget = function(casterid,spellid,x,y)
			if(tileIsOccupied(x,y)==1) then
				return 1
			else
				return 0
			end
		end
	},

	ability2 = {
		id = "Blink",
		name = "Blink",
		text = "Instantly teleports to target unoccupied location.",

		range = 2,
		moveCost = 2,
		targetCount = 1,
		cooldown = 2,

		OnCast = function(casterid,spellid,x,y)
			unitid = casterid
			teleportUnit(unitid,x,y)
		end,

		CheckTarget = function(casterid,spellid,x,y)
			if(tileIsOccupied(x,y)==0) then
				return 1
			else
				return 0
			end
		end
	},

	HandleMessage = function(unitid)
		m_cantAttack(unitid)
		m_explodeOnDeath(unitid,1,2)
	end
}

LandMine = {
	name = "Land Mine",
	text = "Whenever an enemy unit moves adjacent to this unit, this unit explodes and deals 2 damage to nearby enemy units.",

	life = 2,
	attack = 0,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)
		if(getMessageString("msgtype")=="post unitmove") then
			if(getUnitOwner(getMessageInt("unit"))~=getUnitOwner(unitid)) then
				x1,y1 = getUnitPos(unitid)
				x2,y2 = getUnitPos(getMessageInt("unit"))
				if(misc_getChebyshevDistance(x1,y1,x2,y2)<=1) then
					destroyUnit(unitid)
				end
			end
		end
		m_explodeOnDeath(unitid,1,2)
		m_cantMove(unitid)
		m_cantAttack(unitid)
	end
}

HarvestorOfSouls = {
	name = "Harvestor Of Souls",
	text = "Whenever a unit dies within 2 range of this unit, this unit harvests it's soul and gains 1 attack.",

	life = 4,
	attack = 1,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)
		if(getMessageString("msgtype")=="post unitdeath") then
			x1,y1 = getUnitPos(unitid)
			x2,y2 = getUnitPos(getMessageInt("unit"))
			if(misc_getChebyshevDistance(x1,y1,x2,y2)<=2) then
				--setUnitLabelInt(unitid,"souls",getUnitLabelInt(unitid,"souls")+1)
				setUnitBaseAttack(unitid,getUnitBaseAttack(unitid)+1)
			end
		end
	end
}

DarkWanderer = {
	name = "Dark Wanderer",
	text = "",

	life = 3,
	attack = 3,
	attackRange = 1,

	abilityCount = 1,

	ability1 = {
		id = "Weakness",
		name = "Weaken",
		text = "Weakens the target making it deal 2 less damage. Lasts 4 turns.",

		range = 2,
		moveCost = 2,
		targetCount = 1,
		cooldown = 1,

		OnCast = function(casterid,spellid,x,y)
			addBuff("Weakness2",getUnitAt(x,y),4,casterid,0)
		end,

		CheckTarget = function(casterid,spellid,x,y)
			if(tileIsOccupied(x,y)==1) then
				return 1
			else
				return 0
			end
		end
	},

	HandleMessage = function(unitid)
	end
}

Devourer = {
	name = "Devourer",
	text = "",

	life = 6,
	attack = 1,
	attackRange = 1,

	abilityCount = 1,

	ability1 = {
		id = "Devour",
		name = "Devour",
		text = "Devours target allied unit. Gains attack equal to that unit's life. Cannot gain more than 4 attack per unit.",

		range = 1,
		moveCost = 2,
		targetCount = 1,
		cooldown = 1,

		OnCast = function(casterid,spellid,x,y)
			unitid = getUnitAt(x,y)
			caster = casterid
			life = getUnitLife(unitid)
			if(life>4) then
				life = 4
			end
			--setUnitLabelInt(caster,"devour",getUnitLabelInt(caster,"devour")+life)
			setUnitBaseAttack(caster,getUnitBaseAttack(caster)+life)
			destroyUnit(unitid)
		end,

		CheckTarget = function(casterid,spellid,x,y)
			if(tileIsOccupied(x,y)==1) then
				if(getUnitOwner(getUnitAt(x,y))==getUnitOwner(casterid)) then
					return 1
				else
					return 0
				end
			else
				return 0
			end
		end
	},

	HandleMessage = function(unitid)
	end
}

Deathbringer = {
	name = "Deathbringer",
	text = "Battlecry - Deals 2 damage to adjacent enemies.",

	life = 5,
	attack = 4,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)
		func = function(unitid)
			func2 = function(unit,x,y,range,caster,targetflags)
				dealDamage(caster,unit,2,DAMAGETYPE_SPELL)
			end
			cx = getUnitX(unitid)
			cy = getUnitY(unitid)
			f_doUnitArea(cx,cy,1,unitid,TARGET_ENEMY+TARGET_NONSELF,func2)
			playAnimation("Fire2","Fire",cx-1,cy-1,3)
		end
		m_Battlecry(unitid,func)
	end
}

Warbringer = {
	name = "Warbringer",
	text = "Battlecry - Adjacent allied units get +2 attack until the end of the turn.",

	life = 6,
	attack = 6,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)
		func = function(unitid)
			func2 = function(unit,x,y,range,caster,targetflags)
				addBuff("Strength2",unit,1,caster,0)
			end
			cx = getUnitX(unitid)
			cy = getUnitY(unitid)
			f_doUnitArea(cx,cy,1,unitid,TARGET_ALLY+TARGET_NONSELF,func2)
			playAnimation("Special1","Boost",cx-1,cy-1,3)
		end
		m_Battlecry(unitid,func)
	end
}

SpectralLich = {
	name = "Spectral Lich",
	text = "Battlecry - Adjacent enemies are frozen for 2 turns.",

	life = 5,
	attack = 5,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)
		func = function(unitid)
			func2 = function(unit,x,y,range,caster,targetflags)
				addBuff("Frozen",unit,2,caster,0)
			end
			cx = getUnitX(unitid)
			cy = getUnitY(unitid)
			f_doUnitArea(cx,cy,1,unitid,TARGET_ENEMY+TARGET_NONSELF,func2)
			playAnimation("Ice2","Boost",cx-1,cy-1,3)
		end
		m_Battlecry(unitid,func)
	end
}

FlarecloakMage = {
	name = "FlarecloakMage",
	text = "",

	life = 4,
	attack = 3,
	attackRange = 2,

	abilityCount = 1,

	ability1 = {
		id = "ArcaneBolt",
		name = "Arcane Bolt",
		text = "Deals damage equal to half of your mana rounded down, for a maximum of 8 damage.",

		range = 2,
		moveCost = 2,
		targetCount = 1,
		cooldown = 2,

		OnCast = function(casterid,spellid,x,y)
			unitid = getUnitAt(x,y)
			caster = casterid
			mana = getPlayerMana(getUnitOwner(caster))
			if(mana>16) then
				dealDamage(caster,unitid,8,DAMAGETYPE_SPELL)
			else
				dealDamage(caster,unitid,mana/2,DAMAGETYPE_SPELL)
			end
		end,

		CheckTarget = function(casterid,spellid,x,y)
			if(tileIsOccupied(x,y)==1) then
				return 1
			else
				return 0
			end
		end
	},

	HandleMessage = function(unitid)
	end
}

ClericOfTheMist = {
	name = "Cleric Of The Mist",
	text = "",

	life = 4,
	attack = 2,
	attackRange = 1,

	abilityCount = 1,

	ability1 = {
		id = "Silence",
		name = "Silence",
		text = "Silences target non-avatar unit for 4 turns.",

		range = 2,
		moveCost = 2,
		targetCount = 1,
		cooldown = 2,

		OnCast = function(casterid,spellid,x,y)
			uid = getUnitAt(x,y)
			addBuff("Silence",uid,4,casterid,false)
			playAnimation("State3","Silence",x,y,1)
		end,

		CheckTarget = function(casterid,spellid,x,y)
			if(tileIsOccupied(x,y)==1) then
				if(isUnitAvatar(getUnitAt(x,y))==0) then
					return 1
				else
					return 0
				end
			else
				return 0
			end
		end
	},

	HandleMessage = function(unitid)
	end
}

ShadowMystic = {
	name = "Shadow Mystic",
	text = "Whenever a allied unit within 2 range of this unit casts a spell, it gets healed by 2.",

	life = 5,
	attack = 2,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)
		if(getMessageString("msgtype")=="post unitcast") then
			caster = getMessageString("caster")
			if(misc_getChebyshevDistance(getUnitX(caster),getUnitY(caster),getUnitX(unitid),getUnitY(unitid)) <= 2) then
				if(getUnitOwner(caster)==getUnitOwner(unitid)) then
					healUnit(unitid,caster,2)
					playAnimation("Heal3","Heal",getUnitX(caster),getUnitY(caster),1)
				end
			end
		end
	end
}

Arcanist = {
	name = "Arcanist",
	text = "Whenever a unit within 1 range of this unit casts a spell, this unit's attack increases by 1.",

	life = 3,
	attack = 4,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)
		if(getMessageString("msgtype")=="post unitcast") then
			caster = getMessageString("caster")
			if(misc_getChebyshevDistance(getUnitX(caster),getUnitY(caster),getUnitX(unitid),getUnitY(unitid)) <= 1) then
				setUnitBaseAttack(unitid,getUnitBaseAttack(unitid)+1)
			end
		end
	end
}

FluxCapacitor = {
	name = "Flux Capacitor",
	text = "This unit can't move or attack.\nWhenever an enemy unit within 2 range of this unit casts a spell, it takes 2 damage.",

	life = 5,
	attack = 0,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)
		if(getMessageString("msgtype")=="post unitcast") then
			caster = getMessageString("caster")
			if(misc_getChebyshevDistance(getUnitX(caster),getUnitY(caster),getUnitX(unitid),getUnitY(unitid)) <= 2) then
				if(getUnitOwner(caster)~=getUnitOwner(unitid)) then
					dealDamage(unitid,caster,2,DAMAGETYPE_SPELL)
					playAnimation("Thunder2","Shock",getUnitX(caster),getUnitY(caster),1)
				end
			end
		end
		m_cantAttack(unitid)
		m_cantMove(unitid)
	end
}

MinotaurTerraformer = {
	name = "Minotaur Terraformer",
	text = "",

	life = 6,
	attack = 5,
	attackRange = 1,

	abilityCount = 1,

	ability1 = {
		id = "Terraform",
		name = "Terraform",
		text = "Creates a Mountain at target unoccupied location.",

		range = 1,
		moveCost = 6,
		targetCount = 1,
		cooldown = 1,

		OnCast = function(casterid,spellid,x,y)
			changeTileType(x,y,TILE_MOUNTAIN)
			playAnimation("Special6","Summon",x,y,1)
		end,

		CheckTarget = function(casterid,spellid,x,y)
			if(getTileType(x,y)~=TILE_MOUNTAIN) then
				if(isTileOccupied(x,y)==0) then
					return 1
				else
					return 0
				end
			else
				return 0
			end
		end
	},

	HandleMessage = function(unitid)
	end
}

FlamebiteCobra = {
	name = "FlamebiteCobra",
	text = "Burning Attack - This unit Burns the target it attacks for 2 turns.",

	life = 4,
	attack = 3,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)
		if(getMessageString("msgtype")=="post unitdamage") then
			if(getMessageInt("source")==unitid) then
				if(getMessageInt("damagetype")==DAMAGETYPE_ATTACK) then
					addBuff("Burn",getMessageInt("target"),2,unitid,false)
				end
			end
		end
	end
}

RockGiant = {
	name = "Rock Giant",
	text = "",

	life = 6,
	attack = 5,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)
	end
}

MoltenGiant = {
	name = "Molten Giant",
	text = "Molten Body (When this unit recieves damage, it amplifies all damage recieved by the damager by 1 for 2 turns.)\nMolten Attack(When this unit deals damage, it amplifies all damage recieved by the target by 1 for 2 turns.)",

	life = 6,
	attack = 5,
	attackRange = 1,

	abilityCount = 0,

	HandleMessage = function(unitid)
		if(getMessageString("msgtype")=="pre unitdamage") then
			if(getMessageInt("target")==unitid) then
				if(getMessageInt("damagetype")==DAMAGETYPE_ATTACK) then
					addBuff("MoltenArmor",getMessageInt("source"),2,unitid,false)
				end
			end
		end
		if(getMessageString("msgtype")=="post unitdamage") then
			if(getMessageInt("source")==unitid) then
				if(getMessageInt("damagetype")==DAMAGETYPE_ATTACK) then
					addBuff("MoltenArmor",getMessageInt("target"),2,unitid,false)
				end
			end
		end
	end
}

LightningBeast = {
	name = "Lightning Beast",
	text = "",

	life = 3,
	attack = 2,
	attackRange = 3,

	abilityCount = 0,

	HandleMessage = function(unitid)
	end
}

LightningSpirit = {
	name = "Lightning Spirit",
	text = "Fading 5 (This unit will die after 5 turns)",

	life = 3,
	attack = 2,
	attackRange = 3,

	abilityCount = 0,

	HandleMessage = function(unitid)
		if(getMessageString("msgtype")=="post unitsummon") then
			if(getMessageInt("unitid")==unitid) then
				setUnitTimedLife(unitid,5)
			end
		end
	end
}
