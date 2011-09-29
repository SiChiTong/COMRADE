#include "IrisEnv.h"

using namespace std;
using namespace Comrade::IrisFoundation;
using namespace Comrade::IrisXT;

int main()
{

char* fname="d:\\image1.bmp";
ColorFilter colorFilter;
RGB white={255,255,255};
colorFilter.filter_color=white;

BitmapStreamer left;
left.byte_reader.reader.open(fname,ios_base::in|ios_base::out|ios::binary);

if (left.byte_reader.reader.is_open()==false)
{
       cout<<"Files could not be opened...\n";
}

else
{
	cout<<"File could be opened...\n";
}

Buffer<RGB> buf1(&left);
Buffer<RGB> buf2;

// I commented this out because you have not create colorFilter...
colorFilter.filter(&buf1,buf2,FILTER_OUT,0,0,200,200);
}
