#include "Adjacencies.h"
#include "../tools.hpp"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>

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

        for(auto const& l:val)
            for(auto const& c: l.second)
                paths[l.first][c.first] = this->computeShortestPath(l.first, c.first);
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

        std::cout << "\n\n";

        for(auto const& l:paths)
            for(auto const& c:l.second)
            {
                auto path = paths[l.first][c.first];
                std::cout << "From " << l.first << " to " << c.first << ": ";
                for(auto const& step:path)
                    std::cout << step << " - ";
                std::cout << std::endl;
            }

        
    }
    //Next functions implement Dijkstra's algorithm
    //-1 being infinity, each inequality test against distance values are preceded by -1 case
    std::string Adjacencies::getClosestProv (std::string origId, std::map<std::string, int> * d, std::vector<std::string> * Q)
    {
        //std::cout << "getClosest\n";
        int mini = std::numeric_limits<int>::max();
        std::string sommet = "", s;
        //std::cout << "mini " << mini << std::endl;
        for(auto const& s: *Q)
        {
            /*std::cout << "s " << s << std::endl;
            std::cout << (*d)[s] << std::endl;
            std::cout << ((*d)[s] > 0 && (*d)[s] < mini) << std::endl;*/
            if((*d)[s] > 0 && (*d)[s] < mini)
            {
                mini = (*d)[s];
                sommet = s;
            }
        }
        //throw std::runtime_error("debug");
        return sommet;
    }
    void Adjacencies::updateDists (std::map<std::string, int> * d, std::map<std::string, std::string> * p, std::vector<std::string> * Q, int weight, std::string s1, std::string s2)
    {
        //std::cout << "update\ns1 " << s1 << "\ns2 " << s2 << "\nweight " << weight << std::endl;
        //std::cout << "d[s1] " << (*d)[s1] << "\nd[s2] " << (*d)[s2] << std::endl;
        if((*d)[s1] == -1)
            return;
        if((*d)[s2] == -1 || (*d)[s2] > (*d)[s1] + weight)
        {
            (*d)[s2] = (*d)[s1] + weight;
            (*p)[s2] = s1;
        }
        //throw runtime_error("merde");
    }
    std::vector<std::string> Adjacencies::computeShortestPath(std::string origId, std::string destId)
    {
        if(origId == destId)
        {
            std::vector<std::string> r;
            r.push_back(destId);
            return r;
        }
        // Init of Dijkstra's alg
        std::map<std::string, int> d;
        std::map<std::string, std::string> p;
        std::vector<std::string> Q;
        for(auto const& line: val)
        {
            d[line.first] = -1;
            Q.push_back(line.first);
        }
        for(auto const& element: val[origId])
        {
            int a = element.second;
            if(a > 0)
            {
                d[element.first] = a;
                p[element.first] = origId;
            }
        }
        if(d.size() != val.size())
            throw std::runtime_error("Error: unknown origin province.");
        

        while(Q.size())
        {
            auto s1 = this->getClosestProv(origId, &d, &Q);
            //std::cout << s1 << std::endl;
            if(s1.size())
                Q.erase(std::remove(Q.begin(), Q.end(), s1), Q.end());
            for(auto s2: Q)
                if(val[s1][s2] > 0)// If s1 and s2 are neighbors
                    this->updateDists(&d, &p, &Q, val[s1][s2], s1, s2);
        }



        std::vector<std::string> A;
        auto s = destId;
        unsigned int security = 0;
        while(s != origId)
        {
            A.push_back(s);
            s = p[s];
            if(security++ > val.size())
                throw std::runtime_error("Error: no path found.");
        }
        std::reverse(std::begin(A), std::end(A));
        return A;
    }
    std::vector<std::pair<std::string, int>> Adjacencies::getShortestPath(std::string origId, std::string destId)
    {
        auto path = paths[origId][destId];
        std::vector<std::pair<std::string, int>> res;
        std::string prev = origId;
        for(auto const& step:path)
        {
            res.push_back(std::pair<std::string, int>(step, val[prev][step]));
            prev = step;
        }
        return res;
    }
    nlohmann::json Adjacencies::getOrderJson (std::string origId, std::string destId)
    {
        nlohmann::json j, o;
        j["originProvinceId"] = origId;
        j["destinationProvinceId"] = destId;
        j["orders"] = nlohmann::json::array();
        auto path = getShortestPath(origId, destId);
        int duration = 0, dist;
        std::string dest, orig = origId;
        for(auto const& step:path)
        {
            o = nlohmann::json::object();
            dest = step.first;
            dist = step.second;
            o["originProvinceId"] = orig;
            o["destinationProvinceId"] = dest;
            o["duration"] = dist / 5;
            o["elapsed"] = 0;

            j["orders"].push_back(o);
            duration += dist/5;
            orig = dest;
        }
        j["duration"] = duration;
        j["elapsed"] = 0;

        return j;
    }
}



