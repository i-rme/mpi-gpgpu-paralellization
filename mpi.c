#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int createRandomVector2(int size){
        return 288;
};

int createRandom(int max){
        int r;
        srand (time(NULL));
        r = rand() % max;
        return r;
};

int* createRandomVector(int size){
        int *vector = (int*)malloc(size*32);
        for(int i=0 ; i<=size ; i++){
                vector[i] = 3;
        }
        free(vector);
        return vector;
};

void printVector(int vector[], int size){
        for(int i=0 ; i<=size ; i++){
                printf("%i, ", vector[i]);
        }
};


int main(int argc, char* argv[]){

        int size, rank;

        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        //MPI_Get_processor_name(name, &nameLength);

        if(rank == 0){
                printf("Master\n");
                int *vector = createRandomVector(7);
                printVector(vector, 7);
        }else{
                printf("Slave\n");
        }

        printf("Communicator size: %i", size);
        //printf("Funcion: %i", createRandomVector(2));


        MPI_Finalize();
        return 0;

}
