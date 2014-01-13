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
#ifndef EditBoxHPP
#define EditBoxHPP

#include <cstdint>
#include <string>
#include <list>
#include "Component.hpp"
#include "Font.hpp"

namespace io {
  class EditBox;

  class EditBoxActionListener {
  public:
    virtual ~EditBoxActionListener() { }
    virtual void onEditBoxTextChanged(EditBox* which) = 0;
    virtual void onEditBoxTextEntered(EditBox* which) = 0;
  private:
  };

  class EditBox : public Component {
  public:
    EditBox();
    virtual ~EditBox() { }

    void addEditBoxActionListener(EditBoxActionListener* listener) {
      if (listener && !hasEditBoxActionListener(listener)) {
        listeners.push_back(listener);
      }
    }

    Font* getFont() const {
      return font;
    }

    uint32_t getMaxLength() const {
      return maxLength;
    }

    std::string getText() {
      return text;
    }

    virtual bool handleInputEvent(const InputEvent& event) {
      char c;

      if (event.getState() == InputEventState::DOWN) {
        switch(event.getType()) {
          case InputEventType::CANCEL:
            break;
          case InputEventType::ACCEPT:
            textEntered();
            break;
          default:
            c = event.getCharCode();
            if (c != 0 && c != '\t') {
              if (event.getCharCode() == 0x08) {
                if (text.size() > 0) {
                  text.resize(text.size() - 1);
                }
              }
              else {
                if (text.size() < getMaxLength()) {
                  text.push_back(event.getCharCode());
                  textChanged();
                }
              }
            }
            break;
        }
      }

      return true;
    }

    void removeEditBoxActionListener(EditBoxActionListener* listener) {
      if (listener && hasEditBoxActionListener(listener)) {
        listeners.remove(listener);
      }
    }

    void setFont(Font* font) {
      this->font = font;
    }

    void setMaxLength(const uint32_t maxLength) {
      this->maxLength = maxLength;
      if (text.size() > maxLength) {
        text.resize(maxLength);
      }
    }

    void setText(const std::string& text) {
      this->text = text;
    }
  private:
    Font* font;
    std::list<EditBoxActionListener*> listeners;
    std::string text;
    uint32_t maxLength;

    EditBox(const EditBox&);
    EditBox& operator=(const EditBox&);

    bool hasEditBoxActionListener(EditBoxActionListener* listener) {
      for (EditBoxActionListener* l : listeners) {
        if (l == listener) {
          return true;
        }
      }

      return false;
    }

    virtual void render(Graphics* g);

    void textEntered() {
      for (EditBoxActionListener* l : listeners) {
        l->onEditBoxTextEntered(this);
      }
    }

    void textChanged() {
      for (EditBoxActionListener* l : listeners) {
        l->onEditBoxTextChanged(this);
      }
    }
  };
}

#endif
