/*
Traits class, load Trait objects from a Json file and stores them in a vector.
*/
#include "Trait.h"
#include <json/json.h>
#include <fstream>

state::Traits::Traits(std::string path) {
    Json::Value root;
    std::ifstream config_doc("traits.json", std::ifstream::binary);
    if (!config_doc.good()) { 
        throw std::runtime_error("Cannot open  file");
    }
    config_doc >> root;
    //std::string trait = root.get("my-encoding", "UTF-32" ).asString();
    if( root.size() > 0 ) {
        for( Json::ValueIterator itr = root.begin() ; itr != root.end() ; itr++ ) {
            std::string id = root.get("id", "UTF-32").asString();
            std::string opposedId = root.get("opposedId", "UTF-32").asString();
            int diplomacyModifier = root.get("diplomacyModifier", "UTF-32").asInt();
            int stewardshipModifier = root.get("stewardshipModifier", "UTF-32").asInt();
            int martialModifier = root.get("martialModifier", "UTF-32").asInt();
            int intrigueModifier = root.get("intrigueModifier", "UTF-32").asInt();
            state::Trait trait(id, opposedId, diplomacyModifier, stewardshipModifier, martialModifier, intrigueModifier);
            traits.push_back(trait);
        }
    }
}

int main() {

    return 1;
}