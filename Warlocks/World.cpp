#include "World.h"

World world;

World::World()
{
}

World::World(int b)
{
	turn = 0;
	Mana[0] = 0;
	Mana[1] = 0;
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
		units.at(0).abilities.push_back(Spell(spelltype,((HorizontalTileCount*42)/10)*i+5,50+11*42+5,isSpellTypeSummon.at(spelltype)));
		getline(fs2,abilname);
		std::cout << "opening spell: " << abilname << "\n"; 
		spelltype = getSpellTypeFromName(abilname);
		units.at(1).abilities.push_back(Spell(spelltype,((HorizontalTileCount*42)/10)*i+5,12,isSpellTypeSummon.at(spelltype)));
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

	//infobox = TextBox(sf::Vector2f(HorizontalTileCount*42 + 100,50),sf::Vector2f(500,VerticalTileCount*42),5,sf::Color::White,sf::Color(25,150,25,255));
	//infobox.addText("",HorizontalTileCount*42 + 110,60,sf::Color::Black,16);  //icon
	//infobox.addText("",HorizontalTileCount*42 + 150,65,sf::Color::Black,16);  //name
	//infobox.addText("",HorizontalTileCount*42 + 110,100,sf::Color::Black,16); //life
	//infobox.addText("",HorizontalTileCount*42 + 110,120,sf::Color::Black,16); //movepoints
	//infobox.addText("",HorizontalTileCount*42 + 110,190,sf::Color::Black,16); //text
	//infobox.addText("",HorizontalTileCount*42 + 110,350,sf::Color::Black,16); //buff text
	infobox = sf::RectangleShape(sf::Vector2f(500,VerticalTileCount*42));
	infobox.setOutlineThickness(5);
	infobox.setFillColor(sf::Color::White);
	infobox.setOutlineColor(sf::Color(25,150,25,255));
	infobox.setPosition(HorizontalTileCount*42 + 100,50);

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

void World::update()
{
	while(MsgMngr.hasMoreMessages())
	{
		Message msg = MsgMngr.peekMessage();
		MsgMngr.dispatch();
		std::cout << "dispatching message: ";
		for(std::map<std::string,std::string>::iterator i = msg.map.begin();i!=msg.map.end();i++)
		{
			std::cout << i->first << " " << i->second << " ";
		}
		std::cout << "\n";
		dispatchMessage(msg);
	}
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

	window.draw(infobox);
	if(SelectionType==SELECTIONTYPE_UNIT && SelectionId!=-1)
	{
		//draw icon
		sf::Vector2f v = units.at(SelectionId).sprite.getPosition();
		units.at(SelectionId).sprite.setPosition(HorizontalTileCount*42 + 110,60);
		units.at(SelectionId).sprite.setScale(0.6f,0.6f);
		window.draw(units.at(SelectionId).sprite);
		//infobox.setText(0,"%uniticon:0.60:"+UnitNames.at(units.at(SelectionId).UnitType));
		units.at(SelectionId).sprite.setPosition(v);
		units.at(SelectionId).sprite.setScale(0.5f,0.5f);

		//draw name
		std::string s = units.at(SelectionId).name;
		sf::Text text(s,Font,16);
		text.setPosition(HorizontalTileCount*42 + 150,65);
		text.setColor(sf::Color::Black);
		window.draw(text);
		//infobox.setText(1,units.at(SelectionId).name);

		//draw stats
		text.setString("Life: " + std::to_string(units.at(SelectionId).Life) + "/" + std::to_string(units.at(SelectionId).maxLife));
		text.setPosition(HorizontalTileCount*42 + 110,100);
		window.draw(text);
		//infobox.setText(2,"Life: " + std::to_string(units.at(SelectionId).Life) + "/" + std::to_string(units.at(SelectionId).maxLife));
		text.setString("Move Points: " + std::to_string(units.at(SelectionId).MovePoints));
		text.setPosition(HorizontalTileCount*42 + 110,120);
		window.draw(text);
		//infobox.setText(3,"Move Points: " + std::to_string(units.at(SelectionId).MovePoints));

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
		s = units.at(SelectionId).text;
		text = sf::Text(s,Font,14);
		text.setPosition(HorizontalTileCount*42 + 110,190);
		text.setColor(sf::Color::Black);
		//window.draw(text);
		drawBoundedText(text,60,window);
		//infobox.setText(4,units.at(SelectionId).text);

		//draw buff text
		text = sf::Text("Buffs: ",Font,16);
		text.setPosition(HorizontalTileCount*42 + 110,350);
		text.setColor(sf::Color::Black);
		window.draw(text);
		//infobox.setText(5,"Buffs:");

		//draw buffs
		for(int i = 0;i<units.at(SelectionId).buffs.size();i++)
		{
			units.at(SelectionId).buffs.at(i).draw(window,i);
			//infobox.setText(12+i,"%bufficon:0.60:"+BuffNames.at(units.at(SelectionId).buffs.at(i).BuffType));
		}
	}
	//infobox.draw(window);

	//draw endturn button and text
	window.draw(endturnbutton);
	sf::Text endturntext = sf::Text("End Turn",Font,22);
	endturntext.setColor(sf::Color::Black);
	endturntext.setPosition(104,VerticalTileCount*42 + 104);
	window.draw(endturntext);

	//draw player info
	for(int i = 0;i<2;i++)
	{
		playerinfo[i].setString("Life: " + std::to_string(units.at(0).Life) + "\nMana: " + std::to_string(Mana[i]));
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
	if(SelectionId!=-1 && SelectionType==SELECTIONTYPE_UNIT && isUnitAvatar(SelectionId)==false)
	{
		for(int i = 0;i<units.at(SelectionId).abilityCount;i++)
		{
			if(units.at(SelectionId).abilities.at(i).checkCollision(MouseX,MouseY))
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

void World::setValidTargets()
{
	int id = CastingSpellId;
	int x = units.at(CastingUnitId).x;
	int y = units.at(CastingUnitId).y;
	int range = units.at(CastingUnitId).abilities.at(id).getRange();
	std::cout << " set valid targets called, Range is: " << range << ", X: " << x << ", Y: " << y << ", ID: " << id << "\n";
	for(int i = y-range;i<=y+range;i++)
	{
		if(i<0) i=0;
		if(i>=VerticalTileCount) break;
		for(int j = x-range;j<=x+range;j++)
		{
			if(j<0) j=0;
			if(j>=HorizontalTileCount) break;

			lua_State* L;
			if(units.at(getUnitAt(x,y)).UnitType!=0) //unit is not an avatar
			{
				L = LuaUnits;
				lua_getglobal(L,UnitNames.at(units.at(SelectionId).UnitType).c_str());
				lua_getfield(L,-1,("ability"+std::to_string(CastingSpellId+1)).c_str());
			}
			else
			{
				L = LuaSpells;
				lua_getglobal(L,SpellNames.at(units.at(CastingUnitId).abilities.at(CastingSpellId).SpellType).c_str());
			}
			lua_getfield(L,-1,"CheckTarget");
			lua_pushinteger(L,j);
			lua_pushinteger(L,i);
			lua_pcall(L,2,1,0);
			int x = lua_tointeger(L,-1);
			luaClean(L);
			if(x==1)
			{
				isTileHighlighted[j][i] = 1;
			}
		}
	}
}

void World::setValidMoveTargets()
{
	int x = units.at(CastingUnitId).x;
	int y = units.at(CastingUnitId).y;
	int canAttack = getUnitCanAttack(CastingUnitId);
	int canMove = getUnitCanMove(CastingUnitId);
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
				}
			}
		}
	}
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
								lua_State* L;
								if(units.at(CastingUnitId).UnitType!=0) //unit is not an avatar
								{
									L = LuaUnits;
									lua_getglobal(L,UnitNames.at(units.at(CastingUnitId).UnitType).c_str());
									lua_getfield(L,-1,("ability"+std::to_string(CastingSpellId+1)).c_str());
								}
								else
								{
									L = LuaSpells;
									lua_getglobal(L,SpellNames.at(units.at(CastingUnitId).abilities.at(CastingSpellId).SpellType).c_str());
								}
								//lua_getglobal(LuaSpells,SpellNames.at(CastingSpellId).c_str());
								lua_getfield(L,-1,"OnCast");
								lua_pushinteger(L,j);
								lua_pushinteger(L,i);
								lua_pcall(L,2,0,0);
								luaClean(L);
								Mana[units.at(CastingUnitId).Owner] -= units.at(CastingUnitId).abilities.at(CastingSpellId).ManaCost;
								units.at(CastingUnitId).abilities.at(CastingSpellId).currCooldown = units.at(CastingUnitId).abilities.at(CastingSpellId).Cooldown;
								units.at(CastingUnitId).MovePoints -= units.at(CastingUnitId).abilities.at(CastingSpellId).MoveCost;
								CastingSpellId = -1;
								CastingUnitId = -1;
								clearHighlightedTiles();
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
							&& turn==units.at(i).Owner)
						{
							//cast spell
							CastingSpellId = j; //set casting spell constants
							CastingUnitId = i;
							//CastingSpellX = units.at(i).x;
							//CastingSpellY = units.at(i).y;

							lua_getglobal(LuaSpells,SpellNames.at(units.at(i).abilities.at(j).SpellType).c_str()); //get target
							lua_getfield(LuaSpells,-1,"targetCount");
							int targetcount = lua_tointeger(LuaSpells,-1);
							luaClean(LuaSpells);

							lua_getglobal(LuaSpells,SpellNames.at(units.at(i).abilities.at(j).SpellType).c_str()); //get OnCast function
							lua_getfield(LuaSpells,-1,"OnCast");

							if(targetcount!=0) //spell has target
							{
								setValidTargets();
							} 
							else //spell is not targetted
							{
								lua_pushinteger(LuaSpells,units.at(CastingUnitId).x);
								lua_pushinteger(LuaSpells,units.at(CastingUnitId).x);
								lua_pcall(LuaSpells,2,0,0);
								luaClean(LuaSpells);
								Mana[units.at(CastingUnitId).Owner] -= units.at(CastingUnitId).abilities.at(CastingSpellId).ManaCost;
								units.at(CastingUnitId).abilities.at(CastingSpellId).currCooldown = units.at(CastingUnitId).abilities.at(CastingSpellId).Cooldown;
								units.at(CastingUnitId).MovePoints -= units.at(CastingUnitId).abilities.at(CastingSpellId).MoveCost;
								CastingSpellId = -1;
								CastingUnitId = -1;
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
							&& turn==units.at(SelectionId).Owner)
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

							lua_getglobal(LuaUnits,UnitNames.at(units.at(SelectionId).UnitType).c_str());
							lua_getfield(LuaUnits,-1,("ability"+std::to_string(i+1)).c_str());
							lua_getfield(LuaUnits,-1,"OnCast");

							if(targetcount!=0) //spell has target
							{
								setValidTargets();
							}
							else //spell is not targetted
							{
								lua_pushinteger(LuaUnits,0);
								lua_pushinteger(LuaUnits,0);
								lua_pcall(LuaUnits,2,0,0);
								//luaClean(LuaUnits);
								lua_pop(LuaUnits,3);
								units.at(CastingUnitId).abilities.at(CastingSpellId).currCooldown = units.at(CastingUnitId).abilities.at(CastingSpellId).Cooldown;
								units.at(CastingUnitId).MovePoints -= units.at(CastingUnitId).abilities.at(CastingSpellId).MoveCost;
								CastingUnitId = -1;
								CastingSpellId = -1;
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
								setValidMoveTargets();
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

void World::endTurn()
{
	CastingUnitId = -1;
	CastingSpellId = -1;

	//end previous turn
	Message msg("endturn");
	msg.addValue("player",turn);
	MsgMngr.sendMessage(msg);

	clearHighlightedTiles();

	turn = (turn+1)%2;

	//start next turn
	Message msg2("startturn");
	msg2.addValue("player",turn);
	MsgMngr.sendMessage(msg2);
}

int World::handleMessage(Message& msg)
{
	std::string type = msg.getString("msgtype");
	if(type=="startturn")
	{
		int plyr = msg.getInt("player");
		Mana[plyr] += 2;
		std::cout << Mana[0] << " " << Mana[1] << " manas\n";
		for(int i = 0;i<VerticalTileCount;i++)
		{
			for(int j = 0;j<HorizontalTileCount;j++)
			{
				int id = getUnitAt(j,i);
				if(id!=-1)
				{
					if(plyr==units.at(id).Owner)
					{
						for(int k = 0;k<units.at(id).abilityCount;k++) //lower cooldowns
						{
							if(units.at(id).abilities.at(k).currCooldown>0)
							{
								units.at(id).abilities.at(k).currCooldown -= 1;
							}
						}
						units.at(id).hasAttacked = 0; //refresh attack status
						Message msg("unitready");
						msg.addValue("unit",std::to_string(id));
						MsgMngr.sendMessage(msg);
					}
				}
			}
		}
	}
	else if(type=="endturn")
	{
		int plyr = msg.getInt("player");
		for(int i = 0;i<VerticalTileCount;i++)
		{
			for(int j = 0;j<HorizontalTileCount;j++)
			{
				int id = getUnitAt(j,i);
				if(id!=-1)
				{
					for(int i = 0;i<units.at(id).buffs.size();i++)
					{
						if(units.at(id).buffs.at(i).Duration!=-1)
						{
							units.at(id).buffs.at(i).Duration--; //reduce buff duration
							if(units.at(id).buffs.at(i).Duration==0)
							{
								units.at(id).buffs.erase(units.at(id).buffs.begin()+i);
								i--;
							}
						}
					}
					if(units.at(id).timedLife>0)
					{
						units.at(id).timedLife--;
					}
				}
			}
		}
		deadUnitCheck();
	}
	else if(type=="dodeadunitcheck")
	{
		deadUnitCheck();
	}
	else if(type=="unitsummon")
	{
		summonUnit(msg.getString("unittype"),msg.getInt("x"),msg.getInt("y"),msg.getInt("owner"));
	}
	else if(type=="unitmove")
	{
		int uid = msg.getInt("unit");
		int j = msg.getInt("targetx");
		int i = msg.getInt("targety");
		units.at(uid).MovePoints -= getUnitTileEnterCost(uid,units.at(uid).x,units.at(uid).y,j,i)
			+ getUnitTileExitCost(uid,units.at(uid).x,units.at(uid).y,j,i);
		moveUnit(uid,j,i);
	}
	else if(type=="unitattack")
	{
		int uid = msg.getInt("unit");
		int target = msg.getInt("target");

		Message msg("unitdamage");
		msg.addValue("source",uid);
		msg.addValue("target",target);
		msg.addValue("amount",getUnitAttack(uid));
		msg.addValue("damagetype",DAMAGETYPE_ATTACK);
		MsgMngr.sendMessage(msg);

		units.at(uid).hasAttacked = 1;

		Animation a("Attack1","Attack",getSpritePos(units.at(target).x,units.at(target).y),1);
		ActiveAnimations.push_back(a);
		ActiveAnimations.at(ActiveAnimations.size()-1).play();
	}
	else if(type=="unitdamage")
	{
		int src = msg.getInt("source");
		int target = msg.getInt("target");
		int amt = msg.getInt("amount");
		damageUnit(src,target,amt);
		deadUnitCheck();
	}
	else if(type=="unitdamagearea")
	{
		int src = msg.getInt("source");
		int tx = msg.getInt("targetx");
		int ty = msg.getInt("targety");
		int range = msg.getInt("range");
		int amt = msg.getInt("amount");
		int flags = msg.getInt("targetflags");
		damageUnitArea(src,tx,ty,range,amt,flags);
		deadUnitCheck();
	}
	else if(type=="unitheal")
	{
		int target = msg.getInt("target");
		units.at(target).Life = units.at(target).Life+msg.getInt("amount");
		deadUnitCheck();
	}
	else if(type=="unitdestroy")
	{
		units.at(msg.getInt("unit")).Life = 0;
		deadUnitCheck();
	}
	else if(type=="unitteleport")
	{
		moveUnit(msg.getInt("unit"),msg.getInt("targetx"),msg.getInt("targety"));
	}
	else if(type=="unitdeath")
	{
		int uid = msg.getInt("unit");
		tiles[units.at(uid).x][units.at(uid).y].isOccupied = false;
		//units.erase(uid);
		units.at(uid).UniqueId = -1;
		if(SelectionId == uid)
		{
			SelectionId = -1;
		}
	}
	else if(type=="unitready")
	{
		units.at(msg.getInt("unit")).MovePoints = 6;
	}
	else if(type=="unitsetlife")
	{
		units.at(msg.getInt("unit")).Life = msg.getInt("amount");
	}
	else if(type=="unitsetmaxlife")
	{
		units.at(msg.getInt("unit")).maxLife = msg.getInt("amount");
	}
	else if(type=="unitsetbaseattack")
	{
		units.at(msg.getInt("unit")).Attack = msg.getInt("amount");
	}
	else if(type=="unitsetmovepoints")
	{
		units.at(msg.getInt("unit")).MovePoints = msg.getInt("amount");
	}
	else if(type=="unitchangeowner")
	{
		units.at(msg.getInt("unit")).Owner = msg.getInt("owner");
	}
	else if(type=="unitsettimedlife")
	{
		units.at(msg.getInt("unit")).timedLife = msg.getInt("amount");
	}
	else if(type=="buffcreate")
	{
		Buff b(msg.getInt("bufftype"),msg.getInt("target"),msg.getInt("duration"),msg.getInt("owner"),msg.getInt("ispermanent"),0,0);
		units.at(msg.getInt("target")).buffs.push_back(b);
	}
	else if(type=="buffremove")
	{
		int uid = msg.getInt("target");
		int bid = msg.getInt("bufftype");
		for(int i = 0;i<world.units.at(uid).buffs.size();i++)
		{
			if(world.units.at(uid).buffs.at(i).BuffType==bid)
			{
				units.at(uid).buffs.erase(world.units.at(uid).buffs.begin()+i);
				i--;
			}
		}
	}
	else if(type=="buffdispel")
	{
		int uid = msg.getInt("unit");
		for(int i = 0;i<world.units.at(uid).buffs.size();i++)
		{
			if(!(world.units.at(uid).buffs.at(i).Flags&BUFF_NONDISPELLABLE))
			{
				units.at(uid).buffs.erase(world.units.at(uid).buffs.begin()+i);
				i--;
			}
		}
	}
	else if(type=="buffsetduration")
	{
		units.at(msg.getInt("unit")).buffs.at(msg.getInt("buff")).Duration = msg.getInt("duration");
	}
	else if(type=="spellsetcurrentcooldown")
	{
		units.at(msg.getInt("unit")).abilities.at(msg.getInt("spell")).currCooldown = msg.getInt("cooldown");
	}
	else if(type=="tiletypechange")
	{
		world.tiles[msg.getInt("x")][msg.getInt("y")].setType(msg.getInt("tiletype"));
	}
	else if(type=="playanimation")
	{
		Animation a(msg.getString("animation"),msg.getString("sound"),getSpritePos(msg.getInt("x"),msg.getInt("y")),atof(msg.getString("scalingfactor").c_str()));
		ActiveAnimations.push_back(a);
		ActiveAnimations.at(ActiveAnimations.size()-1).play();
	}
	else if(type=="playsound")
	{
		ActiveAnimations.push_back(Animation("Attack1",msg.getString("sound"),0,0,1.0,1));
		ActiveAnimations.at(ActiveAnimations.size()-1).play();
	}
	return 0;
}

void World::dispatchMessage(Message& msg)
{
	std::string type = msg.getString("msgtype");
	std::map<int,Unit>::iterator i;

	currentMessage = msg;

	currentMessage.addValue("msgtype","mod "+type);
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(msg);
	}

	//std::cout << "  mod\n";
	if(currentMessage.getInt("msgContinue")==0) //do we continue?
		return;

	currentMessage.addValue("msgtype","pre "+type);
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(msg);
	}

	//std::cout << "  pre\n";
	if(currentMessage.getInt("msgContinue")==0)
		return;

	currentMessage.addValue("msgtype",type);
	handleMessage(currentMessage);
	//std::cout << "  in\n";

	currentMessage.addValue("msgtype","post "+type);
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(currentMessage);
	}

	//std::cout << "  post\n";
}

