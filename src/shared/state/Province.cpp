#include "Province.h"

#include "GameMap.h"

#include <json.hpp>
#include <iostream>
#include <iomanip>

namespace state
{
    Province::Province()
    {
        /*
        std::string id;
        std::string name;
        /// int value of the unique color code of the province (for ui purposes)
        unsigned int colorCode;
        /// Ranges from 0 to 100. Represent the current development status of the province. The player can spend money to raise this value. Increase this value will offer better levies and tax income. Later we could replace that with a list of improvable buildings that will have more specific modifiers on the province.
        unsigned char development;
        /// Representing the prosperity/devastation of the province (impacts the taxes and levies). Ranges from -100 (total devastation) to 100 (maximum prosperity).
        char prosperity;
        /// Base values used to compute the available levies of this province.
        Levy baseLevy;
        /// Levy of the province.
        Levy levy;
        /// Base values used to compute tax income.
        unsigned int baseTaxIncome;
        /// Tax income from this province for a turn.
        unsigned int taxIncome;
        */
    }
    Province::Province (GameMap * parent, std::string strJson)
    {
        this->parent = parent;
        using json = nlohmann::json;
        
        try
        {
            json j = json::parse(strJson);
            id = j["id"].get<std::string>();
            name = j["name"].get<std::string>();
            colorCode = j["colorCode"].get<int>();
            development = j["development"].get<int>();
            prosperity = j["prosperity"].get<char>();
            baseLevy = Levy(this, j["baseLevy"].dump());
            levy = Levy(this, j["levy"].dump());
            baseTaxIncome = j["baseTaxIncome"].get<unsigned int>();
            taxIncome = j["taxIncome"].get<unsigned int>();
            dispPosX = j["dispPosX"].get<int>();
            dispPosY = j["dispPosY"].get<int>();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl ;
            throw std::runtime_error("Error when loading province.");
        }
    }
    Province::~Province ()
    {

    }
    bool Province::checkConsistency ()
    {
        if(!(id.length() && name.length()))
            return false;
        if(colorCode > std::stoul("ffffff", nullptr, 16))
            return false;
        if(development < 0 || development > 100)
            return false;
        if(prosperity < -100 || prosperity > 100)
            return false;
        if(!(baseLevy.checkConsistency() && levy.checkConsistency()))
            return false;
        return true;
    }
    void Province::debug ()
    {
        std::cout << "Debug Province\nId: " << id << "\nName: " << name << "\nColor Code: " << std::hex << colorCode << "\nDevelopment score: " << development << "\nProsperity level: " << prosperity << std::endl;
    }
    int Province::getLevyMen()
    {
        return levy.getMen();
    }
    void Province::killMenFromLevy (int victims)
    {
        levy.killMen(victims);
    }
    void Province::disbandLevy ()
    {
        levy.disband();
    }
    bool Province::isLevyRaised ()
    {
        return levy.isRaised();
    }
    void Province::setLevyReinforcementRate (float rate)
    {
        levy.setReinforcementRate(rate);
    }
    void Province::reinforceLevy()
    {
        int maxMen = baseLevy.getMen();
        int currentMen = levy.getMen();
        float reinforcementRate = levy.getReinforcementRate();
        int reinforcements = reinforcementRate * (maxMen / 10);
        if(currentMen + reinforcements > maxMen)
            reinforcements = maxMen - currentMen;
        levy.reinforce(reinforcements);
    }

    nlohmann::json Province::fetchProvinceData() {
        using json = nlohmann::json;
        json j;
        j["id"] = id;
        j["name"] = name;
        j["colorCode"] = colorCode;
        j["development"] = development;
        j["prosperity" ]= prosperity;
        j["baseLevy"] = baseLevy.getMen();
        j["levy"] = levy.getMen();
        j["baseTaxIncome"] = baseTaxIncome;
        j["taxIncome"] = taxIncome;
        j["dispPosX"] = dispPosX;
        j["dispPosY"] = dispPosY;
        return j;
    }
}