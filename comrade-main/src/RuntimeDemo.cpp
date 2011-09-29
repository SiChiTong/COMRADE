// COMRADE Preproduction Core - Hades Release
// CPC Hades Runtime Demo
// COMRADE Preproduction Core (c) 2004 Avishek Sen Gupta

/*
The current release of the COMRADE Preproduction Core
can use the FLTK 1.1 library if it has been installed. By
default, however, simple console mode is used. To use
FLTK 1.1, define the symbol FLTK1_1_SUPPORTED in
the SystemDefs.h file.
Also note that for the Windows version (Dev-C++ or
MinGW Developer Studio), the following libraries must be
linked:

1) libfltk.a
2) libole32.a
3) libgdi32.a
4) libuuid.a
5) libwsock32.a
*/

/* Always include glibmm/gtkmm header files before all the IRIS includes.
   We spent a night debugging it, without any result! */

#include "../config.h"

#ifdef WITH_GUI
#include "LocalGUI.h"
#endif

#include <iostream>
#include "FrontEnd.h"
#include "CommandFunctionList.h"
#include "IrisEnv.h"
#include "WorldSpace.h"
#include "SOM_Trainer.h"
#include "OpticalFlowEngine.h"
#include "MulticastSignalBuffer.h"
#include "ByteObjectConvertor.h"
#include "ExecutionEngine.h"

#include "CorelibObject.hh"

#define VIRTUAL_MACHINE_DEMO
//#define SIGNAL_DEMO
//#define HOUGH_DEMO
//#define STEREO_DEMO
//#define SPACE_CARVE_DEMO
//#define QT_SEG_STEREO_DEMO
//#define HOUGH_SHAPE_DEMO
//#define EDGE_DETECTOR_DEMO
//#define OPTICAL_FLOW_DEMO
//#define DIFFERENCE_DEMO
//#define CONVOLUTION_DEMO
//#define COLOR_FILTER_DEMO
//#define FLOW_FIELD_NAV_DEMO

#ifdef FLTK1_1_SUPPORTED
#include <FL/Fl.h>
#include <FL/Fl_Window.h>
#include <FL/Fl_Text_Buffer.h>
#include <FL/Fl_Text_Display.h>
#include <FL/Fl_Multiline_Input.h>
#include <FL/Fl_Button.h>
#include <FL/Fl_File_Chooser.h>
#include <FL/fl_draw.h>
#endif

extern Comrade::ComradeRuntime::ComradeFrontEnd* comrade_cli;
extern Comrade::IO::IO_Redirector stream;
//extern Comrade::IrisFoundation::Buffer<Comrade::IrisFoundation::RGB>* bufptr;
extern bool complete;
extern long carved;

using namespace std;
using namespace Comrade::IrisFoundation;
using namespace Comrade::IrisXT;
using namespace Comrade::Iris3D;
using namespace Comrade::IO;
using namespace Comrade::ComradeRuntime;
using namespace Comrade::Osiris;

#ifdef VIRTUAL_MACHINE_DEMO

#ifdef FLTK1_1_SUPPORTED
Fl_Text_Buffer* buf;
Fl_Text_Buffer* config_buf;
Fl_Text_Buffer* prog_buf;

void xcallback(Fl_Widget* wig);
void load_cb(Fl_Widget* wig);
void load_code(Fl_Widget* wig);
void verify_cb(Fl_Widget* wig);
void unload_cb(Fl_Widget* wig);
void auto_cb(Fl_Widget* wig);
#endif

int main(int argc, char **argv)
{
//    cout<<"Ready...\n";
    //cin.get();
    
	stream.send_to_fltk_text_display=true;
    stream.send_to_stdout=true;
    
#ifdef WITH_GUI
	Gtk::Main kit(argc,argv);
	Comrade::LocalGUI::LocalGUI localgui;
#endif
	
#ifdef FLTK1_1_SUPPORTED
	buf=new Fl_Text_Buffer;
    config_buf=new Fl_Text_Buffer;
    prog_buf=new Fl_Text_Buffer;
    stream.op_handle=buf;
    
    Fl_Window *window = new Fl_Window(800,480,"COMRADE Core v0.1 (Valkyrie Build)");
    Fl_Text_Display* tex=new Fl_Text_Display(10,20,410,200,"Console Output\n");
    Fl_Text_Display* prog_wnd=new Fl_Text_Display(10,290,410,180,"Current loaded code");
    Fl_Input* ip_box=new Fl_Input(90,225,330,24,"Command");
    Fl_Text_Display* conf_wnd=new Fl_Text_Display(430,20,200,200,"Current Configuration\n");
    Fl_Button* config_btn=new Fl_Button(430,225,200,24,"Load Configuration...");
    Fl_Button* prog_btn=new Fl_Button(430,290,200,24,"Load Directive Code...");
    Fl_Button* verify_btn=new Fl_Button(430,320,200,24,"Load image...");
	Fl_Button* unload_btn=new Fl_Button(430,350,200,24,"Unload Directive Code");
    Fl_Button* auto_btn=new Fl_Button(430,380,200,24,"Go Autonomous");
    
    ip_box->when(FL_WHEN_ENTER_KEY|FL_WHEN_NOT_CHANGED);
    ip_box->callback(xcallback);
    config_btn->callback(load_cb);
    prog_btn->callback(load_code);
	verify_btn->callback(verify_cb);
	unload_btn->callback(unload_cb);
	auto_btn->callback(auto_cb);

	conf_wnd->textsize(12);
    conf_wnd->buffer(config_buf);
    tex->textsize(12);
    tex->buffer(buf);
    prog_wnd->textsize(12);
    prog_wnd->buffer(prog_buf);
#endif
    
    Comrade::Corelib::CorelibObject corelibobject;
    IrisEnvironment iris;
    AbstractHardware hw;
    comrade_cli=new ComradeFrontEnd();
comrade_cli->assign_iris_vm(&iris);
    comrade_cli->assign_hw_vm(&hw);

	ExecutionEngine *engine=new ExecutionEngine(comrade_cli);
	
	// Startup code begins
    add_fns();
	//engine.comrade_vm=comrade_cli;
	comrade_cli->show_copyright();
    engine->show_copyright();
	// Startup code ends
	engine->activate();
#ifdef WITH_GUI
	kit.run(localgui.get_mainwindow());
#endif


#ifdef FLTK1_1_SUPPORTED
	window->end();
    window->show();
    
    Fl::visual(FL_RGB);
    return Fl::run();
#endif

#ifndef WITH_GUI
#ifndef FLTK1_1_SUPPORTED
	//comrade_cli->activate();
	Glib::RefPtr<Glib::MainLoop> mainloop=Glib::MainLoop::create();
	mainloop->run();
	return 0;
#endif
#endif
	delete engine;
}

