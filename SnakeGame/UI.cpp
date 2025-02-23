#include "UI.h"
#include <string>
#include "Game.h"
#include "GameStateGameOver.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <assert.h>
#include <algorithm>
#include "MainMenu.h"
#include "GamestateMainMenu.h"
#include "GameStatePlaying.h"
#include "Math.h"
#include "GameSettings.h"
#include "GameSettingSound.h"
#include "GameStateSettingSound.h"

namespace SnakeGame
{
	GameSettingsSound& soundSettings = GameSettingsSound::getInstance();

	void InitUI(UIState& uiState, const sf::Font& font)
	{
		auto game = std::make_unique<Game>();

		auto initText = [&](sf::Text& text, unsigned size, const sf::Color& color,
			const std::string& str = "", float originX = 0.5f, float originY = 0.5f) 
			{
				text.setFont(font);
				text.setCharacterSize(size);
				text.setFillColor(color);
				if (!str.empty()) text.setString(str);
				text.setOrigin(GetItemOrigin(text, { originX, originY }));
			};
		
		
		// Common text configurations
		initText(uiState.scoreText, 20, sf::Color::Yellow);
		initText(uiState.controlsHintText, 20, sf::Color::Yellow, "Use arrows to move, ESC to exit", 0.f, 0.f);
		initText(uiState.gameOverText, 120, sf::Color::Red, "GAME OVER", 0.5f, 1.8f);
		initText(uiState.hintText, 24, sf::Color::White, "Press Space to restart", 0.5f, 1.f);
		initText(uiState.gameOverScoreText, 25, sf::Color::Green, "", 0.f, 0.f);
		initText(uiState.restartText, 20, sf::Color::Yellow);
		initText(uiState.inputHintText, 24, sf::Color::White, "Use arrow keys to move, ESC to pause", 1.f, 0.f);
		
		// Menu items
		initText(uiState.mainMenu, 30, sf::Color::White);
		initText(uiState.mainMenuhintText, 30, sf::Color::White);
		initText(uiState.pauseMenuText, 48, sf::Color::Red, "Pause");

		// Name input
		initText(uiState.inputNamePlayer, 24, sf::Color::Yellow, "Enter Name?", 0.5f, 1.5f);
		initText(uiState.inputNamePlayerHintText, 24, sf::Color::Yellow, game->gameStateGameOverData.playerName,  0.5f, 0.5f );

		// Confirmation dialog
		initText(uiState.noText, 24, sf::Color::Yellow, "No");
		initText(uiState.yesText, 24, sf::Color::Yellow, "Yes");

		// Sound settings
		const std::string soundState = soundSettings.isSoundEnabled() ? "ON" : "OFF";
		initText(uiState.titleSoundMusicText, 24, sf::Color::Yellow, "Settings");
		initText(uiState.soundText, 24, sf::Color::Yellow, "Sound: " + soundState);
		initText(uiState.musicText, 24, sf::Color::Yellow, "Music: " + soundState);
		initText(uiState.backSoundMusicText, 24, sf::Color::White, "Back to Main Menu: Press ESC");

		// Position overrides
		uiState.controlsHintText.setPosition(SCREEN_WIDTH - 300.f, 10.f);
		uiState.gameOverScoreText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f + 50.f);
				
		// Pause timer (use local scope)
		{
			GameStatePlayingData playingData;
			uiState.pauseTimerText.setString(std::to_string(static_cast<int>(std::ceil(playingData.pauseTimer))));
		}
		initText(uiState.pauseTimerText, 40, sf::Color::Yellow, "", 1.f, 0.5f);

