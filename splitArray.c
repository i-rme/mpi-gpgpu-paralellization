#include <stdio.h>
#include <string.h>

void splitArray(int *parts, int numbers[], int num, int numParts, int index) {    
    for(int i = 0; i < num; ++i)
        parts[i] = numbers[i];
}


int main(void)
{
    printf("Hello: ");
    
    //VARIABLES
    int numParts = 1;     //How many parts we want
    int index = 0;        //What part we want
    
    //INPUT
    int numbers[] = { 1, 2, 3, 4, 5, 6 };
    int numbersSize = sizeof numbers / sizeof numbers[0];

    //OUTPUT
    const int partsSize = numbersSize / numParts;
    int parts[partsSize];
    memset(parts, 0, sizeof parts);    //Initialize array with 0s
    
    //FUNCTION
    splitArray(parts, numbers, numbersSize, numParts, index);
    
    //SHOW RESULT
    for (int i=0; i<partsSize; i++){
        printf("%i, ", parts[i]);
    }
    
    
    return 0;
}
