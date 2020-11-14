# include <chrono>
# include <random>
# include <cstdlib>
# include <sstream>
# include <string>
# include <fstream>
# include <iostream>
# include <iomanip>
# include <mpi.h>

// Attention , ne marche qu'en C++ 11 ou supérieur :
double approximate_pi( unsigned long nbSamples ) 
{
    typedef std::chrono::high_resolution_clock myclock;
    myclock::time_point beginning = myclock::now();
    myclock::duration d = beginning.time_since_epoch();
    unsigned seed = d.count();
    std::default_random_engine generator(seed);
    std::uniform_real_distribution <double> distribution ( -1.0 ,1.0);
    unsigned long nbDarts = 0;
    // Throw nbSamples darts in the unit square [-1 :1] x [-1 :1]
    for ( unsigned sample = 0 ; sample < nbSamples ; ++ sample ) {
        double x = distribution(generator);
        double y = distribution(generator);
        // Test if the dart is in the unit disk
        if ( x*x+y*y<=1 ) nbDarts ++;
    }
    // Number of nbDarts throwed in the unit disk
    double ratio = double(nbDarts)/double(nbSamples);
    return 4*ratio;
}

int main( int nargs, char* argv[] )
{
    int rank, nbp, tag =1;
    int samples = 30000000;
    double result = 0,parcial;


	MPI_Init( &nargs, &argv );
	MPI_Comm globComm;
	MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
	MPI_Comm_size(globComm, &nbp);
	MPI_Comm_rank(globComm, &rank);
    MPI_Status Stat; 


    if(rank == 0)
    {
        for(int i=1; i <= nbp -1; i++)
        {
            MPI_Recv(&parcial , 1, MPI_DOUBLE , i , tag, MPI_COMM_WORLD , &Stat);
            result += parcial;
        }

        result = result/(nbp -1);
        std::cout << "PI = " << result <<"\n"<< std::endl;

    }
    else
    {
        parcial = approximate_pi(samples);
        MPI_Send(&parcial , 1, MPI_DOUBLE , 0 , tag, MPI_COMM_WORLD);
    }
	MPI_Finalize();
	return EXIT_SUCCESS;
}