#ifdef FLTK1_1_SUPPORTED
void xcallback(Fl_Widget* wig)
{
    Fl_Input* ip=(Fl_Input*)wig;
    buf->append("\n");
    buf->append(">>> ");
    buf->append(ip->value());
    buf->append("\n");
    
    comrade_cli->interpret(ip->value());
    ip->value("");
}

void load_code(Fl_Widget* wig)
{
    char* fname=fl_file_chooser("Load configuration file","*.txt","",0);
    
    if (fname!=NULL)
    {
        buf->append("Loading ");
        buf->append(fname);
        buf->append("\n");
    }
    
    // Some parsing happens here...

	prog_buf->text("");
	prog_buf->appendfile(fname);
}

void load_cb(Fl_Widget* wig)
{
    char* fname=fl_file_chooser("Load configuration file","*","",0);
    
    if (fname!=NULL)
    {
        buf->append("Loading ");
        buf->append(fname);
        buf->append("\n");
    }

    buf->append("Done...\n");
}

void verify_cb(Fl_Widget* wig)
{
	char* fname=fl_file_chooser("Load image file","*.bmp","",0);
    	
	BitmapStreamer left;
   	left.byte_reader.reader.open(fname,ios_base::in|ios_base::out|ios::binary);
   	left.byte_reader.writer.open(fname,ios_base::in|ios_base::out|ios::binary);
	
	if (left.byte_reader.reader.is_open()==false)
   	{
        	buf->append("Files could not be opened...\n");
	}
	
	else
	{
		buf->append("File could be opened...\n");
	}
	
	Buffer<RGB> buf1(&left);
	RGB_BufferManager man1;
	man1.assign_buffer(&buf1);
	man1.copy_from_image(&left);
	
	buf->append("File was closed...\n");
}

void unload_cb(Fl_Widget* wig)
{
	prog_buf->text("");
	buf->append("Cleared code buffer.\n");
}

void auto_cb(Fl_Widget* wig)
{
    buf->append("Going autonomous...will be overriden by the STOP signal.\n");
}
#endif
#endif

#ifdef HOUGH_DEMO
int main()
{
   cout<<"Starting now...";
   StraightLineDetector detor;
   ColorFilter cfilter;
   long int** arr;
   
   RGB somecolor={45,45,45};
	   
   cfilter.filter_color=somecolor;
   cfilter.red_tol=cfilter.green_tol=cfilter.blue_tol=0;

   EdgeDetectorManager eddie;
   BitmapStreamer left,right;
   
   RGB white={255,255,255};
   RGB black={0,0,0};
   
   right.byte_reader.reader.open("/home/adam/cvs/comrade-main/src/Blank.bmp",ios_base::in|ios_base::out);
   right.byte_reader.writer.open("/home/adam/cvs/comrade-main/src/Blank.bmp",ios_base::in|ios_base::out);
   
   left.byte_reader.reader.open("/home/adam/cvs/comrade-main/src/FatLine.bmp",ios_base::in|ios_base::out);
   left.byte_reader.writer.open("/home/adam/cvs/comrade-main/src/FatLine.bmp",ios_base::in|ios_base::out);
   
   if (left.byte_reader.reader.is_open()==false ||
       right.byte_reader.reader.is_open()==false)
   	{
        	cout<<"Files could not be opened...\n";
		return 0;
	}
	
	else
	{
		cout<<"File could be opened...\n";
	}
	
   //return 0;
   Buffer<RGB> buf(&left),op_buf(&left),final(&left);
   RGB_BufferManager man,opman,finalman;
   opman.assign_buffer(&op_buf);
   finalman.assign_buffer(&final);
	
   man.assign_buffer(&buf);
   man.copy_from_image(&left);
   cfilter.filter(&buf,&op_buf,FILTER_OUT,0,0,319,120);
	
   //eddie.Canny(&buf,&op_buf,.25);
   	
   detor.set_angle_increment(1);
   detor.transform(&op_buf);
   arr=detor.return_array();
   
   for (int y=0; y<=final.maxy-1; y++)
   {
   		for (int x=0; x<=final.maxx-1; x++)
   		{
   			final.at(x,y)=black;
   		}
   }
   
   //goto END;
   
   cout<<detor.return_rows()<<" "<<detor.return_cols()<<endl;
   
   long int max=arr[0][0];
   int maxperp=0,maxtheta=0;
   
   for (int y=0; y<=detor.return_rows()-1; y++)
   {
   	for (int x=0; x<=detor.return_cols()-1; x++)
      	{
   		if (arr[y][x]>max)
		{
			max=arr[y][x];
			maxperp=y;
			maxtheta=x;
		}
	}
   }

   cout<<maxperp<<" "<<maxtheta*detor.return_angle_increment()<<endl;
   
   for (int y=0; y<=detor.return_rows()-1; y++)
   {
   	for (int x=0; x<=detor.return_cols()-1; x++)
      	{
            	final.at(x,y).red=0;
         	final.at(x,y).green=static_cast<unsigned char>(arr[y][x]*255./max);
         	//final.at(x,y).green=255;
		final.at(x,y).blue=0;
		
		//cout<<(int)final.at(x,y).green<<" ";
                          	
            /*if (arr[y][x]>=200)
            {
            	for (int xx=0; xx<=buf.maxx-1; xx++)
            	{
	            	int coordy=static_cast<int>((y-xx*cos(deg_to_rad(x)))/sin(deg_to_rad(x)));
	            	
	            	if (op_buf.at(xx,coordy).red==255 &&
				     	op_buf.at(xx,coordy).green==255 &&
				     	op_buf.at(xx,coordy).blue==255)
				    { 
              			buf.at(xx,coordy)=white;
          			}
            	}
            	
            	for (int yy=0; yy<=buf.maxy-1; yy++)
            	{
	            	int coordx=static_cast<int>((y-yy*sin(deg_to_rad(x)))/cos(deg_to_rad(x)));
	            	
	            	if (op_buf.at(coordx,yy).red==255 &&
				     	op_buf.at(coordx,yy).green==255 &&
				     	op_buf.at(coordx,yy).blue==255)
				    { 
              			buf.at(coordx,yy)=white;
          			}
            	}
	    }*/
      	}
   }
   
   opman.copy_to_image(&right);
   //END: opman.copy_to_image(&right);
   
   left.byte_reader.reader.close();
   left.byte_reader.writer.close();

   right.byte_reader.reader.close();
   right.byte_reader.writer.close();

   cout<<"Done...\n";
   
   cin.get();
   return 0;
}
   
