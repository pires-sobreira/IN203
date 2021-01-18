#include <cstdlib>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>        
#include <SDL2/SDL_image.h>
#include <fstream>
#include <ctime>
#include <iomanip>      // std::setw
#include <chrono>
# include <mpi.h>

#include "parametres.hpp"
#include "galaxie.hpp"

void function_render(galaxie_renderer& gr, galaxie& g) 
{
  gr.render(g);
}
 
int main(int nargs, char* argv[])
{
    int rank, nbp, tag =1;

    MPI_Init( &nargs, &argv );
    MPI_Comm globComm;
    MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
    MPI_Comm_size(globComm, &nbp);
    MPI_Comm_rank(globComm, &rank);
    MPI_Status status; 

    char commentaire[4096];
    int width, height;
    SDL_Event event;
    SDL_Window   * window;

    parametres param;


    std::ifstream fich("parametre.txt");
    fich >> width;
    fich.getline(commentaire, 4096);
    fich >> height;
    fich.getline(commentaire, 4096);
    fich >> param.apparition_civ;
    fich.getline(commentaire, 4096);
    fich >> param.disparition;
    fich.getline(commentaire, 4096);
    fich >> param.expansion;
    fich.getline(commentaire, 4096);
    fich >> param.inhabitable;
    fich.getline(commentaire, 4096);
    fich.close();

    std::cout << "Resume des parametres (proba par pas de temps): " << std::endl;
    std::cout << "\t Chance apparition civilisation techno : " << param.apparition_civ << std::endl;
    std::cout << "\t Chance disparition civilisation techno: " << param.disparition << std::endl;
    std::cout << "\t Chance expansion : " << param.expansion << std::endl;
    std::cout << "\t Chance inhabitable : " << param.inhabitable << std::endl;
    std::cout << "Proba minimale prise en compte : " << 1./RAND_MAX << std::endl;
    std::srand(std::time(nullptr));

    
    galaxie g(width, height, param.apparition_civ);
    galaxie g_next(width, height);
    

    int deltaT = (20*52840)/width;
    std::cout << "Pas de temps : " << deltaT << " années" << std::endl;

    std::cout << std::endl;

    unsigned long long temps = 0;

    std::chrono::time_point<std::chrono::system_clock> start, end1, end2, end3;
    
    std::chrono::duration<double> elapsc;
    std::chrono::duration<double> elapsa;
    std::chrono::duration<double> elapst;
    
    int count = 0;

    if(rank==0)
    {
      SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
      window = SDL_CreateWindow("Galaxie", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                width, height, SDL_WINDOW_SHOWN);
      galaxie_renderer gr(window);

       while (temps < 1000000) {
          count ++;
          start = std::chrono::system_clock::now();
          MPI_Recv(g.data(),height*width, MPI_CHAR,1 ,tag,MPI_COMM_WORLD, &status) ;    
          end1 = std::chrono::system_clock::now();
          function_render(gr, g);
          end2 = std::chrono::system_clock::now();

          //calcul du temp moyen de calcul
          elapsc = elapsc*(count-1)/count + (end1 - start)/count;
          //calcul du temp moyen d'affichage
          elapsa = elapsa*(count-1)/count + (end2 - end1)/count;
          //calcul du temp moyen total
          elapst = elapst*(count-1)/count + (end2 - start)/count;
          
          temps += deltaT;
          
          std::cout << "Temps passe : "
                  << std::setw(10) << temps << " années"
                  << std::fixed << std::setprecision(3)
                  << "  " << "|  CPU(ms) : calcul moyen " << elapsc.count()*1000
                  << "  " << "affichage moyen " << elapsa.count()*1000
                  << "  " << "Total moyen " << elapst.count()*1000
                  << "\r" << std::flush
                  << "\n";

                    
          if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
            std::cout << std::endl << "The end" << std::endl;
            break;
            }
       }

    }
    else
    {
        while(temps < 1000000)
        {
          mise_a_jour(param, width, height, g.data(), g_next.data());
          g_next.swap(g);
          MPI_Send(g.data(), height*width, MPI_CHAR , 0 , tag, MPI_COMM_WORLD);         
        }
    }
    


   
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
