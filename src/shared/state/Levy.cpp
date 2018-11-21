#include "Levy.h"
#include <json.hpp>
#include <iostream>

namespace state
{
    Levy::Levy()
    {
        /*
        /// Number of men in the levy
        unsigned int men;
        bool isRaised;
        /// Ranges from 0 (0%) to 150 (150%).
        float reinforcementRate;
        */
    }
    Levy::Levy (std::string strJson)
    {
        using json = nlohmann::json;
        
        try
        {
            json j = json::parse(strJson);
            men = j["men"].get<int>();
            isRaised = j["isRaised"].get<bool>();
            reinforcementRate = j["reinforcementRate"].get<float>();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl ;
            throw std::runtime_error("Error when loading levy.");
        }
    }
    Levy::~Levy ()
    {

    }
    bool Levy::checkConsistency ()
    {
        return true;
    }
    void Levy::debug ()
    {
        std::cout << "Debug Levy\nMen: " << men << "\nIs raised: " << isRaised << "\nReinforcement rate: " << reinforcementRate << std::endl;
    }
    int Levy::getMen()
    {
        return men;
    }
    void Levy::killMen(int victims)
    {
        men -= victims;
        if(men < 0)
            men = 0;
    }
}