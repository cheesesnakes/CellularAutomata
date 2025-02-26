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
double b = 0.1; // birth rate
int K = 5; // local saturation

// functions
int death(); // determine death
int rep(); // determine reproduction
vector<int> update(vector<int> &grid); // update grid
void draw(sf::RenderWindow &window, vector<int> &grid); // draw grid

int main(int argc, char* argv[]) {
    
    // parse command-line arguments using getopt
    int opt;
    while ((opt = getopt(argc, argv, "L:p:d:b:K:")) != -1) {
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
            default:
                cerr << "Usage: " << argv[0] << " [-L grid_size] [-p window_size] [-d death_rate] [-b birth_rate] [-K local_saturation]" << endl;
                return 1;
        }
    }

    // create grid and fill with random 1s
    
    random_device rd;
    mt19937 gen(rd());
    binomial_distribution<int> dis(1, 0.01);

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

int death() {
    // transition function to change states
    random_device rd;
    mt19937 gen(rd());
    binomial_distribution<int> dis(1, d);

    return dis(gen);
}

int rep() {
    // transition function to change states
    random_device rd;
    mt19937 gen(rd());
    binomial_distribution<int> dis(1, b);

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
        } else {
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
            ((i - 1 + L) % L) * L + ((j - 1 + L) % L), ((i - 1 + L) % L) * L + j, ((i - 1 + L) % L) * L + ((j + 1) % L),
            i * L + ((j - 1 + L) % L), i * L + ((j + 1) % L),
            ((i + 1) % L) * L + ((j - 1 + L) % L), ((i + 1) % L) * L + j, ((i + 1) % L) * L + ((j + 1) % L)
        };

        int count = 0;

        for (int neighbor : neighbors) {
            count += grid[neighbor];
        }

        if (grid[index] == 0) {
            if (count < K && count > 0) {
                new_grid[index] = rep();
            }
        } else {
            if (count == 0) {
                new_grid[index] = death();
            }
        }
    }

    return new_grid;
}