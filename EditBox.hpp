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
    
    virtual bool handleInputEvent(const InputEvent& event);
    
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
  protected:
    virtual void render(Graphics* g);
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
