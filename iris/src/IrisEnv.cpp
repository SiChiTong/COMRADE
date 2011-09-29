#include "IrisEnv.h"

extern Comrade::IO::IO_Redirector stream;

Comrade::ComradeRuntime::IrisEnvironment::IrisEnvironment()
{
    stream.write("IRIS Runtime for Windows/Linux loaded...\n");
    std::cout<<"IrisEnvironment object created...\n";
}

Comrade::ComradeRuntime::IrisEnvironment::~IrisEnvironment()
{
    std::cout<<"IrisEnvironment destroyed...\n";
}


