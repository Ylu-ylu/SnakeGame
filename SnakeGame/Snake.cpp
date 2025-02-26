#include "Snake.h"
#include "Game.h"
#include "Math.h"
#include "GameStatePlaying.h"
#include <assert.h>
#include <cmath>
#include "Food.h"

namespace SnakeGame
{
	void LoadSnakeTextures(Snake& snake)
	{
		assert(snake.textures[(size_t)SnakePart::Head].loadFromFile(RESOURCES_PATH + "Head.png"));
		assert(snake.textures[(size_t)SnakePart::Body].loadFromFile(RESOURCES_PATH + "Body.png"));
		assert(snake.textures[(size_t)SnakePart::BodyBend].loadFromFile(RESOURCES_PATH + "BodyBend.png"));
		assert(snake.textures[(size_t)SnakePart::Tail].loadFromFile(RESOURCES_PATH + "Tail.png"));
	}

	void InitSnake(Snake& snake)
	{
		// Calculate grid cell size
		const float cellSizeX = static_cast<float>(SCREEN_WIDTH) / FoodS_GRID_CELLS_HORIZONTAL;
		const float cellSizeY = static_cast<float>(SCREEN_HEIGHT) / FoodS_GRID_CELLS_VERTICAL;

		// Calculate center grid cell
		int centerGridX = FoodS_GRID_CELLS_HORIZONTAL / 2;
		int centerGridY = FoodS_GRID_CELLS_VERTICAL / 2;

		// Calculate exact center position of the cell
		float centerX = centerGridX * cellSizeX + (cellSizeX / 2.0f);
		float centerY = centerGridY * cellSizeY + (cellSizeY / 2.0f);

		auto addSprite = [](Snake& snake, const sf::Texture& texture, const sf::Vector2f& position)
			{
				snake.sprite.setTexture(texture);
				SetSpriteSize(snake.sprite, SNAKE_SIZE, SNAKE_SIZE);
				SetSpriteRelativeOrigin(snake.sprite, 0.5f, 0.5f);
				snake.sprite.setPosition(position);
				snake.body.push_front(snake.sprite);
			};

		// Place head at grid center
		addSprite(
			snake,
			snake.textures[(size_t)SnakePart::Head],
			
			{ centerX, centerY }
		);

		// Place body segments below the head (since initial direction is Up)
		for (int i = 1; i < INITIAL_SNAKE_SIZE-1; ++i)
		{
			addSprite(
				snake,
				snake.textures[(size_t)SnakePart::Body],
				{ centerX, centerY + i * SNAKE_SIZE }
			);
		}


		// Place tail one cell above (since initial direction is Up)
		addSprite(
			snake,
			snake.textures[(size_t)SnakePart::Tail],
			{ centerX, centerY + SNAKE_SIZE * (INITIAL_SNAKE_SIZE - 1) }
		);

		snake.head = --snake.body.end();
		snake.tail = snake.body.begin();
		snake.speed = INITIAL_SPEED;
		snake.prevDirection = snake.direction = SnakeDirection::Up;

	}

	sf::Vector2f GetDirectionVector(SnakeDirection direction)
	{
		sf::Vector2f result;

		switch (direction)
		{
		case SnakeDirection::Up:
		{
			result = { 0.f, -SNAKE_SIZE };
			break;
		}
		case SnakeDirection::Right:
		{
			result = { SNAKE_SIZE, 0.f };
			break;
		}
		case SnakeDirection::Down:
		{
			result = { 0.f, SNAKE_SIZE };
			break;
		}
		case SnakeDirection::Left:
		{
			result = { -SNAKE_SIZE, 0.f };
			break;
		}
		}
		return result;

	}
	bool IsOppositeDirection(SnakeDirection current, SnakeDirection newDirection)
	{
		return (current == SnakeDirection::Up && newDirection == SnakeDirection::Down) ||
			(current == SnakeDirection::Down && newDirection == SnakeDirection::Up) ||
			(current == SnakeDirection::Left && newDirection == SnakeDirection::Right) ||
			(current == SnakeDirection::Right && newDirection == SnakeDirection::Left);
	}

