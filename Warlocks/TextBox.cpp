#include "TextBox.h"


TextBox::TextBox()
{
	rect = sf::RectangleShape(sf::Vector2f(0,0));
	texts = std::deque<sf::Text>(0);
}

TextBox::TextBox(sf::Vector2f rectpos,sf::Vector2f rectsize,int borderthickness,sf::Color rectcolor,sf::Color bordercolor)
{
	rect = sf::RectangleShape(rectsize);
	rect.setPosition(rectpos);
	rect.setOutlineThickness(borderthickness);
	rect.setFillColor(rectcolor);
	rect.setOutlineColor(bordercolor);

	texts = std::deque<sf::Text>(0);
}

TextBox::~TextBox()
{
}

void TextBox::draw(sf::RenderWindow& window)
{
	window.draw(rect);
	for(int i = 0;i<texts.size();i++)
	{
		sf::Vector2f vtext = texts.at(i).getPosition();
		sf::Vector2f vrect = rect.getPosition();
		texts.at(i).setPosition(vtext.x+vrect.x,vtext.y+vrect.y);
		std::string str = texts.at(i).getString();
		if(str.substr(0,10)=="%uniticon:")
		{
			float scale = atof(str.substr(10,4).c_str());
			sf::Sprite spr(UnitTextures.at(getUnitTypeFromName(str.substr(15))));
			spr.setScale(scale,scale);
			spr.setPosition(texts.at(i).getPosition());
			window.draw(spr);
		}
		else if(str.substr(0,11)=="%spellicon:")
		{
			float scale = atof(str.substr(11,4).c_str());
			sf::Sprite spr(SpellTextures.at(getSpellTypeFromName(str.substr(16))));
			spr.setScale(scale,scale);
			spr.setPosition(texts.at(i).getPosition());
			window.draw(spr);
		}
		else if(str.substr(0,10)=="%bufficon:")
		{
			float scale = atof(str.substr(10,4).c_str());
			sf::Sprite spr(BuffTextures.at(getBuffTypeFromName(str.substr(15))));
			spr.setScale(scale,scale);
			spr.setPosition(texts.at(i).getPosition());
			window.draw(spr);
		}
		else
		{
			//window.draw(texts.at(i));
			drawBoundedText(texts.at(i),ceil((rect.getSize().x-20)/((texts.at(i).getCharacterSize())*(6.0/10))),window);
		}
		texts.at(i).setPosition(vtext.x,vtext.y);
	}
}

void TextBox::addText(std::string text,sf::Vector2f pos,sf::Color color,int charsize)
{
	sf::Text txt(text,Font,charsize);
	txt.setPosition(pos);
	txt.setColor(color);
	texts.push_back(txt);
}

void TextBox::addText(std::string text,int x,int y,sf::Color color,int charsize)
{
	addText(text,sf::Vector2f(x,y),color,charsize);
}

void TextBox::setText(int id,std::string text)
{
	texts.at(id).setString(text);
}

void TextBox::setTextPos(int id,sf::Vector2f pos)
{
	texts.at(id).setPosition(pos);
}

void TextBox::removeText(int id)
{
	texts.erase(texts.begin()+id);
}

void TextBox::clear()
{
	texts.clear();
}

void TextBox::setSize(sf::Vector2f size)
{
	rect.setSize(size);
}

void TextBox::setPos(sf::Vector2f pos)
{
	rect.setPosition(pos);
}