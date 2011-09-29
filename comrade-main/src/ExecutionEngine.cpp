#include "../config.h"
#include <glibmm/main.h>
#ifdef WITH_GUI
#include "GridDrawingArea.h"
#endif

#include "ExecutionEngine.h"
#include "Constants.hh"
#include "CIRC.hh"
#include <sys/time.h>
#include <time.h>

extern Comrade::IO::IO_Redirector stream;

using namespace Comrade::ComradeRuntime;
using namespace Comrade::IrisFoundation;
using namespace Comrade::Corelib;
using namespace Comrade::IrisXT;

Comrade::ComradeRuntime::ExecutionEngine::ExecutionEngine(ComradeFrontEnd* cfe)
{
	comrade_vm=cfe;
// Structure initialisation
	busy=false;

	evidence_grid=new EvidenceGrid<unsigned char>(1,600,800);
	evidence_grid_vfh=new EvidenceGrid<unsigned char>(1,600,800);
	evidence_grid->fill(127);
	evidence_grid_vfh->fill(0);
	local_grid=new EvidenceGrid<unsigned char>
	(1,
	 2*SONAR_MAX_RANGE_READING+ROBOT_UNITS_Y,
	 2*SONAR_MAX_RANGE_READING+ROBOT_UNITS_X);	
	
	img_buf=new Buffer<RGB>(320,240); // Replace with imported constants later...
	//RGB somecolor={82,77,70};
	RGB somecolor={22,21,22};
	
	// Algorithm initialisation
	line_follower.assign_filter_buffer(&cfe->return_iris_vm_ptr()->obj_pool.buf1);
	line_follower.assign_ip_buffer(img_buf);
	line_follower.cfilter=&(cfe->return_iris_vm_ptr()->obj_pool.color_filter);
	line_follower.set_start_scanline(120);
	line_follower.set_filter_color(somecolor);
	
	odometry_engine=new OdometryEngine;
	obstacle_avoider=new ObstacleAvoider(odometry_engine,evidence_grid_vfh);

#ifdef WITH_GUI
	Comrade::LocalGUI::GridDrawingArea::get_singleton().set_evidence_grid(evidence_grid_vfh);
#endif

#ifdef USING_LINUX
	comrade_vm->hw_vm_ptr->interface->on_compass_data_rx.connect
		(sigc::mem_fun(*this,&ExecutionEngine::update_compass_data));
	
//	connect_sonar();
#endif
//	sectordraw.on_pixel_receive.connect(sigc::mem_fun(evidence_grid,&EvidenceGrid<unsigned char>::update));
	sectordraw.on_pixel_receive.connect(sigc::mem_fun(evidence_grid_vfh,&EvidenceGrid<unsigned char>::update_vfh));
	//motor_controller=new MotorController(comrade_vm->hw_vm_ptr->interface);
	// Brought down into AbstractHardware.h
	mode=EM_STOP;
	circ.on_start_receive.connect(sigc::mem_fun(this, &ExecutionEngine::circ_start));
}

void ExecutionEngine::circ_set_speeds(Comrade::Corelib::speed_update& su)
{
	if (mode==EM_TELEOPERATION)
	{
	}
}

void ExecutionEngine::circ_start(unsigned char mode)
{
	ExecutionMode m=ExecutionMode(m);
	if (this->mode!=m)
	{
		if (this->mode==EM_STOP)
		{ // start whatever
			this->mode=m;
			switch(m)
			{
				case EM_VFH:
					start_vfh();
					break;
				case EM_LINEFOLLOW:
					start_linefollow();
					break;
				case EM_TELEOPERATION:
					start_teleoperation();
					break;
			}
		}
		else if (m==EM_STOP)
		{ //stop whatever we were doing
			comrade_vm->return_hw_vm_ptr()->motor_controller->set_target_speeds(0,0);
			switch(this->mode)
			{
				case EM_VFH:
					stop_vfh();
					break;
				case EM_LINEFOLLOW:
					stop_linefollow();
					break;
				case EM_TELEOPERATION:
					stop_teleoperation();
					break;
			}
		}
	}
}

void Comrade::ComradeRuntime::ExecutionEngine::assign_vm
(Comrade::ComradeRuntime::ComradeFrontEnd* vm)
{
	comrade_vm=vm;
}

void Comrade::ComradeRuntime::ExecutionEngine::show_copyright()
{
	stream.write
	("COMRADE Execution Engine (CPC Hades) (C) 2004 Avishek Sen Gupta\n");
}

