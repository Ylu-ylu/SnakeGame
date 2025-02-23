#pragma once

#include "Constants.h"
#include "Math.h"


namespace SnakeGame
{
	struct Game;
	struct GameStatePlayingData;
	struct Rectangle;
	struct FoodsGrid; 
	struct Snake;
	struct DifficultyParams;
	struct Food;

	struct Rock
	{
		//Rocks Data
		Position2D rocksPosition;
		sf::Sprite sprite;			
		
		float visibilityTimer = 0.0f;
		float invisibilityTimer=0.0f;
		bool isVisible = true;
		bool isTemporary=false;

	};

	void InitRock(Rock& rock, const Game& game, GameStatePlayingData& data, bool isTemporary);
	void DrawRock(Rock& rock, sf::RenderWindow& window);
	Rectangle GetRockCollider(const Rock& rock);	
	void SetRockPosition(Rock& rock, const Position2D& rocksPosition, const Snake& snake, const GameStatePlayingData& data);	
	void UpdateRock(float deltaTime,  DifficultyParams& difficulty, Rock& rock, const Snake& snake, GameStatePlayingData& data);
}
