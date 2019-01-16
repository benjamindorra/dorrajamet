#include "Army.h"

#include "GameMap.h"

#include <json.hpp>
#include <iostream>
#include <algorithm>

namespace state
{
    /*Army& Army::operator= (const Army& army){
        currentProvince=army.currentProvince;
        std::cout<<"Copy= current Province for army one is: "<<currentProvince<<" Address "<<(unsigned long)this<<" Address army: "<<(unsigned long)&army<<std::endl;
        return *this;
    }
    Army::Army (const Army& army) : currentProvince(army.currentProvince){
         std::cout<<"Copy current Province for army one is: "<<currentProvince<<" Address "<<(unsigned long)this<<" Address army: "<<(unsigned long)&army<<std::endl;
    }*/
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
    Army& Army::operator= (const Army& army){
        this->parent = army.parent;
        this->id=army.id;
        this->ownerCharacter=army.ownerCharacter;
        /// Id(s) of the province(s) of which levies are in this army
        this->levies =army.levies;
        /// Pointer of the province where the army is currently located
        this->currentProvince=army.currentProvince;
        this->currentBattle=army.currentBattle;
        /// Queue of ongoing moving orders for the army
        this->orders = army.orders;
        for (auto& e : orders) {
            e.setParent(this);
        }
        return *this;
    }
    void Army::setParent (GameMap * parent)
    {
        this->parent = parent;
    }
    bool Army::checkConsistency ()
    {
        return true;
    }
    void Army::debug ()
    {
        std::cout << "Army debug\nCurrentProvince: " << currentProvince << "\nCurrentBattle: " << currentBattle << "\nLevies: \n";
        for(auto levy : levies)
        {
            std::cout << " - Province: " << levy << ": " << parent->getLevyMen(levy) << " men" << std::endl;
        }
        std::cout << "Orders:\n";
        for(auto e: orders)
            std::cout << e.toJson().dump(2) << std::endl;
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
        return false;
        return currentBattle.size();
    }
    bool Army::hasOrder()
    {
        return orders.size();
    }
    void Army::advanceOrders()
    {
        if(orders.empty()){
            return;
        }
        if(orders[0].nextStep()){
            orders.clear();
        }
    }
    int Army::getMen()
    {
        int res = 0;
        for(auto const& levy: levies)
        {
            std::cout << "Levy of province: " << levy << " has still " << parent->getLevyMen(levy) << std::endl;
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
    bool Army::isDead ()
    {
        return(getMen() <= 0);
    }
    void Army::disband ()
    {
        for(auto const& e: levies)
            parent->disbandLevy(e);
    }
    bool Army::ownsLevy(std::string levyProvinceId)
    {
        return (std::find(levies.begin(), levies.end(), levyProvinceId) != levies.end());
    }
    bool Army::canMove ()
    {
        return !isInBattle();
    }
    nlohmann::json Army::toJson ()
    {
        nlohmann::json j;
        j["id"] = id;
        j["ownerCharacter"] = ownerCharacter;
        j["levies"] = levies;
        j["currentProvince"] = currentProvince;
        j["currentBattle"] = currentBattle;
        nlohmann::json jOrders = nlohmann::json::array();
        for(auto e: orders)
            jOrders.push_back(e.toJson());
        j["orders"] = jOrders;
        return j;
    }
}