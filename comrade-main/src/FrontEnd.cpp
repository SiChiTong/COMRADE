#include "FrontEnd.h"

extern Comrade::IO::IO_Redirector stream;

Scorpio::FrontEnd::FrontEnd()
{
    //system("clear");
    
    set_prompt("IRIS>>");
}

void Scorpio::FrontEnd::show_copyright()
{
    stream.write("\nScorpioCLI v2.01 for Linux/Windows\n");
    stream.write("--------------------------------------------------------------------------\n");
    stream.write("(c) 2003 Avishek Sen Gupta\n");
    stream.write("\nHave a nice day :-)\n\n");
}

Scorpio::FrontEnd* Scorpio::FrontEnd::return_front_handle()
{
    return this;
}

void Scorpio::FrontEnd::set_prompt(std::string p)
{
    prompt=p;
}

void Scorpio::FrontEnd::add
(std::string command_string,std::string form,function_handle fn)
{
    interpreter.add(command_string,form,fn);
}

void Scorpio::FrontEnd::erase(std::string cmd_string)
{
    interpreter.erase(cmd_string);
}

void Scorpio::FrontEnd::interpret(const char* cmd)
{
    interpreter.interpret(cmd);
}

void Scorpio::FrontEnd::ext_interpret(char* cmd)
{
    interpreter.store_current();
    interpret(cmd);
    interpreter.restore_current();
}

void Scorpio::FrontEnd::activate()
{
    while (true)
    {
        stream.write(prompt);
        fflush(stdin);
        std::cin.getline(command,79);

        interpret(command);
    }
}

Scorpio::FrontEnd::~FrontEnd()
{}

