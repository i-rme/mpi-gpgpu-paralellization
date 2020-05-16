#include <stdio.h>
#include <string.h>

void splitArray(int *parts, int numbers[], int numbersSize, int numParts, int index) {
    
    int partsSize = numbersSize / numParts;
    
    int start = index * partsSize;
    int end = start + partsSize;
     
    if( index+1 == numParts ){     // If its the last index add the rest of the numbers
        end = numbersSize;
    }
    
    for(int i = start; i <= end; ++i){
        parts[i-start] = numbers[i];
    }
}


int isPrime(int number){

    if(number == 0 || number == 1 || number == 4){    // Special cases
        return 0;
    }
    
    for (int i = 2; i < number / 2; i++){    // General algorithm
        if(number % i == 0) return 0;
    }
    
    return 1;
}

int largest(int arr[], int n){
    
  int max = arr[0];
    
  for (int i=1; i<n; i++){
    if(arr[i]>max){
      max=arr[i];
    }
  }
    
  return max;
    
}


int main(void)
{
    //VARIABLES
    int numParts = 2;     //How many parts we want
    int index = 0;        //What part we want
    
    //INPUT
    int numbers[] = { 1, 2, 3, 4, 5, 6 };
    int numbersSize = sizeof numbers / sizeof numbers[0];

    //OUTPUT
    int partsSize = numbersSize / numParts;
    int parts[partsSize];
    memset(parts, 0, sizeof parts);    //Initialize array with 0s
    
    //FUNCTION
    splitArray(parts, numbers, numbersSize, numParts, index);
    
    printf("Largest number in given array is %i. \n", largest(parts, partsSize));
    
    printf("Hello, world!\n");
    printf("%i", isPrime(7));
    return 0;
}
