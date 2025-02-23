#pragma once
#include <string>

namespace SnakeGame
{
	//Resourse

	const std::string RESOURCES_PATH = "Resources/";

	//Game settings
	const int SCREEN_WIDTH = 900;
	const int SCREEN_HEIGHT = 600;
	
	//Snake
	const float SNAKE_SIZE = 30.f;
	const unsigned int INITIAL_SNAKE_SIZE = 3;

	const float INITIAL_SPEED = 100.f;
	const int NUM_FoodS = 1;
	const float Food_SIZE = 30.f;
	
	const float ROCK_SIZE = 30.f;
	
	//Food Grid
	const unsigned FoodS_GRID_CELLS_HORIZONTAL = unsigned(SCREEN_WIDTH / (Food_SIZE * 1));
	const unsigned FoodS_GRID_CELLS_VERTICAL = unsigned(SCREEN_HEIGHT / (Food_SIZE * 1));

	extern const char* snake_NAME;//Needto defie this constant in SPP;

	const float PAUSE_TO_MOVE = 3.0f;
	
	//Sound and music
	const float SOUND_VOLUME = 100.0f;
	const float MUSIC_VOLUME = 30.0f;


}
