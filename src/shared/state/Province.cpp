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
            kingdomId = j["kingdomId"].get<std::string>();
            colorCode = j["colorCode"].get<unsigned int>();
            dispPosX = j["dispPosX"].get<int>();
            dispPosY = j["dispPosY"].get<int>();
            development = j["development"].get<int>();
            prosperity = j["prosperity"].get<int>();
            baseLevy = Levy(this, j["baseLevy"].dump());
            levy = Levy(this, j["levy"].dump());
            baseTaxIncome = j["baseTaxIncome"].get<unsigned int>();
            taxIncome = j["taxIncome"].get<unsigned int>();
            siegeStatus = j["siegeStatus"].get<int>();
            siegingArmy = j["siegingArmy"].get<std::string>();
            controlledBy = j["controlledBy"].get<std::string>();
            
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
    void Province::setParent (GameMap * parent)
    {
        this->parent = parent;
    }
    bool Province::checkConsistency ()
    {
        if(!(id.length() && name.length()))
            return false;
        /*if(colorCode > std::stoul("ffffff", nullptr, 16))
            return false;*/
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
        std::cout << "========Province========\n"
        << "Id: " << id 
        << "\nName: " << name
        << "\nDevelopment score: " << development 
        << "\nProsperity level: " << prosperity 
        << "\nColor Code: " << colorCode
        << "\nBase tax income: " << baseTaxIncome
        << "\nBase levy men: " << baseLevy.getMen()
        << "\nDispPosX: " << dispPosX
        << "\nDispPosY: " << dispPosY
        << std::endl;
    }
    int Province::getLevyMen()
    {
        return levy.getMen();
    }
    unsigned int Province::getColorCode()
    {
        return colorCode;
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
    void Province::changeSiegeStatusBy(int amount)
    {
        siegeStatus += amount;
        if(siegeStatus < 0)
            siegeStatus = 0;
        else if(siegeStatus > 100)
            siegeStatus = 100;
    }
    void Province::changeProsperityBy (int amount)
    {
        prosperity += amount;
        if(prosperity < -100)
            prosperity = -100;
        else if(prosperity > 100)
            prosperity = 100;
    }
    void Province::updateController()
    {
        if(siegeStatus < 100)
            return;
        auto siegingArmyOwner = parent->getKingdomOfCharacter(parent->getArmy(siegingArmy)->getOwnerCharacter());
        auto provinceOwner = kingdomId;
        if(siegingArmyOwner == provinceOwner)
            controlledBy = "none";
        else
            controlledBy = siegingArmyOwner;
        siegeStatus = 0;
        siegingArmy = "none";
    }
    void Province::updateSiege()
    {
        if(isSieged())
            changeSiegeStatusBy(50);
        else
            changeSiegeStatusBy(-50);
    }
    void Province::setSiegingArmy (std::string armyId)
    {
        siegingArmy = armyId;
    }
    bool Province::isSieged()
    {
        return siegingArmy != "none";
    }
    bool Province::isCaptured()
    {
        return controlledBy != "none";
    }
    std::string Province::getController ()
    {
        return controlledBy;
    }
    std::string Province::getKingdom ()
    {
        return kingdomId;
    }
    void Province::setKingdom (std::string kingdomId)
    {
        this->kingdomId = kingdomId;
    }
    void Province::updateData()
    {
        if(id=="sea")
            return;
        // Sieged provinces lose prosperity
        if(isSieged())
            changeProsperityBy(-20);
        // Controlled provinces lose a little prosperity
        else if(isCaptured())
            changeProsperityBy(-2);
        else
            changeProsperityBy(15);
        // Base levy is computed from dev + prosperity + owner martial score
        int men = 1000 + development * 90;
        if(prosperity <= 0)
            men *= (0.1+(100-prosperity)*0.009);
        else
            men *= (1+prosperity*0.005);
        auto data = parent->fetchCharacterData(parent->getKingdomOwner(kingdomId));
        auto martial = data["martial"].get<int>();
        if(martial <= 10)
            men *= (0.1 + martial * 0.09);
        else
            men *= (1 + 0.05*(martial -10));
        baseLevy.setMen(men);
        // Income is computed from dev + prosperity + owner stewardship score
        int bti = 1 + development * 0.49;
        if(prosperity <= 0)
            bti *= (0.1 + (100 - prosperity) * 0.009);
        else
            bti *= (1 + prosperity * 0.005);
        auto stewardship = data["stewardship"].get<int>();
        if(stewardship <= 10)
            bti *= (0.1 + stewardship * 0.09);
        else
            bti *= (1 + (stewardship - 20) * 0.05);
        baseTaxIncome = bti;
        // Controlled or sieged province have an income malus
        if(isSieged())
            taxIncome = baseTaxIncome * 0.5;
        else if(isCaptured())
            taxIncome = 0;
    }
    std::string Province::getOccupant ()
    {
        return controlledBy;
    }

    nlohmann::json Province::toJson ()
    {
        nlohmann::json j;
        j["id"] = id;
        j["name"] = name;
        j["kingdomId"] = kingdomId;
        j["colorCode"] = colorCode;
        j["dispPosX"] = dispPosX;
        j["dispPosY"] = dispPosY;
        j["development"] = development;
        j["prosperity"] = prosperity;
        j["baseLevy"] = baseLevy.toJson();
        j["levy"] = levy.toJson();
        j["baseTaxIncome"] = baseTaxIncome;
        j["taxIncome"] = taxIncome;
        j["siegeStatus"] = siegeStatus;
        j["siegingArmy"] = siegingArmy;
        j["controlledBy"] = controlledBy;
        return j;
    }
}