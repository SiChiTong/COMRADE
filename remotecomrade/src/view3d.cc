#include "view3d.hh"
#include "model_parts.h"
#include "gui_shared_data.h"
#include <iostream>
#include <cmath>

object3d::object3d(unsigned short int x_res, unsigned short int y_res, unsigned short int z_res, unsigned char*** map)
{
	x=x_res;
	y=y_res;
	z=z_res;
	map3d=map;
	
	tx = sqrt((x*x) + (z*z));
	ty=tz=tx;
	
	e = a = 0;
	
	t3d = new unsigned char**[tx];
	for(int ci=0;ci<tx;ci++)
		t3d[ci] = new unsigned char*[ty];
	
	for(int ci=0; ci<tx; ci++)
		for(int cj=0;cj<ty;cj++)
			t3d[ci][cj] = new unsigned char[tz];
	
	map2d_front = new unsigned char*[x];
	for(int i=0; i<x; i++)
		map2d_front[i] = new unsigned char[y];
	
	// create the 2d projection array
	map2d = new unsigned char*[tx];
	for(int i=0; i<tx; i++)
		map2d[i] = new unsigned char[ty];
	
	
	for(int i=0; i<x; i++)
	{
		for(int j=0; j<y; j++)
		{
			for(int k=z-1; k>0; k--)
			{
				//t3d[i+(tx/2)-(x/2)][j+(ty/2)-(y/2)][k+(tz/2)-(z/2)] = map3d[i][j][k];
				if(map3d[i][j][k] > 0)
				{
					map2d_front[i][j] = k;
					break;
				}
			}
		}
	}
	
	for(int i=1; i<x-1; i++)
	{
		for(int j=1; j<y-1; j++)
		{
			for(int k=1; k<z-1; k++)
			{
				if(!surrounded(i, j, k))
					t3d[(tx/2)-(x/2)+i][(ty/2)-(y/2)+j][(tz/2)-(z/2)+k]=map3d[i][j][k];
				else
				{
					t3d[(tx/2)-(x/2)+i][(ty/2)-(y/2)+j][(tz/2)-(z/2)+k]=0;
					//map3d[i][j][k] = 0;
				}
			}
		}
	}
	
	
	for(int i=0; i<tx; i++)
	{
		for(int j=0; j<ty; j++)
		{
			for(int k=tz-1; k>0; k--)
			{
				if(t3d[i][j][k] > 0)
				{
					map2d[i][j] = k;
					break;
				}
			}
		}
	}
	//std::cerr<<"\ninitialized 3d object\n";
}

object3d::~object3d()
{
	// delete map3d and map2d
	for(int i=0; i<tx; i++)
		delete[] map2d_front[i];
	delete[] map2d_front;
	
	for(int i=0; i<tx; i++)
		delete[] map2d[i];
	delete[] map2d_front;
	
	for(int i=0; i<x; i++)
		for(int j=0; j<y; j++)
			delete[] map3d[i][j];
	for(int i=0; i<x; i++)
		delete[] map3d[i];
	delete[] map3d;
	
	for(int i=0; i<tx; i++)
		for(int j=0; j<ty; j++)
			delete[] t3d[i][j];
	for(int i=0; i<tx; i++)
		delete[] t3d[i];
	delete[] t3d;
}

void object3d::draw(void)
{
	Point p(1,1,1);
	glPushMatrix();
	for(int i=1; i<(tx-1); i++)
	{
		for(int j=1; j<(ty-1); j++)
		{
			for(int k=1; k<(tz-1); k++)
			{
				if(t3d[i][j][k] > 0)
				{
					//if(!surrounded(i, j, k))
					{
						glPushMatrix();
						{
							glTranslatef(i-(tx/2), (ty/2)-j, k-(tz/2));
							glColor3f(0.0f, 0.0f, 0.0f);
							draw_wirecube(p);
							glColor3f(1.0f, 1.0f, 1.0f);
							drawcube(p);
						}
						glPopMatrix();
					}
				}
			}
		}
	}
	glPopMatrix();
}


void object3d::draw_smooth(float azimuth, float elevation)
{
	/*
	if((a == azimuth) && (e == elevation))
	{
		draw_smooth();
		return;
	}
	*/
	a = azimuth;
	for(int i=0; i<tx; i++)
	{
		for(int j=0; j<ty; j++)
			for(int k=0; k<tz; k++)
				t3d[i][j][k] = 0;
	}
	
	//recalculate t3d;
	int ntx, ntz;
	float ta;
	
	for(int i=0; i<x; i++)
	{
		for(int k=0; k<z; k++)
		{
			float ca = cos(a*pbo);
			float sa = sin(a*pbo);
			ntx=int((tx/2)-(((x/2)-i)*ca));
			ntz=int((tz/2)-(((z/2)-k)*sa));
			for(int j=0; j<y; j++)
			{
				/*
				std::cout << "\nx=" << ntx;
				std::cout << "\ny=" << int((ty/2)-((y/2)-j));
				std::cout << "\nz=" << ntz;
				*/
				if(map3d[i][j][k] != 0)
					t3d[ntx][int((ty/2)-((y/2)-j))][ntz]=map3d[i][j][k];
			}
		}
	}
	
	
	
	for(int i=0; i<x; i++)
	{
		for(int k=0; k<z; k++)
		{
			if(i)
				ta=atanf(z/x)/pbo;
			else
				ta=90;
			ta+=a;
			ntx = (i-(x/2))*cos(ta*pbo);
			ntz = (k-(z/2))*sin(ta*pbo);
			
			//ntx=x+(((x/2)-i)*cos(a*pbo));
			//ntz=z+(((z/2)-i)*sin(a*pbo));
			for(int j=0; j<y; j++)
				t3d[ntx+(x/2)][j][ntz+(z/2)] = map3d[i][j][k];
			
		}
	}
	
	
	
	// recalculate map2d;
	
	for(int i=0; i<tx; i++)
	{
		for(int j=0; j<ty; j++)
			map2d[i][j] = 0;
	}
	
	
	for(int i=0; i<tx; i++)
	{
		for(int j=0; j<ty; j++)
		{
			for(int k=tz-1; k>0; k--)
			{
				if(t3d[i][j][k] > 0)
				{
					map2d[i][j] = k;
					break;
				}
			}
		}
	}
	
	draw_smooth();
	
	
	
	Point p(1,1,1);
	glPushMatrix();
	for(int i=1; i<(tx-1); i++)
	{
		for(int j=1; j<(ty-1); j++)
		{
			for(int k=1; k<(tz-1); k++)
			{
				if(t3d[i][j][k] > 0)
				{
					if(!tsurrounded(i, j, k))
					{
						glPushMatrix();
						{
							glTranslatef(i-8, 8-j, k-8);
							glColor3f(0.0f, 0.0f, 0.0f);
							draw_wirecube(p);
							glColor3f(1.0f, 1.0f, 1.0f);
							drawcube(p);
						}
						glPopMatrix();
					}
				}
			}
		}
	}
	glPopMatrix();
}


