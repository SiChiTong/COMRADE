#ifndef LINE_FOLLOWER_H
#define LINE_FOLLOWER_H

#include "newmath.h"
#include "ColorFilter.h"
#include "HoughTransform.h"
#include <utility>

namespace Comrade {
	namespace ComradeRuntime {

class LineFollower
{
protected:	Comrade::IrisFoundation::Buffer
		<Comrade::IrisFoundation::RGB> *ip_buffer,*filter_buffer;
		int start_scanline;
		Comrade::IrisFoundation::RGB filter_color;
		
public:		Comrade::IrisXT::StraightLineDetector* detor;
		Comrade::IrisXT::ColorFilter* cfilter;
		
		LineFollower();
	
		void assign_ip_buffer
		(Comrade::IrisFoundation::Buffer
		<Comrade::IrisFoundation::RGB>* buf);

		void assign_filter_buffer
		(Comrade::IrisFoundation::Buffer
		<Comrade::IrisFoundation::RGB>* buf);

		void set_start_scanline(int scanln);
		void set_filter_color(Comrade::IrisFoundation::RGB color);
		std::pair<int,int> detect_line();

		~LineFollower();
};

}
}
#endif
