#include "OpticalFlowEngine.h"

Comrade::Iris3D::OpticalFlowEngine::OpticalFlowEngine()
{
	std::cout<<"OpticalFlowEngine object created...\n";
}
		
void Comrade::Iris3D::OpticalFlowEngine::estimate_optical_flow
(Comrade::IrisFoundation::Buffer<Comrade::IrisFoundation::RGB>* one,
 Comrade::IrisFoundation::Buffer<Comrade::IrisFoundation::RGB>* two,
 Comrade::IrisFoundation::Buffer<Comrade::IrisFoundation::RGB>* flow,
 int boxsize,int extent)
{
	RGB black={0,0,0};
	RGB white={255,255,255};

	std::cout<<"Clearing optical flow buffer...\n";

	for (int yy=0; yy<=flow->maxy-1; ++yy)
	{
		for (int xx=0; xx<=flow->maxx-1; ++xx)
		{
			flow->at(xx,yy)=black;
		}
	}

	double** var_array;
	var_array=new double*[2*extent+1];
	
	for (int i=0; i<=2*extent; ++i)
	{
		var_array[i]=new double[2*extent+1];
	}

	//double score=0;

	for (int yy=0; yy<=one->maxy-1; yy+=boxsize)
	{
		for (int xx=0; xx<=one->maxx-1; xx+=boxsize)
		{
			//std::cout<<"Center=("<<xx<<","<<yy<<")\n";
			
			int cx=xx+boxsize/2;
			int cy=yy+boxsize/2;
			
			for (int ay=cy-extent; ay<=cy+extent; ++ay)
			{
				for (int ax=cx-extent; ax<=cx+extent; ++ax)
				{
    				//std::cout<<"Here too...("<<ax<<","<<ay<<")\n";
                    var_array[ay+extent-cy][ax+extent-cx]=
                    Comrade::Iris3D::
                    correlate(one,two,xx,yy,xx+boxsize,yy+boxsize,ax,ay);
				}
			}
	
			//std::cout<<"Here...\n";
            int srow=0,scol=0;
			double smallest=var_array[srow][scol];

			for (int fyy=0; fyy<=2*extent; ++fyy)
			{
				for (int fxx=0; fxx<=2*extent; ++fxx)
				{
					if (var_array[fyy][fxx]<=smallest)
					{
						smallest=var_array[fyy][fxx];
						srow=fyy;
						scol=fxx;
					}
				}
			}

			srow-=extent;
			scol-=extent;

			//int movy=cy+srow;
			int movx=cx+scol;

			std::cout<<"Moved by ("<<scol<<","<<srow<<") units..."<<"\n";
			int chgx=((scol>=0)?1:-1);
			//int chgy=((srow>=0)?1:-1);

			if (scol==0 && srow==0)
			{
				//cout<<"Haha\n";
				flow->at(cx,cy)=white;
				continue;
			}
			
			double dy=cy;

			for (int dx=cx; dx<=movx; dx+=chgx)
			{
				//for (int dy=yy; dy<=movy; dy+=chgy)
				//{
					//std::cout<<"Am I stuck ? ";
					flow->at(dx,static_cast<int>(dy))=white;
					dy+=static_cast<double>(srow)/scol;
				//}
				//flow->at(movx,movy)=white;
			}
		}
	}

	for (int i=0; i<=2*extent; ++i)
	{
		delete[] var_array[i];
	}

	delete[] var_array;
	
	std::cout<<"Done processing...\n";
}

Comrade::Iris3D::OpticalFlowEngine::~OpticalFlowEngine()
{}


