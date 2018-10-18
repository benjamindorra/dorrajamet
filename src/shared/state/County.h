// Generated by dia2code
#ifndef STATE__COUNTY__H
#define STATE__COUNTY__H

#include <string>

namespace state {
  class Title;
}

#include "Title.h"

namespace state {

  /// class County - A class inheriting from Title to describe a Count-level title
  class County : public state::Title {
    // Attributes
  private:
    /// Pointer to the related province
    std::string province;
    /// Id of the liege title if any
    std::string liege;
    /// Id of the de Jure liege title if any
    std::string deJureLiege;
    // Operations
  public:
    County ();
    County (std::string strJson);
    ~County ();
    void debug ();
    bool checkConsistency ();
    std::string getLiege ();
    std::string getDeJureLiege ();
    // Setters and Getters
  };

};

#endif
