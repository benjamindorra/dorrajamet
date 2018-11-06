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

/* load a distance map between provinces (in turn) from a CSV file
structured as follow :
id1, id2, id3
distance11,distance12,distance13
distance21,distance22,distance23
distance31,distance32,distance33
*/
/*state::Adjacencies::Adjacencies (const std::string path) {
    std::ifstream file(path);
    if (!file.good()) { 
        throw std::runtime_error("Cannot open adjacencies file");
    }
    std::string line;
    //extracting the keys
    std::vector<std::string> keys;
    std::getline(file, line);
    std::istringstream sline(line);
    std::string item;
    while (std::getline(sline, item, ',')) {
        keys.push_back(item);
    }
    //extracting the distances
    if (!file.good()) { 
        throw std::runtime_error("Cannot read adjacencies file");
    }
    int i=0;
    int j=0;
    while (std::getline(file, line)) {
        std::istringstream sline(line);
        j=0;
        while (std::getline(sline, item, ',')) {
            val[keys[i]][keys[j]] = std::stoi(item);
            j++;
        }
        i++;
    }
}*/

