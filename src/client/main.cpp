#include <iostream>
#include <fstream>
#include <string>
#include "../shared/json.hpp"
#include "../shared/tools.hpp"
#include "../shared/state.h"

void debug(int c)
{
    std::cout << "Jusqu'ici tout va bien : " << c << std::endl;
}

int main(int argc, char ** argv)
{
    using json = nlohmann::json;
    std::string titleString = loadFile("./res/testTitle.json");
    json j = json::parse(titleString);
    state::County county;
    state::Duchy duchy;
    state::Kingdom kingdom;
    int type = j["type"].get<int>();
    switch(type)
    {
        case 1://Count
            county = state::County(titleString);
            std::cout << "Created a County\n";
            break;
        case 2://Duke
            duchy = state::Duchy(titleString);
            std::cout << "Created a Duchy\n";
            break;
        case 3://King
            kingdom = state::Kingdom(titleString);
            std::cout << "Created a Kingdom\n";
            break;
    }

    /*using json = nlohmann::json;
    std::string content = loadFile("./res/test.json");
    std::cout << content << std::endl;
    json j = json::parse(content);
    int n;
    n = j["age"].get<int>();
    n++;
    j["age"] = n;
    std::string res = j.dump(4);
    saveFile("./res/test.json", res);*/
    return 0;
}