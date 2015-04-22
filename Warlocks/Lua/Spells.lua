package.path = package.path .. ';./?.lua;'
require("Common")

--load spells
loadSpells = function()
	--Base
	loadspell("FireBolt")
	loadspell("LightningStrike")
	loadspell("BruteStrength")
	loadspell("HealingTouch")
	loadspell("Paralyze")
	loadspell("Silence")
	loadspell("Disarm")
	loadspell("Dispel")
	loadspell("Poison")
	loadspell("Exhuast")

	loadspell("FlameHammer")
	loadspell("Sprout")
	loadspell("FinishingBlow")
	loadspell("FrostHammer")

	--Defenders Of Ankharia
	loadsummon("IronmanMilitia")
	loadsummon("DragonwindWarrior")
	loadsummon("StarlightCrusader")
	loadsummon("SilverhornCavalier")
	loadsummon("Faithbringer")
	loadsummon("Lifebringer")

	--Defilers Of Ankharia
	loadsummon("AbyssZombie")
	loadsummon("CavernZombie")
	loadsummon("ToxicSkeleton")
	loadsummon("RenegadeTemplar")
	loadsummon("RavenclawAssassin")
	loadsummon("HarvestorOfSouls")
	loadsummon("Devourer")
	loadsummon("DarkWanderer")

	--Machinima
	loadspell("IronFistSmash")
	loadspell("Explode")
	loadsummon("SuicideDrone")
	loadsummon("IronSoldier")
	loadsummon("LandMine")

	--Infidels Of Hell
	loadspell("Rupture")
	loadsummon("Deathbringer")
	loadsummon("Warbringer")
	loadsummon("SpectralLich")

	--Magicka
	loadspell("ManaEssence")
	loadsummon("FlarecloakMage")
	loadsummon("ClericOfTheMist")
	loadsummon("ShadowMystic")
	loadsummon("Arcanist")
	loadsummon("FluxCapacitor")

	loadsummon("MoltenGiant")
	loadsummon("RockGiant")
	loadsummon("MinotaurTerraformer")
	loadsummon("LightningBeast")
	loadsummon("LightningSpirit")
end

--SPELLS

FireBolt = {
	manaCost = 1,
	range = 2,
	targetCount = 1,
	moveCost = 2,
	cooldown = 0,
	name = "Fire Bolt",
	text = "Deals 1 damage to target.",

	OnCast = function(casterid,spellid,x,y)
		uid = getUnitAt(x,y)
		dealDamage(casterid,uid,1,DAMAGETYPE_SPELL)
		playAnimation("Fire2","Fire",x,y,1)
	end,

	CheckTarget = function(casterid,spellid,x,y)
		if(tileIsOccupied(x,y)==1) then
			return 1
		else
			return 0
		end
	end
}

LightningStrike = {
	manaCost = 3,
	range = 3,
	targetCount = 1,
	moveCost = 2,
	cooldown = 2,
	name = "Lightning Strike",
	text = "Deals 3 damage to target.",

	OnCast = function(casterid,spellid,x,y)
		uid = getUnitAt(x,y)
		dealDamage(casterid,uid,3,DAMAGETYPE_SPELL)
		playAnimation("Thunder1","LightningBolt",x,y,1)
	end,

	CheckTarget = function(casterid,spellid,x,y)
		if(tileIsOccupied(x,y)==1) then
			return 1
		else
			return 0
		end
	end
}

BruteStrength = {
	manaCost = 2,
	range = 2,
	targetCount = 1,
	moveCost = 2,
	cooldown = 2,
	name = "Brute Strength",
	text = "Applies Strength 2 to target for 4 turns. (Damage dealt by target is increased by 2)",

	OnCast = function(casterid,spellid,x,y)
		uid = getUnitAt(x,y)
		addBuff("Strength2",uid,4,casterid,false)
		playAnimation("Special1","Boost",x,y,1)
	end,

	CheckTarget = function(casterid,spellid,x,y)
		if(tileIsOccupied(x,y)==1) then
			return 1
		else
			return 0
		end
	end
}

