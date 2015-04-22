#pragma once

#include "Buff.h"

extern lua_State* LuaSpells;
extern lua_State* LuaUnits;

extern std::deque<std::string> SpellNames;
extern std::deque<sf::Texture> SpellTextures;

extern std::deque<int> isSpellTypeSummon;

class Spell
{
public:
	Spell();
	Spell(int id,int x,int y);
	Spell(int id,int x,int y,lua_State* L);
	~Spell();

	int SpellType;
	int Cooldown;
	int currCooldown;
	int ManaCost;
	int MoveCost;
	int Range;
	std::string Text;
	std::string Name;

	sf::Sprite sprite;
	sf::Text cdtext;
	sf::RectangleShape cdbox;

	//lua_State* L;

	//void OnCast(int x,int y,const World& world);

	int getRange();

	void draw(sf::RenderWindow& window);
	bool checkCollision(int x,int y);
};

int getSpellTypeFromName(std::string name);
int initSpells();
void cleanupSpells();

