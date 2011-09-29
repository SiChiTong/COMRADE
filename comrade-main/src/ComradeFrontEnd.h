#ifndef COMRADE_FRONT_END_H
#define COMRADE_FRONT_END_H

#include "FrontEnd.h"
#include "IrisEnv.h"
#include "AbstractHardware.h"
#include "MulticastSignalBuffer.h"
#include "CircularBuffer.h"

namespace Comrade {
    namespace ComradeRuntime {
    
using namespace Scorpio;
using namespace Comrade::ComradeRuntime;
using namespace Comrade::Osiris;
//using namespace Comrade::Corelib;
// Add other namespace directives from Corelib, if any

class ComradeFrontEnd: public FrontEnd
{
protected:  IrisEnvironment* iris_vm_ptr;
            AbstractHardware* hw_vm_ptr;
	    //MulticastSignalBuffer multicast_buffer; // might be shifted somewhere else
	//CircularBuffer<Buffer<RGB> > image_buffer; // may not be needed

	    bool iris_vm_assigned;
            bool hw_vm_assigned;

public:     friend class ExecutionEngine;

			ComradeFrontEnd();

            IrisEnvironment* return_iris_vm_ptr();
            AbstractHardware* return_hw_vm_ptr();
            
            void assign_iris_vm(IrisEnvironment* vm);
            void assign_hw_vm(AbstractHardware* vm);

            ~ComradeFrontEnd();
};

}
}

#endif
