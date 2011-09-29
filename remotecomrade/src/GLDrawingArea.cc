#include <iostream>
#include <cstdlib>
#include <gtkmm.h>
#include <gtkglmm.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "EvidenceGrid.hh"
#include "adam.h"
#include "GLDrawingArea.hh"
#include "model_parts.h"
#include <libglademm/xml.h>
#include "CompassDA.hh"
#include <sys/time.h>
#include "bitmap.h"
#include "view3d.hh"
#include "texture.h"


double lasttime;
//unsigned char env_map[800][600];
bool toggle = 0;
using namespace std;
//using namespace Comrade::Corelib;

// texture stuff
int		Width;
int		Height;
BITMAPINFO	*TexInfo; // Texture bitmap information
GLubyte		*TexBits; // Texture bitmap pixel bits
GLuint		textures;


// for 3d object

//unsigned char*** map3d;
object3d* obj;



// to calculate fps
inline double getclock(void)
{
	struct timeval t; /* Current time of day */
	gettimeofday(&t, NULL);
	return ((double)t.tv_sec + 0.000001 * (double)t.tv_usec);
}


GL3DScene::GL3DScene() : Gtk::GL::Widget(static_cast<Gtk::Widget&>(*this))
{
	// robot parameters
	anglex=0.0; angley = 0.0;
	orientationx=0;
	orientationy=0;
	xdist=400;
	ydist=1.6;
	zdist=300;
	
	// camera parameters
	camerax = -159;
	cameray = -120;
	cameraz = -60;
	camera_ax = 23;
	camera_ay = -76;
	
	mapgrid = NULL;
	
	
	// used for creating/testing the 3d object viewer
/*	
	int mapx=64, mapy=64, mapz=64;
	int hmap=32;
	map3d = new unsigned char**[mapx];
	for(int ci=0;ci<mapx;ci++)
		map3d[ci] = new unsigned char*[mapy];
	
	for(int ci=0; ci<mapx; ci++)
		for(int cj=0;cj<mapy;cj++)
			map3d[ci][cj] = new unsigned char[mapz];
	
	
	for(int ci=1;ci<mapx-1;ci++)
		for(int cj=1;cj<mapy-1;cj++)
			for(int ck=1;ck<mapz-1;ck++)
			{
				if(((ci-hmap)*(ci-hmap))+((cj-hmap)*(cj-hmap))+((ck-hmap)*(ck-hmap)) <= 1000)
					map3d[ci][cj][ck] = 1;
				else
					map3d[ci][cj][ck] = 0;
			}
*/	
	/*
		for(int ck=0; ck<mapz; ck++)
		{
			map3d[30][30][ck]=1;
			map3d[31][30][ck]=1;
			map3d[30][31][ck]=1;
			map3d[31][31][ck]=1;
		}
	*/
	/*
	for(int ci=0;ci<mapx;ci++)
		for(int ck=0;ck<mapz;ck++)
			for(int cj=0;cj<mapy;cj++)
			{
				map3d[ci][cj][ck]=1;
			}
	*/
	
	//obj = new object3d(64, 64, 64, map3d);
	
	// OpenGL rendering context
	Glib::RefPtr<Gdk::GL::Config> glconfig;

	// Try double-buffered visual
	glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB | Gdk::GL::MODE_DEPTH  | Gdk::GL::MODE_DOUBLE | Gdk::GL::MODE_STENCIL);
	
	if(!glconfig)
	{
		std::cerr << "\ncould not enable double buffering";
		glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB | Gdk::GL::MODE_DEPTH | Gdk::GL::MODE_STENCIL);
		if(!glconfig)
		{
			std::cerr << "\ncould not find any GL visual. don't know what to do.";
		}
	}
	
	//GLConfigUtil::examine_gl_attrib(glconfig);
	
	set_gl_capability(glconfig);
	
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glDepthFunc(GL_LEQUAL);

	
	/*
	for(int i=0; i<800; i++)
		for(int j=0;j<600;j++)
			env_map[i][j]=(float(rand())*float(255)/float(RAND_MAX));
	*/	
	/*	
	glGenTextures(1, &textures);
	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		
	TexBits = LoadDIBitmap("land.bmp", &TexInfo);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, TexInfo->bmiHeader.biWidth,
        TexInfo->bmiHeader.biHeight, 0, GL_RGB,	GL_UNSIGNED_BYTE, TexBits);
	*/
	
	textures = TextureLoad("land.bmp", 0, GL_LINEAR, GL_LINEAR, GL_REPEAT);
	std::cerr<<"\ntextures = " << textures;
	glEnable(GL_TEXTURE_2D);	
	/*
	quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	
	init_displaylist();
	render_gear(quadric, dim.gear, 0);
	render_obstacle(-0.5, 5, -0.5, 10);
	Point pos;
	render_wheel(quadric, dim, pos);
	*/
}


