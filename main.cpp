#include <iostream>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <unistd.h> // for getopt

using namespace std;

// global variables
int L = 100; // grid size
int px = 2000; // size of the window
double d = 0.1; // death rate
double b = 0.5; // birth rate
int K = 5; // local saturation
double d_pred = 0.1; // death rate for predators
double b_pred = 0.25; // birth rate for predators

// functions
int death(double death_rate); // determine death
int rep(double birth_rate); // determine reproduction
vector<int> update(vector<int> &grid); // update grid
void draw(sf::RenderWindow &window, vector<int> &grid); // draw grid

int main(int argc, char* argv[]) {

    // parse command-line arguments using getopt
    int opt;
    while ((opt = getopt(argc, argv, "L:p:d:b:K:c:a:")) != -1) {
        switch (opt) {
        case 'L':
            L = std::atoi(optarg);
            break;
        case 'p':
            px = std::atoi(optarg);
            break;
        case 'd':
            d = std::atof(optarg);
            break;
        case 'b':
            b = std::atof(optarg);
            break;
        case 'K':
            K = std::atoi(optarg);
            break;
        case 'c':
            b_pred = std::atoi(optarg);
            break;
        case 'a':
            d_pred = std::atoi(optarg);
            break;
        default:
            cerr << "Usage: " << argv[0] << " [-L grid_size] [-p window_size] [-d death_rate] [-b birth_rate] [-K local_saturation]" << endl;
            return 1;
        }
    }

    // create grid and fill with random 1s

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 2); // 0 empty, 1 prey, 2 predator

    vector<int> grid(L*L, 0);

    for (int index = 0; index < L * L; ++index) {
        grid[index] = dis(gen);
    }

    // create a window
    sf::RenderWindow window(sf::VideoMode(px, px), "Shawn's Game of Life");

    // window logic
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // check for close
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // clear window
        window.clear();

        // draw grid
        draw(window, grid);

        // display window
        window.display();

        // update grid
        grid = update(grid);

        // sleep
        sf::sleep(sf::milliseconds(17));
    }

    return 0;
}

int death(double death_rate) {
    // transition function to change states
    random_device rd;
    mt19937 gen(rd());
    binomial_distribution<int> dis(1, death_rate);

    return dis(gen);
}

int rep(double birth_rate) {
    // transition function to change states
    random_device rd;
    mt19937 gen(rd());
    binomial_distribution<int> dis(1, birth_rate);

    return dis(gen);
}

void draw(sf::RenderWindow &window, vector<int> &grid) {
    // draw grid
    sf::RectangleShape cell(sf::Vector2f(px / L, px / L));

    for (int index = 0; index < L * L; ++index) {
        int i = index / L; // x position
        int j = index % L; // y position

        if (grid[index] == 1) {
            cell.setFillColor(sf::Color::White);
        } else if(grid[index] == 2) {
            cell.setFillColor(sf::Color::Red);
        } else if(grid[index] == 0) {
            cell.setFillColor(sf::Color::Black);
        }

        cell.setPosition(i * px / L, j * px / L);
        window.draw(cell);
    }
}

vector<int> update(vector<int> &grid) {
    // update grid
    vector<int> new_grid(L * L, 0);

    for (int index = 0; index < L * L; ++index) {
        int i = index / L;
        int j = index % L;

        // moore neighborhood with periodic boundary conditions
        vector<int> neighbors = {
            ((i - 1 + L) % L) * L + ((j - 1 + L) % L),
            ((i - 1 + L) % L) * L + j,
            ((i - 1 + L) % L) * L + ((j + 1) % L),
            i * L + ((j - 1 + L) % L),
            i * L + ((j + 1) % L),
            ((i + 1) % L) * L + ((j - 1 + L) % L),
            ((i + 1) % L) * L + j,
            ((i + 1) % L) * L + ((j + 1) % L)
        };

        int prey_count = 0;
        int prey_eaten = 0;

        // calculate number of prey
        for (int neighbor : neighbors) {
            if (grid[neighbor] == 1) {
                prey_count += 1;
            }
        }

        if (grid[index] == 0) { // empty
            if (prey_count < K && prey_count > 0) {

                int birth = rep(b);

                if (birth == 1) {

                    new_grid[index] = 1;

                }
            }
        } else if (grid[index] == 1) { // prey
            if (prey_count == 0) {
                new_grid[index] = death(d);
            } else {
                new_grid[index] = grid[index];
            }
        } else if (grid[index] == 2) { // predator

            // only if prey are present

            if (prey_count > 0 ) {

                // predator eats prey

                for (int neighbor : neighbors) {

                    if (grid[neighbor] == 1 && prey_eaten < 5) {
                        prey_eaten += 1;
                        new_grid[neighbor] = 0;
                    }else if (prey_eaten > 4) {

                      break;
                      
                    }
                }
            }

            // predator reproduces

            while (prey_eaten > 0) {

                int birth = rep(b_pred);
                int reproduced = 0;
                if (birth == 1) {
                    
                    
                    // find empty cell

                    for (int neighbor : neighbors) {
                        if (grid[neighbor] == 0) {
                            new_grid[neighbor] = 2;
                            reproduced = 1;
                            break;
                        }
                    }
                }
                
                if (birth == 1 && reproduced == 0){

                  prey_eaten = 0;
                  break;
                }
                prey_eaten -= 1;

            }

            // predator dies

            int dead = death(d_pred);

            if (dead == 1) {
                new_grid[index] = 0;
            } else {
                new_grid[index] = grid[index];
            }

        }
    }

    return new_grid;
}
