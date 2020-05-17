#include <mpi.h>
#include <stdio.h>
#include <string.h>

void splitArray(int *parts, int numbers[], int numbersSize, int numParts, int index) {
  int partsSize = numbersSize / numParts;

  int start = index * partsSize;
  int end = start + partsSize;

  if (index + 1 == numParts) { // If its the last index add the rest of the numbers
    end = numbersSize;
  }

  for (int i = start; i <= end; ++i) {
    parts[i - start] = numbers[i];
  }
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

int largest(int arr[], int n) {
  int max = arr[0];

  for (int i = 1; i < n; i++) {
    if (arr[i] > max) {
      max = arr[i];
    }
  }

  return max;
}

void printArray(int array[], int arraySize) {
  for (int i = 0; i <= arraySize; i++) {
    printf("%i, ", array[i]);
  }
}

int main(int argc, char* argv[]) {

  // INITIALIZE MPI
  int mpi_comm_size, mpi_comm_rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_comm_rank);

  if(mpi_comm_rank == 0){
  	printf("Master\n");
  }else{
  	printf("Slave\n");
  }

  MPI_Finalize();
  return 0;
}
