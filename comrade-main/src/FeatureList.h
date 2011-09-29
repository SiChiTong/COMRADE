#ifndef FEATURE_LIST_H
#define FEATURE_LIST_H

#include <iostream>
#include <vector>
//#include "Feature.h"

//#ifdef USING_LINUX
#include "StereovisionMotor.h"
#include "MotorController.h"
//#endif

namespace Comrade {
    namespace Osiris {
    
class FeatureList
{
private:	std::vector<Feature*> feature_list;
			std::vector<Feature*>::iterator itor;
			
public:		FeatureList();
			
			void add_feature(Feature* feature);
			bool is_complete();
			bool status_of_feature(int fno);
   			
			~FeatureList();
};

}
}

#endif
