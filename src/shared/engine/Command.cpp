#include "Command.h"

namespace engine
{
    Command::Command ()
    {
        /*
        commandType type;
        std::string argument;
        */
    }
    Command::Command (commandType type, std::string argument)
    {
        this->type = type;
        this->argument = argument;
    }
    Command::~Command ()
    {

    }
    Command::commandType Command::getType()
    {
        return type;
    }
    std::string Command::getArgument()
    {
        return argument;
    }
}

