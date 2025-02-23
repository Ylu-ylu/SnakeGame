#include "Rock.h"
#include "Game.h"
#include "GameStatePlaying.h"
#include "Food.h"
#include "Snake.h"
#include "GameSettings.h"


namespace SnakeGame
{
	void InitRock(Rock& rock, const Game& game, GameStatePlayingData& data, bool isTemporary)
	{
		//Initial Rocks
		rock.sprite.setTexture(data.rockTexture);
		SetSpriteSize(rock.sprite, ROCK_SIZE, ROCK_SIZE);
		SetSpriteRelativeOrigin(rock.sprite, 0.5f, 0.5f);

        // Set temporary status
        rock.isTemporary = isTemporary;
        rock.isVisible = true;
        rock.visibilityTimer = 0.0f;
        rock.invisibilityTimer = 0.0f;
	}

	void DrawRock(Rock& rock, sf::RenderWindow& window)
	{
        if (rock.isVisible) 
        {
            rock.sprite.setPosition(rock.rocksPosition.x, rock.rocksPosition.y);
            window.draw(rock.sprite);
        }
	}

	Rectangle GetRockCollider(const Rock& rock)
	{
		return { { rock.rocksPosition.x - ROCK_SIZE / 2.f, rock.rocksPosition.y - ROCK_SIZE / 2.f },
					{ ROCK_SIZE, ROCK_SIZE } };
	}

	void SetRockPosition(Rock& rock, const Position2D& rocksPosition, const Snake& snake, const GameStatePlayingData& data)
	{
        // Calculate grid cell size
        const float cellSizeX = static_cast<float>(SCREEN_WIDTH) / FoodS_GRID_CELLS_HORIZONTAL;
        const float cellSizeY = static_cast<float>(SCREEN_HEIGHT) / FoodS_GRID_CELLS_VERTICAL;

        bool positionValid = false;

        while (!positionValid)
        {
            // Pick a random grid cell
            int gridX = rand() % (FoodS_GRID_CELLS_HORIZONTAL - 2) + 1;
            int gridY = rand() % (FoodS_GRID_CELLS_VERTICAL - 2) + 1;

            // Calculate center position of the chosen cell
            float posX = gridX * cellSizeX + (cellSizeX / 2.0f);
            float posY = gridY * cellSizeY + (cellSizeY / 2.0f);

            // Check if the position is valid (not overlapping with the snake's body)
            positionValid = true;
            for (const auto& segment : snake.body)
            {
                sf::Vector2f snakePos = segment.getPosition();
                if (std::abs(snakePos.x - posX) < SNAKE_SIZE && std::abs(snakePos.y - posY) < SNAKE_SIZE)
                {
                    positionValid = false;
                    break;
                }
            }
            // Check collision with foods
            if (positionValid)
            {
                rock.rocksPosition.x = posX;
                rock.rocksPosition.y = posY;
                rock.sprite.setPosition(posX, posY);
                // Check if the new position is on top of food
                for (const auto& food : data. food)
                {
                    if (std::abs(rock.rocksPosition.x - food.foodsPosition.x) < Food_SIZE &&
                        std::abs(rock.rocksPosition.y - food.foodsPosition.y) < Food_SIZE)
                    {
                        positionValid = false;
                        break;
                    }
                }
            }       
           
        }
	}
    void UpdateRock(float deltaTime, DifficultyParams& difficulty, Rock& rock, const Snake& snake, GameStatePlayingData& data)
    {
        Game game;
       
        if (!rock.isTemporary || !difficulty.enableTemporaryRocks)
        {
           rock.isVisible =  true;
            return;
        }
        if (rock.isTemporary)
        {
            rock.visibilityTimer += deltaTime;

            rock.visibilityTimer += deltaTime;
            if (rock.visibilityTimer >= difficulty.rockVisibilityDuration)
            {
                rock.isVisible = false;
                rock.visibilityTimer = 0.0f;
            }
            else if (rock.visibilityTimer >= difficulty.rockInvisibilityDuration)
            {
                rock.isVisible = true;
                rock.visibilityTimer = 0.0f;

                // Update rock position
                Position2D randomPos = GetRandomPositionRectangle(rock.sprite, game.screenRect);
                SetRockPosition(rock, rock.rocksPosition, snake, data);
            }
        } 
        else 
        {
        // Update rock position normally
        Position2D randomPos = GetRandomPositionRectangle(rock.sprite, game.screenRect);
        SetRockPosition(rock, randomPos, snake,data);
        }        
        
    }    
}


