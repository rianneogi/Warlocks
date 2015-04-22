#pragma once

#include "TextBox.h"

enum SelectionTypes{SELECTIONTYPE_UNIT,SELECTIONTYPE_UNITTYPE,SELECTIONTYPE_SPELL,SELECTIONTYPE_TILE};

class World
{
public:
	World();
	World(int b);
	~World();

	Tile tiles[15][15];
	std::map<int,Unit> units;
	int Mana[2];
	int ManaCap[2];
	int nextValidUnitId;
	int turn;
	MessageManager MsgMngr;

	int MouseX,MouseY;

	int SelectionId;
	int SelectionType;
	int CastingUnitId;
	int CastingSpellId;
	int isTileHighlighted[15][15];

	Message currentMessage;
	
	TextBox hoverbox;
	TextBox infobox;
	//sf::RectangleShape infobox;
	sf::RectangleShape endturnbutton;
	sf::Text playerinfo[2];
	std::deque<Animation> ActiveAnimations;
	sf::Sound endturnsound;

	void draw(sf::RenderWindow& window);
	void handleEvent(sf::Event event);
	void update();
	void updateInfobox();

	int setValidTargets();
	int setValidMoveTargets();
	void clearHighlightedTiles();

	void dispatchMessage(Message& msg);
	int handleMessage(Message& msg);
	void endTurn();
	int deadUnitCheck();

	int getUnitAt(int x,int y);
	int getUnitAttack(int uid);
	int getUnitAttackRange(int uid);
	int getUnitAttackCost(int uid);
	int getUnitTileEnterCost(int uid,int x1,int y1,int x2,int y2);
	int getUnitTileExitCost(int uid,int x1,int y1,int x2,int y2);
	int getUnitCanAttack(int uid);
	int getUnitCanCast(int uid);
	int getUnitCanMove(int uid);
	int getUnitCanReady(int uid);

	void summonUnit(int type,int x,int y,int owner);
	void summonUnit(std::string name,int x,int y,int owner);
	void damageUnit(int src,int target,int amount);
	void damageUnitArea(int src,int x,int y,int range,int amount,int dmgtype,int flags);
	void healUnit(int src,int target,int amount);
	void healUnitArea(int src,int x,int y,int range,int amount,int flags);
	void moveUnit(int id,int x,int y);
	void dispelUnit(int id,int flags);

	int unitExists(int uid);
	int isUnitAvatar(int uid);

	void castSpell(int castingunit,int castingspell,int targetx,int targety);
};

extern World world;

