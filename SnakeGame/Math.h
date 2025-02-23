#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <list>

namespace sf
{
	class Sprite;	
}

namespace SnakeGame
{
	struct Game;
	struct Snake;

	struct Vector2Df
	{
		float x = 0.f;
		float y = 0.f;

		struct Vector2DfHash
		{
			size_t operator()(const Vector2Df& v) const noexcept
			{
				// Custom hash function combining x and y values
				return std::hash<float>()(v.x) ^ (std::hash<float>()(v.y) << 1);
			}
		};
	};

	struct Vector2D
	{
		int x = 0;
		int y = 0;

		// Define hash function for Vector2D
		struct Vector2DHash
		{
			size_t operator()(const Vector2D& v) const noexcept
			{
				// Custom hash function combining x and y values
				return std::hash<int>()(v.x) ^ std::hash<int>()(v.y) << 1;
			}
		};
	};
	bool operator== (const Vector2D& lhs, const Vector2D& rhs);

	typedef Vector2Df Position2D;

	//operator for Vector2Df
	bool operator==(const Vector2Df& lhs, const Vector2Df& rhs);
	//Need for +vectors
	Vector2Df operator+(const Vector2Df& lhs, const Vector2Df& rhs);
	Vector2Df operator-(const Vector2Df& lhs, const Vector2Df& rhs);

	struct Rectangle
	{
		Position2D position;
		Vector2Df size;
	};

	struct Cicle
	{
		Position2D position;
		float radius;
	};

	//Variation collide
	bool DoShapeCollide(const Rectangle& rect1, const Rectangle& rect2);	
	bool DoShapeCollide(const Rectangle& rect, const Cicle& cicle);
	Position2D GetRandomPositionRectangle(const sf::Sprite& sprite, const Rectangle& rectangle);	
	void SetSpriteSize(sf::Sprite& sprite, float desiredWidth, float desiredHeight);
	void SetSpriteRelativeOrigin(sf::Sprite& sprite, float originX, float orignY);	
	sf::Vector2f GetItemOrigin(const sf::Text& text, const sf::Vector2f& relativePosition);

	void InitSprite(sf::Sprite& sprite, float desiredWidth, float desiredHeight, const sf::Texture& texture);
	void SetHeadSprite(SnakeGame::Snake& snake, std::list<sf::Sprite>::iterator it);
	void SetTailSprite(SnakeGame::Snake& snake, std::list<sf::Sprite>::iterator it);			
	float GetManhattanDistanceBetweenSprites(const sf::Sprite& sprite1, const sf::Sprite& sprite2);	
	sf::Vector2f GetVectorBetweenSprites(const sf::Sprite& sprite1, const sf::Sprite& sprite2);

	void DrawSprite(const sf::Sprite& sprite, sf::RenderWindow& window);			  
}
                 