#pragma once

#include "Unit.h"

class TextBox
{
public:
	TextBox();
	TextBox(sf::Vector2f rectpos,sf::Vector2f rectsize,int borderthickness,sf::Color rectcolor,sf::Color bordercolor);
	~TextBox();

	sf::RectangleShape rect;
	std::deque<sf::Text> texts;

	void addText(std::string text,sf::Vector2f pos,sf::Color color,int charsize);
	void addText(std::string text,int x,int y,sf::Color color,int charsize);
	void setText(int id,std::string text);
	void setTextPos(int id,sf::Vector2f pos);
	void removeText(int id);
	void clear();

	void setSize(sf::Vector2f size);
	void setPos(sf::Vector2f pos);

	void draw(sf::RenderWindow& window);
};

