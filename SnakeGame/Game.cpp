#include"Game.h"
#include <assert.h>
#include "GameStateRecords.h"
#include "GameStatePlaying.h"
#include "GamestateMainMenu.h"
#include "GameStatePauseMenu.h"



namespace SnakeGame
{
	extern GameStateRecordsData recordsData;

	void InitGame(Game& game)
	{
		//Init Records Table
		game.recordsTable =
		{
			{"Alex", 100},
			{"Kirill", 50},
			{"Bobi", 30},
			{"Andrey", 20},
			{"Ylia",10}
		};

		// Reset the player's name
		game.gameStateGameOverData.playerName.clear();

		// Initialize the game state
		game.gameStateChangeType = GameStateChangeType::None;
		game.pendingGameStateType = GameStateType::None;
		game.pendingGameStateIsExclusivelyVisible = false;
		SwitchGameState(game, GameStateType::MainMenu);

		//Initialize game settings Sond
		game.gameSettingsSound.getInstance();
	}
	

	void HandleWindowEvents(Game& game, sf::RenderWindow& window)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			//Close: Escape or close button
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (game.gameStateStack.size() > 0)
			{
				HandleWindowEventGameState(game, game.gameStateStack.back(), event);
			}
		}
	}
	bool UpdateGame(Game& game, float deltaTime)
	{
		if (game.gameStateChangeType == GameStateChangeType::Switch)
		{
			//Shutdown game state all
			while (game.gameStateStack.size() > 0)
			{
				ShutdownGameState(game, game.gameStateStack.back());
				game.gameStateStack.pop_back();
				
			}

		}
		else if (game.gameStateChangeType == GameStateChangeType::Pop)
		{
			// Shutdown only current game state
			if (game.gameStateStack.size() > 0)
			{
				ShutdownGameState(game, game.gameStateStack.back());
				game.gameStateStack.pop_back();
			}
		}
		// Initialize new game state if needed
		if (game.pendingGameStateType != GameStateType::None)
		{
			game.gameStateStack.push_back({ game.pendingGameStateType, nullptr, game.pendingGameStateIsExclusivelyVisible });
			InitGameState(game, game.gameStateStack.back());
		}

		game.gameStateChangeType = GameStateChangeType::None;
		game.pendingGameStateType = GameStateType::None;
		game.pendingGameStateIsExclusivelyVisible = false;
		////
		game.gameStateGameOverData.playerName.clear();

		if (game.gameStateStack.size() > 0)
		{
			UpdateGameState(game, game.gameStateStack.back(), deltaTime);
			return true;
		}

	}
	void DrawGame(Game& game, sf::RenderWindow& window)
	{
		if (game.gameStateStack.size() > 0)
		{
			std::vector<GameState*> visibleGameStates;
			for (auto it = game.gameStateStack.rbegin(); it != game.gameStateStack.rend(); ++it)
			{
				visibleGameStates.push_back(&(*it));
				if (it->isExclusivelyVisible)
				{
					break;
				}
			}
			for (auto it = visibleGameStates.rbegin(); it != visibleGameStates.rend(); ++it)
			{
				DrawGameState(game, **it, window);
			}
		}
	}
	//Update Game State for Bit Mask
	void UpdateGameState(Game& game, GameState& state, float deltaTime)
	{		
		switch (state.gametype)
		{
		case GameStateType::MainMenu:
		{
			UpdateGameStateMainMenu(*(GameStateMainMenuData*)state.data, game, deltaTime);
			break;
		}
		case GameStateType::Playing:
		{
			UpdateGameStatePlaying(*(GameStatePlayingData*)state.data, game, deltaTime);
			break;
		}
		case GameStateType::GameOver:
		{
			UpdateGameStateGameOver(*(GameStateGameOverData*)state.data, game, deltaTime);
			break;
		}
		case GameStateType::Records:
		{
			UpdateGameStateRecords(*(GameStateRecordsData*)state.data, game, deltaTime);
			break;
		}
		case GameStateType::PauseMenu:
		{
			UpdateGameStatePauseMenu(*(GameStatePauseMenuData*)state.data, game, deltaTime);
			break;
		}
		case GameStateType::SettingsSound:
		{
			UpdateGameStateSettings(*(GameStateSettingsSoundData*)state.data, game, deltaTime);
			break;
		}

		default:
			assert(false); // We want to know if we forgot to implement new game state
			break;
		}
	}
	void HandleWindowEventGameState(Game& game, GameState& state, sf::Event& event)
	{		
		switch (state.gametype)
		{
		case GameStateType::MainMenu:
		{
			HandleGameStateMainMenuWindowEvent(*(GameStateMainMenuData*)state.data, game, game.gameSettings, game.gameSettingsSound, event);
			break;
		}
		case GameStateType::Playing:
		{
			HandleGameStatePlayingWindowEvent(*(GameStatePlayingData*)state.data, game, event);
			break;
		}
		case GameStateType::GameOver:
		{
			HandleGameStateGameOverWindowEvent(*(GameStateGameOverData*)state.data, game, event);
			break;
		}
		case GameStateType::Records:
		{
			HandleGameStateRecordsWindowEvent(*(GameStateRecordsData*)state.data, event, game);
			break;
		}
		case GameStateType::PauseMenu:
		{
			HandleGameStatePauseMenuWindowEvent(*(GameStatePauseMenuData*)state.data, event, game);
			break;
		}
		case GameStateType::SettingsSound:
		{
			HandleGameStateSettingsEvent(*(GameStateSettingsSoundData*)state.data,game,game.gameSettingsSound, event);
			break;
		}

		default:
		{
			assert(false); // We want to know if we forgot to implement new game statee
			break;
		}
		}
	}

	void InitGameState(Game& game, GameState& state)
	{
		switch (state.gametype)
		{
		case GameStateType::MainMenu:
		{
			state.data = new GameStateMainMenuData();
			InitGameStatetMainMenu(*(GameStateMainMenuData*)state.data, game);
			break;
		}
		case GameStateType::Playing:
		{
			state.data = new GameStatePlayingData();
			InitGameStatePlaying(*(GameStatePlayingData*)state.data, game);
			break;
		}
		case GameStateType::GameOver:
		{
			state.data = new GameStateGameOverData();
			InitGameStateGameOverEnd(*(GameStateGameOverData*)state.data, game,recordsData);
			break;
		}
		case GameStateType::Records:
		{
			state.data = new GameStateRecordsData();
			InitStateGameStateRecords(*(GameStateRecordsData*)state.data, game);
			break;
		}
		case GameStateType::PauseMenu:
		{
			state.data = new GameStatePauseMenuData();
			InitGameStatePauseMenu(*(GameStatePauseMenuData*)state.data, game);
			break;
		}
		case GameStateType::SettingsSound:
		{
			state.data = new GameStateSettingsSoundData();
			InitGameStateSettings(*(GameStateSettingsSoundData*)state.data, game);
			break;
		}
		default:
			assert(false); // We want to know if we forgot to implement new game state
			break;
		}
	}

	void ShutdownGame(Game& game)
	{	// Shutdown all game states
		while (game.gameStateStack.size() > 0)
		{
			ShutdownGameState(game, game.gameStateStack.back());
			game.gameStateStack.pop_back();
		}
		game.gameStateChangeType = GameStateChangeType::None;
		game.pendingGameStateType = GameStateType::None;
		game.pendingGameStateIsExclusivelyVisible = false;
	}

	void ShutdownGameState(Game& game, GameState& state)
	{
		switch (state.gametype)
		{
		case GameStateType::MainMenu:
		{
			ShutDownGameStateMainMenu(*(GameStateMainMenuData*)state.data, game);
			delete (GameStateMainMenuData*)state.data;
			break;
		}
		case GameStateType::Playing:
		{
			ShutdownGameStatePlaying(*(GameStatePlayingData*)state.data, game);
			delete (GameStatePlayingData*)state.data;
			break;
		}
		case GameStateType::GameOver:
		{
			ShutdownGameStateGameOver(*(GameStateGameOverData*)state.data, game);
			delete (GameStateGameOverData*)state.data;

		//	SwitchToGameOverState(game, dataPlaying);

			break;
		}
		case GameStateType::Records:
		{
			ShutdownGameStateRecords(*(GameStateRecordsData*)state.data, game);
			delete (GameStateRecordsData*)state.data;
			break;
		}
		case GameStateType::PauseMenu:
		{
			ShutdownGameStatePauseMenu(*(GameStatePauseMenuData*)state.data, game);
			delete (GameStatePauseMenuData*)state.data;
			break;
		}
		case GameStateType::SettingsSound:
		{
			ShutdownGameStateSettings(*(GameStateSettingsSoundData*)state.data, game);
			delete (GameStateSettingsSoundData*)state.data;
			break;
		}
		default:
			assert(false); // We want to know if we forgot to implement new game state
			break;
		}
		state.data = nullptr;
	}
	void PushGameState(Game& game, GameStateType stateType, bool isExclusivelyVisible)
	{
		game.pendingGameStateType = stateType;
		game.pendingGameStateIsExclusivelyVisible = isExclusivelyVisible;
		game.gameStateChangeType = GameStateChangeType::Push;
	}

	void PopGameState(Game& game)
	{
		game.pendingGameStateType = GameStateType::None;
		game.pendingGameStateIsExclusivelyVisible = false;
		game.gameStateChangeType = GameStateChangeType::Pop;

	}
	void SwitchGameState(Game& game, GameStateType newState)
	{
		game.pendingGameStateType = newState;
		game.pendingGameStateIsExclusivelyVisible = false;
		game.gameStateChangeType = GameStateChangeType::Switch;
	}
	void DrawGameState(Game& game, GameState& state, sf::RenderWindow& window)
	{		
		switch (state.gametype)
		{
		case GameStateType::MainMenu:
		{
			DrawGameStateMainMenu(*(GameStateMainMenuData*)state.data, game, window);
			break;
		}
		case GameStateType::Playing:
		{
			DrawGameStatePlaying(*(GameStatePlayingData*)state.data, game, window);
			break;
		}
		case GameStateType::GameOver:
		{
			DrawGameStateGameOver(*(GameStateGameOverData*)state.data, game.dataPlaying, game, recordsData, window);
			break;
		}
		case GameStateType::Records:
		{
			DrawGameStateRecords(*(GameStateRecordsData*)state.data, game, window);
			break;
		}

		case GameStateType::PauseMenu:
		{
			DrawGameStatePauseMenu(*(GameStatePauseMenuData*)state.data, game, window);
			break;
		}
		case GameStateType::SettingsSound:
		{
			DrawGameStateSettings(*(GameStateSettingsSoundData*)state.data, game, window);
			break;
		}

		default:
			assert(false); // We want to know if we forgot to implement new game state
			break;
		}
	}
}