int World::deadUnitCheck()
{
	std::map<int,Unit>::iterator i;
	std::vector<int> markedForDeath;
	for(i = units.begin();i!=units.end();++i)
	{
		if(unitExists(i->first))
		{
			if(i->second.Life <= 0)
			{
				markedForDeath.push_back(i->first);
				std::cout << "marked " << i->first << "\n";
			}
			else if(i->second.timedLife == 0)
			{
				markedForDeath.push_back(i->first);
				std::cout << "marked " << i->first << "\n";
			}
			else if(i->second.Life > i->second.maxLife) //check if Life is greater than Max Life
			{
				i->second.Life = i->second.maxLife;
			}
		}
	}
	for(int j = 0;j<markedForDeath.size();j++)
	{
		Message msg("unitdeath");
		msg.addValue("unit",markedForDeath.at(j));
		MsgMngr.sendMessage(msg);
	}
	return 0;
}

int World::getUnitAt(int x,int y)
{
	std::map<int,Unit>::iterator i;
	for(i = units.begin();i!=units.end();++i)
	{
		if(unitExists(i->first))
		{
			if(i->second.x == x && i->second.y == y)
			{
				return i->second.UniqueId;
			}
		}
	}
	return -1;
}

int World::getUnitTileEnterCost(int uid,int x1,int y1,int x2,int y2)
{
	Message oldmsg = currentMessage;
	currentMessage = Message("get unittileentercost");
	currentMessage.addValue("entercost",TileEnterCost[tiles[x2][y2].type]);
	currentMessage.addValue("unit",uid);
	currentMessage.addValue("fromx",x1);
	currentMessage.addValue("fromy",y1);
	currentMessage.addValue("tox",x2);
	currentMessage.addValue("toy",y2);

	std::map<int,Unit>::iterator i;
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(currentMessage);
	}
	int c = currentMessage.getInt("entercost");
	currentMessage = oldmsg;
	return c;
}

