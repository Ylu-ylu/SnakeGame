#include "GameStatePauseMenu.h"
#include "Game.h"
#include <assert.h>
#include "UI.h"



namespace SnakeGame
{
	void InitGameStatePauseMenu(GameStatePauseMenuData& data, Game& game)
	{
		assert(data.font.loadFromFile(RESOURCES_PATH + "\\Fonts\\Roboto-Regular.ttf"));

		data.background.setFillColor(sf::Color(0, 0, 0, 128)); // Semi-transparent black

		data.menu.rootItem.childrenOrientation = Orientation::Vertical;
		data.menu.rootItem.childrenAlignment = Alignment::Center;
		data.menu.rootItem.childrenSpacing = 15.f;
		data.menu.rootItem.children.push_back(&data.resumeItem);
		data.menu.rootItem.children.push_back(&data.exitItem);

		data.resumeItem.mainMenu.setString("Return to game");
		data.resumeItem.mainMenu.setFont(data.font);
		data.resumeItem.mainMenu.setCharacterSize(24);

		data.exitItem.mainMenu.setString("Exit to main menu");
		data.exitItem.mainMenu.setFont(data.font);
		data.exitItem.mainMenu.setCharacterSize(24);

		//TitleText
		InitUI(data.uiState, data.font);

		InitMenuItem(data.menu.rootItem);
		SelectMenuItem(data.menu, &data.resumeItem);
	}

	void ShutdownGameStatePauseMenu(GameStatePauseMenuData& data, Game& game)
	{
		//Do nothing
	}

	void HandleGameStatePauseMenuWindowEvent(GameStatePauseMenuData& data, sf::Event& event, Game& game)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				PopGameState(game);
								
				// Apply sound and music settings when returning to the playing state
				auto& soundSettings = GameSettingsSound::getInstance();
				auto* playingData = static_cast<GameStatePlayingData*>(game.gameStateStack.back().data);
				ApplySoundSettings(soundSettings, *playingData);
				ApplyMusicSettings(soundSettings, *playingData);
			}
			else if (event.key.code == sf::Keyboard::Enter && data.menu.selectedItem != nullptr)
			{
				if (data.menu.selectedItem == &data.resumeItem)
				{
					PopGameState(game);
				}
				else if (data.menu.selectedItem == &data.exitItem)
				{
					SwitchGameState(game, GameStateType::MainMenu);
				}
			}
			else if (event.key.code == sf::Keyboard::Up)
			{
				SelectPreviousMenuItem(data.menu);
			}
			else if (event.key.code == sf::Keyboard::Down)
			{
				SelectNextMenuItem(data.menu);
			}
		}
	}

	void UpdateGameStatePauseMenu(GameStatePauseMenuData& data, Game& game, float deltaTime)
	{
		//Do nothing
	}

	void DrawGameStatePauseMenu(GameStatePauseMenuData& data, Game& game, sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = window.getView().getSize();

		data.background.setSize(viewSize);
		window.draw(data.background);

		data.titleText.setPosition(viewSize.x / 2.f, 100.f);
		window.draw(data.titleText);

		DrawMenu(game.uiState, data.menu, window, viewSize / 2.f, { 0.5f, 0.f });				
	}
}

