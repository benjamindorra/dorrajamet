#include "Army.h"
#include "../json.hpp"
#include <iostream>

namespace state
{
    Army::Army()
    {
        /*
        std::string id;
        /// Id(s) of the province(s) of which levies are in this army
        std::vector<std::string> levies;
        /// Pointer of the province where the army is currently located
        std::string currentProvince;
        std::string currentBattle;
        /// Queue of ongoing moving orders for the army
        std::vector<TravelOrder> orders;
        */
    }
    Army::Army (std::string strJson)
    {
        using json = nlohmann::json;
        
        try
        {
            json j = json::parse(strJson);
            levies = j["levies"].get<std::vector<std::string>>();
            currentProvince = j["currentProvince"].get<std::string>();
            currentBattle = j["currentBattle"].get<std::string>();
            json jOrders = j["orders"];
            for(json::iterator it = jOrders.begin(); it != jOrders.end(); ++it)
            {
                orders.push_back(TravelOrder(it.value().dump()));
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl ;
            throw std::runtime_error("Error when loading army.");
        }
    }
    Army::~Army ()
    {
        
    }
    bool Army::checkConsistency ()
    {
        return true;
    }
    void Army::debug ()
    {
        std::cout << "Army debug\nCurrentProvince: " << currentProvince << "\nCurrentBattle: " << currentBattle << "\nLevies: \n";
        for(auto levy : levies)
            std::cout << " - Province: " << levy << std::endl;
    }
}