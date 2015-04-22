#include "World.h"

World world;

World::World()
{
}

World::World(int b)
{
	turn = 0;
	for(int i = 0;i<2;i++)
	{
		Mana[i] = 0;
		ManaCap[i] = 0;
	}
	for(int i = 0;i<VerticalTileCount;i++)
	{
		for(int j = 0;j<HorizontalTileCount;j++)
		{
			int x = rand()%100;
			sf::Vector2f v = getSpritePos(j,i);
			if(x<50)
				tiles[j][i] = Tile(TILE_GRASS,v.x,v.y);
			else if(x>=50 && x<65)
				tiles[j][i] = Tile(TILE_WATER,v.x,v.y);
			else if(x>=65 && x<80)
				tiles[j][i] = Tile(TILE_FOREST,v.x,v.y);
			else if(x>=80 && x<95)
				tiles[j][i] = Tile(TILE_HILL,v.x,v.y);
			else if(x>=95)
				tiles[j][i] = Tile(TILE_MOUNTAIN,v.x,v.y);

			isTileHighlighted[j][i] = 0;
		}
	}
	CastingSpellId = -1;
	CastingUnitId = -1;
	SelectionId = -1;
	SelectionType = -1;

	MouseX = 0;
	MouseY = 0;
	
	std::pair<int,Unit> pair(0,Unit(0,HorizontalTileCount/2,VerticalTileCount-2,0,0));
	units.insert(pair);
	std::pair<int,Unit> pair2(1,Unit(0,HorizontalTileCount/2,1,1,1));
	units.insert(pair2);

	tiles[HorizontalTileCount/2][VerticalTileCount-2].setType(TILE_GRASS);
	tiles[HorizontalTileCount/2][1].setType(TILE_GRASS);

	units.at(0).MovePoints = 6;
	units.at(1).MovePoints = 6;
	units.at(0).abilityCount = 10;
	units.at(1).abilityCount = 10;
	nextValidUnitId = 2;

	std::fstream fs1("Data\\player1.txt",std::ios::in | std::ios::out);
	std::fstream fs2("Data\\player2.txt",std::ios::in | std::ios::out);
	if(!fs1.is_open() || !fs2.is_open())
	{
		std::cout << "could not open player spellbooks\n";
		return;
	}
	std::string abilname = "";
	int spelltype = -1;
	for(int i = 0;i<10;i++)
	{
		getline(fs1,abilname);
		std::cout << "opening spell: " << abilname << "\n"; 
		spelltype = getSpellTypeFromName(abilname);
		units.at(0).abilities.push_back(Spell(spelltype,((HorizontalTileCount*42)/10)*i+5,50+11*42+5));
		getline(fs2,abilname);
		std::cout << "opening spell: " << abilname << "\n"; 
		spelltype = getSpellTypeFromName(abilname);
		units.at(1).abilities.push_back(Spell(spelltype,((HorizontalTileCount*42)/10)*i+5,12));
	}
	fs1.close();
	fs2.close();

	//for(int i = 0;i<10;i++)
	//{
	//	units.at(0).abilities.push_back(Spell(i+15,((HorizontalTileCount*42)/10)*i+5,50+11*42+5));
	//	units.at(1).abilities.push_back(Spell(i+16,((HorizontalTileCount*42)/10)*i+5,12));
	//	//spells[0][i] = Spell(0,((HorizontalTileCount*42)/10)*i+5,50+11*42+5);
	//	//spells[1][i] = Spell(2,((HorizontalTileCount*42)/10)*i+5,12);
	//}

	tiles[HorizontalTileCount/2][VerticalTileCount-2].isOccupied=1;
	tiles[HorizontalTileCount/2][1].isOccupied=1;

	infobox = TextBox(sf::Vector2f(HorizontalTileCount*42 + 100,50),sf::Vector2f(500,VerticalTileCount*42),5,sf::Color::White,sf::Color(25,150,25,255));
	//infobox.addText("",HorizontalTileCount*42 + 110,60,sf::Color::Black,16);  //icon
	//infobox.addText("",HorizontalTileCount*42 + 150,65,sf::Color::Black,16);  //name
	//infobox.addText("",HorizontalTileCount*42 + 110,100,sf::Color::Black,16); //life
	//infobox.addText("",HorizontalTileCount*42 + 110,120,sf::Color::Black,16); //movepoints
	//infobox.addText("",HorizontalTileCount*42 + 110,190,sf::Color::Black,16); //text
	//infobox.addText("",HorizontalTileCount*42 + 110,350,sf::Color::Black,16); //buff text
	/*infobox = sf::RectangleShape(sf::Vector2f(500,VerticalTileCount*42));
	infobox.setOutlineThickness(5);
	infobox.setFillColor(sf::Color::White);
	infobox.setOutlineColor(sf::Color(25,150,25,255));
	infobox.setPosition(HorizontalTileCount*42 + 100,50);*/

	endturnbutton = sf::RectangleShape(sf::Vector2f(125,30));
	endturnbutton.setOutlineThickness(5);
	endturnbutton.setFillColor(sf::Color::White);
	endturnbutton.setOutlineColor(sf::Color(25,150,25,255));
	endturnbutton.setPosition(100,VerticalTileCount*42 + 100);

	hoverbox = TextBox(sf::Vector2f(0,0),sf::Vector2f(0,0),5,sf::Color::White,sf::Color::Blue);
	hoverbox.setSize(sf::Vector2f(400,250));

	for(int i = 0;i<2;i++) //set player info
	{
		std::string s = "Life: " + std::to_string(units.at(i).Life) + "\nMana: " +  std::to_string(Mana[i]);
		playerinfo[i] = sf::Text(s,Font,16);
		playerinfo[i].setColor(sf::Color::White);
		playerinfo[i].setPosition(HorizontalTileCount*42,VerticalTileCount*42+54-i*(VerticalTileCount*42+44));
	}

	endturnsound = sf::Sound(Sounds["endturn"]);

	std::cout << "World init done\n";
}

