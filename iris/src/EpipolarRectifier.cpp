#include "EpipolarRectifier.h"

Comrade::Iris3D::EpipolarRectifier::EpipolarRectifier()
{
	std::cout<<"EpipolarRectifier object created...\n";
}

Comrade::Iris3D::EpipolarProfile Comrade::Iris3D::EpipolarRectifier::
build_epipolar_profile(Comrade::IrisFoundation::Buffer<RGB>* left,
				       Comrade::IrisFoundation::Buffer<RGB>* right,
				       int scanline,
					   int height,int width)
{
	std::cout<<"Building epipolar profile. Wait...\n";
	
	EpipolarProfile blank;
	return blank;
}
        	
void Comrade::Iris3D::EpipolarRectifier::draw_epipolar_profile
					(Comrade::Iris3D::EpipolarProfile& profile)
{

}
			
Comrade::Iris3D::EpipolarRectifier::~EpipolarRectifier()
{

}


