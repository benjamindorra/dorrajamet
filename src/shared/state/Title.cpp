#include "Title.h"


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
    const std::map<std::string, int>& Title::getHistory() const
    {
        return this->history;
    }
    void Title::setHistory(const std::map<std::string, int>& history)
    {
        this->history = history;
    }
    const std::string& Title::getLiege() const
    {
        return this->liege;
    }
    void Title::setLiege(const std::string& liege)
    {
        this->liege = liege;
    }
    const std::string& Title::getDeJureLiege() const
    {
        return this->deJureLiege;
    }
    void Title::setDeJureLiege(const std::string& deJureLiege)
    {
        this->deJureLiege = deJureLiege;
    }
}