#ifndef STEREOVISION_H
#define STEREOVISION_H

#include <iostream>
#include <cmath>
#include <vector>
#include "BitmapStreamer.h"
#include "Correlator.h"
#include "QuadtreeSegmenter.h"

namespace Comrade {
	namespace Iris3D {

using namespace IrisFoundation;
		
class DepthmapBuilder
{
private:	int step_size;
			int y_offset;
			int x_extent;
			double qualify_thresh;
			int left_right_tolerance;
			double max_allowed_diff;

public:		RGB uncertainty_color;

		DepthmapBuilder();
		
  		void set_stepsize(int step);
		void set_y_offset(int off);
  		void set_x_extent(int ext);
    	void set_qualify_thresh(double qual);
     	void set_max_allowed_diff(double diff);
      	void set_left_right_tolerance(int tol);
      	void build_depthmap(Buffer<RGB>* left,
				    Buffer<RGB>* right,
				    Buffer<RGB>* dmap);
		
		void build_depthmap(Buffer<RGB>* left,
				    Buffer<RGB>* right,
				    Buffer<RGB>* dmap,
        			Tree<Rectangle,Coordinate>* some_tree);
		
  		~DepthmapBuilder();
};

}
}

#endif
