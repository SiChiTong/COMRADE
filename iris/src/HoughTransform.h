#ifndef HOUGH_TRANSFORM_H
#define HOUGH_TRANSFORM_H

#include <vector>
#include "newmath.h"
#include "EdgeDetectors.h"
#include "ShapeSampler.h"
#include "ColorFilter.h"

namespace Comrade {
    namespace IrisXT {

using namespace Comrade::IrisFoundation;

class HoughTransformObject
{
private:

public:     ColorFilter cfilter;
	
	    HoughTransformObject();
            virtual void transform(Buffer<RGB>* ip)=0;
            virtual ~HoughTransformObject();
};

class StraightLineDetector:public HoughTransformObject
{
private:    int rows,cols;
	    int angle_increment;
            long int** param_array;

			void allocate(int perp,int theta);
            void deallocate();
	    void set_param(int perp_levels,int theta_levels);
            
public:     StraightLineDetector(int perp_levels,int theta_levels,int ang_inc);
            StraightLineDetector(Buffer<RGB>* buf,int theta_levels,int ang_inc);
	    //StraightLineDetector();

            int return_rows();
            int return_cols();
	    int return_angle_increment();

            long int** return_array();
	    
            void clear_param_array();

	    void transform(Buffer<RGB>* ip,
			   int x1,int y1,
			   int x2,int y2);

	    void transform(Buffer<RGB>* ip);

	    //void set_angle_increment(int inc);

            ~StraightLineDetector();
};

class CircleDetector:public HoughTransformObject
{
private:	long int*** param_array;
		int rows,cols,min_radius,max_radius,radius_range;
		EdgeDetectorManager edman;

public:		CircleDetector(int width,int height,int minrad,int maxrad);
	
		void allocate(int width,int height,int minrad,int maxrad);
		void deallocate();

		long int*** return_array()
		{
			return param_array;
		}
		
		int return_rows()
		{
			return rows;
		}
		
		int return_cols()
		{
			return cols;
		}
		
		int return_radius_range()
		{
			return radius_range;
		}

		void transform(Buffer<RGB>* ip,
			       int x1,int y1,
			       int x2,int y2);

	    	void transform(Buffer<RGB>* ip);

		~CircleDetector();
};

class BasicShapeDetector:public HoughTransformObject
{
private:	Buffer<int> screen_array;
			int samples;
            EdgeDetectorManager edman;
			
public:		int change_sz;
			std::vector<double> shape_table;
			
			BasicShapeDetector();
			
   			void build_shape_table(Buffer<RGB>* shape_screen,
      							   int cx,int cy,int num_samples);
			
   			void transform(Buffer<RGB>* ip);
			Buffer<int>* return_array();
			
   			~BasicShapeDetector();
};

}
}

#endif
