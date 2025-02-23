#pragma once
#include <SFML/Graphics.hpp>
#include "Math.h"
#include "Constants.h"
#include <list>
#include <array>

namespace SnakeGame
{
	struct Game;
	struct GameStatePlayingData;
	struct Vector2Df;
	struct Food;		

	enum class SnakeDirection
	{
		Right=0,
		Up,
		Left,
		Down
	};
	//Add snake
	enum class SnakePart
	{
		Head=0,
		Body,
		BodyBend,
		Tail,
		Count
	};

	struct Snake
	{
		std::list<sf::Sprite> body;
		std::list<sf::Sprite>::iterator head;
		std::list<sf::Sprite>::iterator tail;

		float speed = 0.f; // Pixels per second
		bool isGrowSnake = false;

		SnakeDirection direction = SnakeDirection::Up;
		SnakeDirection prevDirection = SnakeDirection::Up;
		std::array<sf::Texture, (size_t)SnakePart::Count> textures;
		sf::Sprite sprite;
		Vector2Df snakePosition;	
	};
	void LoadSnakeTextures(Snake& snake);
	void InitSnake(Snake& snake);
	void MoveSnake(Snake& snake, float timeDelta);
	void GrowSnake(Snake& snake);
	void DrawSnake(Snake& snake, sf::RenderWindow& window);
	bool HasSnakeCollisionWithRect(const Snake& snake, const sf::FloatRect& rect);
	bool CheckSnakeCollisionWithHimself(Snake& snake);
	bool CheckSnakeCollisionWithSprite(Snake& snake, const sf::Sprite& sprite);
	sf::Vector2f GetDirectionVector(SnakeDirection direction);
	Rectangle GetSnakeCollider(const Snake& snake);
	void GetRotationSprite(Snake& snake, SnakeDirection& oldDirection, SnakeDirection& newDirection);

	void ApplyBendToSegment(sf::Sprite& segment, SnakeDirection oldDirection, SnakeDirection newDirection, const sf::Texture& bendTexture);	
	void TailRotation(Snake& snake, SnakeDirection oldDirection, SnakeDirection newDirection);
	void UpdateTailAfterRotation(Snake& snake, SnakeDirection oldDirection, SnakeDirection newDirection, float timeDelta);
	void UpdateTail(Snake& snake, float timeDelta);
}
