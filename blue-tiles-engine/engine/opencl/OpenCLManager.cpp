#include "OpenCLManager.h"

namespace
{
	//Magic number to store the platform name.
	const int PLATFORM_NAME_SIZE = 1024;
}

OpenCLManager::OpenCLManager(const std::string& fileName,
	const std::string& kernelName) {
	CreateContext();
	CreateCommandQueue();
	CreateProgram(fileName, kernelName);
}

OpenCLManager::~OpenCLManager() {

}

/* Create an OpenCL context.
 * Selects first platform.
 * CPU or GPU.
*/
bool OpenCLManager::CreateContext() {
	cl_platform_id firstPlatformId;
	cl_uint numPlatforms;
	cl_int errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);

	// Error checking on errNum.
	if (!CheckOpenCLError(errNum, "Failed to find any OpenCL platforms.")) {
		return false;
	}

	if (numPlatforms <= 0) {
		DebugLog::Error("Failed to create context");
	}

	DebugLog::Info("Platforms in total: " + std::to_string(numPlatforms));

	// Get information about the platform
	char pname[PLATFORM_NAME_SIZE];
	size_t retsize;

	/*clGetPlatformInfo
	*Get specific information about the OpenCL platform.
	*Parameters
	* Type cl_platform_id = The platform Id obtained from clGetPlatformIDs.
	* Type cl_platform_info = An enumeration constant that identifies the platform information.
	* Type size_t = Specifies the size in bytes of memory pointed to by the next argument.
	* Type void * = A pointer to memory location where appropriate values for a given cl_platform_info name.
	* Tpye size_t = Returns the actual size in bytes of data being querried by the void pointer.
	*/
	errNum = clGetPlatformInfo(firstPlatformId, CL_PLATFORM_NAME,
		sizeof(pname), (void*)pname, &retsize);

	// Error check to see if we can get any platform information.
	if (!CheckOpenCLError(errNum, "Could not get platform info")) {
		return false;
	}

	DebugLog::Info("Selected platform <" + std::string(pname) + ">");

	// Creates an OpenCL context on the platform.
	cl_context_properties contextProperties[] = {
		CL_CONTEXT_PLATFORM,
		(cl_context_properties)firstPlatformId,
		0
	};

	/*
	* clCreateContextFromType
	* Info obtained from Kronos API Documents.
	* Create an OpenCL context from a device type that identifies the specific device to use.
	* Parameters
	* Type cl_context_properties = A context property from a list of enums. we choose a device here.
	* Type cl_device_type = A bit-field that identifies the type of device. Selected ALL available opencl devices.
	* Next ones are NULL we don't use it.
	* Type cl_int = Error code if something goes wrong.
	*/
	m_context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_ALL,
		NULL, NULL, &errNum);

	if (!CheckOpenCLError(errNum, "Failed to create an OpenCL GPU or CPUL context.")) {
		return false;
	}

	return true;
}

bool OpenCLManager::CreateCommandQueue() {
	// Get number of devices
	cl_int numDevices;
	size_t retSize;
	cl_int errNum = clGetContextInfo(m_context, CL_CONTEXT_NUM_DEVICES, sizeof(numDevices), (void*)&numDevices,
		&retSize);

	// Error check on errNum.
	if (!CheckOpenCLError(errNum, "Could not get context info!")) {
		return false;
	}

	DebugLog::Info("There are " + std::to_string(numDevices) + " devices.");

	// Get list of devices
	m_deviceList = new cl_device_id[numDevices];
	errNum = clGetContextInfo(m_context, CL_CONTEXT_DEVICES, numDevices * sizeof(cl_device_id), (void*)m_deviceList,
		&retSize);

	if (!CheckOpenCLError(errNum, "Could not get device list")) {
		DebugLog::Error(" Error code" + std::to_string(errNum));

		switch (errNum) {
		case CL_INVALID_CONTEXT:
			DebugLog::Error(" (CL_INVALID_CONTEXT)");
			break;
		case CL_INVALID_VALUE:
			DebugLog::Error(" (CL_INVALID_VALUE)");
			break;
		case CL_OUT_OF_RESOURCES:
			DebugLog::Error(" (CL_OUT_OF_RESOURCES)");
			break;
		case CL_OUT_OF_HOST_MEMORY:
			DebugLog::Error(" (CL_OUT_OF_HOST_MEMORY)");
			break;
		default:
			break;
		}

		DebugLog::Error(" size = " + std::to_string(sizeof(cl_device_id)) + ";"
			+ std::to_string(retSize));
		return false;
	}

	// Get device information for each device
	cl_device_type devType;
	DebugLog::Info("Device List:");

	for (int i = 0; i < numDevices; i++) {
		std::cout << " " << m_deviceList[i] << ":";
		// device type
		errNum = clGetDeviceInfo(m_deviceList[i], CL_DEVICE_TYPE, sizeof(cl_device_type), (void*)&devType, &retSize);
		if (!CheckOpenCLError(errNum, "Error getting device info!")) {
			delete m_deviceList;
			return false;
		}
		std::cout << " type " << std::to_string(devType) << ":";

		if (devType & CL_DEVICE_TYPE_CPU) {
			std::cout << " CPU";
		}

		if (devType & CL_DEVICE_TYPE_GPU) {
			std::cout << " GPU";
		}

		if (devType & CL_DEVICE_TYPE_ACCELERATOR) {
			std::cout << " accelerator";
		}

		if (devType & CL_DEVICE_TYPE_DEFAULT) {
			std::cout << " default";
		}

		char devName[PLATFORM_NAME_SIZE];
		errNum = clGetDeviceInfo(m_deviceList[i], CL_DEVICE_NAME, 1024, (void*)devName, &retSize);

		if (!CheckOpenCLError(errNum, "Error getting device name!")) {
			delete m_deviceList;
			return false;
		}
		std::cout << " name<" << std::string(devName) << ">" << std::endl;
	}

	m_commandQueue = clCreateCommandQueueWithProperties(m_context, m_deviceList[0], 0, NULL);

	if (m_commandQueue == NULL) {
		delete m_deviceList;
		DebugLog::Error("Failed to create commandQueue for device 0");
		return false;
	}

	return true;
}

