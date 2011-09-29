#include "CommandFunctionList.h"

Comrade::ComradeRuntime::ComradeFrontEnd* comrade_cli;
extern Comrade::IO::IO_Redirector stream;

using namespace Comrade::IrisXT;

void Comrade::ComradeRuntime::add_fns()
{
    comrade_cli->set_prompt(">>>");
    comrade_cli->add("do1","nw",fn1);
    comrade_cli->add("do2","WWN",fn2);
    comrade_cli->add("quit","",quit);
    comrade_cli->add("exit","",quit);
    comrade_cli->add("q","",quit);
    comrade_cli->add("x","",quit);
    comrade_cli->add("hw-check","",hw_check);
    comrade_cli->add("mul","wnV",multi_args);
    comrade_cli->add("cls","",cls);
    comrade_cli->add("capture","w",take_shot);
    comrade_cli->add("detline","",test_line_detection);
    comrade_cli->add("stereo","www",test_stereovision);
    comrade_cli->add("findcircle","ww",test_circle_detection);
}
    
void Comrade::ComradeRuntime::fn1(std::vector <Scorpio::Argument> arr)
{
    stream.write("Called fn1()");
}

void Comrade::ComradeRuntime::hw_check(std::vector <Scorpio::Argument> arr)
{
    comrade_cli->return_hw_vm_ptr()->check("OBJ_DET");
	comrade_cli->return_hw_vm_ptr()->check("OBJ_RET");
	comrade_cli->return_hw_vm_ptr()->check("LINE_FOLL");
}

void Comrade::ComradeRuntime::take_shot(std::vector <Scorpio::Argument> arr)
{
	#ifdef USING_LINUX

	char* value;
	char path[]="/home/adam/cvs/comrade-main/src/";

	if (arr[0].is_valid==true)
	{
		value=arr[0].argument_cell.string_part;
	}

	else
	{
		value="Picture.bmp";
	}

	std::strcat(path,value);
	
	IrisObjectPool* pool=&(comrade_cli->return_iris_vm_ptr()->obj_pool);

	pool->bmp1.byte_reader.reader.open(path,std::ios_base::in|std::ios_base::out);
	pool->bmp1.byte_reader.writer.open(path,std::ios_base::in|std::ios_base::out);

	std::cout<<path<<std::endl;
	
	if (pool->bmp1.byte_reader.reader.is_open()!=true ||
	    pool->bmp1.byte_reader.writer.is_open()!=true)
	{
		std::cout<<"File(s) could not be opened...\n";
		return;
	}

	std::cout<<"BPP="<<pool->bmp1.byte_reader.read_field(BPP,SZ_BPP)<<std::endl;
	pool->man1.assign_buffer(&(pool->buf1));
	//pool->man2.assign_buffer(&(pool->buf2));

	AbstractHardware* ptr=comrade_cli->return_hw_vm_ptr();
	fg_dump_info(ptr->cam.grabber_ptr);

	pool->man1.copy_from_usb_videostream(ptr->cam.grabber_ptr,ptr->cam.temp_frame);

	//pool->algoman.channel_adjust(&(pool->buf1),&(pool->buf2),20,-10,20);
	//pool->algoman.histogram_equalise(&(pool->buf1),&(pool->buf2),Comrade::IrisFoundation::GREEN);
	//pool->algoman.histogram_equalise(&(pool->buf1),&(pool->buf2),Comrade::IrisFoundation::RED);
	//pool->algoman.histogram_equalise(&(pool->buf1),&(pool->buf2),Comrade::IrisFoundation::BLUE);
	//Comrade::IrisFoundation::RGB black={0,0,0};

	//pool->man1.fill(black);
	pool->man1.copy_to_image(&(pool->bmp1));

	pool->bmp1.byte_reader.reader.close();
	pool->bmp1.byte_reader.writer.close();
	
	#endif
}

void Comrade::ComradeRuntime::fn2(std::vector <Scorpio::Argument> arr)
{
    if (arr[0].is_valid==arr[1].is_valid==arr[2].is_valid==true)
    {
        for (int i=1; i<=arr[2].argument_cell.number_part; i++)
        {
            stream.write(arr[0].argument_cell.string_part);
            stream.write(" likes to eat ");
            stream.write(arr[1].argument_cell.string_part);
            stream.write("\n");
        }
    }

    stream.write("Called fn2()\n");
}

void Comrade::ComradeRuntime::quit(std::vector <Scorpio::Argument> arr)
{
    std::cout<<arr.size()<<std::endl;
    stream.write("Exiting...press a key.");

    std::cin.get();
    exit(0);
}

