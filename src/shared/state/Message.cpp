#include "Message.h"

#include <iostream>

namespace state
{
  Message::Message ()
  {

  }
  Message::Message (nlohmann::json initJson)
  {
    try
    {
      id = initJson["id"].get<std::string>();
      type = initJson["type"].get<int>();
      sourceCharacter = initJson["sourceCharacter"].get<std::string>();
      requiresAnswer = initJson["requiresAnswer"].get<bool>();
      data = initJson["data"].get<std::string>();
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << std::endl;
      throw std::runtime_error("Could not parse message json\n");
    }
  }
  Message::Message (std::string id, int type, std::string source, bool requiresAnswer)
  {
    this->id = id;
    this->type = type;
    this->sourceCharacter = source;
    this->requiresAnswer = requiresAnswer;
  }
  Message::~Message ()
  {

  }
  nlohmann::json Message::toJson(){
    nlohmann::json j;
    j["id"] = id;
    /// warDeclaration, peaceProclamation, claimProclamation, allianceRequest, allianceBreakup
    j["type"] = type;
    j["sourceCharacter"] = sourceCharacter;
    j["requireAnswer"] = requiresAnswer;
    j["data"] = data;
    return j;
  }
  std::string Message::getId ()
  {
    return id;
  }

};
