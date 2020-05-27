#include <mpi.h>
#include <stdio.h>
#include <string.h>

int splitArray(int *parts, int numbers[], int numbersSize, int numParts, int index) {
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
  int primesSize = 0;

  for (int i = 0; i < numbersSize; ++i) {
    if (isPrime(numbers[i])) {
      primes[primesSize++] = numbers[i];
    }
  }

  return primesSize;
}

int largestNumber(int arr[], int n) {
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
  for (int i = 0; i < arraySize; i++) {
    printf("%i, ", array[i]);
  }
}

int main(int argc, char* argv[]) {

  // INITIALIZE MPI
  int mpi_comm_size, mpi_comm_rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_comm_rank);
  MPI_Status status;


  if(mpi_comm_rank == 0){

    // INPUT
    int numbers[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12,
                       13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
                       26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
                       39, 40, 41, 42, 43, 44, 45, 46, 47};

    for (int i = 1; i < mpi_comm_size; i++) {
      // VARIABLES
      int index = i;    // What part we want

      int numbersSize = sizeof numbers / sizeof numbers[0];

      int numParts = mpi_comm_size;

      // OUTPUT
      int partsSize = numbersSize / numParts;
      int parts[partsSize];
      memset(parts, 0, sizeof parts); // Initialize array with 0s

      // FUNCTION
      
      int numeroSplit = splitArray(parts, numbers, numbersSize, numParts, index);

      MPI_Send(&numeroSplit, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
      MPI_Send(&parts, numeroSplit, MPI_INT, i, 1, MPI_COMM_WORLD);

    }

    // MASTER IS WORKING TOO

    int index = 0;
    int numbersSize = sizeof numbers / sizeof numbers[0];
    int numParts = mpi_comm_size;
    int partsSize = numbersSize / numParts;
    int parts[partsSize];
    memset(parts, 0, sizeof parts);
    int numeroSplit = splitArray(parts, numbers, numbersSize, numParts, index);

    int primes[10];
    int primesSize = removeNonPrimes(primes, parts, numeroSplit);
    int largest = largestNumber(primes, primesSize);

    // RECEIVING FROM SLAVES

    int results[mpi_comm_size];
    results[0] = largest;

    for (int i=1; i<mpi_comm_size; i++){
      int largest = 0;
      MPI_Recv(&largest, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
      //printf("Message received from %i: Largest is %i \n", i, largest);
      results[i] = largest;
    }

    // PRINT RESULT ARRAY
    int absoluteLargest = largestNumber(results, mpi_comm_size);

    if(absoluteLargest == -1){
      printf("They were no primes in the numbers provided.");
    }else{
      printf("The largest prime number of the array is %i. ", absoluteLargest);
    }



  }else{
    
    // RECEIVING WORK    
    int numeroSplit = 0;
    MPI_Recv(&numeroSplit, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    int parts[numeroSplit];
    MPI_Recv(&parts, numeroSplit, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

    // REMOVING NON PRIMES
    int primes[10];
    int primesSize = removeNonPrimes(primes, parts, numeroSplit);

    // SENDING LARGEST PRIME
    int largest = largestNumber(primes, primesSize);
    //printf("DEBUG: El worker %i hace send ", mpi_comm_rank);
    MPI_Send(&largest, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    //printf("DEBUG: El worker %i acaba de hacer send ", mpi_comm_rank);


  }

  MPI_Finalize();
  return 0;
}