		// Style overrides
		uiState.gameOverText.setStyle(sf::Text::Bold);

	}

	//For Draw MainMenu:choose Colour
	void SelectMenuItemUI(UIState& uiState, Menu& menu, MenuItem* item)
	{
		if (menu.selectedItem)
		{
			menu.selectedItem->mainMenu.setFillColor(menu.selectedItem->deselectedColor);
		}

		menu.selectedItem = item;

		if (menu.selectedItem)
		{
			menu.selectedItem->mainMenu.setFillColor(menu.selectedItem->selectedColor);
		}
	}

	void InitGameStateMainMenuUI(GameStateMainMenuData& data, Game& game)
	{
		data.menu.rootItem.mainMenuhintText.setString("Snake Game");
		data.menu.rootItem.mainMenuhintText.setFont(data.font);
		data.menu.rootItem.mainMenuhintText.setCharacterSize(48);
		data.menu.rootItem.mainMenuhintText.setFillColor(sf::Color::Red);
		data.menu.rootItem.childrenOrientation = Orientation::Vertical;
		data.menu.rootItem.childrenAlignment = Alignment::Center;
		data.menu.rootItem.childrenSpacing = 15.f;

		data.menu.rootItem.children.push_back(&data.startGameItem);
		data.menu.rootItem.children.push_back(&data.optionsItem);
		data.menu.rootItem.children.push_back(&data.exitGameItem);
		data.menu.rootItem.children.push_back(&data.recordsItem);
		data.menu.rootItem.children.push_back(&data.settingsSoundItem);


		data.startGameItem.mainMenu.setString("Start Game");
		data.startGameItem.mainMenu.setFont(data.font);
		data.startGameItem.mainMenu.setCharacterSize(24);

		data.optionsItem.mainMenu.setString("Difficulty Levels");
		data.optionsItem.mainMenu.setFont(data.font);
		data.optionsItem.mainMenu.setCharacterSize(24);
		data.optionsItem.mainMenuhintText.setString("Difficulty Levels");
		data.optionsItem.mainMenuhintText.setFont(data.font);
		data.optionsItem.mainMenuhintText.setCharacterSize(48);
		data.optionsItem.mainMenuhintText.setFillColor(sf::Color::Red);
		data.optionsItem.childrenOrientation = Orientation::Vertical;
		data.optionsItem.childrenAlignment = Alignment::Center;
		data.optionsItem.childrenSpacing = 15.f;

		//for Level Dificulty
		data.optionsItem.children.push_back(&data.easyLevelItem);
		data.optionsItem.children.push_back(&data.heavierEasyLevelItem);
		data.optionsItem.children.push_back(&data.mediumLevelItem);
		data.optionsItem.children.push_back(&data.hardLevelItem);
		data.optionsItem.children.push_back(&data.maxLevelItem);

		data.easyLevelItem.mainMenu.setString("Easy Level");
		data.easyLevelItem.mainMenu.setFont(data.font);
		data.easyLevelItem.mainMenu.setCharacterSize(24);
		
		data.heavierEasyLevelItem.mainMenu.setString("Harder than easy level");
		data.heavierEasyLevelItem.mainMenu.setFont(data.font);
		data.heavierEasyLevelItem.mainMenu.setCharacterSize(24);

		data.mediumLevelItem.mainMenu.setString("Medium Level");
		data.mediumLevelItem.mainMenu.setFont(data.font);
		data.mediumLevelItem.mainMenu.setCharacterSize(24);

		data.hardLevelItem.mainMenu.setString("Hard Level");
		data.hardLevelItem.mainMenu.setFont(data.font);
		data.hardLevelItem.mainMenu.setCharacterSize(24);

		data.maxLevelItem.mainMenu.setString("Max Level");
		data.maxLevelItem.mainMenu.setFont(data.font);
		data.maxLevelItem.mainMenu.setCharacterSize(24);				

		data.recordsItem.mainMenu.setString("Records");
		data.recordsItem.mainMenu.setFont(data.font);
		data.recordsItem.mainMenu.setCharacterSize(24);

		data.exitGameItem.mainMenu.setString("Exit Game");
		data.exitGameItem.mainMenu.setFont(data.font);
		data.exitGameItem.mainMenu.setCharacterSize(24);
		data.exitGameItem.mainMenuhintText.setString("Are you sure?");
		data.exitGameItem.mainMenuhintText.setFont(data.font);
		data.exitGameItem.mainMenuhintText.setCharacterSize(48);
		data.exitGameItem.mainMenuhintText.setFillColor(sf::Color::Red);
		data.exitGameItem.childrenOrientation = Orientation::Horizontal;
		data.exitGameItem.childrenAlignment = Alignment::Center;
		data.exitGameItem.childrenSpacing = 15.f;

		data.exitGameItem.children.push_back(&data.yesItem);
		data.exitGameItem.children.push_back(&data.noItem);

		data.yesItem.mainMenu.setString("Yes");
		data.yesItem.mainMenu.setFont(data.font);
		data.yesItem.mainMenu.setCharacterSize(24);

		data.noItem.mainMenu.setString("No");
		data.noItem.mainMenu.setFont(data.font);
		data.noItem.mainMenu.setCharacterSize(24);

		//For Sound
		data.settingsSoundItem.mainMenu.setString("Settings");
		data.settingsSoundItem.mainMenu.setFont(data.font);
		data.settingsSoundItem.mainMenu.setCharacterSize(24);
		data.settingsSoundItem.mainMenuhintText.setString("Settings");
		data.settingsSoundItem.mainMenuhintText.setFont(data.font);
		data.settingsSoundItem.mainMenuhintText.setCharacterSize(48);
		data.settingsSoundItem.mainMenuhintText.setFillColor(sf::Color::Red);
		data.settingsSoundItem.childrenOrientation = Orientation::Vertical;
		data.settingsSoundItem.childrenAlignment = Alignment::Center;
		data.settingsSoundItem.childrenSpacing = 15.f;

		data.settingsSoundItem.children.push_back(&data.soundToggleItem);
		data.settingsSoundItem.children.push_back(&data.musicToggleItem);
		
		data.soundToggleItem.mainMenu.setString("Sound: " + std::string(soundSettings.isSoundEnabled() ? "ON" : "OFF"));
		data.soundToggleItem.mainMenu.setFont(data.font);
		data.soundToggleItem.mainMenu.setCharacterSize(48);
		

		data.musicToggleItem.mainMenu.setString("Music: " + std::string(soundSettings.isSoundEnabled() ? "ON" : "OFF"));
		data.musicToggleItem.mainMenu.setFont(data.font);
		data.musicToggleItem.mainMenu.setCharacterSize(48);
		
	}

	void DrawItemsList2(sf::RenderWindow& window, const std::vector<sf::Text*>& items, float spacing, Orientation orientation, Alignment alignment, const sf::Vector2f& position, const sf::Vector2f& origin)
	{
		sf::FloatRect totalRect;
		// Calculate total height/width of all texts
		for (auto it = items.begin(); it != items.end(); ++it)
		{
			sf::FloatRect itemRect = (*it)->getGlobalBounds();

			if (orientation == Orientation::Horizontal)
			{
				totalRect.width += itemRect.width + (it != items.end() - 1 ? spacing : 0.f);
				totalRect.height = std::max(totalRect.height, itemRect.height);
			}
			else
			{
				totalRect.width = std::max(totalRect.width, itemRect.width);
				totalRect.height += itemRect.height + (it != items.end() - 1 ? spacing : 0.f);
			}
		}

		totalRect.left = position.x - origin.x * totalRect.width;
		totalRect.top = position.y - origin.y * totalRect.height;
		sf::Vector2f currentPos = { totalRect.left, totalRect.top };

		for (auto it = items.begin(); it != items.end(); ++it)
		{
			sf::FloatRect itemRect = (*it)->getGlobalBounds();
			sf::Vector2f itemOrigin;

			if (orientation == Orientation::Horizontal)
			{
				itemOrigin.y = alignment == Alignment::Left ? 0.f : alignment == Alignment::Center ? 0.5f : 1.f;
				itemOrigin.x = 0.f;
				currentPos.y = totalRect.top + itemOrigin.y * totalRect.height;
			}
			else
			{
				itemOrigin.y = 0.f;
				itemOrigin.x = alignment == Alignment::Left ? 0.f : alignment == Alignment::Center ? 0.5f : 1.f;
				currentPos.x = totalRect.left + itemOrigin.x * totalRect.width;
			}

			(*it)->setOrigin(GetItemOrigin(**it, itemOrigin));
			(*it)->setPosition(currentPos);
			window.draw(**it);

			if (orientation == Orientation::Horizontal)
			{
				currentPos.x += itemRect.width + spacing;
			}
			else
			{
				currentPos.y += itemRect.height + spacing;
			}
		}
	}
}