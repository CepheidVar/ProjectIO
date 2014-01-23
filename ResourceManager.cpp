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
#include <utility>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "ResourceManager.hpp"
#include "Image.hpp"
#include "PNG.hpp"
#include "Font.hpp"
#include "OBJModel.hpp"
#include "Class.hpp"

using namespace boost::filesystem;

namespace io {
  ResourceManager* ResourceManager::instance = nullptr;

  ResourceManager::ResourceManager() { }

  ResourceManager::~ResourceManager() {
    for (std::pair<std::string, Resource*> resource : resources) {
      delete resource.second;
    }
    resources.clear();
  }
  
  Resource* ResourceManager::getResource(const std::string& resource) {
    Resource* ret = nullptr;

    if (resources.count(resource) > 0) {
      ret = resources.at(resource);
    }
    else {
      ret = loadResource(resource);
      resources[resource] = ret;
    }

    return ret;
  }

  Resource* ResourceManager::loadResource(const std::string& resource) {
    Resource* ret = nullptr;
    path p(resource);

    if (exists(p) && is_regular_file(p)) {
      if (p.extension().compare(".png") == 0) {
        ret = loadPNG(resource);
      }

      if (p.extension().compare(".ttf") == 0) {
        // For now, we only load fonts at 24 point.
        Font* newFont = Font::fontFromFile(resource);
        newFont->setPixelSize(24);
        ret = newFont;
      }

      if (p.extension().compare(".obj") == 0) {
        OBJModel* model = new OBJModel();
        model->loadFile(resource);
        ret = model;
      }

      if (p.extension().compare(".class") == 0) {
        Class* c = Class::classFromFile(resource);
        classes.push_back(c);
        ret = c;
      }
    }

    return ret;
  }
}
