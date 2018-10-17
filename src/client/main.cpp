#include <iostream>
#include <string>
#include "tools.h"



int main(int argc, char ** argv)
{
    if(argc == 2)
    {
        std::string str(argv[1]);
        if(str == "hello")
            std::cout << "Hello World!" << std::endl;
    }

    int a = 42;
    std::string id("0001");

    IdRef<int>::val ref;

    ref.first = id;
    ref.second = &a;

    std::cout << "object id: " << ref.first << "; object value: " << *ref.second << std::endl;


    int b = 43, c = 44;
    std::string id2("0002"), id3("0003");
    IdRefList<int>::val refList;
    refList[id] = &a;
    refList[id2] = &b;
    refList[id3] = &c;

    for(auto const& x : refList)
    {
        std::cout << x.first << ": " << *x.second << std::endl;
    }
    

    return 0;
}