#pragma once

#include "Animation.h"

enum TileType {TILE_GRASS,TILE_FOREST,TILE_HILL,TILE_WATER,TILE_MOUNTAIN};
extern int TileEnterCost[5];
extern int TileExitCost[5];

class Tile
{
public:
	Tile();
	Tile(int t,int x,int y);
	~Tile();

	sf::Sprite sprite;

	int type;
	bool isOccupied;

	void draw(sf::RenderWindow& window);
	void setType(int t);
	void setPos(int x,int y);

	bool checkCollision(int x,int y);
};