int World::getUnitTileExitCost(int uid,int x1,int y1,int x2,int y2)
{
	Message oldmsg = currentMessage;
	currentMessage = Message("get unittileexitcost");
	currentMessage.addValue("exitcost",TileExitCost[tiles[x1][y1].type]);
	currentMessage.addValue("unit",uid);
	currentMessage.addValue("fromx",x1);
	currentMessage.addValue("fromy",y1);
	currentMessage.addValue("tox",x2);
	currentMessage.addValue("toy",y2);

	std::map<int,Unit>::iterator i;
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(currentMessage);
	}
	int c = currentMessage.getInt("exitcost");
	currentMessage = oldmsg;
	return c;
}

int World::getUnitAttack(int uid)
{
	Message oldmsg = currentMessage;
	currentMessage = Message("get unitattack");
	currentMessage.addValue("attack",units.at(uid).Attack);
	currentMessage.addValue("unit",uid);

	std::map<int,Unit>::iterator i;
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(currentMessage);
	}
	int c = currentMessage.getInt("attack");
	currentMessage = oldmsg;
	return c;
}

int World::getUnitAttackRange(int uid)
{
	Message oldmsg = currentMessage;
	currentMessage = Message("get unitattackrange");
	currentMessage.addValue("attackrange",units.at(uid).AttackRange);
	currentMessage.addValue("unit",uid);

	std::map<int,Unit>::iterator i;
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(currentMessage);
	}
	int c = currentMessage.getInt("attackrange");
	currentMessage = oldmsg;
	return c;
}