HealingTouch = {
	manaCost = 1,
	range = 2,
	targetCount = 1,
	moveCost = 2,
	cooldown = 2,
	name = "Healing Touch",
	text = "Heals target by 3.",

	OnCast = function(casterid,spellid,x,y)
		uid = getUnitAt(x,y)
		healUnit(casterid,uid,3)
		playAnimation("Heal3","Heal",x,y,1)
	end,

	CheckTarget = function(casterid,spellid,x,y)
		if(tileIsOccupied(x,y)==1) then
			return 1
		else
			return 0
		end
	end
}

Paralyze = {
	manaCost = 2,
	range = 2,
	targetCount = 1,
	moveCost = 2,
	cooldown = 2,
	name = "Paralyze",
	text = "Applies Paralyze to target for 4 turns. (Paralyzed units can't move)",

	OnCast = function(casterid,spellid,x,y)
		uid = getUnitAt(x,y)
		addBuff("Paralyze",uid,4,casterid,false)
		playAnimation("State6","Boost",x,y,1)
	end,

	CheckTarget = function(casterid,spellid,x,y)
		if(tileIsOccupied(x,y)==1) then
			return 1
		else
			return 0
		end
	end
}

Silence = {
	manaCost = 2,
	range = 2,
	targetCount = 1,
	moveCost = 2,
	cooldown = 2,
	name = "Silence",
	text = "Applies Silence to target for 4 turns. (Silenced units can't cast spells or abilities)",

	OnCast = function(casterid,spellid,x,y)
		uid = getUnitAt(x,y)
		addBuff("Silence",uid,4,casterid,false)
		playAnimation("State3","Silence",x,y,1)
	end,

	CheckTarget = function(casterid,spellid,x,y)
		if(tileIsOccupied(x,y)==1) then
			return 1
		else
			return 0
		end
	end
}

Disarm = {
	manaCost = 2,
	range = 2,
	targetCount = 1,
	moveCost = 2,
	cooldown = 2,
	name = "Disarm",
	text = "Applies Disarm to target for 4 turns. (Disarmed units can't attack)",

	OnCast = function(casterid,spellid,x,y)
		uid = getUnitAt(x,y)
		addBuff("Disarm",uid,4,casterid,false)
		playAnimation("State1","Disarm",x,y,1)
	end,

	CheckTarget = function(casterid,spellid,x,y)
		if(tileIsOccupied(x,y)==1) then
			return 1
		else
			return 0
		end
	end
}

Dispel = {
	manaCost = 3,
	range = 2,
	targetCount = 1,
	moveCost = 2,
	cooldown = 2,
	name = "Dispel",
	text = "Removes all negative buffs from the target.",

	OnCast = function(casterid,spellid,x,y)
		uid = getUnitAt(x,y)
		dispelBuffs(uid)
		playAnimation("Heal4","Dispel",x,y,1)
	end,

	CheckTarget = function(casterid,spellid,x,y)
		if(tileIsOccupied(x,y)==1) then
			return 1
		else
			return 0
		end
	end
}

Poison = {
	manaCost = 1,
	range = 2,
	targetCount = 1,
	moveCost = 2,
	cooldown = 2,
	name = "Poison",
	text = "Applies Poison to target for 4 turns. (Poisoned units take 1 damage at the start of their turn)",

	OnCast = function(casterid,spellid,x,y)
		uid = getUnitAt(x,y)
		addBuff("Poison",uid,4,casterid,false)
		playAnimation("State1","Poison",x,y,1)
	end,

	CheckTarget = function(casterid,spellid,x,y)
		if(tileIsOccupied(x,y)==1) then
			return 1
		else
			return 0
		end
	end
}

Exhaust = {
	manaCost = 1,
	range = 2,
	targetCount = 1,
	moveCost = 2,
	cooldown = 2,
	name = "Exhaust",
	text = "Applies Exhaust to target for 4 turns. (Exhausted units can't ready)",

	OnCast = function(casterid,spellid,x,y)
		uid = getUnitAt(x,y)
		addBuff("Exhaust",uid,4,casterid,false)
		playAnimation("Special1","Boost",x,y,1)
	end,

	CheckTarget = function(casterid,spellid,x,y)
		if(tileIsOccupied(x,y)==1) then
			return 1
		else
			return 0
		end
	end
}

