#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

string loadFile(const char* path);
string loadFile(const string path);
void saveFile(const char* path, string content);
void saveFile(const string path, string content);
vector<string> splitString(string s, char c);
