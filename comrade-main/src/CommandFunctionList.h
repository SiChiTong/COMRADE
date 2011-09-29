#ifndef COMMAND_FN_LIST_H
#define COMMAND_FN_LIST_H

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cassert>
#include <vector>
#include <cstring>
#include "SystemDefs.h"
#include "IrisEnv.h"
#include "ComradeFrontEnd.h"

namespace Comrade {
  namespace ComradeRuntime {
    
using namespace IrisFoundation;
using namespace IrisXT;
using namespace Iris3D;
using namespace Scorpio;

void add_fns();

void fn1(std::vector <Argument> arr);
void hw_check(std::vector <Argument> arr);
void take_shot(std::vector <Argument> arr);
void fn2(std::vector <Argument> arr);
void quit(std::vector <Argument> arr);
void multi_args(std::vector <Argument> arr);
void cls(std::vector <Argument> arr);
void load_bmp(std::vector <Argument> arr);
void test_line_detection(std::vector <Argument> arr);
void test_stereovision(std::vector <Argument> arr);
void test_circle_detection(std::vector <Argument> arr);

bool belongs(Comrade::IrisXT::Rectangle rect,Comrade::Iris3D::Coordinate coord);

}
}

#endif
