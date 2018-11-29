#include "OneProvinceOrder.h"
#include <json.hpp>
#include <iostream>

namespace state
{
    OneProvinceOrder::OneProvinceOrder ()
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
    OneProvinceOrder::OneProvinceOrder (std::string strJson)
    {
        using json = nlohmann::json;
        
        try
        {
            json j = json::parse(strJson);
            
            originProvinceId = j["originProvinceId"].get<std::string>();
            destinationProvinceId = j["destinationProvinceId"].get<std::string>();
            duration = j["duration"].get<unsigned int>();
            elapsed = j["elapsed"].get<unsigned int>();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl ;
            throw std::runtime_error("Error when loading one province order.");
        }
    }
    OneProvinceOrder::~OneProvinceOrder ()
    {

    }
    bool OneProvinceOrder::checkConsistency ()
    {
        return true;
    }
    void OneProvinceOrder::debug ()
    {
        std::cout << "Debug OP Order\nOrigin: " << originProvinceId << "\nDestination: " << destinationProvinceId << "\nDuration: " << duration << "\nElapsed: " << elapsed << std::endl;
    }
    bool OneProvinceOrder::nextStep()
    {
        elapsed++;
        return elapsed >= duration;
    }
    std::string OneProvinceOrder::getDestination()
    {
        return destinationProvinceId;
    }
    nlohmann::json OneProvinceOrder::toJson ()
    {
        nlohmann::json j;
        j["originProvinceId"] = originProvinceId;
        j["destinationProvinceId"] = destinationProvinceId;
        j["duration"] = duration;
        j["elapsed"] = elapsed;
        return j;
    }
}