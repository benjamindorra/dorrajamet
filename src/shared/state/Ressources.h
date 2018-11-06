// Generated by dia2code
#ifndef STATE__RESSOURCES__H
#define STATE__RESSOURCES__H

#include <string>

namespace state {
  class Traits;
  class Adjacencies;
}

#include "Traits.h"
#include "Adjacencies.h"

namespace state {

  /// class Ressources - 
  class Ressources {
    // Associations
    // Attributes
  private:
    Traits traits;
    Adjacencies adjacencies;
    // Operations
  public:
    Ressources ();
    Ressources (std::string strJson);
    Ressources (std::string pathTraits, std::string pathAdjacencies);
    ~Ressources ();
    bool checkConsistency ();
    void debug ();
    void loadAdjacencies (std::string adjacenciesPath);
    void loadTraits (std::string traitsPath);
    // Setters and Getters
  };

};

#endif
