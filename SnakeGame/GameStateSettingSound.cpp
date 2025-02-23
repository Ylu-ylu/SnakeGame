#include "GameStateSettingSound.h"
#include "UI.h"
#include "GameStatePlaying.h"



namespace SnakeGame
{
	void InitGameStateSettings(GameStateSettingsSoundData& data, Game& game)
	{
		InitUI(game.uiState, game.uiState.font);
	}

	void ShutdownGameStateSettings(GameStateSettingsSoundData& data, Game& game)
	{
        //Do nothing
	}

	void UpdateGameStateSettings(GameStateSettingsSoundData& data, Game& game, float deltaTime)
	{
        auto& soundSettings = GameSettingsSound::getInstance();
        game.uiState.soundText.setString("Sound: " + std::string(soundSettings.isSoundEnabled() ? "ON" : "OFF"));
        game.uiState.musicText.setString("Music: " + std::string(soundSettings.isMusicEnabled() ? "ON" : "OFF"));
        game.uiState.soundText.setFillColor(data.selectedOption == 0 ? sf::Color::Yellow : sf::Color::White);
        game.uiState.musicText.setFillColor(data.selectedOption == 1 ? sf::Color::Yellow : sf::Color::White);
        game.uiState.backSoundMusicText.setFillColor(data.selectedOption == 2 ? sf::Color::Yellow : sf::Color::White);
	}

	void DrawGameStateSettings(GameStateSettingsSoundData& data, Game& game, sf::RenderWindow& window)
	{
        window.draw(game.uiState.titleSoundMusicText);
        window.draw(game.uiState.soundText);
        window.draw(game.uiState.musicText);
        window.draw(game.uiState.backSoundMusicText);
	}

	void HandleGameStateSettingsEvent(GameStateSettingsSoundData& data, Game& game, GameSettingsSound& sound, const sf::Event& event)
	{
        if (event.type == sf::Event::KeyPressed)
        {
            auto& soundSettings = GameSettingsSound::getInstance();
           
            switch (event.key.code)
            {
            case sf::Keyboard::Up:
            case sf::Keyboard::W:
                data.selectedOption = (data.selectedOption - 1 + data.NUM_OPTIONS) % data.NUM_OPTIONS;
                break;

            case sf::Keyboard::Down:
            case sf::Keyboard::S:
                data.selectedOption = (data.selectedOption + 1) % data.NUM_OPTIONS;
                break;

            case sf::Keyboard::Enter:
                switch (data.selectedOption)
                {
                case 0: // Sound
                    ToggleSound(sound);
                    ApplySoundSettings(sound,data.playingdata);
                   game.uiState.soundText.setString("Sound: " + std::string(soundSettings.isSoundEnabled() ? "ON" : "OFF"));
                    break;

                case 1: // Music
                    ToggleMusic(sound);
                    ApplyMusicSettings(sound, data.playingdata);
                   game.uiState.musicText.setString("Music: " + std::string(soundSettings.isMusicEnabled() ? "ON" : "OFF"));
                    break;

                case 2: // Back
                    PopGameState(game);
                    break;
                }
                break;

            case sf::Keyboard::Escape:
                PopGameState(game);
                break;
            }
        }

	}

    void SwitchGameStateSound(Game& game, GameStateType newState)
    {
        // Shutdown the current state
        while (!game.gameStateStack.empty())
        {
            ShutdownGameState(game, game.gameStateStack.back());
            game.gameStateStack.pop_back();
        }

        // Initialize the new state
        if (newState != GameStateType::None)
        {
            PushGameState(game, newState, false);

            // If the new state is Playing, apply sound and music settings
            if (newState == GameStateType::Playing)
            {
                auto& soundSettings = GameSettingsSound::getInstance();
                if (!game.gameStateStack.empty()) // Check if the stack is not empty
                {
                    auto* playingData = static_cast<GameStatePlayingData*>(game.gameStateStack.back().data);
                    if (playingData)
                    {
                        ApplySoundSettings(soundSettings, *playingData);
                        ApplyMusicSettings(soundSettings, *playingData);
                    }
                }
            }
        }
    }
}