#endif

#ifdef STEREO_DEMO
int main()
{
   //return 0;
   cout<<"Starting stereovision NOW...";

   BitmapStreamer left,right,dmap;
   left.byte_reader.reader.open("Right.bmp",ios::in|ios::binary|ios::nocreate);
   left.byte_reader.writer.open("Right.bmp",ios::out|ios::binary|ios::nocreate);

   right.byte_reader.reader.open("Left.bmp",ios::in|ios::binary|ios::nocreate);
   right.byte_reader.writer.open("Left.bmp",ios::out|ios::binary|ios::nocreate);

   dmap.byte_reader.reader.open("Blank.bmp",ios::in|ios::binary|ios::nocreate);
   dmap.byte_reader.writer.open("Blank.bmp",ios::out|ios::binary|ios::nocreate);

   if (left.byte_reader.reader.is_open()==false ||
       right.byte_reader.reader.is_open()==false ||
       dmap.byte_reader.reader.is_open()==false)
   {
        cout<<"Files could not be opened...\n";

        cin.get();
        return 0;
   }

   Buffer<RGB> l(&left),r(&right),d(&dmap);

   RGB_BufferManager manl,manr,mand;
   manl.assign_buffer(&l);
   manr.assign_buffer(&r);
   mand.assign_buffer(&d);

   manl.copy_from_image(&left);
   manr.copy_from_image(&right);

   cout<<"Finished copying to buffer...\n";
   
   //iris_vm=new IrisEnvironment;
    
    //iris_vm->activate();
    
    //delete iris_vm;
    
    DepthmapBuilder bldr;
    bldr.set_stepsize(1);

    bldr.build_depthmap(&l,&r,&d);

    mand.copy_to_image(&dmap);

    left.byte_reader.reader.close();
    left.byte_reader.writer.close();

    right.byte_reader.reader.close();
    right.byte_reader.writer.close();

    dmap.byte_reader.reader.close();
    dmap.byte_reader.writer.close();

    cout<<"Done...\n";
    
    cin.get();
    return 0;
}

#endif