void Comrade::ComradeRuntime::ExecutionEngine::build_stereomap()
{
	AbstractHardware* hw=comrade_vm->return_hw_vm_ptr();
	IrisEnvironment* irisenv=comrade_vm->return_iris_vm_ptr();
	
	Buffer<RGB>& bufleft=irisenv->obj_pool.buf1;
	Buffer<RGB>& bufright=irisenv->obj_pool.buf2;
	Buffer<RGB>& opbuf=irisenv->obj_pool.buf3;

	RGB_BufferManager man1,man2;
	man1.assign_buffer(&bufleft);
	man2.assign_buffer(&bufright);
	
	man1.copy_from_usb_videostream(hw->cam.grabber_ptr,hw->cam.temp_frame);
	RobotInterface* iface=hw->stereovision_motor.interface;
	//iface->camera_translate(20);
	// Import camera motor step units here later...
	man2.copy_from_usb_videostream(hw->cam.grabber_ptr,hw->cam.temp_frame);
	
	DepthmapBuilder& stereo=irisenv->obj_pool.stereomapper;
	
	RGB black={0,0,0};
    
    	Comrade::IrisFoundation::Rectangle start={0,0,bufleft.maxx,bufleft.maxy,black};

    	Tree<Comrade::IrisFoundation::Rectangle,Coordinate> oak(start);
    	oak.set_subsethood_fn(Comrade::IrisFoundation::belongs);

    QuadtreeSegmenter seger;
    seger.minx=seger.miny=16;
    seger.threshold=40;
	
    seger.build_quadtree(&oak,&bufleft);
 
	stereo.set_qualify_thresh(5);
 	//bldr.set_y_offset(2);
  	stereo.set_x_extent(40);
  	stereo.set_left_right_tolerance(100);
	//bldr.set_max_allowed_diff(100);
   	//bldr.set_y_offset(3);
	
    stereo.build_depthmap(&bufleft,&bufright,&opbuf,&oak);
}

/*char* Comrade::ComradeRuntime::ExecutionEngine::extract_signal_code
(std::vector<char>& signal)
{
	char* code=Comrade::ComradeRuntime::ByteObjectConvertor::
			   byte_to_object(&signal);

	return code;
}
*/

std::pair<int,int> Comrade::ComradeRuntime::ExecutionEngine::circle_fn()
{
      RGB some={142,168,224};

      CircleDetector cdetor(320,240,8,18);
   cdetor.cfilter.red_tol=cdetor.cfilter.green_tol=cdetor.cfilter.blue_tol=15;
   cdetor.cfilter.filter_color=some;
	
   /*left.byte_reader.reader.open(path1,ios_base::in|ios_base::out);
   left.byte_reader.writer.open(path1,ios_base::in|ios_base::out);

   right.byte_reader.reader.open(path2,ios_base::in|ios_base::out);
   right.byte_reader.writer.open(path2,ios_base::in|ios_base::out);

   if (left.byte_reader.reader.is_open()==false ||
       right.byte_reader.reader.is_open()==false)
   {
        cout<<"Files could not be opened...\n";

        cin.get();
        return;
   }*/

   Buffer<RGB>& l=comrade_vm->return_iris_vm_ptr()->obj_pool.buf1;

   RGB_BufferManager manl,manr;
   manl.assign_buffer(&l);
   //manr.assign_buffer(&r);

   //manl.copy_from_image(&left);
   //manr.copy_from_image(&right);

   cout<<"Finished copying to buffer...\n";
    
   //EdgeDetectorManager& eddie=comrade_cli->return_iris_vm_ptr()->obj_pool.edgeman;
   //eddie.Canny(&l,&r,0.005);
   
   cdetor.transform(&l);
   //manr.copy_to_image(&right);
   cout<<cdetor.return_cols()<<" "<<cdetor.return_rows()<<endl;

   long int*** array=cdetor.return_array();
      
   int max=0,maxx=0,maxy=0;
   
   // R,G,B=140,126,97
   for (int i=0; i<=cdetor.return_radius_range()-1; ++i)
   {
	   for (int j=5; j<=cdetor.return_rows()-5; ++j)
	   {
		   for (int k=5; k<=cdetor.return_cols()-5; ++k)
		   {
			   if (array[i][j][k]>=max)
			   {
				max=array[i][j][k];
				maxx=k;
				maxy=j;
			   }
			   
			   //std::cout<<k<<" "<<j<<"="<<array[i][j][k]<<std::endl;
			   
			//if (array[i][j][k]>255)
			//{
			//	array[i][j][k]=255;
			//}
			
			//RGB whatever={0,array[i][j][k],0};
			
			//l.at(k,j)=whatever;
		   }
	   }
   }
   
   std::cout<<std::endl<<maxx<<" "<<maxy<<std::endl;
   
   RGB green={0,255,0};
   
   for (int i=maxx-5; i<=maxx+5; ++i)
   {
	   l.at(i,maxy)=green;
   }
   
   for (int i=maxy-5; i<=maxy+5; ++i)
   {
	   l.at(maxx,i)=green;
   }

   std::pair<int,int> result;
   result.first=maxx;
   result.second=maxy;
   
   return result;
}

