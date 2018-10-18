#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"

void debug(int c)
{
    std::cout << "Jusqu'ici tout va bien : " << c << std::endl;
}

int main(int argc, char ** argv)
{
    using json = nlohmann::json;
    std::ifstream myfile;
    myfile.open("./res/test.json");
    std::string content;
    char buffer[1024];
    if(myfile.is_open())
    {
        debug(0);
        int i =0;
        while(!myfile.eof())
        {
            myfile >> buffer[i];
            std::cout << "[" << i << "] = " << buffer[i] << std::endl;
            i++;
        }
        buffer[i-1] = 0;
        std::cout << buffer << std::endl;
        debug(1);
        content = std::string(buffer);
        myfile.close();
        std::cout << content << std::endl;
        debug(2);
        json j = json::parse(content);
        int n;
        n = j["age"].get<int>();
        n++;
        j["age"] = n;
        debug(3);
        std::ofstream ofile;
        ofile.open("./res/test.json");
        if(ofile.is_open())
        {
            debug(4);
            ofile << j.dump(4);
            debug(5);
            ofile.close();
            debug(6);
        }
        else
        {
            std::cout << "could not open output file" << std::endl;
        }

    }
    else
    {
        std::cout << "could not open file" << std::endl;
    }
    return 0;
}