#ifndef OPTICAL_FLOW_ENGINE_H
#define OPTICAL_FLOW_ENGINE_H

#include <iostream>
#include <vector>
#include "Buffer.h"
#include "Correlator.h"

namespace Comrade {
	namespace Iris3D {

using namespace Comrade::IrisFoundation;

class OpticalFlowEngine
{
protected:	Buffer<RGB>* first,*second;
            Buffer<RGB>* o_flow;

public:		OpticalFlowEngine();
		
            void estimate_optical_flow(Buffer<RGB>* one,
					   Buffer<RGB>* two,
					   Buffer<RGB>* flow,int boxsize,int extent);

            ~OpticalFlowEngine();

};

}
}

#endif

