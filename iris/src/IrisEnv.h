#ifndef IRIS_ENV_H
#define IRIS_ENV_H

#include <iostream>
#include "RuntimePool.h"
#include "IoRedir.h"

namespace Comrade {
    namespace ComradeRuntime {

using namespace IrisFoundation;
using namespace IrisXT;
using namespace Iris3D;

class IrisEnvironment
{
public:   IrisObjectPool obj_pool;

public:   IrisEnvironment();
              
          ~IrisEnvironment();
};

}
}

#endif

