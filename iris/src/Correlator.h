#ifndef CORRELATOR_H
#define CORRELATOR_H

#include "Buffer.h"
#include <cmath>

namespace Comrade {
    namespace Iris3D {

using namespace Comrade::IrisFoundation;

double correlate(Buffer<RGB>* buffer1,Buffer<RGB>* buffer2,
                 int x1,int y1,int x2,int y2,
                 int bx,int by);

double find_variance(Buffer<RGB>* buffer,
                          			  int x1,int y1,int x2,int y2);

/*double sliding_window_correlate(Buffer<RGB>* buffer1,Buffer<RGB>* buffer2,
                 int x1,int y1,int x2,int y2,
                 int bx,int by,int extent);
*/
}
}

#endif


