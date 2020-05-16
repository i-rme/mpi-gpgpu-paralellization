#include <stdio.h>
#include <string.h>

void splitArray(int *parts, int numbers[], int numbersSize, int numParts, int index) {
    
    int partsSize = numbersSize / numParts;
    
    int start = index * partsSize;
    int end = start + partsSize;
     
    if( index+1 == numParts ){     //If its the last index add the rest of the numbers
        end = numbersSize;
    }
    
    for(int i = start; i <= end; ++i){
        parts[i-start] = numbers[i];
    }
}


int main(void)
{
    
    //VARIABLES
    int numParts = 4;     //How many parts we want
    int index = 3;        //What part we want
    
    //INPUT
    int numbers[] = { 1, 2, 3, 4, 5, 6 };
    int numbersSize = sizeof numbers / sizeof numbers[0];

    //OUTPUT
    int partsSize = numbersSize / numParts;
    int parts[partsSize];
    memset(parts, 0, sizeof parts);    //Initialize array with 0s
    
    //FUNCTION
    splitArray(parts, numbers, numbersSize, numParts, index);
    
    //SHOW RESULT
    
    if( index+1 == numParts ){         //If its the last index print the rest of the numbers
        partsSize = (numbersSize % numParts) +1;
    }
    
    for (int i=0; i<partsSize; i++){
        printf("%i, ", parts[i]);
    }
    
    
    return 0;
}