bool Comrade::ComradeRuntime::ExecutionEngine::line_fn()
{
	RGB_BufferManager man;
	man.assign_buffer(img_buf);
	AbstractHardware* hw=comrade_vm->return_hw_vm_ptr();
	man.copy_from_usb_videostream(hw->cam.grabber_ptr,hw->cam.temp_frame);

	//int ix=80;
	std::pair<int, int> linepair=line_follower.detect_line();
	int ix=linepair.first-160;
	int lineangle=linepair.second;
	cout << "ix = " << ix << " lineangle = " << lineangle << endl;
	if (ix<-160 || ix>160) return true;
	double angle=atan2(double(ix),120.0);
	angle=(180/M_PI)*angle;
	if (angle>45.0) angle=90.0;
	if (angle<-45.0) angle=-90.0;
	cout << "angle = " << angle << endl;
	hw->motor_controller->set_target_direction(angle,70.0);
	return true;
}

void Comrade::ComradeRuntime::ExecutionEngine::activate()
{
	//char* code=extract_signal_code
	//(comrade_vm->multicast_buffer.read_and_discard());

	//comrade_vm->activate();
	// Put conditions for start/stop here.
	// Real code here...

	//Glib::signal_timeout().connect(sigc::mem_fun(this,&ExecutionEngine::line_fn), 500);
	
        // Uncomment line below...
  	//Glib::signal_idle().connect(sigc::mem_fun(this,&ExecutionEngine::line_fn));
	
        //const Glib::RefPtr<Glib::TimeoutSource> timeout_source = Glib::TimeoutSource::create(1000);
	//timeout_source->connect(sigc::mem_fun(this,&ExecutionEngine::line_fn));
 	//timeout_source->attach(Glib::MainContext::get_default());
}

void Comrade::ComradeRuntime::ExecutionEngine::update_compass_data(int bearing)
{
	sensor_pack.compass_bearing=bearing;
}

void Comrade::ComradeRuntime::ExecutionEngine::update_sonar_data
(int bearing,int* array)
{
	map_update mu;
	static int counter=0;
	mu.orientation=sensor_pack.compass_bearing;
	mu.sonar_angle=bearing;
	mu.x=odometry_engine->get_x();
	mu.y=odometry_engine->get_y();
	mu.sonar_data[0]=array[0];
	mu.sonar_data[1]=array[1];
	mu.sonar_data[2]=array[2];
	mu.sonar_data[3]=array[3];
	circ.send_map_update(mu);
	evidence_grid_vfh->update_from_sonar(mu,sectordraw);
	std::pair<double,double> speeds=obstacle_avoider->get_speeds();
	comrade_vm->return_hw_vm_ptr()->motor_controller->set_target_direction(speeds.first, speeds.second);
#ifdef WITH_GUI
	Comrade::LocalGUI::GridDrawingArea::get_singleton().update();
#endif
}

Comrade::ComradeRuntime::ExecutionEngine::~ExecutionEngine()
{
	delete obstacle_avoider;
	delete odometry_engine;
	//delete motor_controller; // Removed to AbstractHardware.h
	delete evidence_grid;
	delete evidence_grid_vfh;
	delete local_grid;
	delete img_buf;
}

void ExecutionEngine::start_vfh()
{
/*	comrade_vm->hw_vm_ptr->interface->on_sonar_data_rx.connect
		(sigc::mem_fun(*this,&ExecutionEngine::update_sonar_data));	*/
	vfh_conn=comrade_vm->hw_vm_ptr->interface->on_sonar_data_rx.connect(sigc::mem_fun(*this,&ExecutionEngine::update_sonar_data));
}

void ExecutionEngine::stop_vfh()
{
	vfh_conn.disconnect();
}

void ExecutionEngine::start_linefollow()
{
	linefollow_conn=Glib::signal_idle().connect(sigc::mem_fun(this,&ExecutionEngine::line_fn));
}

void ExecutionEngine::stop_linefollow()
{
	linefollow_conn.disconnect();
}

void ExecutionEngine::start_teleoperation()
{
	teleoperation_conn=circ.on_set_speeds_receive.connect(sigc::mem_fun(this,&ExecutionEngine::circ_set_speeds));
}

void ExecutionEngine::stop_teleoperation()
{
	teleoperation_conn.disconnect();
}
