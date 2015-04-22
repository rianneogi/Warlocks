#include "GraphicsFiles.h"

const int HorizontalTileCount = 11;
const int VerticalTileCount = 11;

sf::Font Font;

//const int TextureCount = 6;
//sf::Texture Textures[TextureCount];
//std::string TexturePaths[TextureCount] = {"Graphics\\Terrain\\Forest.png","Graphics\\Terrain\\Water.png",
//                                          "Graphics\\Terrain\\Grass.png","Graphics\\Terrain\\Hill.png",
//										  "Graphics\\Terrain\\Mountain.png",
//										  "Graphics\\Animations\\Attack1.png"};

std::map<std::string,sf::Texture> Textures;
std::map<std::string,sf::SoundBuffer> Sounds;

//const int SoundCount = 1;
//sf::SoundBuffer Sounds[SoundCount];
//std::string SoundPaths[SoundCount] = {"Graphics\\Sounds\\Blow1.ogg"};

int loadfiles()
{
	if(!Font.loadFromFile("Graphics\\Fonts\\OxygenMono.ttf"))
	{
		return -1;
	}

	std::cout << "Loading Terrain Textures...\n";
	Textures["Forest"].loadFromFile("Graphics\\Terrain\\Forest.png");
	Textures["Water"].loadFromFile("Graphics\\Terrain\\Water.png");
	Textures["Grass"].loadFromFile("Graphics\\Terrain\\Grass.png");
	Textures["Hill"].loadFromFile("Graphics\\Terrain\\Hill.png");
	Textures["Mountain"].loadFromFile("Graphics\\Terrain\\Mountain.png");

	std::cout << "Loading Animation Textures...\n";
	std::fstream file("Graphics\\Animations\\files.txt",std::ios::in | std::ios::out);
	std::string s;
	std::string s2;
	while(!file.eof())
	{
		getline(file,s);
		int x = s.find_last_of('.');
		s2 = s.substr(0,x);
		Textures[s2] = sf::Texture();
		std::cout << "	loaded texture: " << s2 << "\n";
		Textures[s2].loadFromFile("Graphics\\Animations\\"+s);
		//Textures[s2].setSmooth(true);
	}
	//for(int i = 0;i<TextureCount;i++)
	//{
	//	if(!Textures[i].loadFromFile(TexturePaths[i]))
	//		return -1;
	//	//Textures[i].setSmooth(true);
	//}
	std::cout << "Loading Sounds...\n";
	file = std::fstream("Graphics\\Sounds\\files.txt",std::ios::in | std::ios::out);
	while(!file.eof())
	{
		getline(file,s);
		int x = s.find_last_of('.');
		s2 = s.substr(0,x);
		Sounds[s2] = sf::SoundBuffer();
		std::cout << "	loaded sound: " << s2 << "\n";
		Sounds[s2].loadFromFile("Graphics\\Sounds\\"+s);
	}
	return 0;
}

void luaClean(lua_State* L)
{
    int n = lua_gettop(L);
    lua_pop(L, n);
}

lua_State* luaOpen(std::string file)
{
	lua_State* L = luaL_newstate(); //create new lua state
	luaL_openlibs(L);

	//lua_register(L,"loadunit",loadunit); //register loadspell
	registerLua(L); //register functions
	if (luaL_loadfile(L, file.c_str()) || lua_pcall(L, 0, 0, 0)) 
	{
        std::cout<<"Error: failed to load " << file <<std::endl;
		std::cout << lua_tostring(L,-1) << "\n";
		return NULL;
    }
	return L;
}

void drawBoundedText(sf::Text txt,int bound,sf::RenderWindow& window)
{
	sf::Text t = txt;
	std::string s = txt.getString();
	int lastpos = 0;

	int inc = txt.getFont()->getLineSpacing(txt.getCharacterSize());
	int y = txt.getPosition().y;

	int brcounter = 0;

	for(int i=0;i<s.size();i++)
	{
		if(s.at(i)==' ' || s.at(i)=='\n')
			lastpos = i;
		if(i>bound || s.at(i)=='\n')
		{
			std::string sub = s.substr(0,lastpos);
			t.setString(sub);
			t.setPosition(sf::Vector2f(t.getPosition().x,y));
			y = y+inc*(1+brcounter);
			s = s.substr(lastpos+1);
			window.draw(t);
			brcounter = 0;
			i=0;
		}
	}

	/*while(s.size()>bound)
	{
		std::string sub = s.substr(0,bound);
		t.setString(sub);
		t.setPosition(sf::Vector2f(t.getPosition().x,y));
		y = y+inc;
		s = s.substr(bound);
		window.draw(t);
	}*/
	t.setString(s);
	t.setPosition(sf::Vector2f(t.getPosition().x,y));
	window.draw(t);
}