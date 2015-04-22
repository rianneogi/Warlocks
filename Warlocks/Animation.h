#pragma once

#include "MessageManager.h"

class Animation
{
public:
	Animation();
	Animation(std::string spr,std::string snd,int x,int y,float scalingfactor);
	Animation(std::string spr,std::string snd,sf::Vector2f pos,float scalingfactor);
	Animation(std::string spr,std::string snd,int x,int y,float scalingfactor,int onlysound);
	~Animation();

	sf::Sprite sprite;
	sf::Sound sound;
	sf::Clock timer;
	int delay;
	int xpos;
	int ypos;
	float scaling;
	int isOver;
	int dontDraw;

	void play();
	void update(sf::RenderWindow& window);
};

