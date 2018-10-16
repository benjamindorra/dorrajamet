// tools.h

#ifndef TOOLS_H
#define TOOLS_H

#include <map>
#include <string>

// Defines template structures to wrap a pair of id/pointer, or a map of pointers indexed by ids.
// Do not forget to delete your pointers because the std::map and std::pair will not do it
// These structures are intended to keep a reference in an object for another object within the gamestate, as well as to its ID for saving the game

template<typename T>
struct IdRef
{
    typedef std::pair<std::string, T*> val;
};
// Usage:
/*
IdRef<Foo>::val bar;
bar.first = std::string("foo_id_xxxx");
bar.second = new Foo();
*/

template<typename T>
struct IdRefList
{
    typedef std::map<std::string, T*> val;
};
// Usage:
/*
IdRefList<Foo>::val bar;
bar[std::string("foo_id_xxxx")] = new Foo();
bar[std::string("foo_id_yyyy")] = new Foo();
*/

#endif