#include "Ressources.h"

state::Ressources::Ressources(std::string pathTraits, std::string pathAdjacencies) {
    this.adjacencies = state::Adjacencies::Adjacencies(pathAdjacencies);
    this.traits = state::Traits::Traits(pathTraits);
}

state::Traits state::Ressources::getTraits() {
    return this.traits;
}

state::Adjacencies state::Ressources::getAdjacencies(){
    return this->adjacencies;
}
