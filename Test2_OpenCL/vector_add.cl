__kernel void vector_add(
                        __global const int *numbers,
                        __global const int *start,
                        __global const int *end,
                        __global int *results,
                        int D
                        ) {

    int index = get_global_id(0);
    int partsSize = end[index] - start[index];

    int primes[10];

    int primesSize = 0;

      for (int i = start[index]; i < end[index]; ++i) {

          if (number == 0 || number == 1 || number == 4) { // Special cases
            primes[primesSize++] = numbers[i];
          }

          for (int i = 2; i < number / 2; i++) { // General algorithm
            if (number % i == 0)
              primes[primesSize++] = numbers[i];
          }
      }


      int max = primes[primesSize];

        for (int i = 1; i < primesSize; i++) {
          if (primes[i] > max) {
            max = primes[i];
          }
        }

   // results[index] = max;
   results[index] = 5;
}