bool OpenCLManager::CreateProgram(const std::string &fileName, 
	const std::string &kernelName) {
	cl_int errNum;
	cl_program program;

	std::ifstream kernelFile(fileName, std::ios::in);

	if (!kernelFile.is_open()) {
		DebugLog::Error("Failed to open file for reading: " + fileName);
		return false;
	}

	std::ostringstream oss;

	oss << kernelFile.rdbuf();
	kernelFile.close();

	std::string srcStdStr = oss.str();
	const char* strcStr = srcStdStr.c_str();

	program = clCreateProgramWithSource(m_context, 1,
		(const char**)&strcStr,
		NULL, NULL);

	if (program == NULL) {
		DebugLog::Error("Failed to create CL program from source");
		return false;
	}

	errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

	if (errNum != CL_SUCCESS) {
		//Determine the reason of Error
		size_t logSize{ 0 };
		clGetProgramBuildInfo(program, m_deviceList[0], CL_PROGRAM_BUILD_LOG,
			0, NULL, &logSize);
		char* buildLog = new char[logSize];
		clGetProgramBuildInfo(program, m_deviceList[0], CL_PROGRAM_BUILD_LOG,
			logSize, buildLog, NULL);

		DebugLog::Error("Error in kernel:");
		DebugLog::Error(std::string(buildLog));
		delete[] buildLog;
		clReleaseProgram(program);
		return false;
	}

	// Create the kernel.
	m_kernel = clCreateKernel(program, kernelName.c_str(), NULL);

	return true;
}

cl_mem OpenCLManager::CreateInputBuffer(size_t size, void *data)
{
	cl_mem buffer{ clCreateBuffer(m_context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		size, data, NULL) };

	if (buffer != nullptr)
	{
		m_deviceInputBuffers.push_back(buffer);
	}

	return buffer;
}

cl_mem OpenCLManager::CreateOutputBuffer(size_t size)
{
	cl_mem buffer{ clCreateBuffer(m_context,
		CL_MEM_READ_WRITE,
		size, NULL, NULL) };

	if (buffer != nullptr)
	{
		m_deviceOutputBuffer = buffer;
	}

	return buffer;
}

void OpenCLManager::ReleaseMemoryObject(cl_mem memory)
{
	clReleaseMemObject(memory);
}

bool OpenCLManager::SetKernelArg(cl_uint argIndex, size_t size, const void* argValue)
{
	cl_int err{ clSetKernelArg(m_kernel, argIndex, size, argValue) };

	if (err != CL_SUCCESS)
	{
		DebugLog::Error("Failed to set kernel argument.");
		return false;
	}
	return true;
}

bool OpenCLManager::EnqueueKernel(cl_uint dim, const size_t *globalWorkSize, 
	const size_t *localWorkSize)
{
	cl_int err{ clEnqueueNDRangeKernel(m_commandQueue, m_kernel, dim, NULL,
		globalWorkSize, localWorkSize, 0, NULL, NULL) };

	if (err != CL_SUCCESS)
	{
		DebugLog::Error("Failed to enqueue the kernel.");
		return false;
	}
	return true;
}

bool OpenCLManager::ReadOutput(void *output, size_t size)
{
	cl_int err{ clEnqueueReadBuffer(m_commandQueue, m_deviceOutputBuffer, CL_TRUE, 0,
		size, output, 0, NULL, NULL) };

	if (err != CL_SUCCESS)
	{
		std::cout << err << std::endl;
		DebugLog::Error("Failed to read output.");
		return false;
	}
	return true;
}

bool OpenCLManager::CheckOpenCLError(cl_int errNum, const char* errMsg)
{
	if (errNum != CL_SUCCESS)
	{
		DebugLog::Error(std::string(errMsg));
		return false;
	}
	return true;
}