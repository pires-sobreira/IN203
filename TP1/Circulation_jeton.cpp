#include <iostream>
#include <mpi.h>

int main(int nargs, char *argv[])
{
    int rank, nbp, val=0, tag =1, sum [] = {9,8,7,6,5,4,3,2,1,0,1,2,3,4,5,6};
    MPI_Init(&nargs, &argv);
    MPI_Comm comm;
    MPI_Comm_dup(MPI_COMM_WORLD, &comm);
    MPI_Comm_size(comm, &nbp);
    MPI_Comm_rank(comm, &rank);
    MPI_Status Stat; 

    if(rank == 0)
    {
		val = val + sum[rank];
        MPI_Send(&val , 1, MPI_INT , 1 , tag, MPI_COMM_WORLD);
        std::cout << "Le processus " << rank << " a envoyé " << val << " à le processus"<< 1 <<".\n"<< std::endl;
        MPI_Recv(&val , 1, MPI_INT ,  nbp-1, tag, MPI_COMM_WORLD , &Stat);
        std::cout << "Le processus " << rank << " a reçu " << val << " de le processus"<< nbp-1<<".\n"<< std::endl;
    }

    else if(rank == nbp-1)
    {
        MPI_Recv(&val , 1, MPI_INT , rank-1 , tag, MPI_COMM_WORLD , &Stat);
        std::cout << "Le processus " << rank << " a reçu " << val << " de le processus"<< rank-1<<".\n"<< std::endl;
        val= val + sum[rank];
        MPI_Send(&val , 1, MPI_INT , 0 , tag, MPI_COMM_WORLD);
        std::cout << "Le processus " << rank << " a envoyé " << val << " à le processus"<< 0 <<".\n"<< std::endl;
    }

    else
    {
        MPI_Recv(&val , 1, MPI_INT , rank-1 , tag, MPI_COMM_WORLD , &Stat);
        std::cout << "Le processus " << rank << " a reçu " << val << " de le processus"<< rank-1<<".\n"<< std::endl;
        val= val + sum[rank];
        MPI_Send(&val , 1, MPI_INT , rank+1 , tag, MPI_COMM_WORLD);
        std::cout << "Le processus " << rank << " a envoyé " << val << " à le processus"<< rank +1 <<".\n"<< std::endl;
    }
    MPI_Finalize();
    return EXIT_SUCCESS;
}
