#pragma once
#include <SFML/Graphics.hpp>
#include"MainMenu.h"


namespace SnakeGame
{
	struct Game;
	struct MenuItem;
	struct GameSettings;
	struct GameSettingsSound;
	
	struct GameStateMainMenuData
	{
		sf::Font font;

		MenuItem startGameItem;
		MenuItem optionsItem;
		MenuItem optionsInfiniteApplesItem;
		MenuItem optionsWithAccelerationItem;
		MenuItem exitGameItem;
		MenuItem recordsItem;
		MenuItem yesItem;
		MenuItem noItem;
		Menu menu;

		//For DificultyLeves
		MenuItem levelItem;
		MenuItem easyLevelItem;
		MenuItem heavierEasyLevelItem;
		MenuItem mediumLevelItem;
		MenuItem hardLevelItem;
		MenuItem maxLevelItem;
		
		//For Sound
		MenuItem settingsSoundItem;

		MenuItem soundToggleItem;
		MenuItem musicToggleItem;

		//Background Main Menu
		sf::Texture backgroundTexture;
		sf::Sprite backgroundSprite;

	};
	void InitGameStatetMainMenu(GameStateMainMenuData& data, Game& game);
	void ChooseLevelDifficulty(GameStateMainMenuData& data, Game& game, LevelDifficulty difficulty);
	void ShutDownGameStateMainMenu(GameStateMainMenuData& data, Game& game);
	void HandleGameStateMainMenuWindowEvent(GameStateMainMenuData& data, Game& game, GameSettings &gameSettings, GameSettingsSound& sound,  sf::Event& event);
	void UpdateGameStateMainMenu(GameStateMainMenuData& data, Game& game, float deltaTime);
	void DrawGameStateMainMenu(GameStateMainMenuData& data, Game& game, sf::RenderWindow& window);	
}
