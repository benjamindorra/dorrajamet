#include <iostream>
#include <fstream>
#include <string>
#include "../shared/json.hpp"
#include "../shared/tools.hpp"

void debug(int c)
{
    std::cout << "Jusqu'ici tout va bien : " << c << std::endl;
}

int main(int argc, char ** argv)
{
    using json = nlohmann::json;
    std::string content = loadFile("./res/test.json");
    std::cout << content << std::endl;
    json j = json::parse(content);
    int n;
    n = j["age"].get<int>();
    n++;
    j["age"] = n;
    std::string res = j.dump(4);
    saveFile("./res/test.json", res);
    return 0;
}