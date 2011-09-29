#ifndef FRONT_END_H
#define FRONT_END_H

#include <iostream>
#include <string>
#include <cstdio>
#include "WordFinder.h"
#include "Interpreter.h"
#include "IoRedir.h"

namespace Scorpio {

class FrontEnd
{
protected:      std::string prompt;
                char command[80];
                Interpreter interpreter;

public:         FrontEnd* return_front_handle();

                FrontEnd();
                void show_copyright();
                void interpret(const char* cmd);
                void erase(std::string cmd_string);
                void ext_interpret(char* cmd);
                void add(std::string command_string,std::string form,function_handle fn);
                void set_prompt(std::string p);
                void activate();
                ~FrontEnd();
};

}

#endif

