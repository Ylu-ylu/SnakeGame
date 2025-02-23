#pragma once
#include <SFML/Graphics.hpp>
#include "UI.h"
#include "Math.h"
#include <list>
#include "GameSettings.h"

namespace SnakeGame
{
	struct UIState;
	struct Game;
	struct GameStatePlayingData;
	struct MenuItem
	{
		Orientation childrenOrientation = Orientation::Vertical;
		Alignment childrenAlignment = Alignment::Center;
		float childrenSpacing;

		sf::Color selectedColor = sf::Color::Green;
		sf::Color deselectedColor = sf::Color::White;


		bool isEnabled = true;
		std::vector<MenuItem*> children;

		MenuItem* parent = nullptr;

		sf::Text mainMenu;
		sf::Text mainMenuhintText; // Visible when child item is selected

		LevelDifficulty difficulty;  // Add this for difficulty menu items
	};

	struct Menu
	{
		MenuItem rootItem;
		MenuItem* selectedItem = nullptr;
	};

	// Links children to parent
	void InitMenuItem(MenuItem& menu);
	void SelectMenuItem(Menu& menu, MenuItem* item);
	bool SelectPreviousMenuItem(Menu& menu);
	bool SelectNextMenuItem(Menu& menu);
	bool ExpandSelectedItem(Menu& menu);
	bool CollapseSelectedItem(Menu& menu);
	MenuItem* GetCurrentMenuContext(Menu& menu);
	void DrawMenu(UIState& uiState, Menu& menu, sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin);
}