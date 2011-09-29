#ifndef IO_REDIR_H
#define IO_REDIR_H

#include <iostream>
#include <string>
#include "SystemDefs.h"

#ifdef FLTK1_1_SUPPORTED
#include <FL/Fl.h>
#include <FL/Fl_Window.h>
#include <FL/Fl_Text_Buffer.h>
#include <FL/Fl_Text_Display.h>
#endif

namespace Comrade {
    namespace IO {

class IO_Redirector
{
protected:  

public:     bool send_to_stdout;
            bool send_to_fltk_text_display;

#ifdef FLTK1_1_SUPPORTED
			Fl_Text_Buffer* op_handle;
#endif
            IO_Redirector();

            void write(std::string str);

            ~IO_Redirector();
};

}
}

#endif


