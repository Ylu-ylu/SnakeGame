#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "Game.h"

#include "GameStatePlaying.h"
#include "Snake.h"
#include <cstdlib>
#include <windows.h>
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

using namespace sf;

using namespace SnakeGame;



	int main()
	{
		
		ShowWindow(GetConsoleWindow(), SW_HIDE);//for hide console

		unsigned int seed = (unsigned int)time(nullptr);
		srand(seed);

		//Init window
		sf::RenderWindow window(sf::VideoMode(SnakeGame::SCREEN_WIDTH, SnakeGame::SCREEN_HEIGHT), "SnakeGame!");
		
		// We now use too much memory for stack, so we need to allocate it on heap
		SnakeGame::Game* game = new SnakeGame::Game();
		InitGame(*game);

		//Init Time
		sf::Clock gameClock;		

		sf::Time lastTime = gameClock.getElapsedTime();
		FoodsGrid foodGrid;		

		while (window.isOpen())
		{
			HandleWindowEvents(*game, window);

			if (!window.isOpen())
			{
				break;
			}

			//Reduce framerate to not spawn CPU and GPU
			sf::sleep(sf::milliseconds(16));

			//Calculate time delta
			sf::Time  currentTime = gameClock.getElapsedTime();
			float deltaTime = currentTime.asSeconds() - lastTime.asSeconds();
			lastTime = currentTime;		
			
			if (UpdateGame(*game, deltaTime))
			{
				// Draw everything here
			// Clear the window first
				window.clear();

				DrawGame(*game, window);				
				window.display();
			}
			else
			{
				window.close();
				break;
			}
		}
		ShutdownGame (*game);
		delete game;
		game = nullptr;

		window.close();

		return 0;
	}