void object3d::draw_smooth(void)
{
	unsigned char val[4], val1, val2;
	unsigned diff[5];
	const unsigned char diff_thr = 4;
	for(int i=1; i<x; i++)
	{
		glColor3ub(255,255,255);
		//glBegin(GL_TRIANGLES);
		glColor3f(1,0,0);
		for(int j=1; j<y; j++)
		{
			val[0]=map2d[i-1][j-1];
			val[1]=map2d[i-1][j];
			val[2]=map2d[i][j];
			val[3]=map2d[i][j-1];
			
			diff[0]=abs(val[0]-val[1]);
			diff[1]=abs(val[1]-val[2]);
			diff[2]=abs(val[2]-val[0]);
			diff[3]=abs(val[2]-val[3]);
			diff[4]=abs(val[3]-val[0]);
			
			
			glBegin(GL_TRIANGLES);
			if((!((val[0] == 0) || (val[1] == 0) || (val[2] == 0))) && ((diff[0]<diff_thr) && (diff[1]<diff_thr) && (diff[2]<diff_thr)))
			{
				
				glVertex3f((i-1)-(x/2), (j-1)-(y/2), val[0]);
				glColor3f(0,1,0);
				glVertex3f((i-1)-(x/2), (j)-(y/2), val[1]);
				glColor3f(0,0,1);
				glVertex3f((i)-(x/2), (j)-(y/2), val[2]);
				glColor3f(1,0,0);
				glVertex3f((i-1)-(x/2), (j-1)-(y/2), val[0]);
			}
			glEnd();
			
			glBegin(GL_TRIANGLES);
			if((!((val[0] == 0) || (val[2] == 0) || (val[4] == 0))) && ((diff[2]<diff_thr) && (diff[3]<diff_thr) && (diff[4]<diff_thr)))
			{
				glVertex3f((i-1)-(x/2), (j-1)-(y/2), val[0]);
				glColor3f(0,0,1);
				glVertex3f((i)-(x/2), (j)-(y/2), val[2]);
				glColor3f(0,0,0);
				glVertex3f((i)-(x/2), (j-1)-(y/2), val[3]);
				glColor3f(1,0,0);
				glVertex3f((i-1)-(x/2), (j-1)-(y/2), val[0]);
			}
			glEnd();
			
		}
		//glEnd();
	}
	
	for(int i=1; i<x; i++)
	{
		glBegin(GL_LINE_STRIP);
		glColor3f(0,0,0);
		for(int j=1; j<y; j++)
		{
			val[0]=map2d[i-1][j-1];
			val[1]=map2d[i-1][j];
			val[2]=map2d[i][j];
			if(!((val[0]==0) || (val[1]==0) || (val[2]==0)))//(map2d_front[i-1][j-1] > 0) && (map2d_front[i-1][j] > 0))
			{
				glVertex3f((i-1)-(x/2), (j-1)-(y/2), val[0]);
				glVertex3f((i-1)-(x/2), (j)-(y/2), val[1]);
				glVertex3f((i)-(x/2), (j)-(y/2), val[2]);
				glVertex3f((i-1)-(x/2), (j-1)-(y/2), val[0]);
			}
		}
		glEnd();
	}
}

bool object3d::surrounded(unsigned short int xpos, unsigned short int ypos, unsigned short int zpos)
{
	for(int i=xpos-1; i<=xpos+1; i++)
	{
		for(int j=ypos-1; j<=ypos+1; j++)
		{
			for(int k=zpos-1; k<=zpos+1; k++)
			{
				if(map3d[i][j][k] == 0)
					return(0);
			}
		}
	}
	return(1);
}

bool object3d::tsurrounded(unsigned short int xpos, unsigned short int ypos, unsigned short int zpos)
{
	for(int i=xpos-1; i<=xpos+1; i++)
	{
		for(int j=ypos-1; j<=ypos+1; j++)
		{
			for(int k=zpos-1; k<=zpos+1; k++)
			{
				if(t3d[i][j][k] == 0)
					return(0);
			}
		}
	}
	return(1);
}
