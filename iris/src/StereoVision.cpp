#include "StereoVision.h"

Comrade::Iris3D::DepthmapBuilder::DepthmapBuilder()
{
    step_size=8;
    y_offset=0;
    x_extent=20;
    qualify_thresh=20;
    left_right_tolerance=0;
    max_allowed_diff=10;
    
    uncertainty_color.red=255;
    uncertainty_color.green=0;
    uncertainty_color.blue=0;
    
    std::cout<<"DepthmapBuilder object created...\n";
}

void Comrade::Iris3D::DepthmapBuilder::set_x_extent(int ext)
{
	x_extent=ext;
}
    	
void Comrade::Iris3D::DepthmapBuilder::set_stepsize(int step)
{
	step_size=step;
}

void Comrade::Iris3D::DepthmapBuilder::set_y_offset(int off)
{
	y_offset=off;
}

void Comrade::Iris3D::DepthmapBuilder::set_qualify_thresh(double qual)
{
	qualify_thresh=qual;
}

void Comrade::Iris3D::DepthmapBuilder::set_left_right_tolerance(int tol)
{
	left_right_tolerance=tol;
}

void Comrade::Iris3D::DepthmapBuilder::set_max_allowed_diff(double diff)
{
	max_allowed_diff=diff;
}
      	
void Comrade::Iris3D::DepthmapBuilder::build_depthmap
(Buffer<RGB>* left,Buffer<RGB>* right,Buffer<RGB>* dmap,
Tree<Rectangle,Coordinate>* some_tree)
{
	std::vector<TreeNode<Rectangle>*> vect;
	some_tree->get_terminal_nodes(&vect);
	
	std::cout<<vect.size();
	
  	//int IMAGE_X=left->maxx;
    //int IMAGE_Y=left->maxy;
    
    int cnt=0;
    double ishade=0;
    
    for (unsigned int i=0; i<=vect.size()-1; ++i)
    {
		int x1=vect[i]->data.x1;
		int y1=vect[i]->data.y1;
		int x2=vect[i]->data.x2;
		int y2=vect[i]->data.y2;
		
		std::vector<std::vector<double> > arr1(2*y_offset+1),arr2(2*y_offset+1);
        //IMAGE_X-1-x1

        if (find_variance(left,x1,y1,x2,y2)>=qualify_thresh)
    	{
 			for (int i=0; i<=x_extent; ++i)
    		{
        		for (int off=-y_offset; off<=y_offset; ++off)
        		{
            		//cout<<"Now doing..."<<i<<endl;
        			arr1[off+y_offset].push_back(correlate(left,right,x1,y1,x2,y2,x1+i,y1+off));
     			}
    		}

        	double min1=arr1[0][0];
        	int minpos1=0;
            
        	for (unsigned int i=0; i<=arr1[0].size()-1; ++i)
        	{
            	for (int j=0; j<=2*y_offset; ++j)
        		{
        			if (arr1[j][i]<min1)
            		{
                		min1=arr1[j][i];
                		minpos1=i;
            		}
        		}
        	}

        	// Not sure if this code is useful...
        	/*if (min1>max_allowed_diff)
        	{
        		for (int yy=y1; yy<=y2; ++yy)
        		{
            		for (int xx=x1; xx<=x2; ++xx)
            		{
         				dmap->at(xx,yy)=uncertainty_color;
    				}
				}
            	
        		continue;
    		}*/
            
        	// Left-right consistency check
        	for (int i=0; i<=x_extent; ++i)
        	{
				for (int off=-y_offset; off<=y_offset; ++off)
            	{
            		//cout<<"Now doing..."<<i<<endl;
              		arr2[off+y_offset].push_back(correlate(right,left,x1+minpos1,y1,x2+minpos1,y2,x1+minpos1-i,y1+off));
         		}
        	}

        	double min2=arr2[0][0];
        	int minpos2=0;
            	
        	for (unsigned int i=0; i<=arr2[0].size()-1; ++i)
        	{
            	for (int j=0; j<=2*y_offset; ++j)
        		{
    				if (arr2[j][i]<min2)
            		{
                		min2=arr2[j][i];
                		minpos2=i;
            		}
        		}
        	}
            
        	int diff=minpos1-minpos2;
        	diff=((diff>=0)?diff:(-diff));
        	
         	//cout<<" Is "<<diff<<">"<<left_right_tolerance<<"? ";
		
		if (diff>left_right_tolerance)
        	{
        		//cout<<"Yes...\n";
			
			for (int yy=y1; yy<=y2; ++yy)
        		{
            			for (int xx=x1; xx<=x2; ++xx)
            			{
                			dmap->at(xx,yy)=uncertainty_color;
            			}
        		}
            	
        		continue;
        	}	
		
		else
		{
        		//cout<<"Minimum is at : "<<minpos1<<endl;
        		ishade=(255*(1-(1./(minpos1+1))));
		}
	}
		
	else
	{			
		for (int yy=y1; yy<=y2; ++yy)
        	{
            		for (int xx=x1; xx<=x2; ++xx)
            		{
                		dmap->at(xx,yy)=uncertainty_color;
            		}
        	}
            	
        	continue;
   			//ishade=255;
	}
		
	if (ishade>255)
	{
		#ifdef DEBUG
		std::cout<<"Overflow...\n";
		#endif
       
   		ishade=255;
	}
	
	if (ishade<0)
	{
		ishade=0;
	}
	
	unsigned char shade=static_cast<unsigned char>(ishade);
            
       	for (int yy=y1; yy<=y2; ++yy)
        {
            for (int xx=x1; xx<=x2; ++xx)
            {
                dmap->at(xx,yy).red=

                dmap->at(xx,yy).green=
                dmap->at(xx,yy).blue=shade;
            }
        }
 	}
 
	std::cout<<"Number of overflows="<<cnt;
}

