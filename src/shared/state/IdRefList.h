#ifndef IDREFLIST_H
#define IDREFLIST_H

#include <map>
#include <string>
#include <vector>

template<class T>
class IdRefList
{
    private:
    std::map<std::string, T*> val;
    public:
    IdRefList() : IdRefList(std::vector<std::string>()){}
    IdRefList(std::initializer_list<const char *> ids) : IdRefList(std::vector<std::string>(ids)){}
    IdRefList(std::vector<std::string> ids)
    {
        for(std::string s : ids)
            this->insert(s);
    }
    IdRefList(std::map<std::string, T*> val)
    {
        this->val = val;
    }
    int size()
    {
        return val.size();
    }
    void insert(char* id)
    {
        this.insert(std::string(id));
    }
    void insert(std::string id)
    {
        this->insert(std::pair<id, nullptr);
    }
    void insert(std::pair<std::string, T*> obj)
    {
        this->val.insert(obj);
    }
    void erase(char* id)
    {
        this->erase(std::string(id));
    }
    void erase(std::string id)
    {
        val.erase(id);
    }
    T* operator[](char* id)
    {
        return this->operator[](std::string(id));
    }
    T* operator[](std::string id)
    {
        return val[id];
    }
    
};

#endif