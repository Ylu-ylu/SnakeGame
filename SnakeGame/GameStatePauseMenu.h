#pragma once
#include <SFML/Graphics.hpp>
#include "MainMenu.h"
#include "GamestateMainMenu.h"



namespace SnakeGame
{

	struct GameStateMainMenuData;
	struct UIState;

	struct GameStatePauseMenuData
	{
		//Resource
		sf::Font font;
		sf::Text titleText;

		sf::RectangleShape background;

		Menu menu;
		MenuItem resumeItem;
		MenuItem exitItem;
		UIState uiState;
	};

	void InitGameStatePauseMenu(GameStatePauseMenuData& data, Game& game);
	void ShutdownGameStatePauseMenu(GameStatePauseMenuData& data, Game& game);
	void HandleGameStatePauseMenuWindowEvent(GameStatePauseMenuData& data, sf::Event& event, Game& game);
	void UpdateGameStatePauseMenu(GameStatePauseMenuData& data, Game& game, float deltaTime);
	void DrawGameStatePauseMenu(GameStatePauseMenuData& data, Game& game, sf::RenderWindow& window);
}
