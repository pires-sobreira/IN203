include Make.inc
#include Make_msys2.inc
#include Make_osx.inc

CXXFLAGS = -std=c++11 -fPIC
ifdef DEBUG
CXXFLAGS += -g -O0 -Wall -fbounds-check -pedantic -D_GLIBCXX_DEBUG
else
CXXFLAGS += -O3 -march=native -Wall
endif

default:	help

all:	HelloWorld.exe Circulation_jeton.exe Calcul_de_pi.exe Diffusion_hypercube.exe

HelloWorld.exe:	HelloWorld.cpp
	$(CXX) $(CXXFLAGS) -o HelloWorld.exe HelloWorld.cpp $(LIB)
HelloWorld: HelloWorld.exe
	mpiexec -oversubscribe -np 2 ./HelloWorld.exe

Hello.exe:	Hello.cpp
	$(CXX) $(CXXFLAGS) -o Hello.exe Hello.cpp $(LIB)
Hello: Hello.exe
	mpiexec -oversubscribe -np 16 ./Hello.exe

Circulation_jeton.exe:	Circulation_jeton.cpp
	$(CXX) $(CXXFLAGS) -o Circulation_jeton.exe Circulation_jeton.cpp $(LIB)
Circulation_jeton: Circulation_jeton.exe
	mpiexec -oversubscribe -np 10 ./Circulation_jeton.exe

jeton.exe:	jeton.cpp
	$(CXX) $(CXXFLAGS) -o jeton.exe jeton.cpp $(LIB)
jeton: jeton.exe
	mpiexec -oversubscribe -np 16 ./jeton.exe

Calcul_de_pi.exe:	Calcul_de_pi.cpp
	$(CXX) $(CXXFLAGS) -o Calcul_de_pi.exe Calcul_de_pi.cpp $(LIB)
Calcul_de_pi: Calcul_de_pi.exe
	mpiexec -oversubscribe -np 16 ./Calcul_de_pi.exe

Calcul_de_pi_nb.exe:	Calcul_de_pi_nb.cpp
	$(CXX) $(CXXFLAGS) -o Calcul_de_pi_nb.exe Calcul_de_pi_nb.cpp $(LIB)
Calcul_de_pi_nb: Calcul_de_pi_nb.exe
	mpiexec -oversubscribe -np 16 ./Calcul_de_pi_nb.exe

Diffusion_hypercube.exe:	Diffusion_hypercube.cpp
	$(CXX) $(CXXFLAGS) -o Diffusion_hypercube.exe Diffusion_hypercube.cpp $(LIB)
Diffusion_hypercube: Diffusion_hypercube.exe
	mpiexec -oversubscribe -np 16 ./Diffusion_hypercube.exe

Diffusion_hypercube2.exe:	Diffusion_hypercube2.cpp
	$(CXX) $(CXXFLAGS) -o Diffusion_hypercube2.exe Diffusion_hypercube2.cpp $(LIB)
Diffusion_hypercube2: Diffusion_hypercube2.exe
	mpiexec -oversubscribe -np 16 ./Diffusion_hypercube2.exe

Hypercube.exe:	Hypercube.cpp
	$(CXX) $(CXXFLAGS) -o Hypercube.exe Hypercube.cpp $(LIB)
Hypercube: Hypercube.exe
	mpiexec -oversubscribe -np 16 ./Hypercube.exe

help: 
	@echo "Available targets : "
	@echo "    all                      : compile all executables"
	@echo "    HelloWorld.exe           : compile HelloWorld executable"
	@echo "    Circulation_jeton.exe    : compile Circulation_jeton executable"
	@echo "    Calcul_de_pi.exe         : compile Calcul_de_pi executable"
	@echo "    Diffusion_hypercube.exe  : compile Diffusion_hypercube executable"
	@echo "Add DEBUG=yes to compile in debug"
	@echo "Configuration :"
	@echo "    CXX      :    $(CXX)"
	@echo "    CXXFLAGS :    $(CXXFLAGS)"

clean:
	@rm -f *.exe *~ 