	Rectangle GetSnakeCollider(const Snake& snake)
	{				
		return { { snake.head->getPosition().x - (SNAKE_SIZE / 2.f), snake.head->getPosition().y - (SNAKE_SIZE / 2.f )},
					{ SNAKE_SIZE , SNAKE_SIZE} };
	}

	
	void MoveSnake(Snake& snake, float timeDelta)
	{			
		const float cellSizeX = static_cast<float>(SCREEN_WIDTH) / FoodS_GRID_CELLS_HORIZONTAL;
		const float cellSizeY = static_cast<float>(SCREEN_HEIGHT) / FoodS_GRID_CELLS_VERTICAL;

		float moveAmount = snake.speed * timeDelta;
		sf::Vector2f currentPos = snake.head->getPosition();

		// Get current grid position
		int currentGridX = static_cast<int>(currentPos.x / cellSizeX);
		int currentGridY = static_cast<int>(currentPos.y / cellSizeY);

		// Calculate current cell center
		sf::Vector2f currentCellCenter = {
			currentGridX * cellSizeX + (cellSizeX / 2.0f),
			currentGridY * cellSizeY + (cellSizeY / 2.0f)
		};

		// Calculate next cell center based on direction
		sf::Vector2f nextCellCenter = currentCellCenter;
		switch (snake.direction)
		{
		case SnakeDirection::Up:    nextCellCenter.y -= cellSizeY; break;
		case SnakeDirection::Down:  nextCellCenter.y += cellSizeY; break;
		case SnakeDirection::Left:  nextCellCenter.x -= cellSizeX; break;
		case SnakeDirection::Right: nextCellCenter.x += cellSizeX; break;
		}

		// Calculate movement vector towards next cell center
		sf::Vector2f moveVector = nextCellCenter - currentPos;
		float distance = std::sqrt(moveVector.x * moveVector.x + moveVector.y * moveVector.y);

		if (distance > 0)
		{
			sf::Vector2f normalizedDirection = { moveVector.x / distance, moveVector.y / distance };
			sf::Vector2f newPosition = currentPos + normalizedDirection * moveAmount;

			// Handle rotation at cell center
			if (snake.prevDirection != snake.direction &&
				std::abs(currentPos.x - currentCellCenter.x) < SNAKE_SIZE &&
				std::abs(currentPos.y - currentCellCenter.y) < SNAKE_SIZE)
			{
				if (snake.head != snake.body.end() && snake.tail != snake.body.end())
				{
					// Add new segment
					auto bendSegment = snake.body.insert(snake.head, *snake.head);
					if (!bendSegment->getGlobalBounds().intersects(snake.tail->getGlobalBounds()))
					{				
						size_t originalSize = snake.body.size();

						ApplyBendToSegment(*bendSegment, snake.prevDirection, snake.direction, snake.textures[(size_t)SnakePart::BodyBend]);
						bendSegment->setPosition(currentCellCenter);
						// Update head position
						snake.head->setPosition(currentCellCenter);
						SetHeadSprite(snake, snake.head);					
						
						// Remove tail if not growing
						if (!snake.isGrowSnake && snake.body.size() > originalSize)
						{
							//UpdateTailAfterRotation(snake, snake.prevDirection, snake.direction, timeDelta);
							snake.body.erase(snake.tail++);
						}
						// Update tail position
						newPosition = currentCellCenter + normalizedDirection * moveAmount;
						snake.head->setPosition(newPosition);
					}
				}
			}
			else
			{
				// Move the head to the new position
				snake.head->setPosition(newPosition);
			}
			if (snake.body.size() > 1)
			{
				UpdateTail(snake, timeDelta);
			}
		}
		snake.prevDirection = snake.direction;
	}   

