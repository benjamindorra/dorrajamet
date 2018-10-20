#include "Ressources.h"

namespace state
{
    Ressources::Ressources()
    {
        
    }

    Traits state::Ressources::getTraits()
    {
        return this->traits;
    }

    Adjacencies state::Ressources::getAdjacencies()
    {
        return this->adjacencies;
    }

}
