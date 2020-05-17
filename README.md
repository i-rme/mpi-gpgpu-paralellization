# mpi-gpgpu-paralellization

## main.c: Main implementantion 
## mpi.cpp: Paralellized using MPI

### How to execute mpi.cpp
mpic++ -o mpi mpi.cpp
mpirun -n 4 -hosts mpi-1 ./mpi

https://codebunk.com/b/

// INPUT: The array, the number of parts to split, the index of the part to return
// OUTPUT: Array containing a part of the original array
int * splitArray(int array[], int numParts, int index)

//INPUT: An array filled with integers
//OUTPUT: An array filled with integers that are prime, null if no primes
int * filterPrimes(int array[])

//INPUT: An array filled with integers
//OUTPUT: The highest value of the array, null if empty
int findMaximum(int array[])


TAREAS:

-> MASTER: Dividir vector entre workers
-> SLAVE: Buscar numeros primos en el array
-> SLAVE: Buscar el máximo
-> MASTER: Buscar el máximo de todos los máximos
