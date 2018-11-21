#include "Traits.h"
#include <iostream>
#include <json.hpp>

namespace state
{
    Traits::Traits()
    {

    }
    Traits::Traits (std::string strJson)
    {
        using json = nlohmann::json;

        try
        {
            json j = json::parse(strJson);
            for (auto& element : j["traits"])
            {
                traits[element["id"].get<std::string>()] = Trait(element.dump());
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what();
            throw std::runtime_error("Error: Could not load traits.");
        }
    }
    Traits::~Traits ()
    {

    }
    bool Traits::checkConsistency ()
    {
        for(auto trait:traits)
        {
            if(!trait.second.checkConsistency())
                return false;
            try
            {
                traits.at(trait.second.getOpposedId());
            }
            catch(const std::exception& e)
            {
                return false;
            }
        }
        return true;
    }
    void Traits::debug ()
    {
        std::cout << "Debug Traits:\n";
        for(auto trait:traits)
            trait.second.debug();
    }
}