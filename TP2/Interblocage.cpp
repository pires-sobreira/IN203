#include <mpi.h>
#include <stdio.h>
#include <string>

int main(int argc, char* argv[])
{
    int rank, nbp, recu;
    MPI_Init(&argc, &argv);
    MPI_Comm comm;
    MPI_Comm_dup(MPI_COMM_WORLD, &comm);
    MPI_Comm_size(comm, &nbp);
    MPI_Comm_rank(comm, &rank);
    MPI_Status Status;

    if (rank == 0)
    {
        MPI_Recv(&recu, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &Status);
        MPI_Send(&rank, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        std::cout << rank << "a reçu la valeur " << recu << "du processus 1" << std::endl;
    }

    else
    {
        MPI_Recv(&recu, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &Status);
        MPI_Send(&rank, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        std::cout << rank << "a reçu la valeur " << recu << "du processus 0" << std::endl;
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}