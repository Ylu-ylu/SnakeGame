#include "GameStateRecords.h"
#include "GameStatePlaying.h"
#include <assert.h>
#include <algorithm>
#include <sstream>
#include <iostream>
#include "UI.h"


namespace SnakeGame
{
	void InitStateGameStateRecords(GameStateRecordsData& data, Game& game)
	{		
		//Change "Player" to playerName
		const char* PLAYER_NAME = game.gameStateGameOverData.playerName.c_str();		
		
		assert(data.font.loadFromFile(RESOURCES_PATH + "\\Fonts\\Roboto-Regular.ttf"));
		//TitleText
		data.titleText.setString("Records");
		data.titleText.setFont(data.font);
		data.titleText.setCharacterSize(48);
		data.titleText.setFillColor(sf::Color::White);
		data.titleText.setOrigin(GetItemOrigin(data.titleText, { 0.5f, 0.5f }));

		//TableText
		data.tableTexts.clear();
		data.tableTexts.reserve(game.MAX_RECORDS_TABLE_SIZE);

		multimap<int, string> sortedRecordsTable;
		
		game.recordsTable[PLAYER_NAME] = game.gameStateGameOverData.playerScore;

		for (const auto& item : game.recordsTable)
		{
			sortedRecordsTable.insert(make_pair(item.second, item.first));
		}
		
		bool isPlayerInTable = false;
		auto it = sortedRecordsTable.rbegin();
		for (int i = 0; i < game.MAX_RECORDS_TABLE_SIZE && it != sortedRecordsTable.rend(); ++i, ++it)
		{
			//Create Text in back place
			data.tableTexts.emplace_back();
			sf::Text& text = data.tableTexts.back();		
		
			//Use stream for add to string and read from it
			stringstream sstream;
			sstream << i + 1 << "." << it->second << ":" << it->first;
			text.setString(sstream.str());
			text.setFont(data.font);
			text.setFillColor(sf::Color::White);
			text.setCharacterSize(20);			
			
			if (it->second == PLAYER_NAME)
			{										
				text.setFillColor(sf::Color::Green);
				isPlayerInTable = true;	  				
			}
			else
			{
				text.setFillColor(sf::Color::White);
			}
		}
		//Hint Text to return Main Menu
		data.hintText.setString("Press ESC to return back to main menu");
		data.hintText.setFont(data.font);
		data.hintText.setCharacterSize(24);
		data.hintText.setFillColor(sf::Color::White);
		data.hintText.setOrigin(GetItemOrigin(data.titleText, { 0.5f, 0.5f }));

		//Hint Text to Restart Game
		data.hintTextRestartGame.setString("Press Space to restart Game");
		data.hintTextRestartGame.setFont(data.font);
		data.hintTextRestartGame.setCharacterSize(24);
		data.hintTextRestartGame.setFillColor(sf::Color::White);
		data.hintTextRestartGame.setOrigin(GetItemOrigin(data.titleText, { 0.5f, 0.5f }));
	}
	void HandleGameStateRecordsWindowEvent(GameStateRecordsData& data, sf::Event& event, Game& game)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				PopGameState(game);
				SwitchGameState(game, GameStateType::MainMenu);
			}
			else if (event.key.code == sf::Keyboard::Space)
			{
				PopGameState(game);
				SwitchGameState(game, GameStateType::Playing);
			}
		}
	}

	void DrawGameStateRecords(GameStateRecordsData& data, Game& game, sf::RenderWindow& window)
	{
		sf::Vector2f viewSize = window.getView().getSize();

		data.titleText.setOrigin(GetItemOrigin(data.titleText, { 0.5f, 0.5f }));
		data.titleText.setPosition(viewSize.x / 2.f, 50.f);
		window.draw(data.titleText);

		//Create new vector for DrawItemList
		vector<sf::Text*>textList;
		textList.reserve(data.tableTexts.size());
		for (auto& text : data.tableTexts)
		{
			textList.push_back(&text);
		}

		sf::Vector2f tablePosition = { data.titleText.getGlobalBounds().left, viewSize.y / 2.f };
		///////Position Record  Table
		DrawItemsList2(window, textList, 18.f, Orientation::Vertical, Alignment::Center, tablePosition, { -0.3f,0.3f });

		//Hint Text to return Main Menu
		data.hintText.setOrigin(GetItemOrigin(data.hintText, { 0.5f, 0.5f }));
		data.hintText.setPosition(viewSize.x / 2.f, viewSize.y - 60.f);
		window.draw(data.hintText);	
		
		//Hint Text to Restart Game
		data.hintTextRestartGame.setOrigin(GetItemOrigin(data.hintTextRestartGame, { 0.5f, 0.5f }));
		data.hintTextRestartGame.setPosition(viewSize.x / 2.f, viewSize.y - 25.f);
		window.draw(data.hintTextRestartGame);
	}
	void ShutdownGameStateRecords(GameStateRecordsData& data, Game& game)
	{
		//Do nothing
	}
	void UpdateGameStateRecords(GameStateRecordsData& data, Game& game, float deltaTime)
	{
		//Do nothing
	}	
}