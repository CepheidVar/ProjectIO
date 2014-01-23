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
#include "Class.hpp"
#include "tinyxml2.h"
#include "Log.hpp"
#include <boost/filesystem.hpp>

using namespace boost::filesystem;
using namespace tinyxml2;

namespace io {
  Class* Class::classFromFile(const std::string& filename) {
    Class* newClass = new Class();
    try {
      path p(filename);
      if (!exists(p) || !is_regular_file(p)) {
        throw Exception("File does not exist.");
      }
      
      XMLDocument doc;
      if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        throw Exception("Could not parse class file.");
      }

      XMLElement* root = doc.RootElement();
      if (std::string(root->Name()) != "class") {
        throw Exception("Not a class file.");
      }

      XMLElement* nameNode = root->FirstChildElement("name");
      XMLElement* shortNameNode = root->FirstChildElement("shortName");
      XMLElement* levelNode = root->FirstChildElement("level");

      if (!nameNode || !nameNode->GetText()) {
        throw Exception("Empty or missing name node.");
      }

      if (!shortNameNode || !shortNameNode->GetText()) {
        throw Exception("Empty or missing shortName node.");
      }

      if (!levelNode) {
        throw Exception("No levels nodes found.");
      }

      newClass->name = std::string(nameNode->GetText());
      newClass->shortName = std::string(shortNameNode->GetText());

      int16_t level = 0;
      while (levelNode && level < Class::MAX_LEVEL) {
        XMLElement* hpNode = levelNode->FirstChildElement("hp");
        XMLElement* tpNode = levelNode->FirstChildElement("tp");
        XMLElement* strNode = levelNode->FirstChildElement("str");
        XMLElement* vitNode = levelNode->FirstChildElement("vit");
        XMLElement* tecNode = levelNode->FirstChildElement("tec");
        XMLElement* agiNode = levelNode->FirstChildElement("agi");
        XMLElement* lukNode = levelNode->FirstChildElement("luk");

        int hp = 0;
        if (!hpNode || hpNode->QueryIntText(&hp) != XML_SUCCESS) {
          throw Exception("Invalid, empty or non-existent hp node.");
        }

        int tp = 0;
        if (!tpNode || tpNode->QueryIntText(&tp) != XML_SUCCESS) {
          throw Exception("Invalid, empty or non-existent tp node.");
        }

        int str = 0;
        if (!strNode || strNode->QueryIntText(&str) != XML_SUCCESS) {
          throw Exception("Invalid, empty or non-existent str node.");
        }

        int vit = 0;
        if (!vitNode || vitNode->QueryIntText(&vit) != XML_SUCCESS) {
          throw Exception("Invalid, empty or non-existent vit node.");
        }

        int tec = 0;
        if (!tecNode || tecNode->QueryIntText(&tec) != XML_SUCCESS) {
          throw Exception("Invalid, empty or non-existent tec node.");
        }

        int agi = 0;
        if (!agiNode || agiNode->QueryIntText(&agi) != XML_SUCCESS) {
          throw Exception("Invalid, empty or non-existent agi node.");
        }

        int luk = 0;
        if (!lukNode || lukNode->QueryIntText(&luk) != XML_SUCCESS) {
          throw Exception("Invalid, empty or non-existent luk node.");
        }

        newClass->stats[level].hp = hp;
        newClass->stats[level].tp = tp;
        newClass->stats[level].str = str;
        newClass->stats[level].vit = vit;
        newClass->stats[level].tec = tec;
        newClass->stats[level].agi = agi;
        newClass->stats[level].luk = luk;

        levelNode = levelNode->NextSiblingElement("level");
        level++;
      }

      if (levelNode && level == Class::MAX_LEVEL) {
        //  Too many levels specified.  Print a warning.
        writeToLog(MessageLevel::WARNING, "Class::Class():  Too many class stat blocks.  Ignoring remainder.");
      }

      /**
       * For now, if less than 70 levels are specified, we fill the rest of
       * the table with the last specified level.
       */
      for (int16_t i = level; i < Class::MAX_LEVEL; i++) {
        newClass->stats[i] = newClass->stats[level - 1];
      }
    }
    catch (Exception& e) {
      delete newClass;
      newClass = nullptr;

      e.printStackTrace();
    }
    
    return newClass;
  }
}
