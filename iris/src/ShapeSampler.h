#ifndef SHAPE_SAMPLER_H
#define SHAPE_SAMPLER_H

#include <vector>
#include <cmath>
#include "Buffer.h"

namespace Comrade {
    namespace IrisXT {

using namespace IrisFoundation;

class ShapeSampler
{
protected:  double sin_table[360];
            double cos_table[360];

public:     ShapeSampler();

            void build_sin_table();
            void build_cos_table();

            void sample(Buffer<RGB>* ip_buffer,
                        std::vector<double>* radial,
                        int cx,int cy,int no);

            ~ShapeSampler();
};

}
}

#endif


