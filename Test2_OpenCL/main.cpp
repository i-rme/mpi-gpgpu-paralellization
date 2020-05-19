
#include <iostream>

#ifdef __APPLE__

#include <OpenCL/opencl.h>

#else
#include <CL/cl.h>
#endif

char* getKernelCode(const char* fileName, size_t *kernelSourceSize);

int main() {

    const int VECTOR_LENGTH = 1000;
    int A[VECTOR_LENGTH];
    int B[VECTOR_LENGTH];
    int C[VECTOR_LENGTH];

    for(int i=0; i<VECTOR_LENGTH; i++){
        A[i] = i;
        B[i] = VECTOR_LENGTH - i;
    }

    cl_platform_id platformId = NULL;
    cl_device_id deviceId = NULL;

    clGetPlatformIDs(1, &platformId, NULL);
    clGetDeviceIDs(platformId, CL_DEVICE_TYPE_ALL, 1, &deviceId, NULL);

    cl_context context = clCreateContext(NULL, 1, &deviceId, NULL, NULL, NULL);
    cl_command_queue commandQueue = clCreateCommandQueue(context, deviceId, NULL, NULL);

    cl_mem aMemBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, VECTOR_LENGTH * sizeof(int),
            NULL, NULL);
    cl_mem bMemBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, VECTOR_LENGTH * sizeof(int),
                                       NULL, NULL);
    cl_mem cMemBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, VECTOR_LENGTH * sizeof(int),
                                       NULL, NULL);

    clEnqueueWriteBuffer(commandQueue, aMemBuffer, CL_TRUE, 0, VECTOR_LENGTH*sizeof(int), A, 0,
                        NULL, NULL);
    clEnqueueWriteBuffer(commandQueue, bMemBuffer, CL_TRUE, 0, VECTOR_LENGTH*sizeof(int), B, 0,
                         NULL, NULL);

    size_t kernelSourceSize;
    char* kernelSourceCode = getKernelCode("../vector_add.cl", &kernelSourceSize);
    printf("Code:\n%s\n",kernelSourceCode);

    cl_program program = clCreateProgramWithSource(context, 1,(const char **) &kernelSourceCode,
            (const size_t*) &kernelSourceSize, NULL);

    clBuildProgram(program, 1, &deviceId, NULL, NULL, NULL);

    cl_kernel add_kernel = clCreateKernel(program, "vector_add", NULL);

    int D = 10;
    clSetKernelArg(add_kernel, 0, sizeof(cl_mem), (void *) &aMemBuffer);
    clSetKernelArg(add_kernel, 1, sizeof(cl_mem), (void *) &bMemBuffer);
    clSetKernelArg(add_kernel, 2, sizeof(cl_mem), (void *) &cMemBuffer);
    clSetKernelArg(add_kernel, 3, sizeof(int), (void *) &D);

    size_t VECTOR_LENGTH2 = 100;
    clEnqueueNDRangeKernel(commandQueue, add_kernel, 1, NULL,  &VECTOR_LENGTH2, NULL,
            0, NULL, NULL);

    clEnqueueReadBuffer(commandQueue, cMemBuffer, CL_TRUE, 0, VECTOR_LENGTH * sizeof(int),
            C, 0, NULL, NULL);

    for (int i=0; i<VECTOR_LENGTH;i++)
        printf("%d--> %d + %d = %d\n",i, A[i], B[i], C[i]);

    return 0;
}

char* getKernelCode(const char* fileName, size_t *kernelSourceSize){
    FILE *kernelSourceFile;
    char *kernelSourceCode;

    kernelSourceFile = fopen(fileName, "r");
    if(!kernelSourceFile){
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }

    fseek(kernelSourceFile, 0, SEEK_END);
    *kernelSourceSize = ftell(kernelSourceFile);
    rewind(kernelSourceFile);
    kernelSourceCode = new char[*kernelSourceSize];
    fread(kernelSourceCode, 1, *kernelSourceSize, kernelSourceFile);
    fclose(kernelSourceFile);

    return kernelSourceCode;
}