GL3DScene::~GL3DScene()
{	
	/*
	gluDeleteQuadric(quadric);
	cleanup_displaylist();
	*/
	// Destroy Our Cg Context And All Programs Contained Within It
	//cgDestroyContext(cgContext);
}

void GL3DScene::on_realize()
{
	// We need to call the base on_realize()
	Gtk::DrawingArea::on_realize();
	
	//
	// Get GL::Window.
	//
	
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
	
	//
	// GL calls.
	//
	
	// *** OpenGL BEGIN ***
	if (!glwindow->gl_begin(get_gl_context()))
		return;
	
	glLoadIdentity();
	int w = get_width(), h = get_height();
	if(get_width()>get_height())
		glViewport(0,0,h,h);
	else
		glViewport(0,0,w,w);

	gluPerspective(40.0f, 1.0f, 1.0f, 800.0f);
	
	glwindow->gl_end();
	// *** OpenGL END ***
	add_events(Gdk::ALL_EVENTS_MASK);

	std::cerr << "\nfinished gl onrealize";
}

bool GL3DScene::on_configure_event(GdkEventConfigure* event)
{
	//
	// Get GL::Window.
	//
	//std::cerr<<"\nonconfigure";
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
	
	//
	// GL calls.
	//
	
	// *** OpenGL BEGIN ***
	if (!glwindow->gl_begin(get_gl_context()))
	return false;
	int h=get_height();
	int w=get_width();
	if(w>h)
		glViewport(0,0,h,h);
	else
		glViewport(0,0,w,w);
	
	glwindow->gl_end();
	// *** OpenGL END ***
	std::cerr<<"\nfinished onconfigure";
	return true;
}

bool GL3DScene::on_expose_event(GdkEventExpose* event)
{
	//
	// Get GL::Window.
	//
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
	
	//
	// GL calls.
	//
	
	// *** OpenGL BEGIN ***
	if (!glwindow->gl_begin(get_gl_context()))
		return false;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	// put drawing stuff here
	draw();
	  
	// Swap buffers.
	if (glwindow->is_double_buffered())
  		glwindow->swap_buffers();
	else
  		glFlush();

	glwindow->gl_end();
	// *** OpenGL END ***


	return true;
}

bool GL3DScene::on_button_press_event(GdkEventButton *event)
{
	//cout << "Button press!\n";
	//cout << " Button: " << event->button << "\n x     : " << event->x << "\n y     : "<<event->y << "\n\n";
	//double speed=(oldheight/2)-event->y;
	if (event->button==1 || event->button==2 || event->button==3)
	{
		buttonpressed=event->button;
		buttonstartx=event->x;
		buttonstarty=event->y;
	}
	else
	{
		buttonpressed=false;
	}
	return true;
}

bool GL3DScene::on_button_release_event(GdkEventButton *event)
{
	//cout << "Button release!\n";
	buttonpressed=0;
	//stop();
	return true;
}

bool GL3DScene::on_motion_notify_event (GdkEventMotion *event)
{
	//cout << "Motion notify!\n";
	static int abs_x = 0;
	static int abs_y = 0;
	if (buttonpressed && event && event->window)
	{
		const Glib::RefPtr<Gdk::Window> refWindow =
			Glib::wrap((GdkWindowObject*) event->window,true);
		if (refWindow)
		{
			int x=0,y=0;
			Gdk::ModifierType state = Gdk::ModifierType(0);
			refWindow->get_pointer(x,y,state);
			//cout << buttonstartx-x << " " << buttonstarty-y << endl; 
			int relx=x-buttonstartx;
			int rely=y-buttonstarty;
			buttonstartx=x;
			buttonstarty=y;
			abs_x += relx;
			abs_y += rely;
			if (buttonpressed==1)
			{
				/* for 1st person view */
				
				camera_ax+=rely;
				camera_ay+=relx;
			}
			else if (buttonpressed==2)
			{
				//float xz_projection = float(rely) * cos(camera_ax*pbo);
				/* for 1st person view */
				
				cameraz -= rely*cos(camera_ay*pbo)*cos(camera_ax*pbo);
				camerax += rely*sin(camera_ay*pbo)*cos(camera_ax*pbo);
				cameray -= rely*sin(camera_ax*pbo);
				
				/*
				camerax-=sin(camera_ay*pbo)*rely*cos(camera_ax*pbo);
				cameraz+=cos(camera_ay*pbo)*rely*cos(camera_ax*pbo);
				cameray-=rely*sin(camera_ax*pbo);
				*/
			}
			else if (buttonpressed==3)
			{
				//toggle = !toggle;
				/* for 1st person view */
				
				camerax -= relx*cos(camera_ay*pbo);//*cos(camera_ax*pbo);//sin(camera_ay*pbo)*relx*sin(camera_ax*pbo);
				cameraz -= relx*sin(camera_ay*pbo);//*cos(camera_ax*pbo);//sin(camera_ay)*relx*sin(camera_ax);
				cameray += rely*cos(camera_ax*pbo);
				
				
			}
			/*
			std::cerr<<"\ncamera_ax = "<<camera_ax;
			std::cerr<<"\ncamera_ay = "<<camera_ay;
			std::cerr<<"\ncamerax = "<<camerax;
			std::cerr<<"\ncameray = "<<cameray;
			std::cerr<<"\ncameraz = "<<cameraz;
			*/
			//std::cerr<<"\ncamera_ay = " << camera_ay;
			update(xdist, zdist, angley);
		}
	}	
	return true;
}

