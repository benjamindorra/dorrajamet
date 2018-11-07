#include "Adjacencies.h"
#include "../tools.hpp"
#include <iostream>
#include <iomanip>

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
        unsigned int i, j;
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
        // Matrix must be symetric with a null diagonal
        for(auto line:val)
        {
            for(auto element: line.second)
            {
                if(val[line.first][element.first] != val[element.first][line.first])
                    return false;
                if(element.first == line.first)
                    if(element.second)
                        return false;
            }
        }
        return true;
    }
    void Adjacencies::debug()
    {
        std::string orig, dest;
        int distance;
        std::cout << "======Adjacencies matrix======" << std::endl;
        // Printing the tags on the first line
        std::cout << std::setw(16) << "";
        for(auto line:val)
            std::cout << " - " << std::setw(16) << line.first;
        std::cout << std::endl;
        for(auto line:val)
        {
            orig = line.first;
            std::cout << std::setw(16) << orig;
            for(auto elem:line.second)
            {
                dest = elem.first;
                distance = elem.second;
                std::cout << " - " << std::setw(16) << distance;
            }
            std::cout << std::endl;
        }
    }
}



