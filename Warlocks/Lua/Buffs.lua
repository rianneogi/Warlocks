package.path = package.path .. ';./?.lua;'
require("Common")

loadBuffs = function()
	loadbuff("Paralyze")
	loadbuff("Silence")
	loadbuff("Disarm")
	loadbuff("Exhaust")
	loadbuff("Burn")
	loadbuff("Poison")
	loadbuff("Frost")
	loadbuff("Frozen")
	loadbuff("Strength")
	loadbuff("Strength2")
	loadbuff("Weakness")
	loadbuff("Weakness2")
	loadbuff("Armor")
	loadbuff("Armor2")
	loadbuff("MoltenArmor")
	loadbuff("MoltenArmor2")
	loadbuff("Rupture")
end

Paralyze = {
	name = "Paralyze",
	text = "This unit cannot move.",

	HandleMessage = function(unitid)
		m_cantMove(unitid)
	end
}

Silence = {
	name = "Silence",
	text = "This unit cannot cast spells or abilities.",

	HandleMessage = function(unitid)
		m_cantCast(unitid)
	end
}

Disarm = {
	name = "Disarm",
	text = "This unit cannot attack.",

	HandleMessage = function(unitid)
		m_cantAttack(unitid)
	end
}

Exhaust = {
	name = "Exhaust",
	text = "This unit does not ready at the start of its turn.",

	HandleMessage = function(unitid)
		if(getMessageString("msgtype")=="get unitcanready") then
			if(getMessageInt("unit")==untitid) then
				setMessageInt("canready",0)
			end
		end
	end
}

Burn = {
	name = "Burn",
	text = "This unit takes 2 damage at the end of every turn.",

	HandleMessage = function(unitid)
		if(getMessageString("msgtype")=="post endturn") then
			if(getMessageInt("player")==getUnitOwner(untitid)) then
				dealDamage(unitid,unitid,2,DAMAGETYPE_SPECIAL)
			end
		end
	end
}

Poison = {
	name = "Poison",
	text = "This unit takes 1 damage at the start of its turn.",

	HandleMessage = function(unitid)
		if(getMessageString("msgtype")=="post unitready") then
			if(getMessageInt("unit")==unitid) then
				dealDamage(unitid,unitid,1,DAMAGETYPE_SPECIAL)
			end
		end
	end
}

Frozen = {
	name = "Frozen",
	text = "This unit can't move, attack or cast.",

	HandleMessage = function(unitid)
		m_cantAttack(unitid)
		m_cantMove(unitid)
		m_cantCast(unitid)
	end
}

Frost = {
	name = "Frost",
	text = "Moving costs double move points for this unit.",

	HandleMessage = function(unitid)
		if(getMessageString("msgtype")=="get unittileentercost") then
			if(getMessageInt("unit")==unitid) then
				setMessageInt("entercost",getMessageInt("entercost")*2)
			end
		end
		if(getMessageString("msgtype")=="get unittileexitcost") then
			if(getMessageInt("unit")==unitid) then
				setMessageInt("exitcost",getMessageInt("exitcost")*2)
			end
		end
	end
}

Strength = {
	name = "Strength",
	text = "This unit has +1 attack.",

	HandleMessage = function(unitid)
		m_plusAttack(unitid,1)
	end
}

Strength2 = {
	name = "Strength 2",
	text = "This unit has +2 attack.",

	HandleMessage = function(unitid)
		m_plusAttack(unitid,2)
	end
}

Weakness = {
	name = "Weakness",
	text = "This unit has -1 attack.",

	HandleMessage = function(unitid)
		m_minusAttack(unitid,1)
	end
}

Weakness2 = {
	name = "Weakness 2",
	text = "This unit has -2 attack.",

	HandleMessage = function(unitid)
		m_minusAttack(unitid,2)
	end
}

Armor = {
	name = "Armor",
	text = "All damage recieved by this unit is decreased by 1.",

	HandleMessage = function(unitid)
		m_Armor(unitid,1)
	end
}

Armor2 = {
	name = "Armor 2",
	text = "All damage recieved by this unit is decreased by 2.",

	HandleMessage = function(unitid)
		m_Armor(unitid,2)
	end
}

MoltenArmor = {
	name = "Molten Armor",
	text = "All damage recieved by this unit is increased by 1.",

	HandleMessage = function(unitid)
		m_minusArmor(unitid,1)
	end
}

MoltenArmor2 = {
	name = "Molten Armor 2",
	text = "All damage recieved by this unit is increased by 2.",

	HandleMessage = function(unitid)
		m_minusArmor(unitid,2)
	end
}

Rupture = {
	name = "Rupture",
	text = "Whenever this unit moves, it takes 2 damage.",

	HandleMessage = function(unitid)
		if(getMessageString("msgtype")=="post unitmove") then
			if(getMessageInt("unit")==unitid) then
				dealDamage(unitid,unitid,2,DAMAGETYPE_SPECIAL)
			end
		end
	end
}
