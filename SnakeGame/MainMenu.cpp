#include "MainMenu.h"
#include <cstdlib>
#include <assert.h>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "GameStatePlaying.h"

namespace SnakeGame
{
	void InitMenuItem(MenuItem& menu)
	{
		for (auto& child : menu.children)
		{
			child->parent = &menu;
			InitMenuItem(*child);
		}
	}
	UIState uiState;

	void SelectMenuItem(Menu& menu, MenuItem* item)
	{
		// Check error to select root item
		assert(item != &menu.rootItem);

		if (menu.selectedItem == item)
		{
			return;
		}

		if (item && !item->isEnabled)
		{
			return;
		}
		SelectMenuItemUI(uiState, menu, item);//From UI:choose colour				
	}

	bool SelectPreviousMenuItem(Menu& menu)
	{
		if (menu.selectedItem)
		{
			MenuItem* parent = menu.selectedItem->parent;
			assert(parent);//Should be parent

			auto it = std::find(parent->children.begin(), parent->children.end(), menu.selectedItem);
			if (it != parent->children.begin())
			{
				SelectMenuItem(menu, *(--it));
				return true;
			}
		}
		return false;
	}

	bool SelectNextMenuItem(Menu& menu)
	{
		if (menu.selectedItem)
		{
			MenuItem* parent = menu.selectedItem->parent;
			assert(parent); //  should be parent
			auto it = std::find(parent->children.begin(), parent->children.end(), menu.selectedItem);
			if (it != parent->children.end() - 1)
			{
				SelectMenuItem(menu, *(++it));
				return true;
			}
		}
		return false;
	}

	bool ExpandSelectedItem(Menu& menu)
	{
		if (menu.selectedItem && menu.selectedItem->children.size() > 0)
		{
			SelectMenuItem(menu, menu.selectedItem->children.front());
			return true;
		}
		return false;
	}

	bool CollapseSelectedItem(Menu& menu)
	{
		if (menu.selectedItem && menu.selectedItem->parent && menu.selectedItem->parent != &menu.rootItem)
		{
			SelectMenuItem(menu, menu.selectedItem->parent);
			return true;
		}
		return false;
	}

	MenuItem* GetCurrentMenuContext(Menu& menu)
	{
		return menu.selectedItem ? menu.selectedItem->parent : &menu.rootItem;
	}

	void DrawMenu(UIState& uiState, Menu& menu, sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin)
	{
		MenuItem* expandedItem = GetCurrentMenuContext(menu);

		std::vector<sf::Text*> texts;
		texts.reserve(expandedItem->children.size());
		for (auto& child : expandedItem->children)
		{
			if (child->isEnabled)
			{
				texts.push_back(&child->mainMenu);
			}
		}

		DrawItemsList2(window, texts, expandedItem->childrenSpacing, expandedItem->childrenOrientation, expandedItem->childrenAlignment, position, origin);
	}
}
