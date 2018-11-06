//Handle the distance between provinces. Used to calculate paths.
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Adjacencies.h"
#include "../tools.hpp"

namespace state
{
    Adjacencies::Adjacencies()
    {
        
    }
    Adjacencies::Adjacencies(std::string strCsv)
    {
        std::vector<string> lines = splitString(strCsv, '\n');
        std::vector<string> columns;
        std::string tagString = lines[0], id1, id2, line, distance;
        std::vector<string> tags = splitString(tagString, ';');
        lines.erase(lines.begin());
        int i, j;
        for(i = 0; i < lines.size(); i++)
        {
            id1 = tags[i];
            line = lines[i];
            val[id1] = std::map<std::string, int>();
            columns = splitString(line, ';');
            for(j = 0; j < columns.size(); j++)
            {
                id2 = tags[j];
                distance = columns[j];
                val[id1][id2] = stoi(distance, nullptr, 10);
            }
        }
    }
    Adjacencies::~Adjacencies()
    {

    }
    bool Adjacencies::checkConsistency()
    {

    }
    void Adjacencies::debug()
    {

    }
}