		void GrowSnake(Snake & snake)
		{			
			auto newTail = snake.body.insert(snake.tail, *snake.tail);

			// Move new tail segment slightly back
			sf::Vector2f tailDirection = GetVectorBetweenSprites(*std::next(snake.tail), *snake.tail);
			newTail->setPosition(snake.tail->getPosition() + tailDirection);

			// Update tail iterator
			auto oldTail = snake.tail;
			oldTail->setTexture(snake.textures[static_cast<size_t>(SnakePart::Body)]);
			snake.tail = newTail;
			snake.isGrowSnake = true;

		}	

		void DrawSnake(Snake & snake, sf::RenderWindow & window)
		{
			const float PI = 3.14159f;

			// Draw direct parts of the body
			for (auto it = snake.body.begin(); it != snake.head; ++it)
			{
				auto nextIt = std::next(it);
				auto direction = nextIt->getPosition() - it->getPosition();
				float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

				// Calculate actual distance between segments using Euclidean distance
				sf::Vector2f normalizedDirection = { direction.x / distance, direction.y / distance };					
				
				if (distance > SNAKE_SIZE)
				{					
						float width = SNAKE_SIZE;
						//float height = std::min(distance, SNAKE_SIZE);
						//float height = distance-SNAKE_SIZE;
						float height = GetManhattanDistanceBetweenSprites(*it, *nextIt) - SNAKE_SIZE;
						sf::Sprite sprite;

						InitSprite(sprite, width, height, snake.textures[(size_t)SnakeGame::SnakePart::Body]);

						float angle = std::atan2(direction.y, direction.x) * 180.f / PI;
						sprite.setRotation(angle + 90.f); // Add 90 degrees to align with sprite orientation
						
						// Calculate exact midpoint
						sf::Vector2f midPoint = {
							it->getPosition().x + direction.x * 0.5f,
							it->getPosition().y + direction.y * 0.5f
						};
						sprite.setPosition(midPoint);
						DrawSprite(sprite, window);					
				}				
			}
			// Draw snake segments (head, bends, and tail)
			for (auto it = snake.body.begin(); it != snake.body.end(); ++it)
			{
				window.draw(*it);
			}	
		}

		bool HasSnakeCollisionWithRect(const Snake & snake, const sf::FloatRect & rect)
		{
			// Calculate forward point based on snake's head position
			sf::Vector2f forwardPoint = snake.head->getPosition();

			// Adjust forward point based on direction
			switch (snake.direction)
			{
			case SnakeDirection::Up:
				forwardPoint.y -= SNAKE_SIZE / 2.f;
				break;
			case SnakeDirection::Right:
				forwardPoint.x += SNAKE_SIZE / 2.f;
				break;
			case SnakeDirection::Down:
				forwardPoint.y += SNAKE_SIZE / 2.f;
				break;
			case SnakeDirection::Left:
				forwardPoint.x -= SNAKE_SIZE / 2.f;
				break;
			}

			// Check if forward point intersects with rectangle
			return rect.contains(forwardPoint);
		}

