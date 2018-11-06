#include "Adjacencies.h"
#include "tools.hpp"

namespace state
{
    Adjacencies::Adjacencies()
    {
        
    }
    Adjacencies::Adjacencies (std::string rawCsv)
    {
        auto lines = splitString(rawCsv, '\n');
        auto tags = splitString(lines[0], ';');
        lines.erase(lines.begin());
        for(unsigned int i = 0; i < tags.size(); i++)
        {
            auto tag = tags[i];
            auto line = lines[i];
            val[tag] = std::map<std::string, int>();
            auto cols = splitString(line, ';');
            for(unsigned int j = 0; j < cols.size(); j++)
            {
                auto e = stoi(cols[j]);
                auto tag2 = tags[j];
                val[tag][tag2] = e;
            }
        }
    }
}



