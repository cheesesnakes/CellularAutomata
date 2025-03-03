# Cellular Automata Predator - Prey simulation

A barebones cellular automata simulation of a predator - prey system writen in C++. I got my inspiration for this project from [Cattaneo and Denunzio, 2006](https://web2.qatar.cmu.edu/~gdicaro/15382-Spring18/additional/prey-predator-CA-2006.pdf).

## How to run

To run the simulation, you need to have a C++ compiler installed on your machine. I used the GNU Compiler Collection (g++) to compile the code, along with `cmake` to set compiler instrtuctions and make to compile. To compile the code, run the following command in the terminal:

```bash
# Clone the repository

git clone [repository link]

# Change directory to the cloned repository

cd cellular-automata

# Create a build directory and change to it

mkdir build && cd build

# Run cmake to set compiler instrtuctions

cmake ..

# Run make to compile the code

make
```

After compiling the code, you will find an executable file named `CellularAutomata` in the build directory. To run the simulation, run the following command in the terminal:

```bash
./CellulartAutomata
```

The simulation will continue to run until you close the window.

## Simulation parameters

You can set certain parameters of the simulation as command line arguments. The following are the parameters that you can set:

- `-L` : The length of the grid (default: 100)
- `-b` : birth rate of the prey (default: 0.1)
- `-d` : death rate of the prey (default: 0.1)
- `-m` : death rate of the predator (default: 0.1)
- `-s` : birth rate of the predator (default: 0.1)
- `-K` : The local saturation of the prey (default: 5)

