#include "Kingdom.h"
#include "../json.hpp"

namespace state
{
    Kingdom::Kingdom()
    {
        id = std::string("");
        name = std::string("");
        holder = std::string("");
    }
    Kingdom::Kingdom (std::string strJson)
    {
        using json = nlohmann::json;
        json j = json::parse(strJson);

        try
        {
            id = j["id"].dump();
            name = j["name"].dump();
            holder = j["holder"].dump();
        }
        catch(const std::exception& e)
        {
            throw std::runtime_error("An error occurred when converting Title data from json.");
        }
    }
    Kingdom::~Kingdom ()
    {

    }
}