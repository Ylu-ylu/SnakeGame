#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "GameStatePlaying.h"


namespace SnakeGame
{
    struct Game;
    struct GameStatePlayingData;

    struct GameSettingsSound
    {
        static GameSettingsSound& getInstance();
        GameSettingsSound() = default;
        bool isSoundEnabled() const;
        bool isMusicEnabled() const;

        bool m_isSoundEnabled = true;
        bool m_isMusicEnabled = true;
    };
    
    void InitGameSettingsSound(GameSettingsSound& sound, Game& game);   
    void ToggleSound(GameSettingsSound& sound);
    void ToggleMusic(GameSettingsSound& sound);
    void ApplySoundSettings(GameSettingsSound& sound, GameStatePlayingData& data);
    void ApplyMusicSettings(GameSettingsSound& sound, GameStatePlayingData& data);   
    
}