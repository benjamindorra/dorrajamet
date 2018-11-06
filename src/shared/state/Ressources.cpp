#include "Ressources.h"
#include "../json.hpp"
#include "../tools.hpp"
#include <iostream>

namespace state
{
    Ressources::Ressources ()
    {

    }
    Ressources::Ressources (std::string strJson)
    {
        using json = nlohmann::json;

        try
        {
            json j = json::parse(strJson);
            this->loadAdjacencies(j["adjacenciesPath"].get<std::string>());
            this->loadTraits(j["traitsPath"].get<std::string>());
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what();
            throw std::runtime_error("Error: Could not load ressources.");
        }
    }
    Ressources::Ressources (std::string traitsPath, std::string adjacenciesPath)
    {
        try
        {
            this->loadAdjacencies(adjacenciesPath);
            this->loadTraits(traitsPath);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what();
            throw std::runtime_error("Error: Could not load ressources");
        }
    }
    Ressources::~Ressources ()
    {

    }
    bool Ressources::checkConsistency ()
    {
        return(traits.checkConsistency() && adjacencies.checkConsistency());
    }
    void Ressources::debug ()
    {
        std::cout << "====Ressources====\n";
        traits.debug();
        adjacencies.debug();
    }
    void Ressources::loadAdjacencies(std::string adjacenciesPath)
    {
        try
        {
            std::string rawCsv = loadFile(adjacenciesPath);
            this->adjacencies = Adjacencies(rawCsv);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what();
            throw std::runtime_error("Error: could not load adjacencies.");
        }
    }
    void Ressources::loadTraits(std::string traitsPath)
    {
        try
        {
            std::string rawTraits = loadFile(traitsPath);
            this->traits = Traits(traitsPath);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what();
            throw std::runtime_error("Error: could not load traits.");
        }
    }

}
