#pragma once

#include <string>
#include "Spell.h"

extern std::deque<std::string> UnitNames;
extern std::deque<sf::Texture> UnitTextures;

class Unit
{
public:
	Unit();
	Unit(int id,int tilex,int tiley,int owner,int uniqueid);
	Unit(std::string idname,int x_,int y_,int owner,int uniqueid);
	~Unit();

	int UniqueId;
	int UnitType;
	int x;
	int y;
	int Life;
	int maxLife;
	int Attack;
	int AttackRange;
	int MovePoints;
	int Owner;
	int timedLife;
	std::deque<Spell> abilities;
	std::deque<Buff> buffs;
	int abilityCount;
	int hasAttacked;
	//int StateFlags;

	std::map<std::string,std::string> Labels;

	std::string name;
	std::string text;
	//lua_State* luaS;
	sf::Sprite sprite;

	/*bool isSilenced();
	bool isParalyzed();*/

	void addLabel(std::string key,std::string value);
	void addLabel(std::string key,int value);
	std::string getLabel(std::string key);
	int getLabelAsInt(std::string key);
	void removeLabel(std::string key);

	int sendMessageToBuffs(Message& msg);
	int handleMessage(Message& msg);

	void draw(sf::RenderWindow& window);
};

int getUnitTypeFromName(std::string name);
int initUnits();
sf::Vector2f getSpritePos(int x,int y);

