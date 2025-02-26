#include "Food.h"
#include "Game.h"
#include <string>
#include "GameStatePlaying.h"
#include <cstdlib>
#include <assert.h>
#include "Snake.h"

namespace SnakeGame
{
	void InitFood(Food& food, const Game& game, GameStatePlayingData& data)
	{
		//Initial Food
		food.sprite.setTexture(data.appleTexture);
		SetSpriteSize (food.sprite, Food_SIZE, Food_SIZE);
		SetSpriteRelativeOrigin(food.sprite, 0.5f, 0.5f);
	}
	void DrawFood(Food& food, sf::RenderWindow& window)
	{
		if (food.isEaten)
		{
			return;
		}

		food.sprite.setPosition(food.foodsPosition.x, food.foodsPosition.y);
		window.draw(food.sprite);
	}		
	void ResetFoodState(Food& food)
	{
		const float cellSizeX = static_cast<float>(SCREEN_WIDTH) / FoodS_GRID_CELLS_HORIZONTAL;
		const float cellSizeY = static_cast<float>(SCREEN_HEIGHT) / FoodS_GRID_CELLS_VERTICAL;

		// Generate random grid indices
		int gridX = rand() % (FoodS_GRID_CELLS_HORIZONTAL);
		int gridY = rand() % (FoodS_GRID_CELLS_VERTICAL);

		// Calculate the center of the selected grid cell
		food.foodsPosition.x = gridX * cellSizeX + (cellSizeX / 2.0f);
		food.foodsPosition.y = gridY * cellSizeY + (cellSizeY / 2.0f);

		food.isEaten = false;
	}
	void MarkFoodAsEaten(Food& food)
	{
		food.isEaten = true;
	}
	void ClearFoodGrid(FoodsGrid& foodGrid)
	{
		foodGrid.foodCells.clear();
		foodGrid.cells.clear();
	}
	void AddFoodToGrid(FoodsGrid& foodGrid, Food& food, Snake& snake)
	{
		// Remove the Food from its old position in the grid
		RemoveFoodFromGrid(foodGrid, food);

		// Calculate grid cell size
		const float cellSizeX = static_cast<float>(SCREEN_WIDTH) / FoodS_GRID_CELLS_HORIZONTAL;
		const float cellSizeY = static_cast<float>(SCREEN_HEIGHT) / FoodS_GRID_CELLS_VERTICAL;

		bool foodPlaced = false;
		while (!foodPlaced)
		{
			// Pick a random grid cell
			int gridX = rand() % (FoodS_GRID_CELLS_HORIZONTAL - 2) + 1;
			int gridY = rand() % (FoodS_GRID_CELLS_VERTICAL - 2) + 1;

			// Calculate center position of the chosen cell
			float posX = gridX * cellSizeX + (cellSizeX / 2.0f);
			float posY = gridY * cellSizeY + (cellSizeY / 2.0f);

			// Check if the food position overlaps with the snake's body
			bool overlapsWithSnake = false;
			for (const auto& segment : snake.body)
			{
				if (segment.getPosition() == sf::Vector2f(posX, posY))
				{
					overlapsWithSnake = true;
					break;
				}
			}

			// If the position is valid, place the food
			if (!overlapsWithSnake)
			{
				food.foodsPosition.x = posX;
				food.foodsPosition.y = posY;
				food.sprite.setPosition(posX, posY);

				// Add food to grid tracking
				Vector2D cellCoord = { gridX, gridY };
				foodGrid.cells[cellCoord].insert(&food);
				foodGrid.foodCells.insert({ &food, cellCoord });

				foodPlaced = true;
			}
		}
	}
	void RemoveFoodFromGrid(FoodsGrid& foodsGrid, Food& food)
	{
		auto range = foodsGrid.foodCells.equal_range(&food);
		for (auto it = range.first; it != range.second; ++it)
		{
			foodsGrid.cells[it->second].erase(&food);

		}
		foodsGrid.foodCells.erase(range.first, range.second);
	}
	bool FindPlayerCollisionWhithFoods(const Vector2Df& snakePosition, const FoodsGrid& grid_in, FoodsSet& result)
	{
		result.clear();  // Clear the result set before starting


		const float cellSizeX = static_cast<float>(SCREEN_WIDTH) / FoodS_GRID_CELLS_HORIZONTAL;
		const float cellSizeY = static_cast<float>(SCREEN_HEIGHT) / FoodS_GRID_CELLS_VERTICAL;

		const float playerHalfSize = SNAKE_SIZE / 2.0f-cellSizeX/4.0f;
		const Vector2Df FoodCornerTL = { snakePosition.x - playerHalfSize,snakePosition.y - playerHalfSize };
		const Vector2Df FoodCornerBR = { snakePosition.x + playerHalfSize,	snakePosition.y + playerHalfSize };

		const int minCellX = std::max(static_cast<int>(FoodCornerTL.x / cellSizeX), 0);
		const int maxCellX = std::min(static_cast<int>(FoodCornerBR.x / cellSizeX), (int)(FoodS_GRID_CELLS_HORIZONTAL - 1));
		const int minCellY = std::max(static_cast<int>(FoodCornerTL.y / cellSizeY), 0);
		const int maxCellY = std::min(static_cast<int>(FoodCornerBR.y / cellSizeY), (int)(FoodS_GRID_CELLS_VERTICAL - 1));

		const float collisionDistanceSquared = (SNAKE_SIZE / 2 + Food_SIZE / 2);

		for (int x = minCellX; x <= maxCellX; ++x)
		{
			for (int y = minCellY; y <= maxCellY; ++y)
			{
				Vector2D cellCoord = { x, y };
				const auto it = grid_in.cells.find(cellCoord);
				if (it != grid_in.cells.cend())
				{

					for (Food* Food : it->second)
					{
						const float dx = snakePosition.x - Food->foodsPosition.x;
						const float dy = snakePosition.y - Food->foodsPosition.y;

						const float distanceSquared = sqrt(dx * dx + dy * dy);

						if (distanceSquared < collisionDistanceSquared)
						{
							result.insert(Food);

						}
					}
				}
			}

		}

		return !result.empty();
	}

