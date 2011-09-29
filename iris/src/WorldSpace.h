#ifndef WORLD_SPACE_H
#define WORLD_SPACE_H

#include "Voxel.h"

namespace Comrade {
    namespace Iris3D {

using namespace Comrade::IrisFoundation;

class WorldSpace
{
protected:	void allocate(int z,int y,int x);
			void deallocate();
			
public:     int zsz,ysz,xsz;

			Voxel*** world; //Should be private
			
public:     WorldSpace(int z=WORLD_Z,int y=WORLD_Y,int z=WORLD_X);

            void test();
            void fill_coordinates(int rcz,int rcy,int rcx);

            ~WorldSpace();
};

}
}

#endif

