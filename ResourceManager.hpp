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
#ifndef ResourceManagerHPP
#define ResourceManagerHPP

#include <map>
#include <string>
#include <vector>
#include "Resource.hpp"

namespace io {
  class ResourceManager {
  public:
    static void deleteInstance() {
      if (ResourceManager::instance) {
        delete ResourceManager::instance;
        ResourceManager::instance = nullptr;
      }
    }

    const std::vector<Class*>& getClasses() const {
      return classes;
    }

    static ResourceManager* getInstance() {
      if (!ResourceManager::instance) {
        ResourceManager::instance = new ResourceManager();
      }

      return ResourceManager::instance;
    }

    Resource* getResource(const std::string& resource);
  private:
    static ResourceManager* instance;

    std::map<std::string, Resource*> resources;
    std::vector<Class*> classes;

    ResourceManager();
    ~ResourceManager();
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    Resource* loadResource(const std::string& resource);
  };
}

#endif
