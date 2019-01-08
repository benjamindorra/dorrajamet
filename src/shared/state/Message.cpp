#include "Message.h"

namespace state
{
  Message::Message ()
  {

  }
  Message::Message (nlohmann::json initJson)
  {

  }
  Message::Message (std::string id, std::string type, std::string source, bool requiresAnswer)
  {

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
    return j;
  }

};
