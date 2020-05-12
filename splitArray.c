// Compiled with: gcc -std=c11 -Wall -pthread -lm

#include <stdio.h>

void splitArray(char *part, int numbers[], int num, int numParts, int index) {
    for(int i = 0; i < num; ++i)
        part[i] = numbers[i];
}


int main(void)
{
    
    int numbers[] = { 1, 2, 3, 4, 5, 6 };
    int numbersSize = sizeof numbers / sizeof numbers[0];

    printf("%i, ", numbersSize);
    
    char part[3] = {0};
    splitArray(part, numbers, 3, 2, 0);
    

    for (int i=0; i<3; i++){
        printf("%i, ", part[i]);
    }
    
    
    return 0;
}
