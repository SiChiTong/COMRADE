#include "Buffer.h"

Comrade::IrisFoundation::RGB_BufferManager::RGB_BufferManager()
{}

#ifdef USING_LINUX
void Comrade::IrisFoundation::RGB_BufferManager::
copy_from_usb_videostream(FRAMEGRABBER* grabber,FRAME* frame)
{
	cout<<"OK till here...\n";

	fg_grab_frame(grabber,frame);
	//libfg_to_iris(fg_grab(grabber),buf_ptr);
	libfg_to_iris(frame,buf_ptr);
}


void Comrade::IrisFoundation::RGB_BufferManager::
libfg_to_iris(FRAME* frame,
Comrade::IrisFoundation::Buffer<Comrade::IrisFoundation::RGB>* buf)
{
	unsigned char* ptr=
	reinterpret_cast<unsigned char*>(frame->data);

	for (int yy=0; yy<=frame->height-1; ++yy)
	{
		for (int xx=0; xx<=frame->width-1; ++xx)
		{
			buf->at(xx,yy).blue=*ptr;
			++ptr;
			buf->at(xx,yy).green=(*ptr);
			++ptr;
			buf->at(xx,yy).red=*ptr;
			++ptr;
			//++ptr; // Note that this handles RGB32; to handle RGB24, remove the extra increment :-)
		}
	}
}
#endif


void Comrade::IrisFoundation::RGB_BufferManager::copy_from_image(BitmapStreamer* bmp_reader)
{
    bmp_reader->goto_start();

    Comrade::IrisFoundation::RGB** bptr=buf_ptr->buffer_ptr;

	for (int y=0; y<=buf_ptr->maxy-1; ++y)
    {
        for (int x=0; x<=buf_ptr->maxx-1; ++x)
        {
            bmp_reader->read_RGB(bptr[buf_ptr->maxy-1-y][x]);// maxyminus -1
        }
    }
}

void Comrade::IrisFoundation::RGB_BufferManager::copy_to_image(BitmapStreamer* bmp_reader)
{
    bmp_reader->goto_start();

    Comrade::IrisFoundation::RGB** bptr=buf_ptr->buffer_ptr;

	for (int y=0; y<=buf_ptr->maxy-1; ++y)
    {
        for (int x=0; x<=buf_ptr->maxx-1; ++x)
        {
			bmp_reader->write_RGB(bptr[(buf_ptr->maxy-1)-y][x]);// maxy-1
        }
    }
}

void Comrade::IrisFoundation::RGB_BufferManager::block_copy_from_image(BitmapStreamer* bmp_reader,
                             int x1,int y1,int x2,int y2,
                             int destx,int desty)
{
    bmp_reader->goto_start();

	int stx,sty=desty;
	Comrade::IrisFoundation::RGB** bptr=buf_ptr->buffer_ptr;

	for(int sy=y1; sy<=y2; ++sy)
	{
		stx=destx;
        // Add assertion or mugtraps...
		bmp_reader->read_RGB(x1,sy,bptr[sty][stx]);
		stx++;

		for (int sx=x1+1; sx<=x2; ++sx)
		{
			bmp_reader->read_RGB(bptr[sty][stx]);
			++stx;
		}

		++sty;
	}
}

void Comrade::IrisFoundation::RGB_BufferManager::block_copy_to_image(BitmapStreamer* bmp_reader,
                           int x1,int y1,int x2,int y2,
                           int destx,int desty)
{
    bmp_reader->goto_start();
	int stx=destx,sty=desty;
	Comrade::IrisFoundation::RGB** bptr=buf_ptr->buffer_ptr;

    // Add assertion or mugtraps...
	for(int sy=y1; sy<=y2; ++sy)
	{
        bmp_reader->write_RGB(stx,sty,bptr[sy][x1]);

		for (int sx=x1+1; sx<=x2; ++sx)
		{
			bmp_reader->write_RGB(bptr[sy][sx]);
		}

		++sty;
	}
}

Comrade::IrisFoundation::RGB_BufferManager::~RGB_BufferManager()
{}
