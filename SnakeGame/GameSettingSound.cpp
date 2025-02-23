#include "GameSettingSound.h"
#include "Game.h"

namespace SnakeGame
{  

    GameSettingsSound& GameSettingsSound::getInstance()
    {
        static GameSettingsSound instance;
        return instance;
    }

    bool GameSettingsSound::isSoundEnabled() const
    {
        return  m_isSoundEnabled;
    }

    bool GameSettingsSound::isMusicEnabled() const
    {
        return m_isMusicEnabled; 
    }

    void InitGameSettingsSound(GameSettingsSound& sound, Game& game)
    {
       sound.m_isSoundEnabled = true;
       sound.m_isMusicEnabled = true;
    }

    void ToggleSound(GameSettingsSound& sound)
    {
       sound. m_isSoundEnabled = !sound.m_isSoundEnabled;      
    }

    void ToggleMusic(GameSettingsSound& sound)
    {
       sound. m_isMusicEnabled = !sound.m_isMusicEnabled;
    }     

    void ApplySoundSettings(GameSettingsSound& sound, GameStatePlayingData& data)
    {
        float volume = sound.m_isSoundEnabled ? SOUND_VOLUME : 0.0f;
        data.appleEatSound.setVolume(volume);
        data.gameOverSound.setVolume(volume);      

    }
    void ApplyMusicSettings(GameSettingsSound& sound, GameStatePlayingData& data)
    {
        float volume = sound.m_isMusicEnabled ? MUSIC_VOLUME : 0.0f;
        data.gameBackgroundSound.setVolume(volume);
    } 


}