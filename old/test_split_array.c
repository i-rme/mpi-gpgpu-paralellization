int main(void)
{
    int array[6] = {1,2,3,4,5,6}
    int size = sizeof(array)/sizeof(array[0]);
    int nsplits = 2; //Split in two

    for (int i = 0; i<nsplits; i++){ //Si es par
        int *split = malloc((size/nsplits) * sizeof(int));
        int newArray[size/nsplits];
        memcpy(split, newArray, 3 * sizeof(int));
    }
    
    return 0;
}