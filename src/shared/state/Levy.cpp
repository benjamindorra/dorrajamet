#include "Levy.h"

#include "Province.h"

#include <json.hpp>
#include <iostream>

namespace state
{
    Levy::Levy()
    {
        /*
        /// Number of men in the levy
        unsigned int men;
        bool raised;
        /// Ranges from 0 (0%) to 150 (150%).
        float reinforcementRate;
        */
    }
    Levy::Levy (Province * parent, std::string strJson)
    {   
        this-> parent = parent;
        using json = nlohmann::json;
        
        try
        {
            json j = json::parse(strJson);
            men = j["men"].get<int>();
            raised = j["isRaised"].get<bool>();
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
        std::cout << "Debug Levy\nMen: " << men << "\nIs raised: " << raised << "\nReinforcement rate: " << reinforcementRate << std::endl;
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
    void Levy::disband ()
    {
        raised = false;
    }
    bool Levy::isRaised()
    {
        return raised;
    }
    void Levy::setReinforcementRate(float rate)
    {
        reinforcementRate = rate;
    }
    void Levy::reinforce(int men)
    {
        this->men += men;
    }
    float Levy::getReinforcementRate()
    {
        return reinforcementRate;
    }
    nlohmann::json Levy::toJson ()
    {
        nlohmann::json j;
        j["men"] = men;
        j["isRaised"] = raised;
        j["reinforcementRate"] = reinforcementRate;
        return j;
    }
}