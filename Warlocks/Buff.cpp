#include "Buff.h"

lua_State* LuaBuffs;

std::deque<std::string> BuffNames;
std::deque<sf::Texture> BuffTextures;

int getBuffTypeFromName(std::string name)
{
	for(int i = 0;i<BuffNames.size();i++)
	{
		if(BuffNames.at(i)==name)
		{
			return i;
		}
	}
	return -1;
}

static int loadbuff(lua_State* L)
{
	std::string s = lua_tostring(L,1);
	BuffNames.push_back(s);
	std::cout << "	Loaded Buff : " << s << "\n";
	sf::Texture t;
	s = "Graphics\\Buffs\\" + s + ".png";
	t.loadFromFile(s);
	BuffTextures.push_back(t);
	return 0;
}

int initBuffs()
{
	std::cout << "Loading Buffs...\n";

	LuaBuffs = luaL_newstate(); //create new lua state
	luaL_openlibs(LuaBuffs);

	lua_register(LuaBuffs,"loadbuff",loadbuff); //register loadspell
	registerLua(LuaBuffs); //register functions

	if (luaL_loadfile(LuaBuffs, "Lua\\Buffs.lua") || lua_pcall(LuaBuffs, 0, 0, 0)) 
	{
        std::cout<<"Error: failed to load Buffs.lua"<<std::endl;
		std::cout << lua_tostring(LuaBuffs,-1) << "\n";
		getch();
		return -1;
    }

	lua_getglobal(LuaBuffs,"loadBuffs");
	lua_pcall(LuaBuffs,0,0,0); //execute once to load units
	luaClean(LuaBuffs);

	return 0;
}

Buff::Buff()
{
	BuffType = -1;
	UnitId = -1;
	Duration = -1;
	Flags = 0;
	Owner = 0;
	Name = "";
	Text = "";
}

Buff::Buff(int type,int unitid,int dur,int owner,unsigned int flags,int sprx,int spry)
{
	BuffType = type;
	UnitId = unitid;
	Duration = dur;
	Flags = flags;
	Owner = owner;
	//luaS = luaOpen("Lua\\Buffs.lua");

	lua_getglobal(LuaBuffs,BuffNames.at(BuffType).c_str());
	lua_getfield(LuaBuffs,1,"name");
	Name = lua_tostring(LuaBuffs,-1);
	lua_pop(LuaBuffs,1);

	lua_getglobal(LuaBuffs,BuffNames.at(BuffType).c_str());
	lua_getfield(LuaBuffs,1,"text");
	Text = lua_tostring(LuaBuffs,-1);
	lua_pop(LuaBuffs,1);

	sprite = sf::Sprite(BuffTextures.at(BuffType));
	sprite.setPosition(sprx,spry);
	sprite.setScale(0.6f,0.6f);
	durtext = sf::Text("0",Font,10);
	durtext.setColor(sf::Color::White);
	durtext.setPosition(sprx+4,spry+2);
	durbox = sf::RectangleShape(sf::Vector2f(10,10));
	durbox.setFillColor(sf::Color::Black);
	durbox.setPosition(sprx+3,spry+3);
}

Buff::~Buff()
{
}

void Buff::draw(sf::RenderWindow& window,int index)
{
	sprite.setPosition(HorizontalTileCount*42 + 110 + index*40,375);
	durtext.setPosition(HorizontalTileCount*42 + 110 + index*40 + 4,375 + 2);
	durbox.setPosition(HorizontalTileCount*42 + 110 + index*40 + 3,375 + 3);
	window.draw(sprite);
	if(Duration!=-1)
	{
		durtext.setString(std::to_string(Duration));
		window.draw(durbox);
		window.draw(durtext);
	}
}

bool Buff::checkCollision(int x,int y)
{
	sf::FloatRect b = sprite.getGlobalBounds();
	if(x>=b.left && x<=b.left+b.width && y>=b.top && y<=b.top+b.height)
	{
		return true;
	}
	return false;
}

