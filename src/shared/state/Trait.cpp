#include "Trait.h"

state::Trait::Trait(std::string id, std::string opposedId, 
                    int diplomacyModifier, int stewardshipModifier, 
                    int martialModifier, int intrigueModifier) {
    this->id = id;
    this->opposedId = opposedId;
    this->diplomacyModifier = diplomacyModifier;
    this->stewardshipModifier = stewardshipModifier;
    this->martialModifier = martialModifier;
    this->intrigueModifier = intrigueModifier;
}