void Comrade::Iris3D::DepthmapBuilder::build_depthmap
(Buffer<RGB>* left,Buffer<RGB>* right,Buffer<RGB>* dmap)
{
    double ishade;
            
    int IMAGE_X=left->maxx;
    int IMAGE_Y=left->maxy;
    
    for (int y1=0; y1<=IMAGE_Y-1; y1+=step_size)
    {
        for (int x1=0; x1<=IMAGE_X-1; x1+=step_size)
        {
            //cout<<"Doing ("<<x1<<","<<y1<<")\n";

            //int total=0;

            /*for (int i=y1; i<=y1+step_size; i++)
            {
                for (int j=x1; j<=x1+step_size; j++)
                {
                    if (op_buf.at(j,i).red==255 &&
                        op_buf.at(j,i).green==255 &&
                        op_buf.at(j,i).blue==255)
                    {
                        total++;
                    }
                }
            }

            double ratio=static_cast<double>(total)/(step_size*step_size);
            //cout<<total<<" "<<ratio<<endl;

            if (ratio<.1)
            {
                for (int i=y1; i<=y1+step_size; i++)
                {
                    for (int j=x1; j<=x1+step_size; j++)
                    {
                        dmap.at(j,i).red=255;
                        dmap.at(j,i).green==0;
                        dmap.at(j,i).blue=0;
                    }
                }

                continue;
            }*/

            std::vector<double> arr1,arr2;
            //IMAGE_X-1-x1

            cout<<find_variance(left,x1,y1,x1+step_size,y1+step_size)<<endl;
            
            if (find_variance(left,x1,y1,x1+step_size,y1+step_size)>=qualify_thresh)
        	{
         	for (int i=0; i<=x_extent; ++i)
            {
                //for (int off=-4; off<=4; ++off)
                //{
                	//cout<<"Now doing..."<<i<<endl;
                		arr1.push_back(correlate(left,right,x1,y1,x1+step_size,y1+step_size,x1+i,y1));
             	//}
            }

            /*for (int i=0; i<=arr.size()-1; i++)
            {
                cout<<arr[i]<<" ";
            }*/

            double min1=arr1[0];
            int minpos1=0;
            //double threshold=min-arr[0];

            for (unsigned int i=1; i<=arr1.size()-1; ++i)
            {
                if (arr1[i]<min1)
                {
                    min1=arr1[i];
                    minpos1=i;
                }
            }

            // Left-right consistency check
            for (int i=0; i<=x_extent; ++i)
            {
                //for (int off=-4; off<=4; ++off)
                //{
                	//cout<<"Now doing..."<<i<<endl;
                  	arr2.push_back(correlate(right,left,x1+minpos1,y1,x1+minpos1+step_size,y1+step_size,x1+minpos1-i,y1));
             	//}
            }

            double min2=arr2[0];
            int minpos2=0;
            //double threshold=min-arr[0];

            for (unsigned int i=1; i<=arr2.size()-1; ++i)
            {
                if (arr2[i]<min2)
                {
                    min2=arr2[i];
                    minpos2=i;
                }
            }
            
            //std::cout<<(minpos1-minpos2)<<endl;
            
            if (fabs(minpos1-minpos2)>left_right_tolerance)
            {
            	for (int yy=y1; yy<=y1+step_size; ++yy)
            	{
                	for (int xx=x1; xx<=x1+step_size; ++xx)
                	{
                    	dmap->at(xx,yy).red=255;
                		dmap->at(xx,yy).green=0;
                		dmap->at(xx,yy).blue=0;
            		}
            	}
             	
             	return;
            }	
			
			else
			{
            	cout<<"Minimum is at : "<<minpos1<<endl;
            	ishade=(255*(1-(1./(minpos1+1))));
			}
			}
			
			else
   			{			
   				ishade=255;
			}
			
			if (ishade>255)
			{
				std::cout<<"Overflow...\n";
    			ishade=255;
			}
		
			if (ishade<0)
			{
				ishade=0;
			}
		
			unsigned char shade=static_cast<unsigned char>(ishade);
            
            for (int yy=y1; yy<=y1+step_size; ++yy)
            {
                for (int xx=x1; xx<=x1+step_size; ++xx)
                {
                    dmap->at(xx,yy).red=
                    dmap->at(xx,yy).green=
                    dmap->at(xx,yy).blue=255-shade;
                }
            }
        }
    }
}

Comrade::Iris3D::DepthmapBuilder::~DepthmapBuilder()
{}
