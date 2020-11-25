#include <iostream>
#include <mpi.h>

int main(int nargs, char *argv[])
{   
    //Variables that are used by MPI
    int rank, nbp;
    int recv;
    MPI_Init(&nargs, &argv);
    MPI_Comm comm;
    MPI_Comm_dup(MPI_COMM_WORLD, &comm);
    MPI_Comm_size(comm, &nbp);
    MPI_Comm_rank(comm, &rank);
    MPI_Status Stat; 

    if(rank == 0)
    {
        MPI_Send(&rank , 1, MPI_INT , 1, 1, MPI_COMM_WORLD);
        MPI_Recv(&recv , 1, MPI_INT , 1, 1, MPI_COMM_WORLD , &Stat);
        std::cout << rank << " a reçu "<<recv << std::endl;
    }

    else 
    {
        MPI_Recv(&recv , 1, MPI_INT , 0, 1, MPI_COMM_WORLD , &Stat);
        MPI_Send(&rank , 1, MPI_INT , 0, 1, MPI_COMM_WORLD);
        std::cout << rank << " a reçu "<< recv << std::endl;

    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}