void GL3DScene::update(double x, double y, double robot_orientation)
{
	xdist = x;
	zdist = y;
	angley=robot_orientation;
	Glib::RefPtr<Gdk::Window> window = get_window();
	window->invalidate_rect(Gdk::Rectangle(0,0,get_width(),get_height()),false);
	/*
	int h=get_height();
	int w=get_width();
	if(w>h)
		glViewport(0,0,h,h);
	else
		glViewport(0,0,w,w);
	*/
}

void GL3DScene::set_evidence_grid(Comrade::Corelib::EvidenceGrid<unsigned char> *grid)
{
	mapgrid=grid;
	map_x=grid->return_x();
	map_y=grid->return_y();
}

void GL3DScene::redraw(void)
{
	draw();
	compassview::get_singleton().draw();
}

void GL3DScene::draw(void)
{
//	static int i = 0;
	static int    fps_count = 0;  /* Frames per second count */
	static double fps_time = 0.0; /* Frames per second time */
	float difference;
	double curtime = getclock();
	difference = curtime-lasttime;
	lasttime=curtime;
/*	
	int width=gimp_image.width;
	int height=gimp_image.height;
	map_x=width;
	map_y=height;
*/	
	//std::cerr << "\n drawing";
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
	if (!glwindow->gl_begin(get_gl_context()))
    	return;
	
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	//glEnable(GL_LIGHTING);
	
	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity();
	
	glPushMatrix();
	
		//map[330][250] = 255;

		if(!toggle)
		{
			//this is proper
			glRotatef(camera_ay, 0.0f, 1.0f, 0.0f);
			glRotatef(camera_ax, cos(camera_ay*pbo), 0.0f, sin(camera_ay*pbo));
			glTranslatef(camerax, cameray, cameraz);//w()/2, h()/2, 0.0f);
			glTranslatef(0, 0, cameraz);
		}
		else
		{
			///*
			//not this
			glTranslatef(camerax, cameray, cameraz);//w()/2, h()/2, 0.0f);
			glRotatef(camera_ay, 0.0f, 1.0f, 0.0f);
			glRotatef(camera_ax, cos(camera_ay*pbo), 0.0f, sin(camera_ay*pbo));
			//*/
			//glTranslatef(camerax, cameray, -100);//w()/2, h()/2, 0.0f);
		}
		//float fx, fy;
		//map[320][240]=0;
		//std::cerr<<"\nchecking map";
		
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		/*
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures);
		*/
		
		if(0)//mapgrid)
		{
			unsigned char val1, val2, val;
			
			for(int x=0; x<map_x; x++) // width -> map_x
			{
				//glBegin(GL_TRIANGLE_STRIP);
				for(int y=0; y<map_y; y++)	//height -> map_y
				{
					
					//val1=env_map[x][y-1];
					//val2=env_map[x-1][y-1];
					
					//val1=gimp_image.pixel_data[(x*width*3)+((y-1)*3)];
					//val2=gimp_image.pixel_data[((x-1)*width*3)+(y-1)*3];
					if((val=mapgrid->get_probability(0,y,x)) > 130)
					{
						glColor3ub(val, 255-val, 0);
						
						glPushMatrix();
							draw_obstacle((y-(map_y/2)), 5.0f, ((map_x/2)-x), 20.0f);
						glPopMatrix();
						
						/*
						glColor4ub(val1, 255-val1, 0, val1);
						glVertex3f((x)-(map_x/2), val1, (y-1)-(map_y/2));
						glColor4ub(val2, 255-val2, 0, val2);
						glVertex3f((x-1)-(map_x/2), val2, (y-1)-(map_y/2));
						*/
						/*
						glTexCoord2f(x-1, y-1);
						glVertex3f((x-1)-(map_x/2), val2, (y-1)-(map_y/2));
						glTexCoord2f(x, y-1);
						glVertex3f((x)-(map_x/2), val1, (y-1)-(map_y/2));
						glTexCoord2f(x, y);
						glVertex3f((x)-(map_x/2), val1, (y)-(map_y/2));
						glTexCoord2f(x-1, y);
						glVertex3f((x-1)-(map_x/2), val2, (y)-(map_y/2));
						*/
						
						/*
						glPushMatrix();
							glTranslatef((y-(map_y/2)), 5.0f, ((map_x/2)-x));
							draw_rendered_obstacle();
						glPopMatrix();
						*/
						//std::cerr << "\n"<<x<<","<<y;
					}
				}
				//glEnd();
			}
			
		}
		//glDisable(GL_TEXTURE_2D);
		glFrontFace(GL_CCW);
		glDisable(GL_CULL_FACE);
		
		
		//obj->draw();
		//obj->draw_smooth(camera_ay, 0);//camera_ax);
		
		
		/*
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glColor4ub(10, 245, 0, 10);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		draw_obstacle(8,5,-8,10);
		glFrontFace(GL_CCW);
		glDisable(GL_CULL_FACE);
		//glDisable(GL_BLEND);
		*/
		//std::cerr<<"\ndrawing grid";
		//glLineWidth(0.1);
		
		
		glEnable(GL_CULL_FACE);
		glPushMatrix();
		
			
			//glTranslatef(-6.0f, 0.0f, 6.0f);
			
			
			
			//if(camera_ax<50)
			{
			//glColor3f(0.2f, 0.2f, 0.2f);
			glColor3ub(22,110,30);
			glBegin(GL_LINES);
			
				for(int i=-map_x/2; i < map_x/2; i+=10)
				{
					glVertex3f(i, 0.0f, map_y/2);
					glVertex3f(i, 0.0f, -map_y/2);
				}
				for(int i=-map_y/2; i<map_y/2; i+=10)
				{
					glVertex3f(-map_x/2, 0.0f, i);
					glVertex3f(map_x/2, 0.0f, i);
				}
			
			glEnd();
			}
			
			glBegin(GL_TRIANGLE_FAN);
			{
				glColor3f(0.2f, 1.0f, 0.2f);
				
				glVertex3f(-map_x/2, 0, map_y/2);
				glVertex3f(map_x/2, 0, map_y/2);
				glVertex3f(map_x/2, 0, -map_y/2);
				glVertex3f(-map_x/2, 0, -map_y/2);
			}
			glEnd();
		glPopMatrix();
		glDisable(GL_CULL_FACE);
		
		
		/*
		glEnable(GL_TEXTURE_2D);
//		glBindTexture(GL_TEXTURE_2D, textures);
		glFrontFace(GL_CW);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(-25, 0, -25);
			glTexCoord2f(50, 0);
			glVertex3f(25, 0, -25);
			glTexCoord2f(50, 50);
			glVertex3f(25, 0, 25);
			glTexCoord2f(0, 50);
			glVertex3f(-25, 0, 25);
		glEnd();
		*/
			
		//glDisable(GL_TEXTURE_2D);
		
		
		std::cerr<<"\ndrawing adam";
		
		glPushMatrix();
		
			glTranslatef(xdist-400, 2*ydist, zdist-300);
			//glRotatef(angley, 0.0f, -1.0f, 0.0f);
			//glRotatef(anglex, cos(angley*pbo), 0.0f, sin(angley*pbo));

			adam.draw();
			glBegin(GL_POINTS);
			
				glColor3f(1.0f, 1.0f, 1.0f);
				glPointSize(5.0f);
				glVertex3f(0.0f, 10.0f, 0.f);
			
			glEnd();
		
		glPopMatrix();	
		
		
		
		
		
		
	glPopMatrix();
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	if (glwindow->is_double_buffered())
	glwindow->swap_buffers();
		else
	glFlush();
	glwindow->gl_end();
	
	/* Update the frames-per-second value */
	fps_time += difference;
	fps_count ++;
	if (fps_count >= 20)
	{
		std::cerr<<"\nFrames per second = " << float(fps_count)/fps_time;
		fps_time  = 0.0;
		fps_count = 0;
    	}
}
