// Generated by dia2code
#ifndef STATE__MESSAGE__H
#define STATE__MESSAGE__H

#include <string>
#include <json.hpp>

namespace state {

  /// class Message - 
  class Message {
    // Attributes
  private:
    std::string id;
    /// warDeclaration, peaceProclamation, claimProclamation, allianceRequest, allianceBreakup
    std::string type;
    std::string sourceCharacter;
    bool requiresAnswer;
    // Operations
  public:
    Message ();
    Message (nlohmann::json initJson);
    Message (std::string id, std::string type, std::string source, bool requiresAnswer);
    ~Message ();
    nlohmann::json toJson ();
    // Setters and Getters
  };

};

#endif