#ifdef SPACE_CARVE_DEMO
int main()
{    
    cout<<"Let's first check where the voxels project to...\n";

	int numpics=2;

    BitmapStreamer x;
    BitmapStreamer test;

        // Test code starts
    /*
        test.byte_reader.reader.open("K:\\Datasets\\GargoyleX.bmp",ios::in|ios::binary);
        test.byte_reader.writer.open("K:\\Datasets\\GargoyleX.bmp",ios::in|ios::out|ios::binary);

        if (test.byte_reader.reader.is_open()==true)
        {
            cout<<"File could be opened...";
        }

        else
        {
            cout<<"File could not be opened...\n";
            cin.get();
            return 0;
        }
        //cin.get();
    */
        // Test code ends

    Buffer<HSLstruct> vec[2];
    Buffer<bool> marked[2];

    Buffer<RGB> somergb(400,320);
    Buffer<HSLstruct> somehsl(400,320);

    //vector< Buffer<HSLstruct> > vec(numpics);//,Buffer<HSLstruct>(GARGOYLE_X,GARGOYLE_Y));
    vector<Sensor> svec;
    //vector< Buffer<bool> > marked(numpics);//,Buffer<bool>(GARGOYLE_X,GARGOYLE_Y));

    //cout<<"READY";
    //cin.get();
    // Init marked array

    for (int i=0; i<=numpics-1; i++)
    {
        for (int yy=0; yy<=GARGOYLE_Y-1; yy++)
        {
            for (int xx=0; xx<=GARGOYLE_X-1; xx++)
            {
                marked[i].at(xx,yy)=false;
            }
        }
    }

    //cout<<"Gih!";
    //cin.get();

    WorldSpace one;
    one.fill_coordinates(30,30,30);
    Voxel vox1=one.world[10][10][0];
    Voxel vox2=one.world[0][0][0];
    Voxel vox3=one.world[0][20][0];
    Voxel vox4=one.world[20][0][0];
    Voxel vox5=one.world[20][20][0];
    
    //cout<<"Almost";
    //cin.get();

    RGB_BufferManager bman;

    // Test part starts
    //Buffer<RGB> testrgb(&test);
    //bman.assign_buffer(&testrgb);
    //bman.copy_from_image(&test);
    //bufptr=&testrgb;
    // Test part ends

    x.byte_reader.reader.open("Left",ios::in|ios::binary);
    x.byte_reader.writer.open("Left",ios::in|ios::out|ios::binary);

    if (x.byte_reader.reader.is_open()==true)
    {
        cout<<"File could be opened...";
    }

    //Buffer<RGB> somergb(&x);
    //Buffer<HSLstruct> somehsl(&x);

    bman.assign_buffer(&somergb);
    bman.copy_from_image(&x);
    BufferConvertor::RGB_to_HSL_buffer(&somehsl,&somergb);

    vec[0].copy_from(&somehsl);

    x.byte_reader.writer.close();
    x.byte_reader.reader.close();
    
	x.byte_reader.reader.open("Right",ios::in|ios::binary);
    x.byte_reader.writer.open("Right",ios::in|ios::out|ios::binary);

    if (x.byte_reader.reader.is_open()==true)
    {
        cout<<"File could be opened...";
    }

    //Buffer<RGB> somergb(&x);
    //Buffer<HSLstruct> somehsl(&x);

    bman.assign_buffer(&somergb);
    bman.copy_from_image(&x);
    BufferConvertor::RGB_to_HSL_buffer(&somehsl,&somergb);

    vec[0].copy_from(&somehsl);

    x.byte_reader.writer.close();
    x.byte_reader.reader.close();
    
 	//double radius=50000; // 1000 is nice
    int cnt=0;

    //for (double ang=0; ang<=359; ang+=360./numpics,cnt++)
    //{
        //double xx=radius*cos(deg_to_rad(ang));
        //double yy=radius*sin(deg_to_rad(ang));
		
  		double x1=5000,y1=30;
		double x2=x1,y2=-y1;
		
        Coordinate oc1(x1,y1,0);
        Coordinate oc2(x2,y2,0);
        
        //Coordinate tg(0,0,0);

        double A1=0-x1;
        double B1=0-y1;
        double C1=0;
        
        double A2=0-x2;
        double B2=0-y2;
        double C2=0;
        
		Coordinate fc1(x1+A1*.0095,y1+B1*.0095,0); //.3 is OK
		Coordinate fc2(x2+A2*.0095,y2+B2*.0095,0); //.3 is OK

        Sensor cx,cy;
        cy.set(oc2,fc2,0.825,0.55);//6.6.4.4
        cy.img_buffer_ptr=&vec[1];
        cy.flag_buffer_ptr=&marked[1];

        cx.set(oc1,fc1,0.825,0.55);//6.6.4.4
        cx.img_buffer_ptr=&vec[0];
        cx.flag_buffer_ptr=&marked[0];

		cout<<"Focal plane at ("<<fc1.get_x()<<","<<fc1.get_y()<<",0)\n";
        cout<<"Focal plane at ("<<fc2.get_x()<<","<<fc2.get_y()<<",0)\n";
        
        svec.push_back(cx);
    	svec.push_back(cy);

	/*
    Coordinate gih1(vox1.x,vox1.y,vox1.z);
    Coordinate gih2(vox2.x,vox2.y,vox2.z);
    Coordinate gih3(vox3.x,vox3.y,vox3.z);
    Coordinate gih4(vox4.x,vox4.y,vox4.z);
    Coordinate gih5(vox5.x,vox5.y,vox5.z);

    gih1=VoxelWorker::intersection_of(gih1,svec[0]);
    gih2=VoxelWorker::intersection_of(gih2,svec[0]);
    gih3=VoxelWorker::intersection_of(gih3,svec[0]);
    gih4=VoxelWorker::intersection_of(gih4,svec[0]);
    gih5=VoxelWorker::intersection_of(gih5,svec[0]);
    
    cout<<gih1.get_x()<<" "<<gih1.get_y()<<" "<<gih1.get_z()<<endl;
    cout<<gih2.get_x()<<" "<<gih2.get_y()<<" "<<gih2.get_z()<<endl;
    cout<<gih3.get_x()<<" "<<gih3.get_y()<<" "<<gih3.get_z()<<endl;
    cout<<gih4.get_x()<<" "<<gih4.get_y()<<" "<<gih4.get_z()<<endl;
    cout<<gih5.get_x()<<" "<<gih5.get_y()<<" "<<gih5.get_z()<<endl;
    */

    cout<<"Ready to carve...press [Ctrl-Brk] to abort, any other key to continue\n";
    cin.get();

    //for(int i=0; i<=0; i++)
    //do
    //{
        carved=0;
        complete=true;

        RGB green={0,255,0};

        // Test part starts
        /*
        Coordinate box[8];
        int op[2][2];

        int ex=0,ey=0;

        for (int i=0; i<=WORLD_Z-1; i++)
        {
            for (int j=0; j<=WORLD_Y-1; j++)
            {
                for (int k=0; k<=0; k++)
                {
                    VoxelWorker::bounded_intersection_of(one.world[i][j][k],svec[0],box);
                    VoxelWorker::find_rectangle(op,box);

                    //cout<<"("<<op[0][0]<<","<<op[0][1]<<")-(";
                    //cout<<op[0][1]<<","<<op[1][1]<<")"<<endl;

                    if (op[0][0]==op[1][0])
                    {
                        //cout<<":-("<<endl;
                        ex++;
                    }

                    if (op[0][1]==op[1][1])
                    {
                        //cout<<":-("<<endl;
                    ey++;
                    }

                    for (int u=op[0][0]; u<=op[1][0]; u++)
                    {
                        testrgb.at(GARGOYLE_X/2+u,GARGOYLE_Y/2-op[0][1])=green;
                        testrgb.at(GARGOYLE_X/2+u,GARGOYLE_Y/2-op[1][1])=green;
                    }

                    for (int u=op[0][1]; u<=op[1][1]; u++)
                    {
                        testrgb.at(GARGOYLE_X/2+op[0][0],GARGOYLE_Y/2-u)=green;
                        testrgb.at(GARGOYLE_X/2+op[1][0],GARGOYLE_Y/2-u)=green;
                    }
                }
            }
        }

        cout<<"ex="<<ex<<" "<<"ey="<<ey;

        */
// Test part ends

        x_space_carve_inc(svec,one);
        //x_space_carve_dec(svec,one);

        //y_space_carve_inc(svec,one);
        //y_space_carve_dec(svec,one);

        cout<<carved<<" pixels carved.\n";
    //}
    //while(complete==false);
    
    //ofstream bound("Bound.txt");
    cout<<"Stand by for voxel render procedure...\n";

    // Voxel rendering

    x.byte_reader.reader.open("Blank.bmp",ios::nocreate|ios::in|ios::binary);
    x.byte_reader.writer.open("Blank.bmp",ios::nocreate|ios::out|ios::binary);

    if (x.byte_reader.reader.is_open()==true)
    {
        cout<<"Output file could be opened...\n";
    }

    else
    {
        cout<<"Output file could not be opened...\n";
    }

    Buffer<RGB> buf(&x);
    RGB_BufferManager man1;

    man1.assign_buffer(&buf);
    man1.copy_from_image(&x);

    int g=0,h=0;

    RGB white={255,255,255};
    //RGB green={0,255,0};
    RGB black={0,0,0};

    cout<<"Clears buffers...\n";
    for (int yy=0; yy<=599; yy++)
    {
        for (int xx=0; xx<=799; xx++)
        {
            buf.at(xx,yy)=black;
        }
    }

    cout<<"Before drawing slices...\n";
    for (int yy=0; yy<=WORLD_Y-1; yy++)
    {
        //cout<<"Drawing slice "<<yy<<endl;

        for (int zz=0; zz<=WORLD_Z-1; zz++)
        {
            for (int xx=0; xx<=WORLD_X-1; xx++)
            {
                if (one.world[zz][yy][xx].carved==true)
                {
                    RGB black={0,0,0};
                    buf.at(xx+g*(WORLD_X-1),zz+h*(WORLD_Z-1))=black;
                    continue;
                }

                else
                {
                    //cout<<"Coloring...\n";

                    HSLstruct col=one.world[zz][yy][xx].color;
                    RGB color=ColorSpaceConvertor::HSL_to_uRGB(col);

                    buf.at(xx+g*(WORLD_X-1),zz+h*(WORLD_Z-1))=color;
                }
            }
        }

        g++;

        if (g>800/WORLD_X)
        {
            h++;
            g=0;
        }
    }

    cout<<"Before drawing grid...\n";
    // Draw grid
    for (int sy=0; sy<=599; sy+=60)
    {
        for (int sx=0; sx<=799; sx++)
        {
            buf.at(sx,sy)=green;
        }
    }

    for (int sx=0; sx<=799; sx+=60)
    {
        for (int sy=0; sy<=599; sy++)
        {
            buf.at(sx,sy)=green;
        }
    }
    // End drawing grid

    man1.copy_to_image(&x);

    x.byte_reader.reader.close();
    x.byte_reader.writer.close();
    //bound.close();

    cout<<"Tests completed...\n";

    // Test code starts
    //bman.assign_buffer(&testrgb);
    //bman.copy_to_image(&test);

    //test.byte_reader.reader.close();
    //test.byte_reader.writer.close();
    // Test code ends
    
    cin.get();
    return 0;
}

