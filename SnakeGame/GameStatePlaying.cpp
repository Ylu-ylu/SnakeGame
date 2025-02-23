#include "GameStatePlaying.h"
#include <assert.h>
#include "UI.h"
#include "Game.h"
#include "GameStateGameOver.h"
#include "Rock.h"

namespace SnakeGame
{
	void InitGameStatePlaying(GameStatePlayingData& data, Game& game)
	{
		LoadSnakeTextures(data.snake);
		//Textures
		assert(data.appleTexture.loadFromFile(RESOURCES_PATH + "Apple.png"));
		assert(data.rockTexture.loadFromFile(RESOURCES_PATH + "Rock.png"));		
		
		//Music
		assert(data.eatFoodoundBuffer.loadFromFile(RESOURCES_PATH + "AppleEat.wav"));
		assert(data.gameOverSoundBuffer.loadFromFile(RESOURCES_PATH + "Death.wav"));
		assert(data.gameBackgroundSoundBuffer.loadFromFile(RESOURCES_PATH + "Clinthammer__Background_Music.wav"));
		
		//Font
		assert(data.font.loadFromFile(RESOURCES_PATH + "\\Fonts\\Roboto-Regular.ttf"));			

		// Set amounts based on difficulty level
		int foodAmount = 0;
		int rockAmount = 0;

		// Apply sound and music settings
		auto& soundSettings = GameSettingsSound::getInstance();
		ApplySoundSettings(soundSettings, data);
		ApplyMusicSettings(soundSettings, data);

		// Configure background music
		data.gameBackgroundSound.setLoop(true);     // Make the music loop
		data.gameBackgroundSound.setVolume(soundSettings.isMusicEnabled() ? MUSIC_VOLUME : 0.0f);  // Set volume based on settings
		
		data.appleEatSound.setVolume(game.gameSettingsSound.isSoundEnabled() ? 100.f : 0.f);
		data.gameOverSound.setVolume(game.gameSettingsSound.isSoundEnabled() ? 100.f : 0.f);
		
		if (game.gameSettingsSound.isMusicEnabled())
		{
			data.gameBackgroundSound.setVolume(MUSIC_VOLUME);  // Lower volume for background music
			data.gameBackgroundSound.play();
		}
		else
		{
			data.gameBackgroundSound.setVolume(0.f);
			data.gameBackgroundSound.stop();
		}
		// Get difficulty parameters
		DifficultyParams parameters;
		game.gameSettings.getDifficultyParamsLevel(parameters);

		// Initialize pause timer with difficulty-specific value
		data.pauseTimer = parameters.movePause;
		data.isGameStarted = false;

		switch (game.gameSettings.currentLevelDifficulty)
		{
		case LevelDifficulty::Easy:
			
			foodAmount = 1;
			rockAmount = 1;
			break;
		case LevelDifficulty::HeavierEasy:
			
			foodAmount = 2;
			rockAmount = 2;
			break;
		case LevelDifficulty::Medium:

			foodAmount = 3;
			rockAmount = 3;
			break;
		case LevelDifficulty::Hard:

			foodAmount = 4;
			rockAmount = 4;
			break;
		case LevelDifficulty::Max:

			foodAmount = 5;
			rockAmount = 5;
			// Make rocks disappear and reappear
			for (auto& rock : data.rocks) 
			{
				rock.isTemporary = true;
			}
			break;
		}
		
		// Resize vectors with correct amounts
		data.food.resize(foodAmount);
		data.rocks.resize(rockAmount);		

		// Init game objects
		game.screenRect = { 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT };		

		InitSnake(data.snake);

		//for appleGrid
		ClearFoodGrid(game.foodGrid);

		data.numFood = NUM_FoodS + rand() / (float)RAND_MAX;

		data.numEatenFood = 0;
		// Init Food
		for (auto& elementFood : data.food)
		{
			InitFood(elementFood, game, data);
			ResetFoodState(elementFood);
			AddFoodToGrid(game.foodGrid, elementFood, data.snake);
		}
		// Init rocks		
		for (auto& elementRocks : data.rocks)
		{
			InitRock(elementRocks, game, data,true);
			Position2D randomPos = GetRandomPositionRectangle(elementRocks.sprite, game.screenRect);
			SetRockPosition(elementRocks, randomPos, data.snake,data);			
		}

		//Inicialisation UI
		InitUI(game.uiState, data.font);

		//Init background
		data.background.setSize(sf::Vector2f(game.screenRect.size.x, game.screenRect.size.y));
		data.background.setFillColor(sf::Color::Green);
		data.background.setOrigin(0.f, 0.f);

		//Init ScoreLine
		data.gameRectScores.setSize(sf::Vector2f(game.screenRect.size.x, game.screenRect.size.y/20));
		data.gameRectScores.setFillColor(sf::Color(35, 107, 54));
		data.gameRectScores.setOrigin(0.f, 0.f);

		//Init Sound
		data.appleEatSound.setBuffer(data.eatFoodoundBuffer);
		data.gameOverSound.setBuffer(data.gameOverSoundBuffer);
		data.gameBackgroundSound.setBuffer(data.gameBackgroundSoundBuffer);

		// Configure background music
		data.gameBackgroundSound.setLoop(true);     // Make the music loop
		data.gameBackgroundSound.setVolume(30.0f);  // Set volume to 30% (adjust as needed)
		data.gameBackgroundSound.play();
	}

