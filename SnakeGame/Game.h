#pragma once
#include "GameSettings.h"
#include <SFML/Graphics.hpp>
#include "SFML/Audio.hpp"
#include "Math.h"
#include "Snake.h"
#include "Food.h"
#include "Rock.h"
#include "Constants.h"
#include <vector>
#include <algorithm>
#include "UI.h"
#include <unordered_map>
#include "GameStateGameOver.h"
#include "GameSettingSound.h"
#include "GameStateSettingSound.h"
#include "GameSettings.h"


namespace SnakeGame
{
	struct GameStateRecordsData;
	
	enum class GameStateType
	{
		None = 0,
		MainMenu,
		Playing,
		GameOver,
		Records,
		PauseMenu,
		SettingsSound,
	};

	struct GameState
	{
		GameStateType gametype = GameStateType::None;
		void* data = nullptr;
		bool isExclusivelyVisible = false;
	};

	enum class GameStateChangeType
	{
		None,
		Push,
		Pop,
		Switch
	};

	struct Game
	{		
		std::vector <GameState> gameStateStack;
		GameStateChangeType  gameStateChangeType = GameStateChangeType::None;
		GameStateType  pendingGameStateType = GameStateType::None;
		bool pendingGameStateIsExclusivelyVisible = false;		

		float timeSinceGameFinish = 0.f;

		//Record Table
		std::unordered_map <std::string, int> recordsTable;

		int MAX_RECORDS_TABLE_SIZE = 6;

		short GameMask = 0;//bitmap 0000000000000000*/

		Rectangle screenRect;	
		UIState uiState;
		GameSettings gameSettings;	
		GameStateGameOverData gameStateGameOverData;
		GameSettingsSound gameSettingsSound;
		FoodsGrid foodGrid;

		GameStatePlayingData dataPlaying;			
	};

	void InitGame(Game& game);
	void HandleWindowEvents(Game& game, sf::RenderWindow& window);
	void DrawGame(Game& game, sf::RenderWindow& window);
	void UpdateGameState(Game& game, GameState& state, float deltaTime);
	bool UpdateGame(Game& game, float deltaTime);
	void InitGameState(Game& game, GameState& state);
	void ShutdownGameState(Game& game, GameState& state);
	void ShutdownGame(Game& game);
	// Add new game state on top of the stack
	void PushGameState(Game& game, GameStateType stateType, bool isExclusivelyVisible);
	// Remove current game state from the stack
	void PopGameState(Game& game);
	// Remove all game states from the stack and add new one
	void SwitchGameState(Game& game, GameStateType newState);
	void HandleWindowEventGameState(Game& game, GameState& state, sf::Event& event);
	void DrawGameState(Game& game, GameState& state, sf::RenderWindow& window);
}
