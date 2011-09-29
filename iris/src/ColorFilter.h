#ifndef COLOR_FILTER_H
#define COLOR_FILTER_H

#include <iostream>
#include "ColorSpaces.h"
#include "BufferConvertor.h"

namespace Comrade {
	namespace IrisXT {

#define FILTER_OUT 0
#define FILTER_IN 1

using namespace IrisFoundation;

class ColorFilter
{
protected:

public:		RGB filter_color;
			unsigned char red_tol,green_tol,blue_tol;

			ColorFilter();

			bool within_range(RGB current);
			void filter(Buffer<RGB>* ip,Buffer<RGB>* op,int in_or_out);

			void filter(Buffer<RGB>* ip,Buffer<RGB>* op,
							int in_or_out,
							int x1,int y1,int x2,int y2);

			~ColorFilter();
};

}
}

#endif