World::~World()
{
}

void World::draw(sf::RenderWindow& window)
{
	for(int i = 0;i<11;i++)
	{
		for(int j = 0;j<11;j++)
		{
			if(isTileHighlighted[j][i]==1) //draw highlight
			{
				sf::RectangleShape rect(sf::Vector2f(42,42));
				sf::Vector2f v = getSpritePos(j,i);
				rect.setPosition(v.x-1,v.y-1);
				rect.setFillColor(sf::Color::Blue);
				window.draw(rect);
			}
			tiles[j][i].draw(window);
		}
	}
	for(int i = 0;i<2;i++)
	{
		for(int j = 0;j<10;j++)
		{
			units.at(i).abilities.at(j).draw(window);
			/*if(units.at(i).abilities.at(j).currCooldown>0)
			{
				sf::Text cdtext(std::to_string(units.at(i).abilities.at(j).currCooldown),Font,8);
				cdtext.setPosition(units.at(i).abilities.at(j).sprite.getPosition().x+5,units.at(i).abilities.at(j).sprite.getPosition().y+1);
				cdtext.setColor(sf::Color::Blue);
				window.draw(cdtext);
			}*/
			//spells[i][j].draw(window);
		}
	}
	for(int i = 0;i<units.size();i++)
	{
		if(units.at(i).UniqueId!=-1)
			units.at(i).draw(window); 
	}

	//window.draw(infobox);
	infobox.draw(window);
	if(SelectionType==SELECTIONTYPE_UNIT && SelectionId!=-1)
	{
		////draw icon
		//sf::Vector2f v = units.at(SelectionId).sprite.getPosition();
		//units.at(SelectionId).sprite.setPosition(HorizontalTileCount*42 + 110,60);
		//units.at(SelectionId).sprite.setScale(0.6f,0.6f);
		//window.draw(units.at(SelectionId).sprite);
		////infobox.setText(0,"%uniticon:0.60:"+UnitNames.at(units.at(SelectionId).UnitType));
		//units.at(SelectionId).sprite.setPosition(v);
		//units.at(SelectionId).sprite.setScale(0.5f,0.5f);

		////draw name
		//std::string s = units.at(SelectionId).name;
		//sf::Text text(s,Font,16);
		//text.setPosition(HorizontalTileCount*42 + 150,65);
		//text.setColor(sf::Color::Black);
		//window.draw(text);
		////infobox.setText(1,units.at(SelectionId).name);

		////draw stats
		//text.setString("Life: " + std::to_string(units.at(SelectionId).Life) + "/" + std::to_string(units.at(SelectionId).maxLife));
		//text.setPosition(HorizontalTileCount*42 + 110,100);
		//window.draw(text);
		////infobox.setText(2,"Life: " + std::to_string(units.at(SelectionId).Life) + "/" + std::to_string(units.at(SelectionId).maxLife));
		//text.setString("Move Points: " + std::to_string(units.at(SelectionId).MovePoints));
		//text.setPosition(HorizontalTileCount*42 + 110,120);
		//window.draw(text);
		////infobox.setText(3,"Move Points: " + std::to_string(units.at(SelectionId).MovePoints));

		//draw abilities only if the unit is not an avatar
		if(units.at(SelectionId).UnitType!=0)
		{
			for(int i = 0;i<units.at(SelectionId).abilityCount;i++)
			{
				units.at(SelectionId).abilities.at(i).draw(window);
				//infobox.setText(4+i,"%spellicon:0.60:"+SpellNames.at(units.at(SelectionId).abilities.at(i).SpellType));
			}
		}

		//draw text
		//std::string s = units.at(SelectionId).text;
		//sf::Text text = sf::Text(s,Font,14);
		//text.setPosition(HorizontalTileCount*42 + 110,190);
		//text.setColor(sf::Color::Black);
		////window.draw(text);
		//drawBoundedText(text,60,window);
		////infobox.setText(4,units.at(SelectionId).text);

		////draw buff text
		//text = sf::Text("Buffs: ",Font,16);
		//text.setPosition(HorizontalTileCount*42 + 110,350);
		//text.setColor(sf::Color::Black);
		//window.draw(text);
		////infobox.setText(5,"Buffs:");

		//draw buffs
		for(int i = 0;i<units.at(SelectionId).buffs.size();i++)
		{
			units.at(SelectionId).buffs.at(i).draw(window,i);
			//infobox.setText(12+i,"%bufficon:0.60:"+BuffNames.at(units.at(SelectionId).buffs.at(i).BuffType));
		}
	}

	//draw endturn button and text
	window.draw(endturnbutton);
	sf::Text endturntext = sf::Text("End Turn",Font,22);
	endturntext.setColor(sf::Color::Black);
	endturntext.setPosition(104,VerticalTileCount*42 + 104);
	window.draw(endturntext);

	//draw player info
	for(int i = 0;i<2;i++)
	{
		playerinfo[i].setString("Life: " + std::to_string(units.at(i).Life) + "\nMana: " + std::to_string(Mana[i])+"/"+std::to_string(ManaCap[i]));
		window.draw(playerinfo[i]);
	}

	//draw animations
	for(int i = 0;i<ActiveAnimations.size();i++)
	{
		ActiveAnimations.at(i).update(window);
		if(ActiveAnimations.at(i).isOver==1)
		{
			ActiveAnimations.erase(ActiveAnimations.begin()+i);
			i--;
		}
	}

	//draw hoverbox
	for(int i = 0;i<2;i++)
	{
		for(int j = 0;j<10;j++)
		{
			if(units.at(i).abilities.at(j).checkCollision(MouseX,MouseY))
			{
				if(i==1)
					hoverbox.setPos(sf::Vector2f(MouseX,MouseY));
				else
					hoverbox.setPos(sf::Vector2f(MouseX,MouseY-hoverbox.rect.getSize().y));
				hoverbox.clear();
				hoverbox.addText(units.at(i).abilities.at(j).Name,5,5,sf::Color::Black,18);
				hoverbox.addText("Mana Cost: "+std::to_string(units.at(i).abilities.at(j).ManaCost),10,25,sf::Color::Black,14);
				hoverbox.addText("Cooldown: "+std::to_string(units.at(i).abilities.at(j).Cooldown),10,40,sf::Color::Black,14);
				hoverbox.addText("Move Cost: "+std::to_string(units.at(i).abilities.at(j).MoveCost),10,55,sf::Color::Black,14);
				hoverbox.addText("Range: "+std::to_string(units.at(i).abilities.at(j).Range),10,70,sf::Color::Black,14);
				hoverbox.addText(units.at(i).abilities.at(j).Text,5,90,sf::Color::Black,14);
				hoverbox.draw(window);
			}
		}
	}
	if(SelectionId!=-1 && SelectionType==SELECTIONTYPE_UNIT)
	{
		if(isUnitAvatar(SelectionId)==false)
		{
			for(int i = 0;i<units.at(SelectionId).abilityCount;i++)
			{
				if(units.at(SelectionId).abilities.at(i).checkCollision(MouseX,MouseY)) //draw spell hoverbox
				{
					hoverbox.setPos(sf::Vector2f(MouseX,MouseY));
					hoverbox.clear();
					hoverbox.addText(units.at(SelectionId).abilities.at(i).Name,5,5,sf::Color::Black,18);
					hoverbox.addText("Mana Cost: "+std::to_string(units.at(SelectionId).abilities.at(i).ManaCost),10,25,sf::Color::Black,14);
					hoverbox.addText("Cooldown: "+std::to_string(units.at(SelectionId).abilities.at(i).Cooldown),10,40,sf::Color::Black,14);
					hoverbox.addText("Move Cost: "+std::to_string(units.at(SelectionId).abilities.at(i).MoveCost),10,55,sf::Color::Black,14);
					hoverbox.addText("Range: "+std::to_string(units.at(SelectionId).abilities.at(i).Range),10,70,sf::Color::Black,14);
					hoverbox.addText(units.at(SelectionId).abilities.at(i).Text,5,90,sf::Color::Black,14);
					hoverbox.draw(window);
				}
			}
		}
		for(int i = 0;i<units.at(SelectionId).buffs.size();i++)
		{
			if(units.at(SelectionId).buffs.at(i).checkCollision(MouseX,MouseY)) //draw buff hoverbox
			{
				hoverbox.setPos(sf::Vector2f(MouseX,MouseY));
				hoverbox.clear();
				hoverbox.addText(units.at(SelectionId).buffs.at(i).Name,5,5,sf::Color::Black,18);
				hoverbox.addText(units.at(SelectionId).buffs.at(i).Text,10,25,sf::Color::Black,14);
				hoverbox.draw(window);
			}
		}
	}
}