	//Draw food grid

	void DrawFoodGrid(const FoodsGrid& foodGrid, sf::RenderWindow& window)
	{
		const float cellSizeX = static_cast<float>(SCREEN_WIDTH) / FoodS_GRID_CELLS_HORIZONTAL;
		const float cellSizeY = static_cast<float>(SCREEN_HEIGHT) / FoodS_GRID_CELLS_VERTICAL;

		sf::RectangleShape cellRect;
		cellRect.setSize(sf::Vector2f(cellSizeX - 1, cellSizeY - 1));
		cellRect.setOutlineThickness(1.0f);
		cellRect.setOutlineColor(sf::Color(50, 50, 50));

		// Draw grid with alternating colors
		for (int x = 0; x < FoodS_GRID_CELLS_HORIZONTAL; ++x)
		{
			for (int y = 0; y < FoodS_GRID_CELLS_VERTICAL; ++y)
			{
				cellRect.setPosition(x * cellSizeX, y * cellSizeY);

				// Create checkerboard pattern
				if ((x + y) % 2 == 0) 
				{
					cellRect.setFillColor(sf::Color(20, 80, 20)); // Darker green
				}
				else 
				{
					cellRect.setFillColor(sf::Color(168, 232, 182)); // Lighter green
				}

				// Highlight cells containing food
				Vector2D cellCoord = { x, y };
				auto it = foodGrid.cells.find(cellCoord);
				if (it != foodGrid.cells.end() && !it->second.empty()) 
				{
					cellRect.setFillColor(sf::Color(100, 30, 30)); // Red tint for food cells
				}

				window.draw(cellRect);
			}
		}
	}

}

