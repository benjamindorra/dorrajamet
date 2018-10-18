#include "tools.hpp"

string loadFile(const char* path)
{
    return loadFile(string(path));
}
string loadFile(const string path)
{
    ifstream myFile;
    myFile.open(path);
    if(myFile.is_open())
    {
        stringstream buffer;
        buffer << myFile.rdbuf();
        myFile.close();
        return buffer.str();
    }
    else
    {
        throw runtime_error("Could not open the file: " + path);
    }
}
void saveFile(const char* path, string content)
{
    saveFile(string(path), content);
}
void saveFile(const string path, string content)
{
    ofstream myFile;
    myFile.open(path);
    if(myFile.is_open())
    {
        stringstream buffer(content);
        myFile << buffer.rdbuf();
        myFile.close();
        return;
    }
    else
    {
        throw runtime_error("Could not open the file: " + path);
    }
}