#endif

#ifdef QT_SEG_STEREO_DEMO
bool belongs(Comrade::IrisXT::Rectangle rect,Comrade::Iris3D::Coordinate coord);

int main()
{
 	BitmapStreamer left,right,dmap;
   left.byte_reader.reader.open("hand1.bmp",ios::in|ios::binary);
   left.byte_reader.writer.open("hand1.bmp",ios::in|ios::out|ios::binary);

   right.byte_reader.reader.open("hand2.bmp",ios::in|ios::binary);
   right.byte_reader.writer.open("hand2.bmp",ios::in|ios::out|ios::binary);

   dmap.byte_reader.reader.open("Blank.bmp",ios::in|ios::binary);
   dmap.byte_reader.writer.open("Blank.bmp",ios::in|ios::out|ios::binary);

   if (left.byte_reader.reader.is_open()==false ||
       right.byte_reader.reader.is_open()==false ||
       dmap.byte_reader.reader.is_open()==false)
   {
        cout<<"Files could not be opened...\n";

        cin.get();
        return 0;
   }

   Buffer<RGB> l(&left),r(&right),d(&dmap);

   RGB_BufferManager manl,manr,mand;
   manl.assign_buffer(&l);
   manr.assign_buffer(&r);

   mand.assign_buffer(&d);

   manl.copy_from_image(&left);
   manr.copy_from_image(&right);

   cout<<"Finished copying to buffer...\n";
   
	RGB black={0,0,0};
    
    Comrade::IrisFoundation::Rectangle start={0,0,l.maxx,l.maxy,black};

    Tree<Comrade::IrisFoundation::Rectangle,Coordinate> oak(start);
    oak.set_subsethood_fn(belongs);

    QuadtreeSegmenter seger;
    seger.minx=seger.miny=16;
    seger.threshold=40;
	
    seger.build_quadtree(&oak,&l);
    //seger.draw_quadtree(&oak,&d);

    DepthmapBuilder bldr;
    
    //bldr.uncertainty_color.red=0;
    
    //bldr.set_stepsize(2);
	bldr.set_qualify_thresh(5);
 	//bldr.set_y_offset(2);
  	bldr.set_x_extent(40);
  	bldr.set_left_right_tolerance(100);
  	bldr.set_max_allowed_diff(100);
   	//bldr.set_y_offset(3);
	
    //EpipolarRectifier recter;
    
    bldr.build_depthmap(&l,&r,&d,&oak);

    mand.copy_to_image(&dmap);

    left.byte_reader.reader.close();
    left.byte_reader.writer.close();

    right.byte_reader.reader.close();
    right.byte_reader.writer.close();

    dmap.byte_reader.reader.close();
    dmap.byte_reader.writer.close();

    cout<<"Done...\n";
	cin.get();
	return 0;
}

