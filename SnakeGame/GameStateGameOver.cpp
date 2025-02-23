#include "GameStateGameOver.h"
#include "Game.h"
#include <iostream>
#include <assert.h>
#include <string>
#include "GameStateRecords.h"
#include "UI.h"
#include"GameStatePlaying.h"


namespace SnakeGame
{		
	GameStateRecordsData recordsData;
	void InitGameStateGameOverEnd(GameStateGameOverData& data, Game& game, GameStateRecordsData& recordsData)
	{
		assert(data.font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

		data.timeSinceGameOver = 0.f;

		//Background
		sf::Color backgroundColour = sf::Color::Black;
		backgroundColour.a = 135;//alfa opacity
		data.background.setFillColor(backgroundColour);

		InitUI(game.uiState, data.font);
		if(data.nameInputState == NameInputState::AskToEnterName)
		{
			sf::Color backgroundForNamePlayerColour = sf::Color::Red;
			backgroundForNamePlayerColour.a = 155;//alfa opacity
			data.backgroundForNamePlayer.setFillColor(backgroundForNamePlayerColour);
		}
		else if (data.nameInputState == NameInputState::EnteringName)
		{
			sf::Color backgroundForNamePlayerColour = sf::Color::Red;
			backgroundForNamePlayerColour.a = 175;//alfa opacity
			data.backgroundForNamePlayer.setFillColor(backgroundForNamePlayerColour);
		}
		else if (data.nameInputState == NameInputState::NameEntered)
		{
			InitStateGameStateRecords(recordsData, game);
		}
	}
	void DrawGameStateGameOver(GameStateGameOverData& data, GameStatePlayingData& dataPlaying, Game& game, GameStateRecordsData& recordsData, sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = window.getView().getSize();
		
		// Game Over Text
		game.uiState.gameOverText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f);
		window.draw(game.uiState.gameOverText);
		
		// Background
		data.background.setSize(viewSize);
		data.background.setOrigin(0.f, 0.f);
		window.draw(data.background);
		
		game.uiState.hintText.setPosition(viewSize.x / 2.f, viewSize.y - 10.f);
		window.draw(game.uiState.hintText);
		
		if (data.nameInputState == NameInputState::AskToEnterName)
		{
			data.backgroundForNamePlayer.setSize(sf::Vector2f(200.f, 200.f));
			data.backgroundForNamePlayer.setOrigin(0.5f, 0.5f);
			data.backgroundForNamePlayer.setPosition(viewSize.x / 2.55f, viewSize.y / 2.95f);
			window.draw(data.backgroundForNamePlayer);
			
			//Draw input Player Name("Enter your name: ")
			game.uiState.inputNamePlayer.setPosition(viewSize.x / 2.f, viewSize.y / 2.3f);
			window.draw(game.uiState.inputNamePlayer);

			// Yes Text
			game.uiState.yesText.setFillColor(data.selectedOption == GameOverOptionInputName::Yes ? sf::Color::Green : sf::Color::White);
			game.uiState.yesText.setPosition(viewSize.x / 2.0f, viewSize.y / 2.0f);
			window.draw(game.uiState.yesText);

			// No Text
			game.uiState.noText.setFillColor(data.selectedOption == GameOverOptionInputName::No ? sf::Color::Green : sf::Color::White);
			game.uiState.noText.setPosition(viewSize.x / 2.0f, viewSize.y / 2.f + 50.f);
			window.draw(game.uiState.noText);
		}
		else if (data.nameInputState == NameInputState::EnteringName)
		{
			// Input Text
			game.uiState.inputNamePlayer.setPosition(viewSize.x / 2.f, viewSize.y / 2.5f);
			window.draw(game.uiState.inputNamePlayer);

			
			game.uiState.inputNamePlayerHintText.setPosition(viewSize.x / 2.0f, viewSize.y / 2.0f);
			game.uiState.inputNamePlayerHintText.setOrigin(GetItemOrigin(game.uiState.inputNamePlayerHintText, { 0.5f,0.5f }));
			window.draw(game.uiState.inputNamePlayerHintText);
			
			data.backgroundForNamePlayer.setSize(sf::Vector2f(200.f, 200.f));
			data.backgroundForNamePlayer.setOrigin(0.5f, 0.5f);
			data.backgroundForNamePlayer.setPosition(viewSize.x / 2.55f, viewSize.y / 2.95f);
			window.draw(data.backgroundForNamePlayer);					
		}	
		else if (data.nameInputState == NameInputState::NameEntered)
		{
			//Draw Table Records
			DrawGameStateRecords(recordsData, game, window);
		}
	}