FlameHammer = {
	manaCost = 2,
	range = 2,
	targetCount = 1,
	moveCost = 2,
	cooldown = 2,
	name = "Flame Hammer",
	text = "Deals 2 damage. Target takes an additional 2 damage at the end of every turn.\nLasts 2 turns.",

	OnCast = function(casterid,spellid,x,y)
		uid = getUnitAt(x,y)
		dealDamage(casterid,uid,2,DAMAGETYPE_SPELL)
		addBuff("Burn",uid,2,casterid,false)
	end,

	CheckTarget = function(casterid,spellid,x,y)
		if(tileIsOccupied(x,y)==1) then
			return 1
		else
			return 0
		end
	end
}

IronFistSmash = {
	manaCost = 2,
	range = 1,
	targetCount = 1,
	moveCost = 2,
	cooldown = 2,
	name = "Iron Fist Smash",
	text = "Deals 4 damage to target.",

	OnCast = function(casterid,spellid,x,y)
		dealDamage(casterid,getUnitAt(x,y),4,DAMAGETYPE_SPELL)
		playAnimation("Attack1","Attack",x,y,1)
	end,

	CheckTarget = function(casterid,spellid,x,y)
		if(tileIsOccupied(x,y)==1) then
			return 1
		else
			return 0
		end
	end
}

Explode = {
	manaCost = 2,
	range = 2,
	targetCount = 1,
	moveCost = 2,
	cooldown = 1,
	name = "Explode",
	text = "Destroys target allied unit and deals 2 damage to surrounding enemy units.",

	OnCast = function(casterid,spellid,x,y)
		caster = casterid
		unitid = getUnitAt(x,y)
		func = function(unit,x,y,range,caster,targetflags)
			dealDamage(caster,unit,2,DAMAGETYPE_SPELL)
		end
		cx = getUnitX(unitid)
		cy = getUnitY(unitid)
		f_doUnitArea(cx,cy,1,unitid,TARGET_ENEMY+TARGET_NONSELF,func)
		destroyUnit(unitid)
		playAnimation("Fire2","Fire",cx-1,cy-1,3)
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
}

FinishingBlow = {
	manaCost = 2,
	range = 1,
	targetCount = 1,
	moveCost = 2,
	cooldown = 2,
	name = "Finishing Blow",
	text = "Deals 1 damage. If the target is injured, deals 4 damage instead.",

	OnCast = function(casterid,spellid,x,y)
		uid = getUnitAt(x,y)
		if(getUnitLife(uid)<getUnitMaxLife(uid)) then
			dealDamage(casterid,getUnitAt(x,y),4,DAMAGETYPE_SPELL)
		else
			dealDamage(casterid,getUnitAt(x,y),1,DAMAGETYPE_SPELL)
		end
	end,

	CheckTarget = function(casterid,spellid,x,y)
		if(tileIsOccupied(x,y)==1) then
			return 1
		else
			return 0
		end
	end
}

FrostHammer = {
	manaCost = 2,
	range = 2,
	targetCount = 1,
	moveCost = 2,
	cooldown = 2,
	name = "Frost Hammer",
	text = "Deals 2 damage. Target takes double the amount of movepoints to move.\nLasts 2 turns.",

	OnCast = function(casterid,spellid,x,y)
		uid = getUnitAt(x,y)
		dealDamage(casterid,uid,2,DAMAGETYPE_SPELL)
		addBuff("Frost",uid,4,casterid,false)
	end,

	CheckTarget = function(casterid,spellid,x,y)
		if(tileIsOccupied(x,y)==1) then
			return 1
		else
			return 0
		end
	end
}

Sprout = {
	manaCost = 1,
	range = 2,
	targetCount = 1,
	moveCost = 2,
	cooldown = 1,
	name = "Sprout",
	text = "Converts target unoccupied terrain into Forest.",

	OnCast = function(casterid,spellid,x,y)
		changeTileType(x,y,TILE_FOREST)
		playAnimation("Special6","Summon",x,y,1)
	end,

	CheckTarget = function(casterid,spellid,x,y)
		if(getTileType(x,y)~=TILE_FOREST) then
			if(isTileOccupied(x,y)==0) then
				return 1
			else
				return 0
			end
		else
			return 0
		end
	end
}

