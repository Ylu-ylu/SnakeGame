#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "UI.h"
#include "Constants.h"
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <functional>


using namespace std;

namespace SnakeGame
{
	struct Game;
	struct GameStatePlayingData;
	
	struct Food
	{
		Position2D foodsPosition;
		sf::Sprite sprite;

		bool isEaten = false;
	};
	struct FoodPointerHash
	{
		std::size_t operator()(const Food* food) const
		{
			return Vector2Df::Vector2DfHash{}(food->foodsPosition);
		}
	};

	struct FoodPointerEqual
	{
		bool operator()(const Food* lhs, const Food* rhs) const
		{
			return lhs->foodsPosition == rhs->foodsPosition;
		}
	};

	using FoodsSet = std::unordered_set<Food*, FoodPointerHash, FoodPointerEqual>;
	

	struct FoodsGrid
	{
		std::unordered_map<Vector2D, FoodsSet, Vector2D::Vector2DHash> cells;
		std::unordered_multimap<Food*, Vector2D> foodCells;

		sf::RectangleShape gridCell; // For drawing colored cells
	};

	void InitFood(Food& food, const Game& game, GameStatePlayingData& data);
	void DrawFood(Food& food, sf::RenderWindow& window);		
	void ResetFoodState(Food& foods);
	//For Foods Cells
	void ClearFoodGrid(FoodsGrid& foodGrid);
	void AddFoodToGrid(FoodsGrid& foodGrid, Food& foods, Snake& snake);
	void RemoveFoodFromGrid(FoodsGrid& foodsGrid, Food& food);
	bool FindPlayerCollisionWhithFoods(const Vector2Df& snakePosition, const FoodsGrid& grid_in, FoodsSet& result);
	void MarkFoodAsEaten(Food& food);
	void DrawFoodGrid(const FoodsGrid& foodGrid, sf::RenderWindow& window);
}
