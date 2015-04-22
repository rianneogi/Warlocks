#pragma once

#include "Tile.h"

extern lua_State* LuaBuffs;

extern std::deque<std::string> BuffNames;
extern std::deque<sf::Texture> BuffTextures;

class Buff
{
public:
	Buff();
	Buff(int type,int unitid,int dur,int owner,unsigned int flags,int sprx,int spry);
	~Buff();

	int BuffType;
	int UnitId;
	int Duration;
	int Owner;
	unsigned int Flags;
	std::string Name;
	std::string Text;

	sf::Sprite sprite;
	sf::Text durtext;
	sf::RectangleShape durbox;
	//lua_State* luaS;

	void draw(sf::RenderWindow& window,int index);
	bool checkCollision(int x,int y);
};

int initBuffs();
int getBuffTypeFromName(std::string name);