int World::getUnitAttackCost(int uid)
{
	Message oldmsg = currentMessage;
	currentMessage = Message("get unitattackcost");
	currentMessage.addValue("attackcost",2);
	currentMessage.addValue("unit",uid);

	std::map<int,Unit>::iterator i;
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(currentMessage);
	}
	int c = currentMessage.getInt("attackcost");
	currentMessage = oldmsg;
	return c;
}

int World::getUnitCanAttack(int uid)
{
	Message oldmsg = currentMessage;
	currentMessage = Message("get unitcanattack");
	currentMessage.addValue("canattack",1);
	currentMessage.addValue("unit",uid);

	std::map<int,Unit>::iterator i;
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(currentMessage);
	}
	int c = currentMessage.getInt("canattack");
	currentMessage = oldmsg;
	return c;
}

int World::getUnitCanCast(int uid)
{
	Message oldmsg = currentMessage;
	currentMessage = Message("get unitcancast");
	currentMessage.addValue("cancast",1);
	currentMessage.addValue("unit",uid);

	std::map<int,Unit>::iterator i;
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(currentMessage);
	}
	int c = currentMessage.getInt("cancast");
	currentMessage = oldmsg;
	return c;
}

int World::getUnitCanMove(int uid)
{
	Message oldmsg = currentMessage;
	currentMessage = Message("get unitcanmove");
	currentMessage.addValue("canmove",1);
	currentMessage.addValue("unit",uid);

	std::map<int,Unit>::iterator i;
	for(i = units.begin();i!=units.end();i++)
	{
		if(unitExists(i->first))
			i->second.handleMessage(currentMessage);
	}
	int c = currentMessage.getInt("canmove");
	currentMessage = oldmsg;
	return c;
}

