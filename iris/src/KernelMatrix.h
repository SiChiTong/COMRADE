#ifndef KERNEL_MATRIX_H
#define KERNEL_MATRIX_H

#include "BitmapStreamer.h"
#include "ColorSpaces.h"
#include "BufferConvertor.h"
//#include "Gollum.h"

namespace Comrade {
    namespace IrisXT {

using namespace Comrade::IrisFoundation;

struct KernelMatrix
{
    int xsize,ysize;
    int half_xsize,half_ysize;
    double** matrix;

    KernelMatrix(int r,int c);
    void set_values(double* arr);
    ~KernelMatrix();
};

class KernelOperator
{
protected:  KernelMatrix* rgb_kernel;
            BitmapStreamer* reader;
            double factor;
            int xsz,ysz;

public:     Buffer<RGB>* ip_buffer;
            Buffer<RGB>* op_buffer;

            KernelOperator();

            // Deprecated, do NOT use this function!!
            //void set_bounds(int x,int y);

            void set_bounds(BitmapStreamer* bmp_reader);
            void set_factor(double fac);
            void assign_buffer_ptrs(Buffer<RGB>* ip,Buffer<RGB>* op);
            void assign_kernel(KernelMatrix* mat);
            
			void convolve(int x1,int y1,int x2,int y2);
            void convolve();
            void test_convolve(int x1,int y1,int x2,int y2);
			
			double test_unit_convolve(int x,int y,unsigned char channel_strength);
			double unit_convolve(int x,int y,Buffer<HSLstruct>* hsl_buf);
            double greyscale_unit_convolve(int x,int y,Buffer<RGB>* rgb_ip);

            ~KernelOperator();
};

}
}

#endif