	void ShutdownGameStateGameOver(GameStateGameOverData& data, Game& game)
	{
		// We dont need to free resources here, because they will be freed automatically
	}
	void HandleGameStateGameOverWindowEvent(GameStateGameOverData& data, Game& game, const sf::Event& event)
	{			
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Space)
			{
				SwitchGameState(game, GameStateType::Playing);
			}
			else if (event.key.code == sf::Keyboard::Escape)
			{
				SwitchGameState(game, GameStateType::MainMenu);
			}
			
			if (data.nameInputState == NameInputState::AskToEnterName)
			{
				if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W)
				{
					data.selectedOption = GameOverOptionInputName::Yes;
				}
				else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)
				{
					data.selectedOption = GameOverOptionInputName::No;
				}
				else if (event.key.code == sf::Keyboard::Enter)
				{
					if (data.selectedOption == GameOverOptionInputName::Yes)
					{
						data.nameInputState = NameInputState::EnteringName;
					}
					else
					{
						data.playerName = "Player";
						data.isNoSelected = true;
						SwitchGameState(game, GameStateType::Records);
					}
				}
				else if (event.key.code == sf::Keyboard::Escape)
				{
					SwitchGameState(game, GameStateType::MainMenu);
				}
			}
			else if (data.nameInputState == NameInputState::EnteringName)
			{
				//Handle Name Input
				if (event.key.code == sf::Keyboard::Return && !data.playerName.empty())
				{					
					data.playerScore= game.dataPlaying.numEatenFood;
				
					game.gameStateGameOverData.playerName = data.playerName;
					
					game.recordsTable[data.playerName] = data.playerScore; // Use stored score					
					
					data.nameInputState = NameInputState::NameEntered;
					InitStateGameStateRecords(recordsData, game);
					SwitchGameState(game, GameStateType::Records);

				}
				else if (event.key.code == sf::Keyboard::Escape)
				{
					data.nameInputState = NameInputState::AskToEnterName;
				}
				else if (event.key.code == sf::Keyboard::Return)
				{
					if (event.key.code == sf::Keyboard::Enter&&!data.playerName.empty())
					{						
						game.recordsTable[data.playerName] = game.dataPlaying.numEatenFood;
						
						InitStateGameStateRecords(recordsData, game);
						data.nameInputState = NameInputState::NameEntered;
						SwitchGameState(game, GameStateType::Records);
					}
				}				
			}
		}
		else if (event.type == sf::Event::TextEntered && data.nameInputState == NameInputState::EnteringName)
		{
			if (event.text.unicode == '\b' && !data.playerName.empty()) // Backspace
			{
				data.playerName.pop_back();
			}
			else if (event.text.unicode < 128 && event.text.unicode != '\r' && event.text.unicode != '\n')
			{
				data.playerName += static_cast<char>(event.text.unicode);
			}
			game.uiState.inputNamePlayerHintText.setString(data.playerName);
		}			
	}
	void UpdateGameStateGameOver(GameStateGameOverData& data, Game& game, float deltaTime)
	{
		data.timeSinceGameOver += deltaTime;
		if (data.nameInputState == NameInputState::NameEntered && !data.isNoSelected)
		{
			std::cout << "Update game over" << std::endl;
		
			// Add the player's name and score to the records table
			game.recordsTable[data.playerName] = game.dataPlaying.numEatenFood;

			// Sort the records table
			//sort(game.recordsTable.begin(), game.recordsTable.end(), [](const pair<string, int>& a, const pair<string, int>& b) { return a.second > b.second; });
			InitStateGameStateRecords(recordsData, game);
			// Switch to Records state
			SwitchGameState(game, GameStateType::Records);
		}
	}
}
		

	
	
