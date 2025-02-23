#include "GameStateMainMenu.h"
#include "Game.h"
#include <assert.h>
#include "UI.h"
#include "GameSettings.h"
#include "GameSettingSound.h"

namespace SnakeGame
{
	void InitGameStatetMainMenu(GameStateMainMenuData& data, Game& game)
	{
		assert(data.font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));
		assert(data.backgroundTexture.loadFromFile(RESOURCES_PATH + "Background.png"));

		InitGameStateMainMenuUI(data, game);

		InitMenuItem(data.menu.rootItem);
		SelectMenuItem(data.menu, &data.startGameItem);

		data.backgroundSprite.setTexture(data.backgroundTexture);
		SetSpriteRelativeOrigin(data.backgroundSprite, 0.5f, 0.5f);
		data.backgroundSprite.setPosition(450.f, 300.f);

		SetSpriteSize(data.backgroundSprite, SCREEN_WIDTH, SCREEN_HEIGHT);
	}


	void ChooseLevelDifficulty(GameStateMainMenuData& data, Game& game, LevelDifficulty difficulty)
	{		
		game.gameSettings.SetDifficultyLevel(difficulty);
		CollapseSelectedItem(data.menu);
	}

	void ShutDownGameStateMainMenu(GameStateMainMenuData& data, Game& game)
	{
		// No need to do anything here

	}
	void HandleGameStateMainMenuWindowEvent(GameStateMainMenuData& data, Game& game, GameSettings& gameSettings, GameSettingsSound& sound,  sf::Event& event)
	{
		if (!data.menu.selectedItem)
		{
			return;
		}
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				CollapseSelectedItem(data.menu);
			}
			else if (event.key.code == sf::Keyboard::Enter)
			{
				if (data.menu.selectedItem == &data.startGameItem)
				{
					SwitchGameState(game, GameStateType::Playing);						
					
				}
				else if (data.menu.selectedItem == &data.optionsItem)
				{
					ExpandSelectedItem(data.menu);
				}	
				//For Sound and Music
				else if (data.menu.selectedItem == &data.soundToggleItem)
				{					
					// Toggle sound effects
					ToggleSound(sound);							
				}

				else if (data.menu.selectedItem == &data.musicToggleItem)
				{					
					// Toggle background music
					ToggleMusic(sound);					
				}
				//For Levels Dificulty
				else if (data.menu.selectedItem == &data.easyLevelItem)
				{
					ChooseLevelDifficulty(data, game, LevelDifficulty::Easy);
				}
				else if (data.menu.selectedItem == &data.heavierEasyLevelItem)
				{
					ChooseLevelDifficulty(data, game, LevelDifficulty::HeavierEasy);
				}
				else if (data.menu.selectedItem == &data.mediumLevelItem)
				{
					ChooseLevelDifficulty(data, game, LevelDifficulty::Medium);
				}
				else if (data.menu.selectedItem == &data.hardLevelItem)
				{
					ChooseLevelDifficulty(data, game, LevelDifficulty::Hard);
				}
				else if (data.menu.selectedItem == &data.maxLevelItem)
				{
					ChooseLevelDifficulty(data, game, LevelDifficulty::Max);
				}				
				else if (data.menu.selectedItem == &data.recordsItem)
				{
					PushGameState(game, GameStateType::Records, true);
				}
				else if (data.menu.selectedItem == &data.exitGameItem)
				{
					ExpandSelectedItem(data.menu);
				}
				else if (data.menu.selectedItem == &data.yesItem)
				{
					SwitchGameState(game, GameStateType::None);
				}
				else if (data.menu.selectedItem == &data.noItem)
				{
					CollapseSelectedItem(data.menu);
				}
				else
				{
					ExpandSelectedItem(data.menu);
				}

			}
			Orientation orientation = data.menu.selectedItem->parent->childrenOrientation;
			if (orientation == Orientation::Vertical && event.key.code == sf::Keyboard::Up ||
				orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::Left)
			{
				SelectPreviousMenuItem(data.menu);
			}
			else if (orientation == Orientation::Vertical && event.key.code == sf::Keyboard::Down ||
				orientation == Orientation::Horizontal && event.key.code == sf::Keyboard::Right)
			{
				SelectNextMenuItem(data.menu);
			}
		}
	}

	void UpdateGameStateMainMenu(GameStateMainMenuData& data, Game& game, float deltaTime)
	{
		data.soundToggleItem.mainMenu.setString("Sound: "+
			std::string(game.gameSettingsSound.isSoundEnabled() ? "ON" : "OFF"));

		data.musicToggleItem.mainMenu.setString("Music: "+
			std::string(game.gameSettingsSound.isMusicEnabled() ? "ON" : "OFF"));

		//For Levels Dificulty
		 // Update difficulty level displays
		DifficultyParams parameters;
		game.gameSettings.getDifficultyParamsLevel(parameters);

		// Reset all difficulty items to default text
		data.easyLevelItem.mainMenu.setString("Easy (Speed 1)");
		data.heavierEasyLevelItem.mainMenu.setString("Heavier Easy (Speed 2)");
		data.mediumLevelItem.mainMenu.setString("Medium (Speed 3)");
		data.hardLevelItem.mainMenu.setString("Hard (Speed 4)");
		data.maxLevelItem.mainMenu.setString("Max (Speed 5)");

		// Highlight current difficulty
		switch (game.gameSettings.currentLevelDifficulty)
		{
		case LevelDifficulty::Easy:
			data.easyLevelItem.mainMenu.setString("> Easy (Speed 1)");
			break;
		case LevelDifficulty::HeavierEasy:
			data.heavierEasyLevelItem.mainMenu.setString("> Heavier Easy (Speed 2)");
			break;
		case LevelDifficulty::Medium:
			data.mediumLevelItem.mainMenu.setString("> Medium (Speed 3)");
			break;
		case LevelDifficulty::Hard:
			data.hardLevelItem.mainMenu.setString("> Hard (Speed 4)");
			break;
		case LevelDifficulty::Max:
			data.maxLevelItem.mainMenu.setString("> Max (Speed 5)");
			break;
		}
	}

	void DrawGameStateMainMenu(GameStateMainMenuData& data, Game& game, sf::RenderWindow& window)
	{
		window.draw(data.backgroundSprite);

		sf::Vector2f viewSize = (sf::Vector2f)window.getSize();

		sf::Text* hintText = &GetCurrentMenuContext(data.menu)->mainMenuhintText;
		hintText->setOrigin(GetItemOrigin(*hintText, { 0.5f, 0.5f }));
		hintText->setPosition(viewSize.x / 2.f, viewSize.y / 2.f -50.f);
		window.draw(*hintText);

		DrawMenu(game.uiState, data.menu, window, viewSize / 2.f, { 0.5f, 0.f });				
	}	
}