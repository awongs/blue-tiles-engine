#pragma once
#include <CL/cl.h>
#include "debugbt/DebugLog.h"

class OpenCLManager {

//Default Contructor
OpenCLManager();
//Deconstructor
~virtual OpenCLManager();

cl_context createContext();
cl_command_queue createCommandQueue(cl_context context, cl_device_id *device);
cl_program createProgram(cl_context context, cl_device_id device, const char* fileName);

};