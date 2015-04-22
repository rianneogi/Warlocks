#include "Animation.h"

Animation::Animation()
{
	scaling = 42.0/192;
	sprite = sf::Sprite(Textures["Attack1"],sf::IntRect(0,0,192,192));
	sprite.setPosition(100,100);
	sprite.setScale(scaling,scaling);
	sound = sf::Sound(Sounds["Blow1"]);
	delay = 100;
	xpos = 0;
	ypos = 0;
	isOver = 0;
	dontDraw = 0;
}

Animation::Animation(std::string spr,std::string snd,int x,int y,float scalingfactor)
{
	scaling = (42.0/192)*scalingfactor;
	sprite = sf::Sprite(Textures[spr],sf::IntRect(0,0,192,192));
	sprite.setPosition(x,y);
	sprite.setScale(scaling,scaling);
	sound = sf::Sound(Sounds[snd]);
	delay = 100;
	xpos = 0;
	ypos = 0;
	isOver = 0;
	dontDraw = 0;
}

Animation::Animation(std::string spr,std::string snd,sf::Vector2f pos,float scalingfactor)
{
	*this = Animation(spr,snd,pos.x,pos.y,scalingfactor);
}

Animation::Animation(std::string spr,std::string snd,int x,int y,float scalingfactor,int onlysound)
{
	scaling = (42.0/192)*scalingfactor;
	sprite = sf::Sprite(Textures[spr],sf::IntRect(0,0,192,192));
	sprite.setPosition(x,y);
	sprite.setScale(scaling,scaling);
	sound = sf::Sound(Sounds[snd]);
	delay = 100;
	xpos = 0;
	ypos = 0;
	isOver = 0;
	dontDraw = onlysound;
}

Animation::~Animation()
{
}

void Animation::play()
{
	sound.play();
	timer.restart();
}

void Animation::update(sf::RenderWindow& window)
{
	if(dontDraw==0)
		window.draw(sprite);
	if(timer.getElapsedTime().asMilliseconds()>=delay)
	{
		xpos++;
		if(xpos>=5)
		{
			xpos=0;
			ypos++;
			if(ypos>=5)
			{
				isOver = 1;
			}
		}
		sprite.setTextureRect(sf::IntRect(xpos*192,ypos*192,192,192));
		timer.restart();
	}
}