bool belongs(Comrade::IrisXT::Rectangle rect,Comrade::Iris3D::Coordinate coord)
{
    if (coord.get_x()>=rect.x1 &&
        coord.get_y()>=rect.y1 &&
        coord.get_x()<=rect.x2 &&
        coord.get_y()<=rect.y2)
    {
        return true;
    }

    else
    {
        return false;
    }
}
#endif

#ifdef HOUGH_SHAPE_DEMO
int main()
{
	cout<<"Starting Hough shape detection demo...\n";

	BitmapStreamer one,two,three,four;

    one.byte_reader.reader.open("circle.bmp",ios_base::in|ios_base::out);
   	one.byte_reader.writer.open("circle.bmp",ios_base::in|ios_base::out);

	two.byte_reader.reader.open("blank.bmp",ios_base::in|ios_base::out);
   	two.byte_reader.writer.open("blank.bmp",ios_base::in|ios_base::out);

 	three.byte_reader.reader.open("some.bmp",ios_base::in|ios_base::out);
   	three.byte_reader.writer.open("some.bmp",ios_base::in|ios_base::out);

  	four.byte_reader.reader.open("edge.bmp",ios_base::in|ios_base::out);
   	four.byte_reader.writer.open("edge.bmp",ios_base::in|ios_base::out);

   	if (one.byte_reader.reader.is_open()==false ||
       	two.byte_reader.reader.is_open()==false ||
       three.byte_reader.reader.is_open()==false ||
       four.byte_reader.reader.is_open()==false)
   {
        cout<<"Files could not be opened...\n";

        cin.get();
        return 0;
   }

	
	Buffer<RGB> b1(&one),b2(&two),b3(&three),b4(&four);
 	
  	EdgeDetectorManager eddie;
  	
   	RGB_BufferManager man1,man2,man3,man4;
	man1.assign_buffer(&b1);
	man2.assign_buffer(&b2);
	man3.assign_buffer(&b3);
	man4.assign_buffer(&b4);

	man1.copy_from_image(&one);
	man3.copy_from_image(&three);
	eddie.Canny(&b3,&b4,.002);

  	BasicShapeDetector detor;
	detor.build_shape_table(&b1,109,97,40);
	
	//cout<<detor.shape_table.size()<<endl;

  	for (unsigned int i=0; i<=detor.shape_table.size()-1; ++i)
	{
		cout<<detor.shape_table[i]<<endl;
	}

 	detor.change_sz=0;
  	
    RGB black={0,0,0};

    man2.fill(black);
    detor.transform(&b3);

	Buffer<int>* ptr=detor.return_array();
	
 	for (int yy=0; yy<=ptr->maxy-1; ++yy)
	{
		for (int xx=0; xx<=ptr->maxx-1; ++xx)
		{
   			double inter=20*ptr->at(xx,yy);
			inter=(inter>255)?255:inter;
			
            b2.at(xx,yy).green=static_cast<unsigned char>(inter);
   			b2.at(xx,yy).red=b2.at(xx,yy).blue=0;
 		}
	}
	
	man2.copy_to_image(&two);
	man4.copy_to_image(&four);

 	one.byte_reader.reader.close();
	one.byte_reader.writer.close();

    two.byte_reader.reader.close();
    two.byte_reader.writer.close();

  	three.byte_reader.reader.close();
	three.byte_reader.writer.close();

 	four.byte_reader.reader.close();
	four.byte_reader.writer.close();

   	cout<<"Processing done.\n";
	cin.get();
	return 0;
}

#endif

#ifdef EDGE_DETECTOR_DEMO
int main()
{
	BitmapStreamer ip,op;
	
 	ip.byte_reader.reader.open("circle.bmp",ios::in|ios::binary|ios::nocreate);
   	ip.byte_reader.writer.open("circle.bmp",ios::out|ios::binary|ios::nocreate);

	op.byte_reader.reader.open("blank.bmp",ios::in|ios::binary|ios::nocreate);
   	op.byte_reader.writer.open("blank.bmp",ios::out|ios::binary|ios::nocreate);

	EdgeDetectorManager eddie;
	
	Buffer<RGB> ip_buf(&ip),op_buf(&op);
   	RGB_BufferManager ipman,opman;
   	
    ipman.assign_buffer(&ip_buf);
    opman.assign_buffer(&op_buf);
   	
    ipman.copy_from_image(&ip);
   	eddie.Canny(&ip_buf,&op_buf,.25);
   	
   	opman.copy_to_image(&op);
   	
   	ip.byte_reader.reader.close();
   	ip.byte_reader.writer.close();
   	
	op.byte_reader.reader.close();
   	op.byte_reader.writer.close();

	cout<<"Processing finished.\n";
 	cin.get();
	return 0;
}

#endif

