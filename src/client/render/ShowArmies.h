// Generated by dia2code
#ifndef RENDER__SHOWARMIES__H
#define RENDER__SHOWARMIES__H

#include <vector>
#include <string>

namespace render {
  class ShowArmy;
}

#include "ShowArmy.h"

namespace render {

  /// class ShowArmies - 
  class ShowArmies {
    // Associations
    // Attributes
  private:
    std::vector<ShowArmy*> armies;
    // Operations
  public:
    ShowArmies ();
    ~ShowArmies ();
    void addArmy (ShowArmy * showArmy);
    std::vector<ShowArmy*> * getArmies ();
    std::string selectArmy (int x, int y);
    std::string moveSelected (int x, int y);
    void draw ();
    // Setters and Getters
  };

};

#endif
