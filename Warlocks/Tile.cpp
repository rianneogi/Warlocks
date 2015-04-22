#include "Tile.h"

int TileEnterCost[5] = {1,1,2,99,99};
int TileExitCost[5] = {1,2,1,99,99};

Tile::Tile()
{
	type = TILE_GRASS;
	sprite = sf::Sprite(Textures["Grass"]);
	isOccupied = false;
}

Tile::Tile(int t,int x,int y)
{
	type = t;
	if(type==TILE_FOREST)
		sprite = sf::Sprite(Textures["Forest"]);
	else if(type==TILE_WATER)
		sprite = sf::Sprite(Textures["Water"]);
	else if(type==TILE_GRASS)
		sprite = sf::Sprite(Textures["Grass"]);
	else if(type==TILE_HILL)
		sprite = sf::Sprite(Textures["Hill"]);
	else if(type==TILE_MOUNTAIN)
		sprite = sf::Sprite(Textures["Mountain"]);
	//sprite.setScale(sf::Vector2f(0.25f,0.25f));
	sprite.setPosition(x,y);
	isOccupied = false;
}

void Tile::setType(int t)
{
	int x = sprite.getPosition().x;
	int y = sprite.getPosition().y;
	type = t;
	if(type==TILE_FOREST)
		sprite = sf::Sprite(Textures["Forest"]);
	else if(type==TILE_WATER)
		sprite = sf::Sprite(Textures["Water"]);
	else if(type==TILE_GRASS)
		sprite = sf::Sprite(Textures["Grass"]);
	else if(type==TILE_HILL)
		sprite = sf::Sprite(Textures["Hill"]);
	else if(type==TILE_MOUNTAIN)
		sprite = sf::Sprite(Textures["Mountain"]);
	sprite.setPosition(x,y);
}

void Tile::setPos(int x,int y)
{
	sprite.setPosition(x,y);
}

Tile::~Tile()
{
}

void Tile::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

bool Tile::checkCollision(int x,int y)
{
	sf::FloatRect b = sprite.getGlobalBounds();
	if(x>=b.left && x<=b.left+b.width && y>=b.top && y<=b.top+b.height)
	{
		return true;
	}
	return false;
}
