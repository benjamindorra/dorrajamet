// Generated by dia2code
#ifndef STATE__DUCHY__H
#define STATE__DUCHY__H

#include <string>

namespace state {
  class Title;
}

#include "Title.h"

namespace state {

  /// class Duchy - 
  class Duchy : public state::Title {
    // Attributes
  private:
    /// Id of the liege title if any
    std::string liege;
    /// Id of the de Jure liege title if any
    std::string deJureLiege;
    // Operations
  public:
    Duchy ();
    Duchy (std::string strJson);
    ~Duchy ();
    // Setters and Getters
  };

};

#endif
