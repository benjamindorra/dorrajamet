#include "Title.h"
#include "json.hpp"


namespace state
{
    const std::string& Title::getId() const
    {
        return this->id;
    }
    void Title::setId(const std::string& id)
    {
        this->id = id;
    }
    const std::string& Title::getName() const
    {
        return this->name;
    }
    void Title::setName(const std::string& name)
    {
        this->name = name;
    }
    const std::string& Title::getHolder() const
    {
        return this->holder;
    }
    void Title::setHolder(const std::string& holder)
    {
        this->holder = holder;
    }
    unsigned int Title::getTitleColorCode() const
    {
        return this->titleColorCode;
    }
    void Title::setTitleColorCode(unsigned int titleColorCode)
    {
        this->titleColorCode = titleColorCode;
    }
    
    nlohmann::json Title::toJson() {
        nlohmann::json j;
        j["id"] = id;
        j["name"] = name;
        /// Pointer to the holder if any
        j["holder"] = holder;
        j["titleColorCode"] = titleColorCode;
        return j;
    }
}