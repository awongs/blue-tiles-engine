#pragma once
#include <CL/cl.h>
#include "../debugbt/DebugLog.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class OpenCLManager {
public:
	// Default Constructor
	OpenCLManager(const std::string& fileName, const std::string& kernelName);

	// Deconstructor
	virtual ~OpenCLManager();

	// OpenCL wrapper functions.
	// Return true if successful, else return false.
	cl_mem CreateInputBuffer(size_t size, void *data);
	cl_mem CreateOutputBuffer(size_t size);
	void ReleaseMemoryObject(cl_mem memory);
	bool SetKernelArg(int kernelIndex, cl_uint argIndex, size_t size, const void *argValue);
	bool EnqueueKernel(int kernelIndex, cl_uint dim, const size_t *globalWorkSize, const size_t *localWorkSize);
	bool ReadOutput(void *output, size_t outputSize);
	bool CheckOpenCLError(cl_int errNum, const char* errMsg);

	// Create a new program for the context and return its index to the
	// program/kernel vector. Return -1 if failed to create program.
	int CreateProgram(const std::string& fileName, const std::string& kernelName);

	// Release all programs.
	void ReleasePrograms();

private:
	// OpenCL wrapper functions.
	// Return true if successful, else return false.
	bool CreateContext();
	bool CreateCommandQueue();

	cl_context m_context{ 0 };
	cl_command_queue m_commandQueue{ 0 };
	std::vector<cl_program> m_programs;
	cl_device_id *m_deviceList{ nullptr };
	std::vector<cl_kernel> m_kernels;

	std::vector<cl_mem> m_deviceInputBuffers;
	cl_mem m_deviceOutputBuffer;
};