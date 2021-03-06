// Generated by dia2code
#ifndef ENGINE__COMMAND__H
#define ENGINE__COMMAND__H

#include <string>

namespace engine {

  /// class Command - 
  class Command {
    // Attributes
  public:
    /// Possible types of command
    enum commandType{army=0,claim=1,turn=2,war=3,ok=4,yes=5,no=6,peace=7,surr=8,createArmy=9};
  private:
    /// Type of command
    commandType type;
    /// Additional data for this command (type of button, etc)
    std::string argument;
    // Operations
  public:
    Command ();
    Command (commandType type, std::string argument);
    ~Command ();
    commandType getType ();
    std::string getArgument ();
    // Setters and Getters
  };

};

#endif