void World::updateInfobox()
{
	//infobox.addText("",HorizontalTileCount*42 + 110,60,sf::Color::Black,16);  //icon
	//infobox.addText("",HorizontalTileCount*42 + 150,65,sf::Color::Black,16);  //name
	//infobox.addText("",HorizontalTileCount*42 + 110,100,sf::Color::Black,16); //life
	//infobox.addText("",HorizontalTileCount*42 + 110,120,sf::Color::Black,16); //movepoints
	//infobox.addText("",HorizontalTileCount*42 + 110,190,sf::Color::Black,16); //text
	//infobox.addText("",HorizontalTileCount*42 + 110,350,sf::Color::Black,16); //buff text
	infobox.clear();
	if(SelectionType==SELECTIONTYPE_UNIT && SelectionId!=-1)
	{
		infobox.addText("%uniticon:0.60:"+UnitNames.at(units.at(SelectionId).UnitType),10,10,sf::Color::Black,16);  //icon
		infobox.addText(units.at(SelectionId).name,50,15,sf::Color::Black,16);  //name
		infobox.addText("Life: "+std::to_string(units.at(SelectionId).Life)+"/"+std::to_string(units.at(SelectionId).maxLife)
			,10,50,sf::Color::Black,16);  //life
		infobox.addText("Attack: "+std::to_string(getUnitAttack(SelectionId)),10,70,sf::Color::Black,16); //attack
		infobox.addText("Move Points: "+std::to_string(units.at(SelectionId).MovePoints)+"/6"
			,10,90,sf::Color::Black,16);  //movepoints
		infobox.addText(units.at(SelectionId).text,10,160,sf::Color::Black,14);  //text
		infobox.addText("Buffs:",10,300,sf::Color::Black,16);  //buff text

		////draw abilities only if the unit is not an avatar
		//if(units.at(SelectionId).UnitType!=0)
		//{
		//	for(int i = 0;i<units.at(SelectionId).abilityCount;i++)
		//	{
		//		units.at(SelectionId).abilities.at(i).draw(window);
		//		//infobox.setText(4+i,"%spellicon:0.60:"+SpellNames.at(units.at(SelectionId).abilities.at(i).SpellType));
		//	}
		//}
		
		////draw text
		//s = units.at(SelectionId).text;
		//text = sf::Text(s,Font,14);
		//text.setPosition(HorizontalTileCount*42 + 110,190);
		//text.setColor(sf::Color::Black);
		////window.draw(text);
		//drawBoundedText(text,60,window);
		////infobox.setText(4,units.at(SelectionId).text);

		////draw buff text
		//text = sf::Text("Buffs: ",Font,16);
		//text.setPosition(HorizontalTileCount*42 + 110,350);
		//text.setColor(sf::Color::Black);
		//window.draw(text);
		////infobox.setText(5,"Buffs:");

		////draw buffs
		//for(int i = 0;i<units.at(SelectionId).buffs.size();i++)
		//{
		//	units.at(SelectionId).buffs.at(i).draw(window,i);
		//	//infobox.setText(12+i,"%bufficon:0.60:"+BuffNames.at(units.at(SelectionId).buffs.at(i).BuffType));
		//}
	}
}

