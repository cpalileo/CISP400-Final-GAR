#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cstdlib> // For rand()
using namespace std;

class Grid {
private:
    static const int GRID_SIZE = 10; // Constant for grid size
    char grid[GRID_SIZE][GRID_SIZE]; // 2D array representing the grid
    int numBatteries;
    bool robotPlaced;
    int roboGridX; // X coordinate of robot
    int roboGridY; // Y coordinate of robot

    void initializeGrid() {
        cout << "Initializing grid cells to empty..." << endl;
        for (int row = 0; row < GRID_SIZE; ++row) {
            for (int col = 0; col < GRID_SIZE; ++col) {
                grid[row][col] = ' ';
            }
        }
    cout << "Grid cells initialized." << endl;
    }

public:
    Grid(int batteryCount = 40) : numBatteries(batteryCount), robotPlaced(false), roboGridX(0), roboGridY(0) {
        cout << "Initializing Grid..." << endl;
        initializeGrid();
        cout << "Distributing Batteries..." << endl;
        distributeBatteries();
        cout << "Adding Robot..." << endl;
        addRobot();
        cout << "Grid Initialization Complete." << endl;
    }


    void distributeBatteries() {
    cout << "Placing batteries on the grid..." << endl;
        int placedBatteries = 0;
        while (placedBatteries < numBatteries) {
            int x = rand() % GRID_SIZE;
            int y = rand() % GRID_SIZE;

            if (grid[x][y] != 'B') {
                grid[x][y] = 'B';
                ++placedBatteries;
            }
        }
    cout << "Batteries placed: " << placedBatteries << endl;
    }

    void addRobot() {
    cout << "Placing robot on the grid..." << endl;
        if (robotPlaced) return;

        do {
            roboGridX = rand() % GRID_SIZE;
            roboGridY = rand() % GRID_SIZE;
        } while (grid[roboGridX][roboGridY] == 'B');

        robotPlaced = true;
    cout << "Robot placed at X: " << roboGridX << ", Y: " << roboGridY << endl;
    }

    void displayGrid() const {
    cout << "Displaying grid:" << endl;
        for (int row = 0; row < GRID_SIZE; ++row) {
            for (int col = 0; col < GRID_SIZE; ++col) {
                if (row == roboGridY && col == roboGridX) {
                    cout << 'R' << ' ';
                } else {
                    cout << grid[row][col] << ' ';
                }
            }
            cout << endl;
        }
    cout << "Grid displayed." << endl;
    }

    int getRoboGridX() const { return roboGridX; }
    int getRoboGridY() const { return roboGridY; }
};

class Genes {
private:
    static const int NUM_STATES = 4;
    int states[NUM_STATES]; // Array to store gene states
    char direction; // Direction associated with these states

    // Private method to randomly set direction
    void setRandomDirection() {
        cout << "Setting random direction..." << endl;
        int directionCode = rand() % NUM_STATES;
        switch (directionCode) {
        case 0: direction = 'N'; break;
        case 1: direction = 'E'; break;
        case 2: direction = 'S'; break;
        case 3: direction = 'W'; break;
        default: direction = 'N'; break;
        }
        cout << "Random direction set to: " << direction << endl;
    }


public:
    Genes() {
        cout << "Initializing Genes..." << endl;
        for (int i = 0; i < NUM_STATES; ++i) {
            states[i] = rand() % 3;
            cout << "State " << i << " set to: " << states[i] << endl;
        }
        setRandomDirection();
        cout << "Direction set to: " << direction << endl;
    }


    // Method to mutate a specific state
    void mutateState(int index) {
        cout << "Mutating state at index: " << index << endl;
        if (index >= 0 && index < NUM_STATES) {
            states[index] = rand() % 3;
        }
        cout << "State " << index << " mutated to: " << states[index] << endl;
    }


    // Method to mutate a random state
    void mutateRandomState() {
        cout << "Mutating a random state..." << endl;
        int index = rand() % NUM_STATES;
        mutateState(index);
        // Note: Detailed output will be printed from mutateState
    }


    // Method to mutate the direction
    void mutateDirection() {
        cout << "Mutating direction..." << endl;
        setRandomDirection();
        cout << "Direction mutated to: " << direction << endl;
    }


    // Getters
    int getState(int index) const {
        return (index >= 0 && index < NUM_STATES) ? states[index] : -1;
    }

    char getDirection() const { return direction; }
};

class Robot {
private:
    static const int MAX_ENERGY = 5;
    static const int NUM_GENES = 16;
    static const int BATTERY_ENERGY_INCREASE = 5;

    int energy;
    int harvestedEnergy;
    Genes genes[NUM_GENES];
    Grid grid;

    // Update the state of the robot based on its surrounding
    void updateState() {
        // Checking adjacent cells and updating N, E, S, W states
        N = (roboGridY > 0 && grid[roboGridY - 1][roboGridX] != ' ') ? 1 : 0;
        E = (roboGridX < 9 && grid[roboGridY][roboGridX + 1] != ' ') ? 1 : 0;
        S = (roboGridY < 9 && grid[roboGridY + 1][roboGridX] != ' ') ? 1 : 0;
        W = (roboGridX > 0 && grid[roboGridY][roboGridX - 1] != ' ') ? 1 : 0;
    }


    // Choose the best gene based on the robot's current state
    int selectBestGene() {
        updateState();
        int bestGeneIndex = 0;
        int bestScore = 0;

        for (int i = 0; i < NUM_GENES; ++i) {
            int score = calculateGeneScore(genes[i]); // Assume this function calculates the gene's score
            if (score > bestScore) {
                bestScore = score;
                bestGeneIndex = i;
            }
        }

        return bestGeneIndex;
    }

