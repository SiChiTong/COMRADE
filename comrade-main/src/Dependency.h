#ifndef DEPENDENCY_H
#define DEPENDENCY_H

#include <string>
#include <map>
#include "FeatureList.h"

namespace Comrade {
    namespace Osiris {
    
class Dependency
{
private:	

public:		std::map<std::string, FeatureList*> dep_map;
			
			Dependency();
			~Dependency();
};

}
}

#endif

