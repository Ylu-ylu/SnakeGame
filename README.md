### Start Game
To start the game, run the SnakeGame.sln file in your IDE.

### Core Gameplay
- Classic snake movement mechanics with collision detection
- Progressive difficulty system with 5 levels (Easy to Max)
- Dynamic food spawning system with multiple food types
- Score tracking with bonus multipliers
- Growing snake mechanics (expands from tail)

### Enhanced Features
- **Smart Collision System**
  - Adjustable collision boundaries
  - Precise hitbox detection
  - Visual/mechanical collision separation
- **Obstacle System**
  - Stationary rocks with timed visibility
  - Temporary invisible obstacles (Max difficulty)
- **Audio System**
  - Background music with volume control
  - Sound effects toggle
  - Separate controls for music/SFX

### Technical Features
- SFML-based graphics rendering
- Object-oriented architecture
- Configurable difficulty parameters
- Responsive controls with input buffering
- Adaptive frame rate handling

## Installation

### Requirements
- C++17 compiler
- SFML 2.5.1+
- CMake 3.12+

## Controls
| Action          | Key           |
|-----------------|---------------|
| Move Up         | W / ↑         |
| Move Down       | S / ↓         |
| Move Left       | A / ←         |
| Move Right      | D / →         |
| Pause           | ESC           |

## Settings
Customize in `GameSettings.h`:
- Initial snake speed
- Rock spawn intervals
- Food score values
- Visibility timers
- Collision buffer sizes

## Difficulty Levels
1. **Easy** - Basic gameplay
2. **HeavierEasy** - Increased speed x2, amount food x2
3. **Medium** - Increased speed x4, amount food x4
4. **Hard** - Increased speed x6, amount food x6
5. **Max** - Expert mode with combo system
