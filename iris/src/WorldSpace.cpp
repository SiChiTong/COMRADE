#include "WorldSpace.h"

Comrade::Iris3D::WorldSpace::WorldSpace(int z,int y,int x)
{
	cout<<"Creating...";
	allocate(z,y,x);
}

void Comrade::Iris3D::WorldSpace::allocate(int z,int y,int x)
{
	zsz=z;
	ysz=y;
	xsz=x;
	
 	world=new Voxel**[zsz];
 	
  	for (int zz=0; zz<=zsz-1; ++zz)
  	{
  		world[zz]=new Voxel*[ysz];
  		
  		for (int yy=0; yy<=ysz-1; ++yy)
  		{
  			world[zz][yy]=new Voxel[xsz];
		}
	}
}

void Comrade::Iris3D::WorldSpace::test()
{
	std::cout<<xsz<<" "<<ysz<<" "<<zsz<<endl;
	
 	for (int zz=0; zz<=zsz-1; ++zz)
	{
 		for (int yy=0; yy<=ysz-1; ++yy)
		{
			for (int xx=0; xx<=xsz-1; ++xx)
			{
				//std::cout<<xx;
    			world[zz][yy][xx].x=20;
				std::cout<<world[zz][yy][xx].x;
			}
		}
	}
}

void Comrade::Iris3D::WorldSpace::deallocate()
{
	for (int zz=0; zz<=zsz-1; ++zz)
	{
 		for (int yy=0; yy<=ysz-1; ++yy)
		{
			delete[] world[zz][yy];
		}
	}
	
	for (int zz=0; zz<=zsz-1; ++zz)
	{
		delete[] world[zz];
	}
	
	delete[] world;
}
			
void Comrade::Iris3D::WorldSpace::fill_coordinates(int rcz,int rcy,int rcx)
{
    int startz=-rcz*VOXEL_Z;

    RGB rgbwhite={255,255,255};
    HSLstruct hslwhite=ColorSpaceConvertor::uRGB_to_HSL(rgbwhite);
                
    for (int z=0; z<=WORLD_Z-1; z++)
    {
        int starty=-rcy*VOXEL_Y;

        for (int y=0; y<=WORLD_Y-1; y++)
        {
            int startx=-rcx*VOXEL_X;

            for (int x=0; x<=WORLD_X-1; x++)
            {
                world[z][y][x].x=startx;
                world[z][y][x].y=starty;
                world[z][y][x].z=startz;
                world[z][y][x].carved=false;

                world[z][y][x].color=hslwhite;

                //cout<<"Coordinates\("<<startz<<" "<<starty<<" "<<startx<<")\n";

                startx+=VOXEL_X;
            }

            starty+=VOXEL_Y;
        }

        startz+=VOXEL_Z;
    }
}

Comrade::Iris3D::WorldSpace::~WorldSpace()
{
	deallocate();
}

