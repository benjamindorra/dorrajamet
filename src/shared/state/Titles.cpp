#include "Titles.h"
#include "../json.hpp"
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
}