void Comrade::ComradeRuntime::test_line_detection
(std::vector <Scorpio::Argument> arr)
{
   ColorFilter cfilter;
   long int** array;
   
   RGB somecolor={82,77,70};
	   
   cfilter.filter_color=somecolor;
   cfilter.red_tol=cfilter.green_tol=cfilter.blue_tol=40;

   BitmapStreamer left,right;
   
   RGB white={252,252,252};
   RGB black={0,0,0};
   
   right.byte_reader.reader.open("/home/adam/cvs/comrade-main/src/Blank.bmp",ios_base::in|ios_base::out);
   right.byte_reader.writer.open("/home/adam/cvs/comrade-main/src/Blank.bmp",ios_base::in|ios_base::out);
   
   left.byte_reader.reader.open("/home/adam/cvs/comrade-main/src/Picture.bmp",ios_base::in|ios_base::out);
   left.byte_reader.writer.open("/home/adam/cvs/comrade-main/src/Picture.bmp",ios_base::in|ios_base::out);
   
   if (left.byte_reader.reader.is_open()==false ||
       right.byte_reader.reader.is_open()==false)
   	{
        	cout<<"Files could not be opened...\n";
		return;
	}
	
	else
	{
		cout<<"File could be opened...\n";
	}
	
   Buffer<RGB> buf(&left),op_buf(&left),histo(&left),final(&left);
   StraightLineDetector detor(&buf,180,1);

   RGB_BufferManager man,opman,finalman;
   opman.assign_buffer(&op_buf);
   finalman.assign_buffer(&final);
	
   man.assign_buffer(&buf);
   man.copy_from_image(&left);
   cfilter.filter(&buf,&op_buf,FILTER_IN,0,130,319,239);
	
   //eddie.Canny(&buf,&op_buf,.25);
   	
   detor.transform(&op_buf);
   array=detor.return_array();
   
   for (int y=0; y<=final.maxy-1; y++)
   {
   		for (int x=0; x<=final.maxx-1; x++)
   		{
   			final.at(x,y)=black;
   		}
   }
   
   cout<<detor.return_rows()<<" "<<detor.return_cols()<<endl;
   
   long int max=array[0][0];
   int maxperp=0,maxtheta=0;
   
   for (int y=0; y<=detor.return_rows()-1; y++)
   {
   	for (int x=0; x<=detor.return_cols()-1; x++)
      	{
   		if (array[y][x]>max)
		{
			max=array[y][x];
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
         	final.at(x,y).green=static_cast<unsigned char>(array[y][x]*(255./max));
         	//final.at(x,y).green=255;
		final.at(x,y).blue=0;
		
		//cout<<(int)final.at(x,y).green<<" ";
                          	
      	}
   }
   
   opman.copy_to_image(&right);
   //END: opman.copy_to_image(&right);
   
   left.byte_reader.reader.close();
   left.byte_reader.writer.close();

   right.byte_reader.reader.close();
   right.byte_reader.writer.close();

   cout<<"Done...\n";
}

void Comrade::ComradeRuntime::cls(std::vector <Scorpio::Argument> arr)
{
    std::cout<<arr.size();
    system("clear");
}

void Comrade::ComradeRuntime::multi_args(std::vector <Scorpio::Argument> arr)
{
    //assert(arr.size()!=0);
    //Remove the comments above for debug versions

    if (arr.size()!=0)
    {
        for (unsigned int i=0; i<=arr.size()-1; i++)
        {
            if (arr[i].is_valid==true)
            {
                //cout<<"At least one is there...\n";
                stream.write(arr[i].argument_cell.string_part);
                stream.write("\n");
            }
        }

        stream.write("\n");;
    }

    else
    {
        stream.write("No arguments passed.\n");
    }
}

void Comrade::ComradeRuntime::load_bmp(std::vector <Scorpio::Argument> arr)
{
    stream.write("Loading a bitmap...\n");
}

void Comrade::ComradeRuntime::test_stereovision(std::vector <Argument> arr)
{
	/*if (arr[0].is_valid==false ||
	    arr[1].is_valid==false ||
	    arr[2].is_valid==false)
	{
		stream.write("Expected number of arguments=3...\n");
		
		return;
	}*/
	
	char path1[]="/home/adam/cvs/comrade-main/src/Blank.bmp";
	char path2[]="/home/adam/cvs/comrade-main/src/Picture.bmp";
	char path3[]="/home/adam/cvs/comrade-main/src/dmap.bmp";
	cout<<"Starting stereovision NOW...";

   BitmapStreamer left,right,dmap;
   left.byte_reader.reader.open(path1,ios_base::in|ios_base::out);
   left.byte_reader.writer.open(path1,ios_base::in|ios_base::out);

   right.byte_reader.reader.open(path2,ios_base::in|ios_base::out);
   right.byte_reader.writer.open(path2,ios_base::in|ios_base::out);

   dmap.byte_reader.reader.open(path3,ios_base::in|ios_base::out);
   dmap.byte_reader.writer.open(path3,ios_base::in|ios_base::out);

   if (left.byte_reader.reader.is_open()==false ||
       right.byte_reader.reader.is_open()==false ||
       dmap.byte_reader.reader.is_open()==false)
   {
        cout<<"Files could not be opened...\n";

        cin.get();
        return;
   }

   Buffer<RGB> l(&left),r(&right),d(&dmap);

   RGB_BufferManager manl,manr,mand;
   manl.assign_buffer(&l);
   manr.assign_buffer(&r);
   mand.assign_buffer(&d);

   manl.copy_from_image(&left);
   manr.copy_from_image(&right);

   cout<<"Finished copying to buffer...\n";
    
    Comrade::IrisFoundation::RGB black={0,0,0};
    
    Comrade::IrisFoundation::Rectangle start={0,0,l.maxx,l.maxy,black};

    Tree<Comrade::IrisFoundation::Rectangle,Coordinate> oak(start);
    oak.set_subsethood_fn(Comrade::ComradeRuntime::belongs);

    QuadtreeSegmenter seger;
    seger.minx=seger.miny=2;
    seger.threshold=20;
	
    seger.build_quadtree(&oak,&l);
    seger.draw_quadtree(&oak,&d);

    DepthmapBuilder bldr;
    //goto HERE;
    //bldr.uncertainty_color.red=0;
    
    //bldr.set_stepsize(2);
	bldr.set_qualify_thresh(0);
 	//bldr.set_y_offset(2);
  	bldr.set_x_extent(100);
  	bldr.set_left_right_tolerance(80);
  	//bldr.set_max_allowed_diff(75);
   	bldr.set_y_offset(7);
	
	bldr.build_depthmap(&l,&r,&d,&oak);

//Comrade::IrisFoundation::RGB blah={0,0,0};
    //mand.fill(blah);
 mand.copy_to_image(&dmap);

    left.byte_reader.reader.close();
    left.byte_reader.writer.close();

    right.byte_reader.reader.close();
    right.byte_reader.writer.close();

    dmap.byte_reader.reader.close();
    dmap.byte_reader.writer.close();

    cout<<"Done stereo...\n";
}

void Comrade::ComradeRuntime::test_circle_detection(std::vector <Argument> arr)
{
	char path1[]="/home/adam/cvs/comrade-main/src/Blank.bmp";
	char path2[]="/home/adam/cvs/comrade-main/src/Picture.bmp";

   RGB some={142,168,224};

   BitmapStreamer left,right;
   //ColorFilter cf;
   //cf.red_tol=cf.green_tol=cf.blue_tol=15;
   //cf.filter_color=some;
   CircleDetector cdetor(320,240,8,18);
   cdetor.cfilter.red_tol=cdetor.cfilter.green_tol=cdetor.cfilter.blue_tol=15;
   cdetor.cfilter.filter_color=some;
	
   left.byte_reader.reader.open(path1,ios_base::in|ios_base::out);
   left.byte_reader.writer.open(path1,ios_base::in|ios_base::out);

   right.byte_reader.reader.open(path2,ios_base::in|ios_base::out);
   right.byte_reader.writer.open(path2,ios_base::in|ios_base::out);

   if (left.byte_reader.reader.is_open()==false ||
       right.byte_reader.reader.is_open()==false)
   {
        cout<<"Files could not be opened...\n";

        cin.get();
        return;
   }

   Buffer<RGB> l(&left),r(&right);

   RGB_BufferManager manl,manr;
   manl.assign_buffer(&l);
   manr.assign_buffer(&r);

   manl.copy_from_image(&left);
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
   
   //cf.filter(&l,&r,FILTER_IN);
   
   //manr.copy_to_image(&right);
   manl.copy_to_image(&right);
   
   left.byte_reader.reader.close();
    left.byte_reader.writer.close();

    right.byte_reader.reader.close();
    right.byte_reader.writer.close();

}

bool Comrade::ComradeRuntime::belongs
(Comrade::IrisXT::Rectangle rect,Comrade::Iris3D::Coordinate coord)
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
