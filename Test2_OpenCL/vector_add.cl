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

         int currentNum = numbers[i];
         int isFinished = 0;

         if (currentNum <= 1){
            isFinished = 1;
            continue;
         }
         if (currentNum % 2 == 0 && currentNum > 2){
            isFinished = 1;
            continue;
         }
         for(int j = 3; j < currentNum / 2; j+= 2){
             if (currentNum % j == 0){
                isFinished = 1;
                break;
             }

         }
         if (isFinished == 0){
            primes[primesSize++] = numbers[i];
         }

    }


    //results[index] = primesSize;
    results[index] = primes[1];

}
