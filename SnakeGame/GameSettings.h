#pragma once
#include <SFML/Graphics.hpp>
#include "GameStatePlaying.h"
#include <assert.h>
#include <vector>
#include "Snake.h"
#include "Constants.h"
#include "Rock.h"

namespace SnakeGame
{
    enum class LevelDifficulty
    {
        Easy,
        HeavierEasy,
        Medium,
        Hard,
        Max
    };
    struct Snake;
    struct Game;
    
  
    struct DifficultyParams
    {
        float speedLevel;
        float scorePerFoodLevel;
       // float movePause = PAUSE_TO_MOVE;
        float movePause;
        // Add new parameters for rock timing
        float rockVisibilityDuration = 5.0f;    // How long rocks stay visible (seconds)
        float rockInvisibilityDuration = 3.0f;  // How long rocks stay invisible (seconds)
        bool enableTemporaryRocks = false;      // Only true for Max difficulty
        float temporaryRocksPercentage = 0.0f;
    };

    struct GameSettings
    {
        LevelDifficulty currentLevelDifficulty = LevelDifficulty::Easy;
        std::vector <LevelDifficulty> levelDifficulty;

        // Difficulty parameters
        GameStatePlayingData data;

        DifficultyParams getDifficultyParamsLevel(DifficultyParams& parameters) const;
        void SetDifficultyLevel(LevelDifficulty& difficulty);     
        void SwitchGameDifficulty(LevelDifficulty& difficulty);       
        
    };


}