#ifdef OPTICAL_FLOW_DEMO
int main()
{
    BitmapStreamer one,two,three;

    one.byte_reader.reader.open("Cord1.bmp",ios_base::in|ios_base::out|ios_base::binary);
   	one.byte_reader.writer.open("Cord1.bmp",ios_base::in|ios_base::out|ios_base::binary);

	two.byte_reader.reader.open("Cord2.bmp",ios_base::in|ios_base::out|ios_base::binary);
   	two.byte_reader.writer.open("Cord2.bmp",ios_base::in|ios_base::out|ios_base::binary);

 	three.byte_reader.reader.open("Blank.bmp",ios_base::in|ios_base::out|ios_base::binary);
   	three.byte_reader.writer.open("Blank.bmp",ios_base::in|ios_base::out|ios_base::binary);

   	if (one.byte_reader.reader.is_open()==false ||
       	two.byte_reader.reader.is_open()==false ||
        three.byte_reader.reader.is_open()==false)
   {
        cout<<"Files could not be opened...\n";

        cin.get();
        return 0;
   }

   	cout<<"Files could be opened...press <Enter> to begin demo.\n";
   	cin.get();

   	Buffer<RGB> b1(&one),b2(&two),b3(&three);
 	
  	OpticalFlowEngine openg;
  	
   	RGB_BufferManager man1,man2,man3;
	
	man1.assign_buffer(&b1);
	man2.assign_buffer(&b2);
	man3.assign_buffer(&b3);

    man1.copy_from_image(&one);
    man2.copy_from_image(&two);
    man3.copy_from_image(&three);
    
    openg.estimate_optical_flow(&b1,&b2,&b3,20,15);

    man3.copy_to_image(&three);
    
    one.byte_reader.reader.close();
	one.byte_reader.writer.close();

    two.byte_reader.reader.close();
    two.byte_reader.writer.close();

  	three.byte_reader.reader.close();
	three.byte_reader.writer.close();

    cin.get();
    return 0;
}

#endif

#ifdef SIGNAL_DEMO
int main()
{
	Signal_OBJ_DET one;
	Signal_START two;
	
	vector<char> some=ByteObjectConvertor<Signal_OBJ_DET>::object_to_byte(&one);

	for (unsigned int i=0; i<=some.size()-1; ++i)
	{
		cout<<static_cast<int>(some[i])<<" ";
	}
	
	Signal rx_base=ByteObjectConvertor<Signal>::byte_to_object(&some);

	cout<<endl<<rx_base.signal_code<<endl;

	Signal_OBJ_DET rx_d=ByteObjectConvertor<Signal_OBJ_DET>
						::byte_to_object(&some);

	MulticastSignalBuffer buf;
	
	buf.add_signal(ByteObjectConvertor<Signal_START>::object_to_byte(&two));
	buf.add_signal(ByteObjectConvertor<Signal_OBJ_DET>::object_to_byte(&one));
	
	Signal_OBJ_DET sig=ByteObjectConvertor<Signal_OBJ_DET>::
					   byte_to_object(&buf.return_signal_at(1));
	
	cout<<sig.signal_code<<" "<<sig.line<<" "
		<<sig.posx<<" "<<sig.posy<<endl;
	
	//cout<<endl<<rx_d.signal_code<<" "<<rx_d.line<<" "<<rx_d.posx<<" "<<rx_d.posy<<endl;
	cout<<"Done...\n";
	cin.get();
	return 0;
}

#endif

#ifdef DIFFERENCE_DEMO
int main()
{
    BitmapStreamer one,two,three;

    one.byte_reader.reader.open("kathuria1.bmp",ios_base::in|ios_base::out|ios_base::binary);
   	one.byte_reader.writer.open("kathuria1.bmp",ios_base::in|ios_base::out|ios_base::binary);

	two.byte_reader.reader.open("kathuria2.bmp",ios_base::in|ios_base::out|ios_base::binary);
   	two.byte_reader.writer.open("kathuria2.bmp",ios_base::in|ios_base::out|ios_base::binary);

 	three.byte_reader.reader.open("Blank.bmp",ios_base::in|ios_base::out|ios_base::binary);
   	three.byte_reader.writer.open("Blank.bmp",ios_base::in|ios_base::out|ios_base::binary);

   	if (one.byte_reader.reader.is_open()==false ||
       	two.byte_reader.reader.is_open()==false ||
        three.byte_reader.reader.is_open()==false)
   {
        cout<<"Files could not be opened...\n";

        cin.get();
        return 0;
   }

   	cout<<"Files could be opened...press <Enter> to begin demo.\n";
   	cin.get();

   	Buffer<RGB> b1(&one),b2(&two),b3(&three);
 	RGB_BufferManager man1,man2,man3;
	
	man1.assign_buffer(&b1);
	man2.assign_buffer(&b2);
	man3.assign_buffer(&b3);

    man1.copy_from_image(&one);
    man2.copy_from_image(&two);
    man3.copy_from_image(&three);
    
    for (int yy=0; yy<=b1.maxy-1; ++yy)
    {
    	for (int xx=0; xx<=b1.maxx-1; ++xx)
    	{
    		RGB col1=b1.at(xx,yy);
    		RGB col2=b2.at(xx,yy);
    		RGB coldiff;

    		coldiff.red=abs(col1.red-col2.red);
    		coldiff.green=abs(col1.green-col2.green);
    		coldiff.blue=abs(col1.blue-col2.blue);

    		b3.at(xx,yy)=coldiff;
    	}
    }

    man3.copy_to_image(&three);
    
    one.byte_reader.reader.close();
	one.byte_reader.writer.close();

    two.byte_reader.reader.close();
    two.byte_reader.writer.close();

  	three.byte_reader.reader.close();
	three.byte_reader.writer.close();

    cin.get();
    return 0;
}

#endif

