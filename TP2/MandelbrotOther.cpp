# include <iostream>
# include <cstdlib>
# include <string>
# include <chrono>
# include <cmath>
# include <vector>
# include <fstream>
# include <mpi.h>

/** Une structure complexe est définie pour la bonne raison que la classe
 * complex proposée par g++ est très lente ! Le calcul est bien plus rapide
 * avec la petite structure donnée ci--dessous
 **/

struct Complex
{
    Complex() : real(0.), imag(0.)
    {}
    Complex(double r, double i) : real(r), imag(i)
    {}
    Complex operator + ( const Complex& z )
    {
        return Complex(real + z.real, imag + z.imag );
    }
    Complex operator * ( const Complex& z )
    {
        return Complex(real*z.real-imag*z.imag, real*z.imag+imag*z.real);
    }
    double sqNorm() { return real*real + imag*imag; }
    double real,imag;
};

std::ostream& operator << ( std::ostream& out, const Complex& c )
{
  out << "(" << c.real << "," << c.imag << ")" << std::endl;
  return out;
}

/** Pour un c complexe donné, calcul le nombre d'itérations de mandelbrot
 * nécessaires pour détecter une éventuelle divergence. Si la suite
 * converge, la fonction retourne la valeur maxIter
 **/

int iterMandelbrot( int maxIter, const Complex& c)
{
    Complex z{0.,0.};
    // On vérifie dans un premier temps si le complexe
    // n'appartient pas à une zone de convergence connue :
    // Appartenance aux disques  C0{(0,0),1/4} et C1{(-1,0),1/4}
    if ( c.real*c.real+c.imag*c.imag < 0.0625 )
        return maxIter;
    if ( (c.real+1)*(c.real+1)+c.imag*c.imag < 0.0625 )
        return maxIter;
    // Appartenance à la cardioïde {(1/4,0),1/2(1-cos(theta))}    
    if ((c.real > -0.75) && (c.real < 0.5) ) {
        Complex ct{c.real-0.25,c.imag};
        double ctnrm2 = sqrt(ct.sqNorm());
        if (ctnrm2 < 0.5*(1-ct.real/ctnrm2)) return maxIter;
    }
    int niter = 0;
    while ((z.sqNorm() < 4.) && (niter < maxIter))
    {
        z = z*z + c;
        ++niter;
    }
    return niter;
}

/** Construit et sauvegarde l'image finale **/

void savePicture( const std::string& filename, int W, int H, const std::vector<int>& nbIters, int maxIter )
{
    double scaleCol = 1./maxIter;//16777216
    std::ofstream ofs( filename.c_str(), std::ios::out | std::ios::binary );
    ofs << "P6\n"
        << W << " " << H << "\n255\n";
    for ( int i = 0; i < W * H; ++i ) {
        double iter = scaleCol*nbIters[i];
        unsigned char r = (unsigned char)(256 - (unsigned (iter*256.) & 0xFF));
        unsigned char b = (unsigned char)(256 - (unsigned (iter*65536) & 0xFF));
        unsigned char g = (unsigned char)(256 - (unsigned( iter*16777216) & 0xFF));
        ofs << r << g << b;
    }
    ofs.close();
}

int main(int nargs, char *argv[]) 
 { 
    int rank, nbp;
    MPI_Init(&nargs, &argv);
    MPI_Comm comm;
    MPI_Comm_dup(MPI_COMM_WORLD, &comm);
    MPI_Comm_size(comm, &nbp);
    MPI_Comm_rank(comm, &rank);
    MPI_Status Stat; 

    const int W = 800;
    const int H = 600;
    // Normalement, pour un bon rendu, il faudrait le nombre d'itérations
    // ci--dessous :
    //const int maxIter = 16777216;
    const int maxIter = 8*65536;

    //Start counting the process time
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    //Set process 0 as the master, resposable for coordinate, send and receive information
    if(rank == 0)
    {
        int posi = 0;
        int recu = 0;
        int resultat;

        //Vector that will store the calculated value for each position
        std::vector<int> pixels(W*H);

        //Sending one position for each process
        for( int i = 1; i < nbp ; ++i ) 
        {
            MPI_Send(&posi, 1 , MPI_INT, i , 1, MPI_COMM_WORLD );
            posi += 1;
        }
        //Ensure that all positions have been sent
        while (posi < W*H)
        {
            MPI_Recv(&resultat , 1, MPI_INT, MPI_ANY_SOURCE , MPI_ANY_TAG, MPI_COMM_WORLD , &Stat);
            recu += 1;
            pixels[Stat.MPI_TAG] = resultat;
            MPI_Send(&posi, 1 , MPI_INT, Stat.MPI_SOURCE , 1, MPI_COMM_WORLD ) ;
            posi += 1;
        }
        //Ensure that all positions have been received
        while (recu < W*H)
        {
            MPI_Recv(&resultat , 1, MPI_INT, MPI_ANY_SOURCE , MPI_ANY_TAG, MPI_COMM_WORLD , &Stat);
            recu += 1;
            pixels[Stat.MPI_TAG] = resultat;
        }
        
        //Sending termination signal
        posi = -1;
        for( int i = 1; i < nbp ; ++i ) 
        {
            MPI_Send(&posi, 1 , MPI_INT, i , 1, MPI_COMM_WORLD ) ;
        }

        // Printing the figure and the time spent
        savePicture("mandelbrot.tga", W, H, pixels, maxIter);
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::cout << "Temps calcul ensemble mandelbrot : " << elapsed_seconds.count() <<std::endl;

    }

    //All other processes will be "slaves". They must wait for process 0 orders, perform the calculations and send the results
    else
    {
        //Variables to store the value that will be received and the value that will be stored.
        int posi = 0;
        int pixel;

        // Calcul le facteur d'échelle pour rester dans le disque de rayon 2 centré en (0,0)
        double scaleX = 3./(W-1);
        double scaleY = 2.25/(H-1.);
        int row, column;

        //While process 0 does not send the termination signal
        while (posi >= 0)
        {   
            //Reciving the position for which the process should calculate.
            MPI_Recv(&posi, 1, MPI_INT, 0 , 1, MPI_COMM_WORLD , &Stat);

            if (posi >= 0)
            {
                //Calculating the row and column according to the position received
                column = posi%W;
                row = posi/800;

                //Calculating the value of this pixel
                Complex c{-2.+column*scaleX,-1.125+ row*scaleY};
                pixel = iterMandelbrot( maxIter, c);

                //Sent the calculated value to the master process tagged by the position so the master process can fit the value into the global array
                MPI_Send(&pixel , 1, MPI_INT , 0 , posi, MPI_COMM_WORLD);
            }
            
        }
        
    }
    


    MPI_Finalize();
    return EXIT_SUCCESS;
 }