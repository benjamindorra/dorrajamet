// Generated by dia2code
#ifndef STATE__TITLES__H
#define STATE__TITLES__H

#include <string>
#include <map>
#include <vector>
#include <json.hpp>

namespace state {
  class Title;
  class Duchy;
  class Kingdom;
  class County;
}

#include "Duchy.h"
#include "Kingdom.h"
#include "County.h"
#include "Title.h"

namespace state {

  /// class Titles - 
  class Titles {
    // Associations
    // Attributes
  private:
    enum titleType{kingdom = 'k', duchy = 'd', county = 'c'};
    std::map<std::string, titleType> titles;
    std::map<std::string, Kingdom> kingdoms;
    std::map<std::string, Duchy> duchies;
    std::map<std::string, County> counties;
    // Operations
  public:
    Titles ();
    Titles (std::string strJson);
    ~Titles ();
    Title * operator[] (const char* a);
    Title * operator[] (const std::string key);
    bool checkConsistency ();
    void debug ();
    std::string getProvinceOwner (std::string provinceId);
    std::vector<std::string> getProvincesOf (std::string characterId);
    std::vector<std::string> getTitlesOf (std::string characterId);
    /// If titleId is independant, will return empty string
    /// @param titleId		(???) 
    std::string getLiege (std::string titleId);
    std::string getHolder (std::string titleId);
    /// If titleId is independant, will return titleId
    /// @param titleId		(???) 
    std::string getTopLiege (std::string titleId);
    std::vector<std::string> getTitleDirectVassals (std::string titleId);
    unsigned int getTitleColor (std::string titleId);
    int computeCharacterPrestige (std::string characterId);
    void setTitleHolder (std::string newHolder, std::string titleId);
    nlohmann::json fetchTitleData (std::string titleId);
    // Setters and Getters
  };

};

#endif
