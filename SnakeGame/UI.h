#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Constants.h"
#include <sstream> 
#include "GameSettingSound.h"

namespace SnakeGame
{
	struct Game;
	struct GameStatePlayingData;

	enum class Orientation
	{
		Horizontal,
		Vertical
	};
	enum class Alignment
	{
		Left,
		Center,
		Right
	};

	struct MenuItem;
	struct Menu;
	struct GameStateMainMenuData;
	struct GameStatePlayingData;
	struct GameStateGameOverData;
	struct GameStateExitDialogData;
	struct DifficultyParams;
	struct GameSettings;
	struct GameSettingsSound;

	struct UIState
	{
		sf::Text scoreText;
		sf::Text controlsHintText;
		sf::Text gameOverText;
		sf::Text hintText;
		sf::Text gameOverScoreText;
		sf::Text restartText;
		sf::Text gameModeChoosingText;
		sf::Text gameModeChoosingTextGameOver;

		//for PauseMenu
		sf::Text pauseMenuText;

		//for MainMenu
		sf::Text mainMenu;
		sf::Text mainMenuhintText; // Visible when child item is selected
		std::vector <sf::Text*> texts;

		//for GameStatePlaying
		sf::Text inputHintText;
		sf::Font font;


		//for Level Difficulty
		sf::Text levelItem;
		sf::Text currentLevelDifficulty;


		//for Input Name Player
		sf::Text inputNamePlayer;
		sf::Text inputNamePlayerHintText;

		sf::Text noText;
		sf::Text yesText;

		//for Sound and Music
		sf::Text titleSoundMusicText;
		sf::Text soundText;
		sf::Text musicText;
		sf::Text backSoundMusicText;		
		
		//Text for pause before Game
		sf::Text pauseTimerText;
		
	};

	void InitUI(UIState& uiState, const sf::Font& font);
	//Draw map RecordTable	
	void DrawItemsList2(sf::RenderWindow& window, const std::vector<sf::Text*>& items, float spacing, Orientation orientation, Alignment alignment, const sf::Vector2f& position, const sf::Vector2f& origin);
	//Draw MainMenu
	void SelectMenuItemUI(UIState& uiState, Menu& menu, MenuItem* item);
	//Draw GameStateMainMenu
	void InitGameStateMainMenuUI(GameStateMainMenuData& data, Game& game);
}