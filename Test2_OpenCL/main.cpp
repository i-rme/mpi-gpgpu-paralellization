#include <iostream>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <time.h>

char *getKernelCode(const char *fileName, size_t *kernelSourceSize);
int getStart(int numbersSize, int numParts, int index);
int getEnd(int numbersSize, int numParts, int index);
int largestNumber(int arr[], int n);
void showDevices();
int askPlatform();
int askDevice();

int main() {

    showDevices();                  // Show beautiful device list

    int platformIndex, deviceIndex;
    platformIndex = askPlatform();  // Nvidia, Intel, or AMD
    deviceIndex = askDevice();      // Device ID

    // OPTION 1 START, fixed array of numbers
    //const int numbersSize = 48;     // How many numbers we want to work with in total
    //int numbers[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
    //                26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47};
    // OPTION 1 END, fixed array of number

    // OPTION 2 START, big array of numbers
    const int numbersSize = 100000;     // How many numbers we want to work with in total
    int numbers[numbersSize];
    for (int n = 0; n < numbersSize; ++n) { numbers[n] = n; }   // Fill some numbers
    // OPTION 2 END, big array of numbers

    //const int numParts = 6;         // Number of parts to split our work into
    const int numParts = (numbersSize/5000)+1;         // Number of parts to split our work into

    int results[numParts];
    int start[numParts];
    int end[numParts];

    clock_t time_begin = clock();    //Start measuring execution time

    for (int i = 0; i < numParts; i++) {
        start[i] = getStart(numbersSize, numParts, i);
        end[i] = getEnd(numbersSize, numParts, i);
    }

    //Setup OpenCL
    cl_uint numPlatforms;
    cl_platform_id *platformId;

    clGetPlatformIDs(0, NULL, &numPlatforms);
    platformId = new cl_platform_id[numPlatforms];
    clGetPlatformIDs(numPlatforms, platformId, NULL);

    cl_uint numDevices;
    cl_device_id *devices;
    clGetDeviceIDs(platformId[platformIndex], CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
    devices = new cl_device_id[numDevices];
    clGetDeviceIDs(platformId[platformIndex], CL_DEVICE_TYPE_ALL, numDevices, devices, NULL);

    // Printing device selection
    const int deviceLength = 100;
    char deviceName[deviceLength];
    clGetDeviceInfo(devices[deviceIndex], CL_DEVICE_NAME, deviceLength, deviceName, NULL);
    printf("\nYou have selected %s\n", deviceName);

    cl_context context = clCreateContext(NULL, 1, &devices[deviceIndex], NULL, NULL, NULL);
    cl_command_queue commandQueue = clCreateCommandQueue(context, devices[deviceIndex], NULL, NULL);

    //Create the memory buffers
    cl_mem numbersMemBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, numbersSize * sizeof(int),
                                             NULL, NULL);
    cl_mem startMemBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, numParts * sizeof(int),
                                           NULL, NULL);
    cl_mem endMemBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, numParts * sizeof(int),
                                         NULL, NULL);
    cl_mem resultsMemBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, numParts * sizeof(int),
                                             NULL, NULL);
    //Copy the data to the inputs
    clEnqueueWriteBuffer(commandQueue, numbersMemBuffer, CL_TRUE, 0, numbersSize * sizeof(int), numbers, 0,
                         NULL, NULL);
    clEnqueueWriteBuffer(commandQueue, startMemBuffer, CL_TRUE, 0, numParts * sizeof(int), start, 0,
                         NULL, NULL);
    clEnqueueWriteBuffer(commandQueue, endMemBuffer, CL_TRUE, 0, numParts * sizeof(int), end, 0,
                         NULL, NULL);

    //Define our kernel
    size_t kernelSourceSize;
    char *kernelSourceCode = getKernelCode("../return_largest_prime.cl", &kernelSourceSize);
    //printf("Code:\n%s\n",kernelSourceCode);

    //Compile the kernel
    cl_program program = clCreateProgramWithSource(context, 1, (const char **) &kernelSourceCode,
                                                   (const size_t *) &kernelSourceSize, NULL);

    clBuildProgram(program, 1, &devices[deviceIndex], NULL, NULL, NULL);

    cl_kernel add_kernel = clCreateKernel(program, "return_largest_prime", NULL);

    //Execute the kernel
    clSetKernelArg(add_kernel, 0, sizeof(cl_mem), (void *) &numbersMemBuffer);
    clSetKernelArg(add_kernel, 1, sizeof(cl_mem), (void *) &startMemBuffer);
    clSetKernelArg(add_kernel, 2, sizeof(cl_mem), (void *) &endMemBuffer);
    clSetKernelArg(add_kernel, 3, sizeof(cl_mem), (void *) &resultsMemBuffer);

    //Read back the results
    size_t unsigned_numParts = numParts;
    clEnqueueNDRangeKernel(commandQueue, add_kernel, 1, NULL, &unsigned_numParts, NULL,
                           0, NULL, NULL);

    clEnqueueReadBuffer(commandQueue, resultsMemBuffer, CL_TRUE, 0, numParts * sizeof(int),
                        results, 0, NULL, NULL);

    //Wait for everything to finish
    clFinish(commandQueue);

/*
    //Debug to see the result of each worker
    for (int i=0; i<numParts;i++){
        printf("%d--> %d = %d\n",i, numbers[i], results[i]);
    }
*/

    int max = largestNumber(results, numParts);

    clock_t time_end = clock();
    double time_spent = (double)(time_end - time_begin) / CLOCKS_PER_SEC;


    printf("\nThe biggest prime number is: %d", max);
    printf("\nTime spent: %ims", (int)floor(time_spent*1000));

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

int askPlatform() {
    int i;
    printf("[ ] Enter the platform index you want to use: ");
    scanf("%d", &i);
    return i;
}

int askDevice() {
    int i;
    printf("       ( ) Enter the device index you want to use: ");
    scanf("%d", &i);
    return i;
}

void showDevices() {
    printf("Welcome, here is the list of platform, devices, and its index numbers: \n");

    cl_uint numPlatforms;
    cl_platform_id *platformId;

    clGetPlatformIDs(0, NULL, &numPlatforms);

    platformId = new cl_platform_id[numPlatforms];
    clGetPlatformIDs(numPlatforms, platformId, NULL);

    for (int i = 0; i < numPlatforms; i++) {
        const int nameLength = 100;
        char platformName[nameLength];
        clGetPlatformInfo(platformId[i], CL_PLATFORM_NAME, nameLength, platformName, NULL);
        printf("[%d]. %s\n", i, platformName);

        cl_uint numDevices;
        cl_device_id *devices;
        clGetDeviceIDs(platformId[i], CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
        devices = new cl_device_id[numDevices];
        clGetDeviceIDs(platformId[i], CL_DEVICE_TYPE_ALL, numDevices, devices, NULL);

        for (int j = 0; j < numDevices; j++) {
            const int deviceLength = 100;
            char deviceName[deviceLength];
            cl_uint deviceMaxComputeUnits;
            clGetDeviceInfo(devices[j], CL_DEVICE_NAME, deviceLength, deviceName, NULL);
            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint),
                            &deviceMaxComputeUnits, NULL);
            printf("        (%d). %s with %u CU\n", j, deviceName, deviceMaxComputeUnits);
        }
    }

    delete[] platformId;

}