void World::clearHighlightedTiles()
{
	for(int i = 0;i<VerticalTileCount;i++)
	{
		for(int j = 0;j<HorizontalTileCount;j++)
		{
			isTileHighlighted[j][i] = 0;
		}
	}
}

int World::setValidTargets()
{
	clearHighlightedTiles();
	int id = CastingSpellId;
	int x = units.at(CastingUnitId).x;
	int y = units.at(CastingUnitId).y;
	int range = units.at(CastingUnitId).abilities.at(id).getRange();
	std::cout << " set valid targets called, Range is: " << range << ", X: " << x << ", Y: " << y << ", ID: " << id << "\n";

	lua_State* L;
	int clearcount = 0;
	if(units.at(getUnitAt(x,y)).UnitType!=0) //unit is not an avatar
	{
		L = LuaUnits;
		lua_getglobal(L,UnitNames.at(units.at(SelectionId).UnitType).c_str());
		lua_getfield(L,-1,("ability"+std::to_string(CastingSpellId+1)).c_str());
		clearcount = 2;
	}
	else
	{
		L = LuaSpells;
		lua_getglobal(L,SpellNames.at(units.at(CastingUnitId).abilities.at(CastingSpellId).SpellType).c_str());
		clearcount = 1;
	}

	int count = 0;
	for(int i = y-range;i<=y+range;i++)
	{
		if(i<0) i=0;
		if(i>=VerticalTileCount) break;
		for(int j = x-range;j<=x+range;j++)
		{
			if(j<0) j=0;
			if(j>=HorizontalTileCount) break;

			
			lua_getfield(L,-1,"CheckTarget");
			lua_pushinteger(L,CastingUnitId);
			lua_pushinteger(L,CastingSpellId);
			lua_pushinteger(L,j);
			lua_pushinteger(L,i);
			lua_pcall(L,4,1,0);
			int x = lua_tointeger(L,-1);
			lua_pop(L,1); //pop the return value from stack
			if(x==1)
			{
				isTileHighlighted[j][i] = 1;
				count++;
			}
		}
	}
	lua_pop(L,clearcount);
	return count;
}

