#ifndef EPIPOLAR_RECTFIER_H
#define EPIPOLAR_RECTIFIER_H

#include <iostream>
#include "Correlator.h"
#include "BitmapStreamer.h"

namespace Comrade {
	namespace Iris3D {

using namespace Comrade::IrisFoundation;

struct EpipolarProfile
{

};

class EpipolarRectifier
{
private:

public:		EpipolarRectifier();
			
			EpipolarProfile build_epipolar_profile(Buffer<RGB>* left,
												   Buffer<RGB>* right,
               									   int scanline,
                           						   int height,int width);
        	
			void draw_epipolar_profile(EpipolarProfile& profile);
			
   			~EpipolarRectifier();
};

}
}

#endif


