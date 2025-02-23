#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"



namespace SnakeGame
{
	struct Game;
	struct GameStatePlayingData;
	struct MenuItem;
	struct GameStateRecordsData;
	struct UIState;

	enum class NameInputState
	{
		AskToEnterName,
		EnteringName,
		NameEntered
	};
	enum class GameOverOptionInputName
	{
		Yes,
		No
	};

	struct GameStateGameOverData
	{
		float timeSinceGameOver = 0.f;
		int playerScore = 0; // Add this line to store the score

		sf::Text gameOverText;
		sf::Text hintText;
		sf::Font font;
		sf::RectangleShape background;

		//For Enter Player Name
		bool isNamePlayerEntered = false;
		bool isNoSelected = false;
		sf::RectangleShape backgroundForNamePlayer;
		std::string playerName;
		NameInputState nameInputState = NameInputState::AskToEnterName;
		GameOverOptionInputName selectedOption = GameOverOptionInputName::No;		
	};
	void InitGameStateGameOverEnd(GameStateGameOverData& data, Game& game, GameStateRecordsData& recordsData);
	void DrawGameStateGameOver(GameStateGameOverData& data, GameStatePlayingData& dataPlaying, Game& game, GameStateRecordsData & recordsData, sf::RenderWindow& window);
	void ShutdownGameStateGameOver(GameStateGameOverData& data, Game& game);
	void HandleGameStateGameOverWindowEvent(GameStateGameOverData& data, Game& game, const sf::Event& event);
	void UpdateGameStateGameOver(GameStateGameOverData& data, Game& game, float deltaTime);	
}