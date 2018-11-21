#include "Army.h"

#include "GameMap.h"

#include <json.hpp>
#include <iostream>

namespace state
{
    Army::Army()
    {
        /*
        std::string id;
        std::string ownerCharacter;
        /// Id(s) of the province(s) of which levies are in this army
        std::vector<std::string> levies;
        /// Pointer of the province where the army is currently located
        std::string currentProvince;
        std::string currentBattle;
        /// Queue of ongoing moving orders for the army
        std::vector<TravelOrder> orders;
        */
    }
    Army::Army (GameMap * parent, std::string strJson)
    {
        this->parent = parent;
        using json = nlohmann::json;
        
        try
        {
            json j = json::parse(strJson);
            id = j["id"].get<std::string>();
            ownerCharacter = j["ownerCharacter"].get<std::string>();
            levies = j["levies"].get<std::vector<std::string>>();
            currentProvince = j["currentProvince"].get<std::string>();
            currentBattle = j["currentBattle"].get<std::string>();
            json jOrders = j["orders"];
            for(json::iterator it = jOrders.begin(); it != jOrders.end(); ++it)
            {
                orders.push_back(TravelOrder(this, it.value().dump()));
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
    std::string Army::getId ()
    {
        return id;
    }
    std::string Army::getOwnerCharacter()
    {
        return ownerCharacter;
    }
    std::string Army::getCurrentProvince()
    {
        return currentProvince;
    }
    void Army::setOrders(nlohmann::json orderJson)
    {
        TravelOrder order(this, orderJson.dump());
        orders.clear();
        orders.push_back(order);
    }
    std::string Army::getCurrentBattle ()
    {
        return currentBattle;
    }
    bool Army::isInBattle ()
    {
        return currentBattle.size();
    }
    bool Army::hasOrder()
    {
        return orders.size();
    }
    void Army::advanceOrders()
    {
        auto travelOrder = orders[0];
        if(travelOrder.nextStep())
        {
            orders.clear();
        }
    }
    int Army::getMen()
    {
        int res = 0;
        for(auto const& levy: levies)
        {
            res += parent->getLevyMen(levy);
        }
        return res;
    }
    void Army::killMen(int victims)
    {
        int totalMen = getMen();
        for(auto const& e: levies)
        {
            int men = parent->getLevyMen(e);
            parent->killMenFromLevy(e, victims * (men / totalMen));
        }
    }
    void Army::setPosition(std::string provinceId)
    {
        currentProvince = provinceId;
    }
}