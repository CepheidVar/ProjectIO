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
#ifndef CharacterHPP
#define CharacterHPP

#include <cstdint>
#include <stdexcept>
#include "Class.hpp"

namespace io {
  /**
   * FIXME:  All the get<STAT>() methods return the base stat.  Remember
   * to fix this up to base stat + armour/weapon bonuses.
   */
  /**
   * Represents a character.
   */
  class Character {
    public:
      const static int16_t MAXIMUM_HP = 999;
      const static int16_t MAXIMUM_TP = 999;

      Character() {
        characterClass = nullptr;
        name = "";
        level = 0;
        curHP = 0;
        maxHP = 0;
        curTP = 0;
        maxTP = 0;
        str = 0;
        tec = 0;
        vit = 0;
        agi = 0;
        luk = 0;
      }

      ~Character() {

      }

      void adjustHP(const int16_t amount) {
        setCurHP(getCurHP() + amount);
      }

      void adjustTP(const int16_t amount) {
        setCurTP(getCurTP() + amount);
      }

      bool enoughHP(const int16_t amount) {
        return (amount < getCurHP());
      }

      bool enoughTP(const int16_t amount) {
        return (amount < getCurTP());
      }

      void levelUp() {
        if (getClass()) {
          level++;
          if (level > Class::MAX_LEVEL) {
            throw std::runtime_error("Character::levelUp:  Level exceeds 70.");
          }

          StatBlock s = getClass()->getStatsForLevel(level);

          str = s.str;
          vit = s.vit;
          tec = s.tec;
          agi = s.agi;
          luk = s.luk;

          setMaxHP(s.hp);
          setMaxTP(s.tp);
        }
      }

      Class* getClass() const {
        return characterClass;
      }

      std::string getName() const {
        return name;
      }

      uint16_t getLevel() const {
        return level;
      }

      int16_t getCurHP() const {
        return curHP;
      }

      int16_t getMaxHP() const {
        return maxHP;
      }

      int16_t getCurTP() const {
        return curTP;
      }

      int16_t getMaxTP() const {
        return maxTP;
      }

      int16_t getSTR() const {
        return str;
      }

      int16_t getTEC() const {
        return tec;
      }

      int16_t getVIT() const {
        return vit;
      }

      int16_t getAGI() const {
        return agi;
      }

      int16_t getLUK() const {
        return luk;
      }

      //  FIXME:  Should return STR + Currently equipped weapon ATK.
      int16_t getATK() const {
        return str;
      }

      //  FIXME:  Should return VIT + Currently equipped armour DEF.
      int16_t getDEF() const {
        return vit;
      }

      void setClass(Class* characterClass) {
        this->characterClass = characterClass;
        if (characterClass) {
          //  Reset stats all to minimum.
          level = 1;
          StatBlock s = characterClass->getStatsForLevel(level);

          str = s.str;
          vit = s.vit;
          tec = s.tec;
          agi = s.agi;
          luk = s.luk;

          setMaxHP(s.hp);
          setCurHP(s.hp);
          setMaxTP(s.tp);
          setCurTP(s.tp);
        }
      }

      void setName(const std::string& name) {
        this->name = name;
      }

      void setCurHP(const int16_t amount) {
        curHP = amount;
        if (curHP > maxHP) {
          curHP = maxHP;
        }

        if (curHP < 0) {
          curHP = 0;
        }
      }

      void setMaxHP(const int16_t amount) {
        maxHP = amount;
        if (curHP > maxHP) {
          setCurHP(maxHP);
        }
      }

      void setCurTP(const int16_t amount) {
        curTP = amount;
        if (curTP > maxTP) {
          curTP = maxTP;
        }

        if (curTP < 0) {
          curTP = 0;
        }
      }

      void setMaxTP(const int16_t amount) {
        maxTP = amount;
        if (curTP > maxTP) {
          setCurTP(maxTP);
        }
      }

      void setSTR(const int16_t str) {
        this->str = (str > 0) ? str : 1;
      }

      void setTEC(const int16_t tec) {
        this->tec = (tec > 0) ? tec : 1;
      }

      void setVIT(const int16_t vit) {
        this->vit = (vit > 0) ? vit : 1;
      }

      void setAGI(const int16_t agi) {
        this->agi = (agi > 0) ? agi : 1;
      }

      void setLUK(const int16_t luk) {
        this->luk = (luk > 0) ? luk : 1;
      }
    private:
      //  Associated character class.
      Class* characterClass;

      //  Currently, no equipment is supported.

      std::string name;
      uint16_t level;

      //  HP, TP.
      int16_t curHP;
      int16_t maxHP;
      int16_t curTP;
      int16_t maxTP;

      //  Stats.
      int16_t str;
      int16_t tec;
      int16_t vit;
      int16_t agi;
      int16_t luk;
      
      Character(const Character&) = delete;
      Character& operator=(const Character&) = delete;
  };
}

#endif
