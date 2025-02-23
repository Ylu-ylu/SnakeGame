#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <vector>
#include "Constants.h"
#include "Food.h"
#include "Snake.h"
#include "Math.h"


namespace SnakeGame
{
	struct Game;
	struct UIState;
	struct Rock;
	struct GameStateGameOverData;
	struct Snake;
	struct Vector2Df;	

	struct GameStatePlayingData
	{
		//Game Object
		sf::RectangleShape background;
		sf::RectangleShape gameRectScores;
		sf::Sound appleEatSound;
		sf::Sound gameOverSound;
		sf::Sound gameBackgroundSound;
		//Recourse
		sf::Texture snakeTexture;
		sf::Texture appleTexture;
		sf::Texture rockTexture;		

		sf::SoundBuffer eatFoodoundBuffer;
		sf::SoundBuffer gameOverSoundBuffer;
		sf::SoundBuffer gameBackgroundSoundBuffer;

		sf::Font font;

		sf::Text inputHintText;

		// Game data
		Snake snake;

		std::vector < Food> food;
		float deltaTime;

		std::vector <Rock> rocks;
			
		//FoodsGrid foodGrid;
		int numFood = 0;
		int numEatenFood = 0;

		// Add pause timer
		float pauseTimer;
		bool isGameStarted;
	};

	void InitGameStatePlaying(GameStatePlayingData& data, Game& game);
	void ShutdownGameStatePlaying(GameStatePlayingData& data, Game& game);
	void HandleGameStatePlayingWindowEvent(GameStatePlayingData& data, Game& game, const sf::Event& event);
	void UpdateGameStatePlaying(GameStatePlayingData& data, Game& game, float deltaTime);
	void DrawGameStatePlaying(GameStatePlayingData& data, Game& game, sf::RenderWindow& window);		
}