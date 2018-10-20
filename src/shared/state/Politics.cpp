#include "Politics.h"
#include "../tools.hpp"
#include "../json.hpp"
#include <iostream>
#include <fstream>

namespace state
{
    Politics::Politics()
    {

    }
    Politics::Politics(std::string strJson)
    {
        using json = nlohmann::json;

        try
        {
            json j = json::parse(strJson);
            titles = Titles(j["titles"].dump());
            if(titles.checkConsistency())
                std::cout << "Titles consistency check success\n\n\n";
            else
                throw std::string("Titles consistency check failed");
            characters = Characters(j["characters"].dump());
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl ;
            throw std::runtime_error("Error when loading titles.");
        }
    }
    Politics::~Politics()
    {
        
    }
    void Politics::debug()
    {
        titles.debug();
        characters.debug();
    }
}
