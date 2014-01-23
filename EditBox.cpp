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
#include "EditBox.hpp"
#include "ResourceManager.hpp"

namespace io {
  EditBox::EditBox() {
    ResourceManager* rm = ResourceManager::getInstance();
    font = rm->getResource("data/DejaVuSansMono.ttf")->toFont();
    maxLength = UINT32_MAX;
  }

  bool EditBox::handleInputEvent(const InputEvent& event) {
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
  void EditBox::render(Graphics* g) {
    Font* f = getFont();
    if (f) {
      f->drawText(getText(), Colour(255,255,255,255));
    }
  }
}
