#ifndef RUNTIME_POOL_H
#define RUNTIME_POOL_H

#include <iostream>
#include "ModelBuilder.h"
#include "BitmapStreamer.h"
#include "KernelMatrix.h"
#include "NonkernelAlgorithms.h"
#include "MiscRoutines.h"
#include "HoughTransform.h"
#include "AutomataEdge.h"
#include "AutomataSkeleton.h"
#include "EdgeDetectors.h"
#include "SpecialKernels.h"
#include "EpipolarRectifier.h"
#include "StereoVision.h"
#include "QuadtreeSegmenter.h"
#include "ColorFilter.h"
#include "WorldBuilder.h"

namespace Comrade {
	namespace ComradeRuntime {
		
using namespace Comrade::IrisFoundation;
using namespace Comrade::IrisXT;
using namespace Comrade::Iris3D;

class IrisObjectPool
{
public: 	RGB_BufferManager man1,man2,man3;
			BitmapStreamer bmp1,bmp2,bmp3;
			Buffer<RGB> buf1,buf2,buf3;
			Tree<Rectangle,Coordinate>* pooltree;
			QuadtreeSegmenter seger;
			DepthmapBuilder stereomapper;
			AlgorithmManager algoman;
			//StraightLineDetector* linedet; // Moved to LineFollower.h
			EdgeDetectorManager edgeman;
			KernelOperator kernelop;
			ColorFilter color_filter;
			WorldBuilder world_builder;

public:		IrisObjectPool();
		      ~IrisObjectPool();	
};

}
}

#endif
