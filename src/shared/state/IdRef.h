#ifndef IDREF_H
#define IDREF_H

#include <string>

template<class T>
class IdRef
{
    private:
    std::pair<std::string, T*> val;
    public:
    // Constructors
    IdRef() : IdRef(""){}
    IdRef(char* str) : IdRef(std::string(str)){}
    IdRef(std::string str) : IdRef(str, nullptr){}
    IdRef(std::string id, T* pointer)
    {
        val = std::pair<std::string, T*>(id, pointer);
    }
    IdRef(const IdRef<T> obj)
    {
        this->val = obj.val;
    }
    //Get/Setters
    std::string getId(){return val.first;}
    void setId(std::string id){val.first = id;}
    T* getPtr(){return val.second}
    void setPtr(T* obj){val.second = obj}
};

#endif