void World::summonUnit(int type,int x,int y,int owner)
{
	Unit u(type,x,y,owner,nextValidUnitId);
	tiles[x][y].isOccupied = true;
	units.insert(std::pair<int,Unit>(nextValidUnitId,u));

	/*lua_getglobal(LuaUnits,UnitNames.at(type).c_str());
	lua_getfield(LuaUnits,-1,"OnSummon");
	lua_pushinteger(LuaUnits,nextValidUnitId);
	lua_pcall(LuaUnits,1,0,0);*/

	nextValidUnitId++;
}

void World::damageUnit(int src,int target,int amount)
{
	if(amount>0)
		units.at(target).Life = units.at(target).Life-amount;
}

void World::damageUnitArea(int src,int x,int y,int range,int amount,int flags)
{
	int myowner = units.at(src).Owner;
	for(int i = x-range;i<=x+range;i++)
	{
		for(int j = y-range;j<=y+range;j++)
		{
			int uid = getUnitAt(i,j);
			if(uid!=-1 && (uid!=src || !(flags&TARGET_NONSELF)))
			{
				if(isUnitAvatar(uid)==0 || !(flags&TARGET_NONAVATAR))
				{
					int owner = units.at(uid).Owner;
					if(flags&TARGET_ALLY && !(flags&TARGET_ENEMY) && owner==myowner)  //damage only allies
						damageUnit(src,uid,amount);
					else if(!(flags&TARGET_ALLY) && flags&TARGET_ENEMY && owner!=myowner)  //damage only enemies
						damageUnit(src,uid,amount);
					else if(!(flags&TARGET_ALLY) && !(flags&TARGET_ENEMY))  //damage both
						damageUnit(src,uid,amount);
					else if(flags&TARGET_ALLY && flags&TARGET_ENEMY)  //damage both
						damageUnit(src,uid,amount);
				}
			}
		}
	}
}

void World::summonUnit(std::string name,int x,int y,int owner)
{
	summonUnit(getUnitTypeFromName(name),x,y,owner);
}

void World::moveUnit(int id,int x,int y)
{
	tiles[units.at(id).x][units.at(id).y].isOccupied = false;
	units.at(id).x = x;
	units.at(id).y = y;
	tiles[x][y].isOccupied = true;
	units.at(id).sprite.setPosition(getSpritePos(x,y).x+5,getSpritePos(x,y).y+5);
}

int World::unitExists(int uid)
{
	if(units.at(uid).UniqueId==-1)
	{
		//units.erase(uid);
		return 0;
	}
	return 1;
}

int World::isUnitAvatar(int uid)
{
	if(units.at(uid).UnitType==0)
		return 1;
	return 0;
}

void World::castSpell(int castingunit,int castingspell) //not used
{

}