	void ShutdownGameStatePlaying(GameStatePlayingData& data, Game& game)
	{
		// We dont need to free resources here, because they will be freed automatically
	}

	void HandleGameStatePlayingWindowEvent(GameStatePlayingData& data, Game& game, const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				PushGameState(game, GameStateType::PauseMenu, false);
			}
		}
	}

	void UpdateGameStatePlaying(GameStatePlayingData& data, Game& game, float deltaTime)
	{
		// Update pause timer if game hasn't started
		if (!data.isGameStarted)
		{
			int currentSecond = static_cast<int>(std::ceil(data.pauseTimer));
			int newSecond = static_cast<int>(std::ceil(data.pauseTimer));

			data.pauseTimer -= deltaTime; 
			game.uiState.pauseTimerText.setString("Game starting in: " + std::to_string((int)std::ceil(data.pauseTimer)));
			
			if (data.pauseTimer <= 0)
			{
				data.isGameStarted = true;
			}
			return;  // Don't process game logic during pause
		}
				
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)||(sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
		{
			data.snake.direction = SnakeDirection::Right;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || (sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
		{
			data.snake.direction = SnakeDirection::Up;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A)))
		{
			data.snake.direction = SnakeDirection::Left;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || (sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
		{
			data.snake.direction = SnakeDirection::Down;
		}		
		
		// Set snake speed based on current difficulty level
		switch (game.gameSettings.currentLevelDifficulty)
		{
		case LevelDifficulty::Easy:
			data.snake.speed = INITIAL_SPEED * 1.0f;
			break;
		case LevelDifficulty::HeavierEasy:
			data.snake.speed = INITIAL_SPEED * 2.0f;
			break;
		case LevelDifficulty::Medium:
			data.snake.speed = INITIAL_SPEED * 3.0f;
			break;
		case LevelDifficulty::Hard:
			data.snake.speed = INITIAL_SPEED * 4.0f;
			break;
		case LevelDifficulty::Max:
			data.snake.speed = INITIAL_SPEED * 5.0f;
			break;
		}
		//Update snake 
		MoveSnake(data.snake, deltaTime);

		FoodsSet collidingFood;
		Vector2Df forwardPointPosition = { data.snake.head->getPosition().x, data.snake.head->getPosition().y };
		//New find snake collision in gridcells
		if (FindPlayerCollisionWhithFoods(forwardPointPosition, game.foodGrid, collidingFood))
		{
			for (Food* food : collidingFood)
			{
				if (game.gameSettingsSound.isSoundEnabled())
				{
					data.appleEatSound.setVolume(SOUND_VOLUME);
					data.appleEatSound.play();
				}			

				MarkFoodAsEaten(*food);
				RemoveFoodFromGrid(game.foodGrid, *food);

				//Grow Snake body
				GrowSnake(data.snake);

				//For infinity apple
				ResetFoodState(*food);
				AddFoodToGrid(game.foodGrid, *food, data.snake);

				// Add points based on difficulty level
				switch (game.gameSettings.currentLevelDifficulty)
				{
				case LevelDifficulty::Easy:
					data.numEatenFood += 2;
					break;
				case LevelDifficulty::HeavierEasy:
					data.numEatenFood += 4;
					break;
				case LevelDifficulty::Medium:
					data.numEatenFood += 6;
					break;
				case LevelDifficulty::Hard:
					data.numEatenFood += 8;
					break;
				case LevelDifficulty::Max:
					data.numEatenFood += 10;
					break;
				}
								
			}
		}
		
		for (auto& elementRocks : data.rocks)
		{
			DifficultyParams parameters;
			const DifficultyParams& parameter = game.gameSettings.getDifficultyParamsLevel(parameters);
			UpdateRock(deltaTime, parameters, elementRocks,data.snake, data);
		}

		bool isGameFinished = (data.numEatenFood == data.food.size());

		// Find snake collisions with rocks
		const char* PLAYER_NAME = game.gameStateGameOverData.playerName.c_str();
		
		for (auto& elementRocks : data.rocks)
		{
			if (isGameFinished || DoShapeCollide(GetSnakeCollider(data.snake), GetRockCollider(elementRocks)))
			{				
				data.gameBackgroundSound.stop();
				
				game.gameStateGameOverData.playerScore = data.numEatenFood; // Add this line
				// Activate Table Records
				game.recordsTable[game.gameStateGameOverData.playerName] = std::max(game.recordsTable[game.gameStateGameOverData.playerName], data.numEatenFood);

				PushGameState(game, GameStateType::GameOver, false);
				if (game.gameSettingsSound.isSoundEnabled())
				{
					data.gameOverSound.setVolume(SOUND_VOLUME);
					data.gameOverSound.play();
				}
				// Ensure the player's name is not empty
				std::string playerName = game.gameStateGameOverData.playerName.empty() ? "Player" : game.gameStateGameOverData.playerName;
			}
		}
		//Collide with gameRectScore
		sf::FloatRect globalBounds = data.background.getGlobalBounds();
		globalBounds.width = game.screenRect.size.x;
		globalBounds.height =  (game.screenRect.size.y - game.screenRect.size.y / 20);
		globalBounds.top = game.screenRect.position.y + game.screenRect.size.y / 20;

		// Check screen borders collision and himself
		if (isGameFinished || !HasSnakeCollisionWithRect(data.snake,globalBounds)
			|| CheckSnakeCollisionWithHimself(data.snake))
		{
			data.gameBackgroundSound.stop();
			game.gameStateGameOverData.playerScore = data.numEatenFood; // Add this line
			
			// Activate Table Records
			game.recordsTable[game.gameStateGameOverData.playerName] =(game.recordsTable[game.gameStateGameOverData.playerName], data.numEatenFood);

			PushGameState(game, GameStateType::GameOver, false);
			if (game.gameSettingsSound.isSoundEnabled())
			{
				data.gameOverSound.setVolume(SOUND_VOLUME);
				data.gameOverSound.play();
			}
			// Ensure the player's name is not empty
			std::string playerName = game.gameStateGameOverData.playerName.empty() ? "Player" : game.gameStateGameOverData.playerName;
		}
		
		game.uiState.scoreText.setString("Food eaten:" + std::to_string(data.numEatenFood));

		// Update background music state
		if (game.gameSettingsSound.isMusicEnabled())
		{
			data.gameBackgroundSound.setVolume(MUSIC_VOLUME);
			if (data.gameBackgroundSound.getStatus() != sf::Sound::Playing)
			{
				data.gameBackgroundSound.play();
			}
		}
		else
		{
			data.gameBackgroundSound.stop();
			data.gameBackgroundSound.setVolume(0.f);
		}		
	}

	void DrawGameStatePlaying(GameStatePlayingData& data, Game& game, sf::RenderWindow& window)
	{
		// Draw background
		window.draw(data.background);
		
		DrawFoodGrid( game.foodGrid,  window);

		window.draw(data.gameRectScores);

		//Draw snake
		DrawSnake(data.snake, window);

		//Draw Food
		for (auto& elementFood : data.food)
		{
			DrawFood(elementFood, window);
		}
		//Draw Rocks
		for (auto& elementRocks : data.rocks)
		{
			DrawRock(elementRocks, window);
		}

		game.uiState.scoreText.setOrigin(GetItemOrigin(game.uiState.scoreText, { 0.f,0.f }));
		game.uiState.scoreText.setPosition(10.f, 4.5f);
		window.draw(game.uiState.scoreText);

		sf::Vector2f viewSize = window.getView().getSize();
		data.inputHintText.setPosition(viewSize.x - 10.f, 10.f);
		window.draw(data.inputHintText);

		if (!data.isGameStarted)
		{
			//Draw Pause Time to Start Game
			game.uiState.pauseTimerText.setPosition(viewSize.x / 1.78f, 170.f);
			window.draw(game.uiState.pauseTimerText);
		}		
	}	
}