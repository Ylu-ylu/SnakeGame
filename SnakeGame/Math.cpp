#include "Math.h"
#include <cstdlib>
#include "Snake.h"


namespace SnakeGame
{
	Vector2Df operator+(const Vector2Df& lhs, const Vector2Df& rhs)
	{
		Vector2Df result;
		result.x = lhs.x + rhs.x;
		result.y = lhs.y + rhs.y;
		return result;
	}

	Vector2Df  operator-(const Vector2Df& lhs, const Vector2Df& rhs)
	{
		Vector2Df result;
		result.x = lhs.x - rhs.x;
		result.y = lhs.y - rhs.y;
		return result;
	}

	bool operator==(const Vector2D& lhs, const Vector2D& rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}
	bool operator==(const Vector2Df& lhs, const Vector2Df& rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}
	bool DoShapeCollide(const Rectangle& rect1, const Rectangle& rect2)
	{
		return rect1.position.x < rect2.position.x + rect2.size.x &&
			rect1.position.x + rect1.size.x > rect2.position.x &&
			rect1.position.y < rect2.position.y + rect2.size.y &&
			rect1.position.y + rect1.size.y > rect2.position.y;
	}	

	bool DoShapeCollide(const Rectangle& rect, const Cicle& cicle)
	{
		const float dx = cicle.position.x - std::max(rect.position.x, std::min(cicle.position.x, rect.position.x + rect.size.x));
		const float dy = cicle.position.y - std::max(rect.position.y, std::min(cicle.position.y, rect.position.y + rect.size.y));
		return (dx * dx + dy * dy) < (cicle.radius * cicle.radius);
	}

	Position2D GetRandomPositionRectangle(const sf::Sprite& sprite,const Rectangle& rectangle)
	{
		Position2D result;
		
		const auto spriteWidth = sprite.getGlobalBounds().width;
		const auto spriteHeight = sprite.getGlobalBounds().height;

		// Calculate position within screen bounds
		result.x = rectangle.position.x + spriteWidth +
			(rand() / (float)RAND_MAX) * (rectangle.size.x - 2 * spriteWidth);
		result.y = rectangle.position.y + spriteHeight +
			(rand() / (float)RAND_MAX) * (rectangle.size.y - 2 * spriteHeight);
		return result;

	}
	void SetSpriteSize(sf::Sprite& sprite, float desiredWidth, float desiredHeight)
	{
		sf::FloatRect  spriteRect = sprite.getLocalBounds();
		sf::Vector2f scale = { desiredWidth / spriteRect.width, desiredHeight / spriteRect.height };
		sprite.setScale(scale);
	}
	void SetSpriteRelativeOrigin(sf::Sprite& sprite, float originX, float orignY)
	{
		sf::FloatRect  spriteRect = sprite.getLocalBounds();
		sprite.setOrigin(originX * (spriteRect.width), orignY * (spriteRect.height));
	}
	sf::Vector2f GetItemOrigin(const sf::Text& text, const sf::Vector2f& relativePosition)
	{
		sf::FloatRect textSize = text.getLocalBounds();
		return {
			(textSize.left + textSize.width) * relativePosition.x,
			(textSize.top + textSize.height) * relativePosition.y,
		};
	}	
	//SnakeGame
	void InitSprite(sf::Sprite& sprite, float desiredWidth, float desiredHeight, const sf::Texture& texture)
	{
		sprite.setTexture(texture);
		SetSpriteRelativeOrigin(sprite, 0.5f, 0.5f);
		SetSpriteSize(sprite, desiredWidth, desiredHeight);
	}		
	void SetHeadSprite(SnakeGame::Snake& snake, std::list<sf::Sprite>::iterator it)
	{
		float angle = 0.f; // Up
		if (snake.direction == SnakeGame::SnakeDirection::Right)
		{ 
			angle = 90.f;
		}
		else if (snake.direction == SnakeGame::SnakeDirection::Down)
		{ 
			angle = 180.f;
		}
		else if (snake.direction == SnakeGame::SnakeDirection::Left) 
		{ 
			angle = -90.f;
		}

		it->setTexture(snake.textures[(size_t)SnakeGame::SnakePart::Head]);
		it->setRotation(angle);

	}
	void SetTailSprite(SnakeGame::Snake& snake, std::list<sf::Sprite>::iterator it)
	{
		it->setTexture(snake.textures[(size_t)SnakeGame::SnakePart::Tail]);

		auto nextIt = std::next(it);
		if (nextIt == snake.body.end()) 
		{
			snake.body.erase(nextIt);//I added this line
			return;
		}
		float angle = 0.f; // Up
		if (nextIt->getPosition().x < it->getPosition().x) 
		{ // Left
			angle = -90.f;
		}
		else if (nextIt->getPosition().x > it->getPosition().x) 
		{ // Right
			angle = 90.f;
		}
		else if (nextIt->getPosition().y > it->getPosition().y) 
		{ // Down
			angle = 180.f;
		}

		it->setRotation(angle);
	}	
	float GetManhattanDistanceBetweenSprites(const sf::Sprite& sprite1, const sf::Sprite& sprite2)
	{
		sf::Vector2f pos1 = sprite1.getPosition();
		sf::Vector2f pos2 = sprite2.getPosition();
		return std::abs(pos1.x - pos2.x) + std::abs(pos1.y - pos2.y);	

	}

	sf::Vector2f GetVectorBetweenSprites(const sf::Sprite& sprite1, const sf::Sprite& sprite2)
	{
		sf::Vector2f pos1 = sprite1.getPosition();
		sf::Vector2f pos2 = sprite2.getPosition();

		return sf::Vector2f(pos2.x - pos1.x, pos2.y - pos1.y);
	}
	void DrawSprite(const sf::Sprite& sprite, sf::RenderWindow& window)
	{
		window.draw(sprite);
	}		
}

