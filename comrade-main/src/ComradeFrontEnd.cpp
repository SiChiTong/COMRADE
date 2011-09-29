#include "ComradeFrontEnd.h"

Comrade::ComradeRuntime::ComradeFrontEnd::ComradeFrontEnd()
{
    iris_vm_assigned=false;
    hw_vm_assigned=false;
}

Comrade::ComradeRuntime::IrisEnvironment* 
Comrade::ComradeRuntime::ComradeFrontEnd::return_iris_vm_ptr()
{
    return iris_vm_ptr;
}

Comrade::Osiris::AbstractHardware* 
Comrade::ComradeRuntime::ComradeFrontEnd::return_hw_vm_ptr()
{
    return hw_vm_ptr;
}
            
void Comrade::ComradeRuntime::ComradeFrontEnd::assign_iris_vm
(Comrade::ComradeRuntime::IrisEnvironment* vm)
{
    iris_vm_ptr=vm;
    iris_vm_assigned=true;
}

void Comrade::ComradeRuntime::ComradeFrontEnd::assign_hw_vm
(Comrade::Osiris::AbstractHardware* vm)
{
    hw_vm_ptr=vm;
    hw_vm_assigned=true;
}

Comrade::ComradeRuntime::ComradeFrontEnd::~ComradeFrontEnd()
{}


