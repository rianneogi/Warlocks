#ifndef GRAPHICSFILES_H
#define GRAPHICSFILES_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <conio.h>
#include <string>
#include <deque>
#include <map>
#include <fstream>

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

enum BuffFlag {BUFF_NONSTACKABLE=1,BUFF_NONDISPELLABLE=2};
enum TargetFlag {TARGET_ALLY=1,TARGET_ENEMY=2,TARGET_NONAVATAR=4,TARGET_NONSELF=8};
enum DamageType {DAMAGETYPE_ATTACK,DAMAGETYPE_SPELL,DAMAGETYPE_SPECIAL};

extern const int HorizontalTileCount;
extern const int VerticalTileCount;
extern sf::Font Font;

//enum TextureName{TEXTURE_FOREST,TEXTURE_WATER,TEXTURE_GRASS,TEXTURE_HILL,TEXTURE_MOUNTAIN,TEXTURE_ATTACK1};
//extern const int TextureCount;
//extern sf::Texture Textures[];
//extern std::string TexturePaths[];

extern std::map<std::string,sf::Texture> Textures;
extern std::map<std::string,sf::SoundBuffer> Sounds;

//enum SoundName{SOUND_ATTACK1};
//extern const int SoundCount;
//extern sf::SoundBuffer Sounds[];
//extern std::string SoundPaths[];

int loadfiles();
void luaClean(lua_State* L);
lua_State* luaOpen(std::string file);
void registerLua(lua_State* L);

void drawBoundedText(sf::Text txt,int bound,sf::RenderWindow& window);

#endif