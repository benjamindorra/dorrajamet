// Generated by dia2code
#ifndef STATE__PROVINCE__H
#define STATE__PROVINCE__H

#include <string>

namespace state {
  class Levy;
  class Army;
}

#include "Levy.h"
#include "Army.h"

namespace state {

  /// class Province - 
  class Province {
    // Associations
    state::Army is located at;
    // Attributes
  private:
    std::string id;
    std::string name;
    /// 3 bytes rgb color code of the province on the province layer of the map. Unique.
    char[3] colorCode;
    /// Ranges from 0 to 100. Represent the current development status of the province. The player can spend money to raise this value. Increase this value will offer better levies and tax income.
Later we will replace that with a list of improvable buildings that will have more specific modifiers on the province.
    unsigned char development;
    /// Representing the prosperity/devastation of the province (impacts the taxes and levies). Ranges from -100 (total devastation) to 100 (maximum prosperity).
    char prosperity;
    /// Base values used to compute the available levies of this province.
    Levy baseLevy;
    /// Levy of the province.
    Levy levy;
    /// Base values used to compute tax income.
    unsigned int baseTaxIncome;
    /// Tax income from this province for a turn.
    unsigned int taxIncome;
    // Operations
  public:
    Province ();
    // Setters and Getters
    const Army& getIs located at() const;
    void setIs located at(const Army& is located at);
  };

};

#endif
