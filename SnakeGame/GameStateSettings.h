#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "GameSettingsSound.h"

namespace SnakeGame
{
    struct Game;

    struct GameStateSettingsData
    {
        sf::Text titleText;
        sf::Text soundText;
        sf::Text musicText;
        sf::Text backText;
        
        int selectedOption = 0;
        const int NUM_OPTIONS = 3;
    };

    void InitGameStateSettings(GameStateSettingsData& data, Game& game);
    void ShutdownGameStateSettings(GameStateSettingsData& data, Game& game);
    void UpdateGameStateSettings(GameStateSettingsData& data, Game& game, float deltaTime);
    void DrawGameStateSettings(GameStateSettingsData& data, Game& game, sf::RenderWindow& window);
    void HandleGameStateSettingsEvent(GameStateSettingsData& data, Game& game, const sf::Event& event);
} 