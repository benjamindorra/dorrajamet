#include "TravelOrder.h"

#include "Army.h"

#include <json.hpp>
#include <iostream>

namespace state
{
    TravelOrder::TravelOrder ()
    {
        /*
        std::string originProvinceId;
        std::string destinationProvinceId;
        /// Total number of turns necessary to execute the order
        unsigned int duration;
        /// Number of travel turns elapsed.
        unsigned int elapsed;
        */
    }
    TravelOrder::TravelOrder (Army * parent, std::string strJson)
    {
        this->parent = parent;
        using json = nlohmann::json;
        
        try
        {
            json j = json::parse(strJson);
            
            originProvinceId = j["originProvinceId"].get<std::string>();
            destinationProvinceId = j["destinationProvinceId"].get<std::string>();
            duration = j["duration"].get<unsigned int>();
            elapsed = j["elapsed"].get<unsigned int>();
            json jOrders = j["orders"];
            for(json::iterator it = jOrders.begin(); it != jOrders.end(); ++it)
                orderList.push_back(OneProvinceOrder(it.value().dump()));
            
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl ;
            throw std::runtime_error("Error when loading travel order.");
        }
    }
    TravelOrder::~TravelOrder ()
    {

    }
    bool TravelOrder::checkConsistency ()
    {
        return true;
    }
    void TravelOrder::debug ()
    {
        std::cout << "Debug Travel Order\nOrigin: " << originProvinceId << "\nDestination: " << destinationProvinceId << "\nDuration: " << duration << "\nElapsed: " << elapsed << std::endl;
        for(auto order: orderList)
        {
            std::cout << "    - ";
            order.debug();
        }
    }
    bool TravelOrder::nextStep ()
    {
        elapsed ++;
        if(orderList[0].nextStep())
        {
            parent->setPosition(orderList[0].getDestinationProvinceId());
            orderList.erase(orderList.begin());
        }
        return elapsed >= duration;
    }
    unsigned int TravelOrder::size ()
    {
        return orderList.size();
    }
    nlohmann::json TravelOrder::toJson ()
    {
        nlohmann::json j;
        j["originProvinceId"] = originProvinceId;
        j["destinationProvinceId"] = destinationProvinceId;
        j["duration"] = duration;
        j["elapsed"] = elapsed;
        nlohmann::json jOrders = nlohmann::json::array();
        for(auto e: orderList)
            jOrders.push_back(e.toJson());
        j["orders"] = jOrders;
        return j;
    }
}