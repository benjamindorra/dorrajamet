// Generated by dia2code
#ifndef RENDER__SHOWARMY__H
#define RENDER__SHOWARMY__H


namespace render {
  class InteractiveElement;
}

#include "InteractiveElement.h"

namespace render {

  /// class ShowArmy - 
  class ShowArmy : public render::InteractiveElement {
    // Attributes
  public:
    Army army;
    // Operations
  public:
    ShowArmy ();
    ShowArmy (Army army);
    // Setters and Getters
  };

};

#endif
