__kernel void return_largest_prime(
                        __global const int *numbers,
                        __global const int *start,
                        __global const int *end,
                        __global int *results
                        ) {

      int index = get_global_id(0); //returns the global ID of the workitem in the first dimension.
      //Each workitem needs to know its global ID and then use it to access the data.

      int partsSize = end[index] - start[index];

      int primes[100];
      int primesSize = 0;

      // Loop into our batch of numbers and stores primes in array
      for (int i = start[index]; i < end[index]; ++i) {

        int currentNum = numbers[i];
        int isFinished = 0;

        if (currentNum <= 1) {
          isFinished = 1;
          continue;
        }
        if (currentNum % 2 == 0 && currentNum > 2) {
          isFinished = 1;
          continue;
        }
        for (int j = 3; j < currentNum / 2; j += 2) {
          if (currentNum % j == 0) {
            isFinished = 1;
            break;
          }

        }
        if (isFinished == 0) {
          primes[primesSize++] = numbers[i];
        }

      }

      // Finds the maximum number from the prime array and stores it on results
      int max = primes[0];

      for (int k = 1; k < primesSize; k++) {
        if (primes[k] > max) {
          max = primes[k];
        }
      }

      results[index] = max;

}