int World::setValidMoveTargets()
{
	int x = units.at(CastingUnitId).x;
	int y = units.at(CastingUnitId).y;
	int canAttack = getUnitCanAttack(CastingUnitId);
	int canMove = getUnitCanMove(CastingUnitId);
	int count = 0;
	if(canMove)
	{
		for(int i = y-1;i<=y+1;i++)
		{
			if(i<0) i=0;
			if(i>=VerticalTileCount) break;
			for(int j = x-1;j<=x+1;j++)
			{
				if(j<0) j=0;
				if(j>=HorizontalTileCount) break;

				if(tiles[j][i].isOccupied==0 && 
					units.at(CastingUnitId).MovePoints>=getUnitTileExitCost(CastingUnitId,x,y,j,i)+getUnitTileEnterCost(CastingUnitId,x,y,j,i))
				{
					isTileHighlighted[j][i] = 1;
					count++;
				}
			}
		}
	}
	if(canAttack && units.at(CastingUnitId).hasAttacked==0)
	{
		int range = getUnitAttackRange(CastingUnitId);
		for(int i = y-range;i<=y+range;i++)
		{
			if(i<0) i=0;
			if(i>=VerticalTileCount) break;
			for(int j = x-range;j<=x+range;j++)
			{
				if(j<0) j=0;
				if(j>=HorizontalTileCount) break;

				if(canAttack && tiles[j][i].isOccupied==1 && units.at(getUnitAt(j,i)).Owner!=turn 
					&& units.at(CastingUnitId).MovePoints>=getUnitAttackCost(CastingUnitId))
				{
					isTileHighlighted[j][i] = 1;
					count++;
				}
			}
		}
	}
	return count;
}