Rupture = {
	manaCost = 2,
	range = 2,
	targetCount = 1,
	moveCost = 2,
	cooldown = 2,
	name = "Rupture",
	text = "Applies Rupture to target for 4 turns. (Ruptured units take 2 damage when they move)",

	OnCast = function(casterid,spellid,x,y)
		uid = getUnitAt(x,y)
		addBuff("Rupture",uid,4,casterid,false)
		playAnimation("Special3","Rupture",x,y,1)
	end,

	CheckTarget = function(casterid,spellid,x,y)
		if(tileIsOccupied(x,y)==1) then
			return 1
		else
			return 0
		end
	end
}

ManaEssence = {
	manaCost = 3,
	range = 1,
	targetCount = 0,
	moveCost = 2,
	cooldown = 3,
	name = "Essence Of Mana",
	text = "You get 6 mana to use until the end of the turn.",

	OnCast = function(casterid,spellid,x,y)
		setPlayerMana(getPlayerMana(getUnitOwner(casterid))+6)
	end,

	CheckTarget = function(casterid,spellid,x,y)
		if(tileIsOccupied(x,y)==1) then
			return 1
		else
			return 0
		end
	end
}

--summons

newSummon = function(unitName,spellMana,spellCD,spellRange,spellMove)
	tmpspell = {}
	tmpspell.manaCost = spellMana
	tmpspell.range = spellRange
	tmpspell.moveCost = spellMove
	tmpspell.cooldown = spellCD
	tmpspell.targetCount = 1
	tmpspell.name = unitName
	tmpspell.text = "Summons a unit"

	tmpspell.OnCast = function(casterid,spellid,x,y)
		summonUnit(unitName,x,y,getUnitOwner(casterid))
		playAnimation("Special6","Summon",x,y,1);
	end

	tmpspell.CheckTarget = function(casterid,spellid,x,y)
		if(tileIsOccupied(x,y)==0) then
			return 1
		else
			return 0
		end
	end

	return tmpspell
end

IronmanMilitia = newSummon("IronmanMilitia",2,2,1,2)
DragonwindWarrior = newSummon("DragonwindWarrior",3,3,1,2)
SilverhornCavalier = newSummon("SilverhornCavalier",4,4,1,2)
StarlightCrusader = newSummon("StarlightCrusader",5,5,1,2)
Faithbringer = newSummon("Faithbringer",6,6,1,2)
Lifebringer = newSummon("Lifebringer",6,6,1,2)

AbyssZombie = newSummon("AbyssZombie",1,0,1,2)
CavernZombie = newSummon("CavernZombie",2,2,1,2)
ToxicSkeleton = newSummon("ToxicSkeleton",2,2,1,2)
RenegadeTemplar = newSummon("RenegadeTemplar",5,5,1,2)
RavenclawAssassin = newSummon("RavenclawAssassin",3,3,1,2)
HarvestorOfSouls = newSummon("HarvestorOfSouls",3,2,1,2)
DarkWanderer = newSummon("DarkWanderer",3,3,1,2)
Devourer = newSummon("Devourer",5,5,1,2)

IronSoldier = newSummon("IronSoldier",3,3,1,2)
SuicideDrone = newSummon("SuicideDrone",2,2,1,2)
LandMine = newSummon("LandMine",2,1,1,2)

Deathbringer = newSummon("Deathbringer",5,5,1,2)
Warbringer = newSummon("Warbringer",7,7,1,2)
SpectralLich = newSummon("SpectralLich",6,6,1,2)

MoltenGiant = newSummon("MoltenGiant",6,6,1,2)
RockGiant = newSummon("RockGiant",5,5,1,2)

FlarecloakMage = newSummon("FlarecloakMage",4,4,1,2)
ClericOfTheMist = newSummon("ClericOfTheMist",3,3,1,2)
ShadowMystic = newSummon("ShadowMystic",4,4,1,2)
Arcanist = newSummon("Arcanist",4,4,1,2)
FluxCapacitor = newSummon("FluxCapacitor",3,3,1,2)

MinotaurTerraformer = newSummon("MinotaurTerraformer",6,6,1,2)
LightningBeast = newSummon("LightningBeast",3,3,1,2)
LightningSpirit = newSummon("LightningSpirit",2,2,1,2)
