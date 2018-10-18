#include "County.h"
#include "../json.hpp"

namespace state
{
    County::County()
    {
        id = std::string("");
        name = std::string("");
        holder = std::string("");
        liege = std::string("");
        deJureLiege = std::string("");
        province = std::string("");
    }
    County::County (std::string strJson)
    {
        using json = nlohmann::json;
        json j = json::parse(strJson);

        try
        {
            id = j["id"].dump();
            name = j["name"].dump();
            holder = j["holder"].dump();
            liege = j["liege"].dump();
            deJureLiege = j["deJureLiege"].dump();
            province = j["province"].dump();
        }
        catch(const std::exception& e)
        {
            throw std::runtime_error("An error occurred when converting Title data from json.");
        }
    }
    County::~County ()
    {

    }
}