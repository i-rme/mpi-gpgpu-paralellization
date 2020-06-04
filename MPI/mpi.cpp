#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/*
  Compile using    mpic++ -o mpi mpi.cpp
  Run using        mpirun -n <numberOfWorkers> -hosts <listOfHosts> ./mpi
  Example          mpirun -n 6 -hosts mpi-1 ./mpi
*/

int splitArray(int *parts, int numbers[], int numbersSize, int numParts, int index) {
/*
  splitArray(): Given an array, it gets a slice.
  *parts: Output array, contains the slice
  partsSize: Number of elements in the parts array
  numbers[]: Input array, contains all numbers
  numbersSize: Number of elements in the numbers array
  numParts: Number of slices to divide the array
  index: What slice we want
*/
  int partsSize = numbersSize / numParts;

  int start = index * partsSize;
  int end = start + partsSize;

  if (index + 1 == numParts) { // If its the last index add the rest of the numbers
    end = numbersSize;
  }

  for (int i = start; i <= end; ++i) {
    parts[i - start] = numbers[i];
  }

  return end - start;
}

int isPrime(int number) {
/*
  isPrime(): Returns 1 if prime, 0 if not
  number: Input number
*/
  if (number == 0 || number == 1 || number == 4) { // Special cases
    return 0;
  }

  for (int i = 2; i < number / 2; i++) { // General algorithm
    if (number % i == 0)
      return 0;
  }

  return 1;
}

int removeNonPrimes(int *primes, int numbers[], int numbersSize) {
/*
  removeNonPrimes(): Given an array, returns just the primes.
  *primes: Output array, contains the primes
  numbers[]: Input array, contains numbers
  numbersSize: Number of elements in the numbers array
*/
  int primesSize = 0;

  for (int i = 0; i < numbersSize; ++i) {
    if (isPrime(numbers[i])) {
      primes[primesSize++] = numbers[i];
    }
  }

  return primesSize;
}

int largestNumber(int arr[], int n) {
/*
  largestNumber(): Given an array, returns the largest number.
  arr[]: Input array, contains numbers
  n: Number of elements in the numbers array
*/
  int max = arr[0];

  for (int i = 1; i < n; i++) {
    if (arr[i] > max) {
      max = arr[i];
    }
  }

  if(n == 0){
    return -1;  // No largest number
  }

  return max;
}

void printArray(int array[], int arraySize) {
/*
  printArray(): Given an array, it prints its elements.
  array[]: Input array, contains numbers
  arraySize: Number of elements in the input array
*/
  for (int i = 0; i < arraySize; i++) {
    printf("%i, ", array[i]);
  }
}

int main(int argc, char* argv[]) {
/*
  main(): Defines work, splits it, sends work to nodes, receives answer, outputs biggest answer.
  numbers[]: Input array, contains numbers
*/

  // INITIALIZE MPI
  int mpi_comm_size, mpi_comm_rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_comm_rank);
  MPI_Status status;


  if(mpi_comm_rank == 0){ // If we are runnin on master

    printf("MPI innitialized sucessfully, starting to work...\n");
    clock_t time_begin = clock();    //Start measuring execution time


    // INPUT

    // OPTION 1 START, fixed array of numbers
    //int numbers[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
    //                26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47};
    // OPTION 1 END, fixed array of number

    // OPTION 2 START, big array of numbers
    const int arraySize = 100000;     // How many numbers we want to work with in total
    int numbers[arraySize];
    for (int n = 0; n < arraySize; ++n) { numbers[n] = n; }   // Fill some numbers
    // OPTION 2 END, big array of numbers


    for (int i = 1; i < mpi_comm_size; i++) { //Preparing and sending the slices to the workers

      int index = i;    // What slice we want
      int numbersSize = sizeof numbers / sizeof numbers[0];
      int numParts = mpi_comm_size;

      // OUTPUT
      int partsSize = numbersSize / numParts;
      int parts[partsSize];
      memset(parts, 0, sizeof parts); // Initialize array with 0s

      // FUNCTION
      
      int numeroSplit = splitArray(parts, numbers, numbersSize, numParts, index); // Number of elements in the slice

      MPI_Send(&numeroSplit, 1, MPI_INT, i, 1, MPI_COMM_WORLD);     //Sending the number of elements
      MPI_Send(&parts, numeroSplit, MPI_INT, i, 1, MPI_COMM_WORLD); //Sending the slice

    }

    // MASTER HAS TO WORK TOO
    int index = 0;  // First slice
    int numbersSize = sizeof numbers / sizeof numbers[0];
    int numParts = mpi_comm_size;
    int partsSize = numbersSize / numParts;
    int parts[partsSize];
    memset(parts, 0, sizeof parts);   // List of numbers gets initialized
    int numeroSplit = splitArray(parts, numbers, numbersSize, numParts, index); // Number of elements in the slice

    int primes[1024];  // List of primes gets initialized
    int primesSize = removeNonPrimes(primes, parts, numeroSplit);
    int largest = largestNumber(primes, primesSize);  

    // RECEIVING FROM SLAVES
    int results[mpi_comm_size];    // Results holds the maximum prime from each worker
    results[0] = largest;   // We store the result for master

    for (int i=1; i<mpi_comm_size; i++){
      int largest = 0;
      MPI_Recv(&largest, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
      //printf("Message received from %i: Largest is %i \n", i, largest);
      results[i] = largest;   // We store the result for each worker
    }

    // PRINT RESULT ARRAY
    int absoluteLargest = largestNumber(results, mpi_comm_size);   // We get the absolute maximum

    if(absoluteLargest == -1){
      printf("They were no primes in the numbers provided.");
    }else{
      printf("The largest prime number of the array is %i. ", absoluteLargest);
    }

    clock_t time_end = clock();
    double time_spent = (double)(time_end - time_begin) / CLOCKS_PER_SEC;
    printf("\nTime spent: %ims\n", (int)(time_spent*1000));



  }else{  // Here is where the workers do its thing
    
    // RECEIVING WORK
    int numeroSplit = 0;  // Initialization
    MPI_Recv(&numeroSplit, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);  // Getting the array size from the master
    int parts[numeroSplit];   // Innitialize array
    MPI_Recv(&parts, numeroSplit, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);   // Receive slice

    // REMOVING NON PRIMES
    int primes[1024];   // Innitialize array
    int primesSize = removeNonPrimes(primes, parts, numeroSplit);   // Get the number of primes found

    // SENDING LARGEST PRIME
    int largest = largestNumber(primes, primesSize);   // Get largest prime
    //printf("DEBUG: Worker %i is sending ", mpi_comm_rank);
    MPI_Send(&largest, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);   // Send largest prime
    //printf("DEBUG: Worker %i has finished its work ", mpi_comm_rank);

  }

  MPI_Finalize();
  return 0;
}