void World::handleEvent(sf::Event event)
{
	if(event.type==sf::Event::MouseMoved)
	{
		MouseX = event.mouseMove.x;
		MouseY = event.mouseMove.y;
	}
	else if(event.type==sf::Event::MouseButtonPressed)
	{
		if(event.mouseButton.button == sf::Mouse::Left)
		{
			for(int i = 0;i<VerticalTileCount;i++)
			{
				for(int j = 0;j<HorizontalTileCount;j++)
				{
					if(tiles[j][i].checkCollision(event.mouseButton.x,event.mouseButton.y))
					{
						//return spell target
						if(isTileHighlighted[j][i]==1)
						{
							if(CastingSpellId!=-1) //spell being cast
							{
								Message msg("unitcast");
								msg.addValue("caster",CastingUnitId);
								msg.addValue("targetx",j);
								msg.addValue("targety",i);
								msg.addValue("spell",CastingSpellId);
								MsgMngr.sendMessage(msg);
								CastingSpellId = -1;
								CastingUnitId = -1;
								clearHighlightedTiles();

								//lua_State* L;
								//if(units.at(CastingUnitId).UnitType!=0) //unit is not an avatar
								//{
								//	L = LuaUnits;
								//	lua_getglobal(L,UnitNames.at(units.at(CastingUnitId).UnitType).c_str());
								//	lua_getfield(L,-1,("ability"+std::to_string(CastingSpellId+1)).c_str());
								//}
								//else
								//{
								//	L = LuaSpells;
								//	lua_getglobal(L,SpellNames.at(units.at(CastingUnitId).abilities.at(CastingSpellId).SpellType).c_str());
								//}
								////lua_getglobal(LuaSpells,SpellNames.at(CastingSpellId).c_str());
								//lua_getfield(L,-1,"OnCast");
								//lua_pushinteger(L,j);
								//lua_pushinteger(L,i);
								//lua_pcall(L,2,0,0);
								//luaClean(L);
								//Mana[units.at(CastingUnitId).Owner] -= units.at(CastingUnitId).abilities.at(CastingSpellId).ManaCost;
								//units.at(CastingUnitId).abilities.at(CastingSpellId).currCooldown = units.at(CastingUnitId).abilities.at(CastingSpellId).Cooldown;
								//units.at(CastingUnitId).MovePoints -= units.at(CastingUnitId).abilities.at(CastingSpellId).MoveCost;
								
							}
							else if(CastingUnitId!=-1) //unit moving and attacking
							{
								if(tiles[j][i].isOccupied==0)//unit moving
								{
									Message msg("unitmove");
									msg.addValue("unit",CastingUnitId);
									msg.addValue("targetx",j);
									msg.addValue("targety",i);
									MsgMngr.sendMessage(msg);
								}
								else //unit attacking
								{
									Message msg("unitattack");
									msg.addValue("unit",CastingUnitId);
									msg.addValue("target",getUnitAt(j,i));
									MsgMngr.sendMessage(msg);
								}
								CastingUnitId = -1;
								CastingSpellId = -1;
								clearHighlightedTiles();
							}
						}
						else //select tile/unit
						{
							int unitat = getUnitAt(j,i);
							if(unitat!=-1)
							{
								SelectionType = SELECTIONTYPE_UNIT;
								SelectionId = unitat;
								updateInfobox();
							}
						}
					}
				}
			}

			//cast spells
			for(int i = 0;i<2;i++)
			{
				for(int j = 0;j<10;j++)
				{
					if(units.at(i).abilities.at(j).checkCollision(event.mouseButton.x,event.mouseButton.y))
					{
						if(units.at(i).abilities.at(j).MoveCost <= units.at(i).MovePoints //check if all conditions(move points, mana etc.) are OK
							&& units.at(i).abilities.at(j).currCooldown==0 
							&& Mana[units.at(i).Owner] >= units.at(i).abilities.at(j).ManaCost
							&& turn==units.at(i).Owner
							&& getUnitCanCast(i)==1)
						{
							//cast spell
							CastingSpellId = j; //set casting spell constants
							CastingUnitId = i;
							//CastingSpellX = units.at(i).x;
							//CastingSpellY = units.at(i).y;

							lua_getglobal(LuaSpells,SpellNames.at(units.at(i).abilities.at(j).SpellType).c_str()); //get target
							lua_getfield(LuaSpells,-1,"targetCount");
							int targetcount = lua_tointeger(LuaSpells,-1);
							lua_pop(LuaSpells,2);

							//lua_getglobal(LuaSpells,SpellNames.at(units.at(i).abilities.at(j).SpellType).c_str()); //get OnCast function
							//lua_getfield(LuaSpells,-1,"OnCast");

							if(targetcount!=0) //spell has target
							{
								int targetsfound = setValidTargets();
								if(targetsfound<=0) //no targets found
								{
									CastingUnitId = -1;
									CastingSpellId = -1;
								}
							} 
							else //spell is not targetted
							{
								Message msg("unitcast");
								msg.addValue("caster",CastingUnitId);
								msg.addValue("targetx",units.at(CastingUnitId).x);
								msg.addValue("targety",units.at(CastingUnitId).y);
								msg.addValue("spell",CastingSpellId);
								MsgMngr.sendMessage(msg);
								CastingSpellId = -1;
								CastingUnitId = -1;
								clearHighlightedTiles();

								/*lua_pushinteger(LuaSpells,units.at(CastingUnitId).x);
								lua_pushinteger(LuaSpells,units.at(CastingUnitId).y);
								lua_pcall(LuaSpells,2,0,0);
								luaClean(LuaSpells);
								Mana[units.at(CastingUnitId).Owner] -= units.at(CastingUnitId).abilities.at(CastingSpellId).ManaCost;
								units.at(CastingUnitId).abilities.at(CastingSpellId).currCooldown = units.at(CastingUnitId).abilities.at(CastingSpellId).Cooldown;
								units.at(CastingUnitId).MovePoints -= units.at(CastingUnitId).abilities.at(CastingSpellId).MoveCost;
								CastingSpellId = -1;
								CastingUnitId = -1;*/
							}
							//spells[i][j].OnCast(0,0,*this);
						}
					}
				}
			}

			//casting abilities
			if(SelectionType==SELECTIONTYPE_UNIT && SelectionId!=-1 && units.at(SelectionId).UnitType!=0)
			{
				for(int i = 0;i<units.at(SelectionId).abilityCount;i++)
				{
					if(units.at(SelectionId).abilities.at(i).checkCollision(event.mouseButton.x,event.mouseButton.y))
					{
						if(units.at(SelectionId).abilities.at(i).MoveCost <= units.at(SelectionId).MovePoints //check if all conditions(move points, mana etc.) are OK
							&& units.at(SelectionId).abilities.at(i).currCooldown==0 
							&& Mana[units.at(SelectionId).Owner] >= units.at(SelectionId).abilities.at(i).ManaCost
							&& turn==units.at(SelectionId).Owner
							&& getUnitCanCast(SelectionId)==1)
						{
							//cast ability
							CastingSpellId = i;
							CastingUnitId = SelectionId;

							lua_getglobal(LuaUnits,UnitNames.at(units.at(SelectionId).UnitType).c_str()); //get target
							lua_getfield(LuaUnits,-1,("ability"+std::to_string(i+1)).c_str());
							lua_getfield(LuaUnits,-1,"targetCount");
							int targetcount = lua_tointeger(LuaUnits,-1);
							//luaClean(LuaSpells);
							lua_pop(LuaUnits,3);

							/*lua_getglobal(LuaUnits,UnitNames.at(units.at(SelectionId).UnitType).c_str());
							lua_getfield(LuaUnits,-1,("ability"+std::to_string(i+1)).c_str());
							lua_getfield(LuaUnits,-1,"OnCast");*/

							if(targetcount!=0) //spell has target
							{
								int targetsfound = setValidTargets();
								if(targetsfound<=0) //no targets found
								{
									CastingUnitId = -1;
									CastingSpellId = -1;
								}
							}
							else //spell is not targetted
							{
								Message msg("unitcast");
								msg.addValue("caster",CastingUnitId);
								msg.addValue("targetx",units.at(CastingUnitId).x);
								msg.addValue("targety",units.at(CastingUnitId).y);
								msg.addValue("spell",CastingSpellId);
								MsgMngr.sendMessage(msg);
								CastingSpellId = -1;
								CastingUnitId = -1;
								clearHighlightedTiles();

								//lua_pushinteger(LuaUnits,0);
								//lua_pushinteger(LuaUnits,0);
								//lua_pcall(LuaUnits,2,0,0);
								////luaClean(LuaUnits);
								//lua_pop(LuaUnits,3);
								//units.at(CastingUnitId).abilities.at(CastingSpellId).currCooldown = units.at(CastingUnitId).abilities.at(CastingSpellId).Cooldown;
								//units.at(CastingUnitId).MovePoints -= units.at(CastingUnitId).abilities.at(CastingSpellId).MoveCost;
								//CastingUnitId = -1;
								//CastingSpellId = -1;
							} 
						}
					}
				}
			}

			//end turn button
			if(event.mouseButton.x >= endturnbutton.getGlobalBounds().left && event.mouseButton.y >= endturnbutton.getGlobalBounds().top
				&& event.mouseButton.x <= endturnbutton.getGlobalBounds().left+endturnbutton.getGlobalBounds().width
				&& event.mouseButton.y <= endturnbutton.getGlobalBounds().top+endturnbutton.getGlobalBounds().height)
			{
				endturnsound.play();
				endTurn();
			}
		}
		if(event.mouseButton.button == sf::Mouse::Right)
		{
			//deselect casting spell/unit
			if(CastingSpellId!=-1 || CastingUnitId!=-1)
			{
				CastingSpellId = -1;
				CastingUnitId = -1;
				clearHighlightedTiles();
			}
			else
			{
				for(int i = 0;i<VerticalTileCount;i++)
				{
					for(int j = 0;j<HorizontalTileCount;j++)
					{
						if(tiles[j][i].checkCollision(event.mouseButton.x,event.mouseButton.y)) //move unit
						{
							if(tiles[j][i].isOccupied==1 && turn==units.at(getUnitAt(j,i)).Owner)
							{
								CastingUnitId = getUnitAt(j,i);
								CastingSpellId = -1;
								int targetsfound = setValidMoveTargets();
								if(targetsfound<=0) //no targets found
								{
									CastingUnitId = -1;
									CastingSpellId = -1;
								}
							}
						}
					}
				}
			}
		}
	}
	else if(event.type==sf::Event::KeyPressed)
	{
	}
}