    // Move the robot based on the direction specified in the gene
    void move(char direction) {
        // Implementing movement logic based on direction
        switch (direction) {
            case 'N':
                if (N != 0 && roboGridY > 0) roboGridY--;
                break;
            case 'E':
                if (E != 0 && roboGridX < 9) roboGridX++;
                break;
            case 'S':
                if (S != 0 && roboGridY < 9) roboGridY++;
                break;
            case 'W':
                if (W != 0 && roboGridX > 0) roboGridX--;
                break;
        }
        if (grid[roboGridY][roboGridX] == 'B') {
            grid[roboGridY][roboGridX] = ' ';  // Remove battery after collecting
            getHarvestedEnergy();            // Increase robot's energy
        }
    }


    // Calculate the score for a gene
    int calculateGeneScore(const Genes& gene) {
        int score = 0;
        char direction = gene.getDirection();
        // Scoring based on the feasibility of the direction given the current state
        if (direction == 'N' && N != 0) score++;
        if (direction == 'E' && E != 0) score++;
        if (direction == 'S' && S != 0) score++;
        if (direction == 'W' && W != 0) score++;
        return score;
    }


public:
    Robot() : energy(MAX_ENERGY), harvestedEnergy(0), grid() {
        // Initialize genes and other robot states
    }

    // Method to run the robot's routine
    void run() {
        while (energy > 0) {
            int bestGeneIndex = selectBestGene();
            move(genes[bestGeneIndex].getDirection());
            // Additional logic for energy consumption or harvesting
        }
    }

    // Method to increase energy when a battery is harvested
    void harvestBattery() {
        energy += BATTERY_ENERGY_INCREASE;
        harvestedEnergy++;
    }

    // Getters
    int getHarvestedEnergy() const { return harvestedEnergy; }
};

class DynamicArray {
private:
    int* elements;
    int capacity;
    int size;

    void resizeArray() {
        int newCapacity = capacity * 2;
        int* newArray = new int[newCapacity];

        for (int i = 0; i < size; ++i) {
            newArray[i] = elements[i];
        }

        delete[] elements;
        elements = newArray;
        capacity = newCapacity;
    }

public:
    DynamicArray() : elements(new int[1]), capacity(1), size(0) {}

    ~DynamicArray() {
        delete[] elements;
    }

    void add(int value) {
        if (size == capacity) {
            resizeArray();
        }
        elements[size++] = value;
    }

    void remove(int index) {
        if (index < 0 || index >= size) {
            cout << "Error: Invalid Index\n";
            return;
        }

        for (int i = index; i < size - 1; ++i) {
            elements[i] = elements[i + 1];
        }
        size--;
    }

    int getSize() const { return size; }

    int& operator[](int index) {
        if (index >= 0 && index < size) {
            return elements[index];
        }
        throw out_of_range("Index out of range");
    }

    // Additional methods like getArray, sort, max, min, etc., can be added here.
};

class Breeding {
private:
    static const int DEFAULT_POPULATION_SIZE = 200;
    Robot* population;
    int populationSize;

    // Manual sorting of robots based on harvested energy
    void rankPopulation() {
        for (int i = 0; i < populationSize - 1; ++i) {
            for (int j = i + 1; j < populationSize; ++j) {
                if (population[i].getHarvestedEnergy() < population[j].getHarvestedEnergy()) {
                    Robot temp = population[i];
                    population[i] = population[j];
                    population[j] = temp;
                }
            }
        }
    }

    // Resize the population array while keeping the existing elements
    void resizePopulation(int newSize) {
        Robot* newPopulation = new Robot[newSize];
        for (int i = 0; i < min(populationSize, newSize); ++i) {
            newPopulation[i] = population[i];
        }
        delete[] population;
        population = newPopulation;
        populationSize = newSize;
    }

    double getAverageFitnessScore() {
        double totalFitness = 0;
        for (int i = 0; i < populationSize; ++i) {
            totalFitness += population[i].getHarvestedEnergy();
        }
        return totalFitness / populationSize;
    }

public:
    Breeding(int size = DEFAULT_POPULATION_SIZE) : populationSize(size) {
        population = new Robot[populationSize];
    }

    ~Breeding() {
        delete[] population;
    }

    // Run simulation for each robot
    void runSimulation() {
        for (int i = 0; i < populationSize; ++i) {
            population[i].run();
        }
    }

    // Perform natural selection to keep the fittest robots
    void survivalFittest() {
        rankPopulation();
        resizePopulation(populationSize / 2);
    }

    // Additional methods like advanceTimes, displayFitnessScores, etc., can be added here.


    void advanceTime(int numberOfGenerations, int displayInterval = 10) {
        for (int gen = 0; gen < numberOfGenerations; ++gen) {
            runSimulation(); // Run the simulation for each robot
            survivalFittest(); // Perform natural selection

            // Optionally display fitness scores at specified intervals
            if (gen % displayInterval == 0) {
                cout << "Generation " << gen << ": Average Fitness Score = " << getAverageFitnessScore() << endl;
            }
        }
    }
};

void displayWelcomeMessage() {
    cout << "Trials of Life: Artificial Life Edition" << endl;
}

int main() {
    displayWelcomeMessage();

    // Test creation of a Genes object
    cout << "Creating a test Genes object..." << endl;
    Genes testGenes;
    cout << "Test Genes object created." << endl;

    // Initialize the grid
    cout << "Creating Grid..." << endl;
    Grid grid; // Assuming default constructor

    // Display the grid
    cout << "Displaying the initial grid state:" << endl;
    grid.displayGrid();

    // Further code...
    return 0;
}
