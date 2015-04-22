#include "LuaFunctions.h"

//NOTE: Remember to remove the "-d" in the .lib files in the linker input setting before building Release

//TODO
// Make the buff system more flexible (stackable buffs, non-stackable buffs and refreshing buffs) -- POSTPONED
// Add functionality to dispel only friendly or enemy buffs
// Add a way for the user to see labels on a unit -- NOT NEEDED FOR NOW

int main()
{
	srand(time(0));
	
	if(initSpells()!=0)
	{
		getch();
		return -1;
	}
	std::cout << "Spell Count: " << SpellTextures.size() << "\n";
	getch();
	if(initUnits()!=0)
	{
		getch();
		return -1;
	}
	std::cout << "Unit Count: " << UnitTextures.size() << "\n";
	getch();
	if(initBuffs()!=0)
	{
		getch();
		return -1;
	}
	std::cout << "Buff Count: " << BuffTextures.size() << "\n";
	getch();
	if(loadfiles()!=0)
	{
		getch();
		return -1;
	}
	std::cout << "Texture Count: " << Textures.size() << "\n";
	std::cout << "Sound Count: " << Sounds.size() << "\n";

	sf::RenderWindow window(sf::VideoMode(1200, 680), "Warlocks");
	window.setVerticalSyncEnabled(true);
	window.setPosition(sf::Vector2i(0,0));

    // Load a music to play
    sf::Music music;
    if (!music.openFromFile("Graphics//Music//song_game.ogg"))
        return EXIT_FAILURE;
	music.setLoop(true);

	std::cout << "Loading Done.\n";

    // Play the music
    //music.play();

	world = World(0);

	std::cout << "World Started.\n";

	Message msg("startturn");
	msg.addValue("player",world.turn);
	world.MsgMngr.sendMessage(msg);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
			{
				world.handleEvent(event);
                window.close();
			}
			world.handleEvent(event);
			world.update();
        }

        window.clear();
        
		world.draw(window);

        window.display();
    }
	cleanupSpells();
	lua_close(LuaUnits);
	lua_close(LuaBuffs);
    return 0;
}