#ifdef CONVOLUTION_DEMO
int main()
{
	BitmapStreamer x,y;
	
	x.byte_reader.reader.open("castacheck.bmp",ios_base::in|ios_base::out|ios_base::binary);
    x.byte_reader.writer.open("castacheck.bmp",ios_base::in|ios_base::out|ios_base::binary);

	y.byte_reader.reader.open("Blank.bmp",ios_base::in|ios_base::out|ios_base::binary);
    y.byte_reader.writer.open("Blank.bmp",ios_base::in|ios_base::out|ios_base::binary);

	if (x.byte_reader.reader.is_open()==true &&
		x.byte_reader.writer.is_open()==true &&
		y.byte_reader.reader.is_open()==true &&
		y.byte_reader.writer.is_open()==true)
    {
        cout<<"File could be opened...";
    }

    else
    {
        cout<<"File could not be opened...";

		cin.get();
		return 0;
	}

	Buffer<RGB> op_buf(&x);
    Buffer<RGB> ip_buf(&x);
	
	//RGB black={0,0,0};
	
	RGB_BufferManager man1,man2;
	man1.assign_buffer(&ip_buf);
	man2.assign_buffer(&op_buf);

	man1.copy_from_image(&x);
    //man2.fill(black);
	//man2.copy_from_image(&y);

	/*KernelMatrix vec(25,25);
    double some=50.4;
	KernelGenerator::generate_Gaussian(&vec,some);

    double bmat[]={ 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1,
                    1, 1, 1, 1, 1};

    double cmat[]={ 2, 4, 5, 4, 2,
                    4, 9,12, 9, 4,
                    5,12,15,12, 5,
                    4, 9,12, 9, 4,
                    2, 4, 5, 4, 2};

    KernelMatrix mat(5,5);
    mat.set_values(cmat);

	KernelOperator op;
    
	op.assign_buffer_ptrs(&ip_buf,&op_buf);
	op.assign_kernel(&mat);
    //op.set_factor(2*PI*some*some);
	op.set_factor(150);*/
	//op.convolve();

	AlgorithmManager god;
	EdgeDetectorManager eddie;
	
	//eddie.Laplacian(&ip_buf,&op_buf,155);
	eddie.isotropic(&ip_buf,&op_buf,20);
	//op_buf=ip_buf;
	//god.histogram_equalise(&ip_buf,&op_buf,GREEN);
	//god.histogram_equalise(&ip_buf,&op_buf,BLUE);
	//god.histogram_equalise(&ip_buf,&op_buf,RED);

	man2.copy_to_image(&y);

    x.byte_reader.writer.close();
    x.byte_reader.reader.close();

    y.byte_reader.writer.close();
    y.byte_reader.reader.close();

	cout<<"Done processing...\n";

	cin.get();
	return 0;
}
#endif

#ifdef COLOR_FILTER_DEMO
int main()
{
	BitmapStreamer x,y;
	
	x.byte_reader.reader.open("castacheck.bmp",ios_base::in|ios_base::out|ios_base::binary);
    x.byte_reader.writer.open("castacheck.bmp",ios_base::in|ios_base::out|ios_base::binary);

	y.byte_reader.reader.open("Blank.bmp",ios_base::in|ios_base::out|ios_base::binary);
    y.byte_reader.writer.open("Blank.bmp",ios_base::in|ios_base::out|ios_base::binary);

	if (x.byte_reader.reader.is_open()==true &&
		x.byte_reader.writer.is_open()==true &&
		y.byte_reader.reader.is_open()==true &&
		y.byte_reader.writer.is_open()==true)
    {
        cout<<"File could be opened...";
    }

    else
    {
        cout<<"File could not be opened...";

		cin.get();
		return 0;
	}

	Buffer<RGB> op_buf(&x);
    Buffer<RGB> ip_buf(&x);
	
	//RGB black={0,0,0};
	
	RGB_BufferManager man1,man2;
	man1.assign_buffer(&ip_buf);
	man2.assign_buffer(&op_buf);

	man1.copy_from_image(&x);

	ColorFilter some_filter;
	some_filter.filter_color=ip_buf.at(416,515);
	some_filter.red_tol=140;
	some_filter.green_tol=130;
	some_filter.blue_tol=130;

	some_filter.filter(&ip_buf,&op_buf,FILTER_OUT);

	man2.copy_to_image(&y);

    x.byte_reader.writer.close();
    x.byte_reader.reader.close();

    y.byte_reader.writer.close();
    y.byte_reader.reader.close();

	cout<<"Done processing...\n";

	cin.get();
	return 0;
}
#endif

#ifdef FLOW_FIELD_NAV_DEMO
int main()
{
	BitmapStreamer x,y;
	
	x.byte_reader.reader.open("Map.bmp",ios_base::in|ios_base::out|ios_base::binary);
    x.byte_reader.writer.open("Map.bmp",ios_base::in|ios_base::out|ios_base::binary);

	y.byte_reader.reader.open("Blank.bmp",ios_base::in|ios_base::out|ios_base::binary);
    y.byte_reader.writer.open("Blank.bmp",ios_base::in|ios_base::out|ios_base::binary);

	if (x.byte_reader.reader.is_open()==true &&
		x.byte_reader.writer.is_open()==true &&
		y.byte_reader.reader.is_open()==true &&
		y.byte_reader.writer.is_open()==true)
    {
        cout<<"File could be opened...";
    }

    else
    {
        cout<<"File could not be opened...";

		cin.get();
		return 0;
	}

	Buffer<RGB> op_buf(&x);
    Buffer<RGB> ip_buf(&x);
	
	//RGB black={0,0,0};
	
	RGB_BufferManager man1,man2;
	man1.assign_buffer(&ip_buf);
	man2.assign_buffer(&op_buf);

	man1.copy_from_image(&x);

	
	man2.copy_to_image(&y);

    x.byte_reader.writer.close();
    x.byte_reader.reader.close();

    y.byte_reader.writer.close();
    y.byte_reader.reader.close();

	cout<<"Done processing...\n";

	cin.get();
	return 0;
}
#endif
