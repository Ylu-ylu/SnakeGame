#include "GameSettings.h"
#include "Game.h"




namespace SnakeGame
{
    DifficultyParams GameSettings::getDifficultyParamsLevel(DifficultyParams& parameters) const
    {
        switch (currentLevelDifficulty)
        {
        case LevelDifficulty::Easy:
            parameters.speedLevel = INITIAL_SPEED * 1.0f;
            parameters.scorePerFoodLevel = 2.0f;
            parameters.movePause = PAUSE_TO_MOVE;  // 3 seconds for Easy
            parameters.enableTemporaryRocks = false;
            break;

        case LevelDifficulty::HeavierEasy:
            parameters.speedLevel = INITIAL_SPEED * 2.0f;
            parameters.scorePerFoodLevel = 4.0f;
            parameters.movePause = PAUSE_TO_MOVE * 0.8f;  // 2.4 seconds
            parameters.enableTemporaryRocks = false;
            break;

        case LevelDifficulty::Medium:
            parameters.speedLevel = INITIAL_SPEED * 3.0f;
            parameters.scorePerFoodLevel = 6.0f;
            parameters.movePause = PAUSE_TO_MOVE * 0.6f;  // 1.8 seconds
            parameters.enableTemporaryRocks = false;
            break;

        case LevelDifficulty::Hard:
            parameters.speedLevel = INITIAL_SPEED * 4.0f;
            parameters.scorePerFoodLevel = 8.0f;
            parameters.movePause = PAUSE_TO_MOVE * 0.4f;  // 1.2 seconds
            parameters.enableTemporaryRocks = false;
            break;

        case LevelDifficulty::Max:
            parameters.speedLevel = INITIAL_SPEED * 5.0f;
            parameters.scorePerFoodLevel = 10.0f;
            parameters.movePause = PAUSE_TO_MOVE * 0.2f;  // 0.6 seconds
            parameters.enableTemporaryRocks = true;
            parameters.temporaryRocksPercentage = 1.0f;
            break;
        }
        return parameters;
    }

    void GameSettings::SetDifficultyLevel(LevelDifficulty& difficulty)
	{
        // Validate difficulty range
        if (difficulty >= LevelDifficulty::Easy && difficulty <= LevelDifficulty::Max)
        {
            currentLevelDifficulty = difficulty;
        }
        else
        {
            // Default to Easy
            currentLevelDifficulty = LevelDifficulty::Easy;
        }
	}

    void GameSettings::SwitchGameDifficulty(LevelDifficulty& difficulty)
    {
        // Check if difficulty already exists in vector
        auto it = std::find(levelDifficulty.begin(), levelDifficulty.end(), difficulty);

        if (it != levelDifficulty.end())
        {
            // If found, remove it
            levelDifficulty.erase(it);
        }
        else
        {
            // If not found, add it
            levelDifficulty.push_back(difficulty);
        }

        // Update current difficulty
        currentLevelDifficulty = difficulty;
    }


}


