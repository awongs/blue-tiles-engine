#include "OpenCLManager.h"

//Magic number to store the platform name.
static constexpr int PLATFORM_NAME_SIZE = 1024;
OpenCLManager::OpenCLManager() {

}

OpenCLManager::~OpenCLManager() {

}

/* Create an OpenCL context.
 * Selects first platform.
 * CPU or GPU.  
*/
cl_context OpenCLManager::createContext() {
    cl_platform_id firstPlatformId;
    cl_uint numPlatforms;
    cl_int errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);

    //Error checking on errNum.
    if(!CheckOpenCLError(errNum, "Failed to find any OpenCL platforms.")) {
        return NULL;
    }

    if(numPlatforms <=0) {
        DebugLog::Error("Failed to create context");
    }

    DebugLog::Info("platforms in total" + std::toString(numPlatforms));

    //Get information about the platform
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
    errNum = clGetPlatformInfo(firstPlatformId, CL_PLATFORM_NAME
                                sizeof(pname), (void *)pname, &retsize);

    //Error check to see if we can get any platform information.
    if(!CheckOpenCLError(errNum, "Could not get platform info")) {
        return NULL;
    }

    DebugLog::Info("\n Selected platform <" + std::string(pname) + "> \n");

    //Creates an OpenCL context on the platform.
    cl_context_properties contextProperties[] = {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)firstPlatformId,
        0
    };

    cl_context context = NULL;

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
    context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_ALL,
                                      NULL, NULL, &errNum);

    if(!CheckOpenCLError(errNum, "Failed to create an OpenCL GPU or CPUL context.")) {
        return NULL;
    }

    return context;
}
