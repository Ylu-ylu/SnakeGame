#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "GameSettingsSound.h"
#include "Game.h"

namespace SnakeGame
{
  
    struct UIState;
    struct Game;
    struct GameStatePlayingData;
    struct GameSettingsSound;
    enum class GameStateType;

    struct GameStateSettingsSoundData
    {
        GameStatePlayingData playingdata;
        int selectedOption = 0;
        const int NUM_OPTIONS = 3;
    };

    void InitGameStateSettings(GameStateSettingsSoundData& data, Game& game);
    void ShutdownGameStateSettings(GameStateSettingsSoundData& data, Game& game);
    void UpdateGameStateSettings(GameStateSettingsSoundData& data, Game& game, float deltaTime);
    void DrawGameStateSettings(GameStateSettingsSoundData& data, Game& game, sf::RenderWindow& window);
    void HandleGameStateSettingsEvent(GameStateSettingsSoundData& data, Game& game, GameSettingsSound& sound, const sf::Event& event);
    void SwitchGameStateSound(Game& game, GameStateType newState);
}