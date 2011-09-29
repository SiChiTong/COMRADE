#include "IoRedir.h"

Comrade::IO::IO_Redirector stream;

Comrade::IO::IO_Redirector::IO_Redirector()
{
    send_to_stdout=true;
    send_to_fltk_text_display=false;
}

void Comrade::IO::IO_Redirector::write(std::string str)
{
#ifdef STDOUT_SUPPORTED
    if (send_to_stdout==true)
    {
        std::cout<<str;
    }
#endif

#ifdef FLTK1_1_SUPPORTED
	if (send_to_fltk_text_display==true)
    {
        op_handle->append(str.c_str());
    }
#endif
}

Comrade::IO::IO_Redirector::~IO_Redirector()
{}

