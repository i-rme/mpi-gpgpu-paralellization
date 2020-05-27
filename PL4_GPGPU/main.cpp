#include <iostream>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else

#include <CL/cl.h>

#endif

char *getKernelCode(const char *fileName, size_t *kernelSourceSize);

int getStart(int numbersSize, int numParts, int index);

int getEnd(int numbersSize, int numParts, int index);

int largestNumber(int arr[], int n);

int main() {

    const int numParts = 6; //numero de cores a usar
    const int numbersSize = 48;

    int results[numParts];
    int start[numParts];
    int end[numParts];

    int numbers[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                     13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
                     26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
                     39, 40, 41, 42, 43, 44, 45, 46, 47};

    for (int i = 0; i < numParts; i++) {
        start[i] = getStart(numbersSize, numParts, i);
        end[i] = getEnd(numbersSize, numParts, i);
    }


    cl_platform_id platformId = NULL;
    cl_device_id deviceId = NULL;

    clGetPlatformIDs(1, &platformId, NULL);
    clGetDeviceIDs(platformId, CL_DEVICE_TYPE_ALL, 1, &deviceId, NULL);

    cl_context context = clCreateContext(NULL, 1, &deviceId, NULL, NULL, NULL);
    cl_command_queue commandQueue = clCreateCommandQueue(context, deviceId, NULL, NULL);

    cl_mem numbersMemBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, numbersSize * sizeof(int),
                                             NULL, NULL);
    cl_mem startMemBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, numParts * sizeof(int),
                                           NULL, NULL);
    cl_mem endMemBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, numParts * sizeof(int),
                                         NULL, NULL);
    cl_mem resultsMemBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, numParts * sizeof(int),
                                             NULL, NULL);

    clEnqueueWriteBuffer(commandQueue, numbersMemBuffer, CL_TRUE, 0, numbersSize * sizeof(int), numbers, 0,
                         NULL, NULL);
    clEnqueueWriteBuffer(commandQueue, startMemBuffer, CL_TRUE, 0, numParts * sizeof(int), start, 0,
                         NULL, NULL);
    clEnqueueWriteBuffer(commandQueue, endMemBuffer, CL_TRUE, 0, numParts * sizeof(int), end, 0,
                         NULL, NULL);

    size_t kernelSourceSize;
    char *kernelSourceCode = getKernelCode("../return_largest_prime.cl", &kernelSourceSize);
    //printf("Code:\n%s\n",kernelSourceCode);

    cl_program program = clCreateProgramWithSource(context, 1, (const char **) &kernelSourceCode,
                                                   (const size_t *) &kernelSourceSize, NULL);

    clBuildProgram(program, 1, &deviceId, NULL, NULL, NULL);

    cl_kernel add_kernel = clCreateKernel(program, "return_largest_prime", NULL);

    clSetKernelArg(add_kernel, 0, sizeof(cl_mem), (void *) &numbersMemBuffer);
    clSetKernelArg(add_kernel, 1, sizeof(cl_mem), (void *) &startMemBuffer);
    clSetKernelArg(add_kernel, 2, sizeof(cl_mem), (void *) &endMemBuffer);
    clSetKernelArg(add_kernel, 3, sizeof(cl_mem), (void *) &resultsMemBuffer);

    size_t unsigned_numParts = numParts;
    clEnqueueNDRangeKernel(commandQueue, add_kernel, 1, NULL, &unsigned_numParts, NULL,
                           0, NULL, NULL);

    clEnqueueReadBuffer(commandQueue, resultsMemBuffer, CL_TRUE, 0, numParts * sizeof(int),
                        results, 0, NULL, NULL);


/*
    //Debug to see the result of each worker
    for (int i=0; i<numParts;i++){
        printf("%d--> %d = %d\n",i, numbers[i], results[i]);
    }
*/

    int max = largestNumber(results, numParts);
    printf("The biggest prime number is: %d", max);

    return 0;
}

char *getKernelCode(const char *fileName, size_t *kernelSourceSize) {
    FILE *kernelSourceFile;
    char *kernelSourceCode;

    kernelSourceFile = fopen(fileName, "r");
    if (!kernelSourceFile) {
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

int getStart(int numbersSize, int numParts, int index) {
    int partsSize = numbersSize / numParts;

    int start = index * partsSize;

    return start;
}

int getEnd(int numbersSize, int numParts, int index) {
    int partsSize = numbersSize / numParts;

    int start = index * partsSize;
    int end = start + partsSize;

    if (index + 1 == numParts) { // If its the last index add the rest of the numbers
        end = numbersSize;
    }

    return end;
}

int largestNumber(int arr[], int n) {
    int max = arr[0];

    for (int i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    return max;
}

void printArray(int array[], int arraySize) {
    for (int i = 0; i < arraySize; i++) {
        printf("%i, ", array[i]);
    }
}