		bool CheckSnakeCollisionWithHimself(Snake & snake)
		{
			auto curIt = snake.tail;
			auto nextIt = std::next(snake.tail);

			while (nextIt != snake.head)
			{
				// Get bounds of current segment pair
				auto curRect = curIt->getGlobalBounds();
				auto nextRect = nextIt->getGlobalBounds();

				// Create union rectangle for segment pair
				sf::FloatRect unionRect;
				// Take the minimum top and left coordinates
				unionRect.top = std::min(curRect.top, nextRect.top);
				unionRect.left = std::min(curRect.left, nextRect.left);
				// Calculate width and height to cover both segments
				unionRect.width = std::fabs(curRect.left - nextRect.left) + SNAKE_SIZE;
				unionRect.height = std::fabs(curRect.top - nextRect.top) + SNAKE_SIZE;

				// Check if head collides with this segment
				if (HasSnakeCollisionWithRect(snake, unionRect))
				{
					return true;
				}

				curIt = nextIt;
				nextIt = std::next(nextIt);
			}
			return false;
		}		

		
		void ApplyBendToSegment(sf::Sprite& segment, SnakeDirection oldDirection, SnakeDirection newDirection, const sf::Texture& bendTexture)
		{
			// Set texture and fix the sprite's size.
			segment.setTexture(bendTexture);
			SetSpriteSize(segment, SNAKE_SIZE, SNAKE_SIZE);

			// Center the sprite's origin.
			SetSpriteRelativeOrigin(segment, 0.5f, 0.5f);

			// Compute the correct rotation angle based on old and new directions.
			float angle = 0.f;
			if ((oldDirection == SnakeDirection::Right && newDirection == SnakeDirection::Up) ||
				(oldDirection == SnakeDirection::Down && newDirection == SnakeDirection::Left))
			{
				angle = 0.f;
			}
			else if ((oldDirection == SnakeDirection::Down && newDirection == SnakeDirection::Right) ||
				(oldDirection == SnakeDirection::Left && newDirection == SnakeDirection::Up))
			{
				angle = 90.f;
			}
			else if ((oldDirection == SnakeDirection::Left && newDirection == SnakeDirection::Down) ||
				(oldDirection == SnakeDirection::Up && newDirection == SnakeDirection::Right))
			{
				angle = 180.f;
			}
			else if ((oldDirection == SnakeDirection::Up && newDirection == SnakeDirection::Left) ||
				(oldDirection == SnakeDirection::Right && newDirection == SnakeDirection::Down))
			{
				angle = 270.f;
			}

			// Apply the rotation.
			segment.setRotation(angle);
		}
		void TailRotation(Snake& snake, SnakeDirection oldDirection, SnakeDirection newDirection)
		{
			auto nextTail = std::next(snake.tail);
			auto tailDirection = GetVectorBetweenSprites(*snake.tail, *nextTail);
			
			// Calculate tail rotation angle based on movement direction
			float tailAngle = 0.0f;
			if (std::abs(tailDirection.x) > std::abs(tailDirection.y))
			{
				tailAngle = (tailDirection.x > 0) ? 90.0f : -90.0f;
			}
			else
			{
				tailAngle = (tailDirection.y > 0) ? 180.0f : 0.0f;
			}
			snake.tail->setRotation(tailAngle);
		}
		void UpdateTailAfterRotation(Snake& snake, SnakeDirection oldDirection, SnakeDirection newDirection, float timeDelta)
		{
			auto tailSegment = snake.tail;
			auto nextSegment = std::next(tailSegment);

			if (nextSegment != snake.body.end())
			{
				const float moveAmount = snake.speed * timeDelta;
				// Calculate needed tail adjustment
				const auto direction = GetVectorBetweenSprites(*tailSegment, *nextSegment);
				float distance = std::hypot(direction.x, direction.y);
				//float moveStep = std::min(moveAmount, distance);
				const sf::Vector2f moveAdjustment = (direction / distance);
				if (distance > SNAKE_SIZE)
				{
					//Recalculate position adjustment
					tailSegment->setPosition(tailSegment->getPosition() + moveAdjustment * (distance - SNAKE_SIZE));
					
				}
				else
				{
					tailSegment->setPosition(tailSegment->getPosition() + moveAdjustment * moveAmount);
					
				}					
			}
		}
		void UpdateTail(Snake& snake, float timeDelta)
		{
			// Move the tail
			auto nextTail = std::next(snake.tail);
			float moveAmount = snake.speed * timeDelta;
			if (nextTail != snake.body.end())
			{
				auto tailDirection = GetVectorBetweenSprites(*snake.tail, *nextTail);
				float tailDist = std::sqrt(tailDirection.x * tailDirection.x + tailDirection.y * tailDirection.y);
				sf::Vector2f normalizeTailDirection = { tailDirection.x / tailDist, tailDirection.y / tailDist };

				// Calculate tail rotation angle based on movement direction
				TailRotation(snake, snake.prevDirection, snake.direction);

				if (moveAmount >= tailDist)				{
					
					snake.tail = snake.body.erase(snake.tail);
					SetTailSprite(snake, snake.tail);
				}
				else
				{
					snake.tail->setPosition(snake.tail->getPosition() + normalizeTailDirection * moveAmount);
				}
			}

		}
}























	
	