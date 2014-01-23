/*
 * Copyright 2013 Cepheid
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
*/
#ifndef MenuHPP
#define MenuHPP

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include "Component.hpp"
#include "Font.hpp"
#include "MenuItem.hpp"

namespace io {
  class Menu;

  class MenuActionListener {
  public:
    virtual ~MenuActionListener() { }
    virtual void onMenuItemSelected(Menu* notifier, MenuItem* which) = 0;
  private:
  };

  class Menu : public Component {
  public:
    Menu();
    virtual ~Menu() { }

    void addMenuActionListener(MenuActionListener* listener) {
      if (listener && !hasMenuActionListener(listener)) {
        menuActionListeners.push_back(listener);
      }
    }

    MenuItem* addMenuItem(const uint32_t id, const std::string& text, bool enabled) {
      MenuItem* ret = nullptr;
      if (!hasID(id)) {
        ret = new MenuItem(id, text, enabled);
        menuItems.push_back(ret);
      }
      return ret;
    }

    void clear() {
      for (MenuItem* m : menuItems) {
        delete m;
      }
      menuItems.clear();
    }

    Font* getFont() const {
      return font;
    }

    uint32_t getMaxVisibleItems() const {
      return maxVisibleItems;
    }

    MenuItem* getMenuItem(const uint32_t id) {
      for (MenuItem* i : menuItems) {
        if (i->getID() == id) {
          return i;
        }
      }

      return nullptr;
    }

    MenuItem* getSelectedMenuItem() const {
      if (menuItems.size() == 0) {
        return nullptr;
      }

      return menuItems.at(menuSelection);
    }

    virtual bool handleInputEvent(const InputEvent& event) {
      if (event.getState() == InputEventState::DOWN) {
        switch(event.getType()) {
          case InputEventType::UP:
            moveSelectionUp();
            return true;
          case InputEventType::DOWN:
            moveSelectionDown();
            return true;
          case InputEventType::ACCEPT:
            activateSelectedItem();
            return true;
          default:
            break;
        }
      }

      return false;
    }

    void moveSelectionDown();
    void moveSelectionUp();

    void removeMenuActionListener(MenuActionListener* listener) {
      if (listener && hasMenuActionListener(listener)) {
        menuActionListeners.remove(listener);
      }
    }

    void removeMenuItem(const uint32_t id) {
      std::vector<MenuItem*>::iterator iter = menuItems.begin();
      while ((*iter)->getID() != id && iter != menuItems.end()) {
        iter++;
      }

      if (iter != menuItems.end()) {
        menuItems.erase(iter);
      }
    }

    void setFont(Font* font) {
      this->font = font;
    }

    void setMaxVisibleItems(const uint32_t maxVisibleItems) {
      this->maxVisibleItems = maxVisibleItems;

      windowEnd = windowStart + maxVisibleItems;

      validateWindow();
    }
  protected:
    virtual void render(Graphics* g);
  private:
    Menu(const Menu&);
    Menu& operator=(const Menu&);

    Font* font;
    uint32_t maxVisibleItems;
    std::list<MenuActionListener*> menuActionListeners;
    std::vector<MenuItem*> menuItems;
    uint32_t menuSelection;

    //  These are related to the number of visible items.
    uint32_t windowStart;
    uint32_t windowEnd;

    void activateSelectedItem() {
      if (getSelectedMenuItem() && getSelectedMenuItem()->isEnabled()) {
        notifyListeners();
      }
    }

    uint32_t getMenuSelection() const {
      return menuSelection;
    }

    bool hasID(const uint32_t id) {
      for (MenuItem* i : menuItems) {
        if (i->getID() == id) {
          return true;
        }
      }

      return false;
    }

    bool hasMenuActionListener(MenuActionListener* listener) {
      for (MenuActionListener* l : menuActionListeners) {
        if (l == listener) {
          return true;
        }
      }

      return false;
    }

    void notifyListeners() {
      for (MenuActionListener* l : menuActionListeners) {
        l->onMenuItemSelected(this, getSelectedMenuItem());
      }
    }

    void validateWindow() {
      if (getMenuSelection() < windowStart) {
        windowStart = getMenuSelection();
        windowEnd = windowStart + getMaxVisibleItems();
      }

      if (getMenuSelection() >= windowEnd) {
        /*
         * The menu is drawn from windowStart inclusive to windowEnd exclusive,
         * therefore, we must add 1 to windowEnd in order to ensure that the
         * selected item is visible.
         */
        windowEnd = getMenuSelection() + 1;
        windowStart = windowEnd - getMaxVisibleItems();
      }
    }
  };
}

#endif
