#include <mpi.h>
#include <stdio.h>

int main (int argc, char* argv[]) {
	int size, rank, nameLength;
	char name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size); //Obtain the size of each process
	MPI_Comm_rank(MPI_COMM_WORLD, &size); //Obtain the id of each process
	MPI_Get_processor_name(name, &nameLength); //Length of the string that we have

	if(rank==0) printf("First program in MPI\n\n");

	MPI_Barrier(MPI_COMM_WORLD);

	printf("Communicator size: %i\n - In process %i\n", size, rank, name); //Creo que tengo algo mal

	MPI_Finalize();

	return 0;
}