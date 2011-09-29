#include "Correlator.h"
#include <cmath>

double Comrade::Iris3D::correlate(Buffer<RGB>* buffer1,
                                  Buffer<RGB>* buffer2,
                                  int x1,int y1,int x2,int y2,
                                  int bx,int by)
{
    double sum=0;
    double total=0;

    for (int yy=y1; yy<=y2; ++yy)
    {
        for (int xx=x1; xx<=x2; ++xx)
        {
            if (xx>=buffer1->maxx-1 ||
                yy>=buffer1->maxy-1 ||
                bx+xx-x1>=buffer2->maxx-1 ||
                by+yy-y1>=buffer2->maxy-1)
            {
                continue;
            }

            double val1=buffer1->at(xx,yy).green/255.0;
            double val2=buffer2->at(bx+xx-x1,by+yy-y1).green/255.0;

            sum+=(val1-val2)*(val1-val2);
            ++total;
        }
    }

    sum/=total;

    return sum;
}

double Comrade::Iris3D::find_variance(Buffer<RGB>* buffer,
                          			  int x1,int y1,int x2,int y2)
{
 	double sum=0;
    double total=0;

    for (int yy=y1; yy<=y2; ++yy)
    {
        for (int xx=x1; xx<=x2; ++xx)
        {
			sum+=buffer->at(xx,yy).green;
		}
	}
	
	sum/=((x2-x1)*(y2-y1));
	
	for (int yy=y1; yy<=y2; ++yy)
    {
        for (int xx=x1; xx<=x2; ++xx)
        {
			total+=(buffer->at(xx,yy).green-sum)*
				   (buffer->at(xx,yy).green-sum);
		}
	}
	
	total/=((x2-x1)*(y2-y1));
	
	return sqrt(total);
}
                          			  

/*double Comrade::Iris3D::sliding_window_correlate(Buffer<RGB>* buffer1,
											     Buffer<RGB>* buffer2,
                 							     int x1,int y1,int x2,int y2,
                 							     int bx,int by,int extent)
{
    double avg1=0,avg2=0;
    double var1=0,var2=0;
    double total=0;

    for (int yy=y1; yy<=y2; yy++)
    {
        for (int xx=x1; xx<=x2; xx++)
        {
            if (xx>=buffer1->maxx-1 ||
                yy>=buffer1->maxy-1 ||
                bx+xx-x1>=buffer2->maxx-1 ||
                by+yy-y1>=buffer2->maxy-1)
            {
                continue;
            }

            double val1=buffer1->at(xx,yy).green/255.0;
            double val2=buffer2->at(bx+xx-x1,by+yy-y1).green/255.0;

            avg1+=val1;
            avg2+=val2;
            
            total++;
        }
    }

    avg1/=total;
    avg2/=total;
    
    for (int yy=y1; yy<=y2; yy++)
    {
        for (int xx=x1; xx<=x2; xx++)
        {
            if (xx>=buffer1->maxx-1 ||
                yy>=buffer1->maxy-1 ||
                bx+xx-x1>=buffer2->maxx-1 ||
                by+yy-y1>=buffer2->maxy-1)
            {
                continue;
            }

            double val1=buffer1->at(xx,yy).green/255.0;
            double val2=buffer2->at(bx+xx-x1,by+yy-y1).green/255.0;

			var1=(val1-avg1)*(val1-avg1);
			var2=(val2-avg2)*(val2-avg2);
			
			

}
*/
                 				

