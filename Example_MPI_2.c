#include <mpi.h>
#include <stdio.h>

int main (int argc, char* argv[]) {
	int size, rank, nameLength;
	char name[MPI_MAX_PROCESSOR_NAME];

	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(name, &nameLength);

	if(rank==0){
		printf("First program in MPI\n");
		printf("Group size: %i\n", size);		
		for(int i=1; i<size; i++){
			int value = i*i+1;
			MPI_Send(&value, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
		}
		for (int i=1; i<size; i++){
			int value=0;
			MPI:Recv(&value, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
			printf("Message received from %1: %i - Message at %i (%s)\n", i, value, rank, name);
		}
	} else{
		int buffer = -1;

		MPI_Recv(&buffer, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		printf("Message received: %i - Message at %i (%s)\n", buffer, rank, name);

		buffer = -buffer;
		MPI_Send(&buffer, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	}
	MPI_Finelize();
	return 0;
}