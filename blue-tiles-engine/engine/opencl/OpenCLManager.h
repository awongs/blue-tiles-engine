#pragma once
#include <CL/cl.h>
#include "debugbt/DebugLog.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
class OpenCLManager {
public:
//Default Constructor
OpenCLManager();
//Deconstructor
~virtual OpenCLManager();

cl_context createContext();
cl_command_queue createCommandQueue(cl_context context, cl_device_id *device);
cl_program createProgram(cl_context context, cl_device_id device, const char* fileName);

private:
    cl_context m_context;
    cl_command_queue m_commandQueue;
    cl_program m_program;
    cl_device_id m_device;
    cl_kernel m_kernel;
};