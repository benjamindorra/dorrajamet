// Generated by dia2code
#ifndef STATE__TRAITS__H
#define STATE__TRAITS__H

#include <string>
#include <map>

namespace state {
  class Trait;
}

#include "Trait.h"

namespace state {

  /// class Traits - 
  class Traits {
    // Associations
    // Attributes
  private:
    std::map<std::string, Trait> traits;
    // Operations
  public:
    Traits ();
    Traits (std::string strJson);
    ~Traits ();
    bool checkConsistency ();
    void debug ();
    // Setters and Getters
  };

};

#endif
