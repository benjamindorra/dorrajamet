#include "Titles.h"
#include <json.hpp>
#include <iostream>

namespace state
{
    Titles::Titles()
    {

    }
    Titles::Titles(std::string strJson)
    {
        using json = nlohmann::json;

        try
        {
            json j = json::parse(strJson);
            std::string id;
            char type;
            for(json::iterator it = j.begin(); it!= j.end(); ++it)
            {
                id = it.value()["id"].get<std::string>();
                type = id.at(0);
                switch(type)
                {
                    case 'k':
                        this->kingdoms.insert(std::pair<std::string, Kingdom>(id, Kingdom(it.value().dump())));
                        this->titles[id] = kingdom;
                        break;
                    case 'd':
                        this->duchies.insert(std::pair<std::string, Duchy>(id, Duchy(it.value().dump())));
                        this->titles[id] = duchy;
                        break;
                    case 'c':
                        this->counties.insert(std::pair<std::string, County>(id, County(it.value().dump())));
                        this->titles[id] = county;
                        break;
                    default:
                        throw std::string("Error: unrecognized title type: " + id);
                }
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl ;
            throw std::runtime_error("Error when loading titles.");
        }
    }
    Titles::~Titles()
    {

    }
    Title * Titles::operator[](const char * a)
    {
        return this->operator[](std::string(a));
    }
    Title * Titles::operator[](const std::string a)
    {
        if(titles.count(a))
        {
            switch(titles[a])
            {
                case kingdom:
                    return &kingdoms[a];
                    break;
                case duchy:
                    return &duchies[a];
                    break;
                case county:
                    return &counties[a];
                    break;
                default:
                    throw std::string("Invalid title: " + a);
            }
        }
        throw std::string("Unknown title: " + a);
    }
    bool Titles::checkConsistency()
    {
        std::string temp1, temp2;
        Title * title;
        for(auto const& titleIndex : titles)
        {
            title = this->operator[](titleIndex.first);
            if(!title->checkConsistency())
            {
                std::cerr << "Error: a title is not consistent with id: " << titleIndex.first;
                return false;
            }
            temp1 = title->getDeJureLiege();
            temp2 = title->getLiege();
            if(!((temp1 == "none" || titles.count(temp1)) && (temp2 == "none" || titles.count(temp2))))
            {
                std::cerr << "Error: a title has a liege or a deJure liege that does not exist with id: " << titleIndex.first;
                return false;
            }
        }
        return true;
    }
    void Titles::debug()
    {
        std::cout << "====Titles====\n";
        for(auto const& title : titles)
            ((*this)[title.first])->debug();
    }
    std::string Titles::getProvinceOwner (std::string provinceId)
    {
        for(auto const& e: counties)
            if(counties[e.first].getProvince() == provinceId)
                return counties[e.first].getHolder();
        return std::string("");
    }
    std::string Titles::getLiege(std::string titleId)
    {
        switch(titles[titleId])
        {
            case kingdom:
                return "none";
            case duchy:
                return duchies[titleId].getLiege();
            case county:
                return counties[titleId].getLiege();
            default:
                throw std::runtime_error("Error: getLiege(titleId): unknown id: " + titleId + "\n");
        }
    }
    std::string Titles::getHolder(std::string titleId)
    {
        switch(titles[titleId])
        {
            case kingdom:
                return kingdoms[titleId].getHolder();
            case duchy:
                return duchies[titleId].getHolder();
            case county:
                return counties[titleId].getHolder();
            default:
                throw std::runtime_error("Error: getHolder(titleId): unknown id: " + titleId + "\n");
        }
    }
    std::string Titles::getTopLiege(std::string titleId)
    {
        auto liege = getLiege(titleId);
        if(liege == "none")
            return titleId;
        else
            return getTopLiege(liege);
    }
    std::vector<std::string> Titles::getTitlesOf (std::string characterId)
    {
        std::vector<std::string> res;
        for(auto const& e: titles)
            if(getHolder(e.first) == characterId)
                res.push_back(e.first);
        return res;
    }
    std::vector<std::string> Titles::getProvincesOf(std::string characterId)
    {
        std::vector<std::string> res;
        for(auto const& e: titles)
            if(getHolder(e.first) == characterId && titles[e.first] == county)
                res.push_back(counties[e.first].getProvince());
        return res;
    }
    std::vector<std::string> Titles::getTitleDirectVassals (std::string titleId)
    {
        std::vector<std::string> res;
        for(auto const& e: titles)
        if(getLiege(e.first) == titleId)
            res.push_back(e.first);
        return res;
    }
    unsigned int Titles::getTitleColor (std::string titleId)
    {
        switch(titles[titleId])
        {
            case kingdom:
                return kingdoms[titleId].getTitleColorCode();
            case duchy:
                return duchies[titleId].getTitleColorCode();
            case county:
                return counties[titleId].getTitleColorCode();
            default:
                throw std::runtime_error("Error: getTitleColorCode(titleId): unknown id: " + titleId + "\n");
        }
    }
    int Titles::computeCharacterPrestige (std::string characterId)
    {
        int res = 0;
        for(auto const& e: titles)
            if(getHolder(e.first) == characterId)
            {
                switch(titles[e.first])
                {
                    case kingdom:
                        res += 10;
                        break;
                    case duchy:
                        res += 5;
                        break;
                    case county:
                        res += 2;
                        break;
                }
            }
        return res;
    }
    void Titles::setTitleHolder (std::string newHolder, std::string titleId)
    {
        switch(titles[titleId])
        {
            case kingdom:
                kingdoms[titleId].setHolder(newHolder);
                return;
            case duchy:
                duchies[titleId].setHolder(newHolder);
                return;
            case county:
                counties[titleId].setHolder(newHolder);
                return;
            default:
                throw std::runtime_error("Error: setHolder(titleId): unknown id: " + titleId + "\n");
        }
    }

    nlohmann::json Titles::fetchTitleData (std::string titleId)
    {
        try
        {
            switch (titles.at(titleId)) {
                case kingdom:
                    return kingdoms.at(titleId).toJson();
                case duchy:
                    return duchies.at(titleId).toJson();
                case county:
                    return counties.at(titleId).toJson();
                default:
                    throw std::runtime_error("Error: setHolder(titleId): unknown id: " + titleId + "\n");
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("Error: Titles::fetchTitleData - unknown id: " + titleId);
        }
    }
}