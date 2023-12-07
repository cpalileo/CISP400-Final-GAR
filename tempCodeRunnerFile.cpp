#include<iostream>
#include<iomanip>
#include<cstdlib>
#include<ctime>
using namespace std;


class Grid {
private:
    // Sets up 2D array
    int GridArray[10][10];

public:
    // Constructor to initialize all spaces to blank
    Grid(){
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                GridArray[i][j] = 0;
            }
        }
    }

    // Optional Task: Place obstacles around map
    void InitializeObjects(int numObjects) {
        srand(static_cast<unsigned int>(time(0)));

        for (int i = 0; i < numObjects; ++i) {
            int x, y;

            do {
                x = rand() % 10;
                y = rand() % 10;
            } while (GridArray[x][y] != 0 && AdjacentObject(x, y));

            GridArray[x][y] = 1;  // Random Object is placed in cel
        }
    }

    // Check adjacent cel other objects to prevent possible robot encapsulation
    bool AdjacentObject(int x, int y) const {
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                int newX = x + i;
                int newY = y + j;

                // Check if cel is within map walls and contains object
                if (newX >= 0 && newX < 10 && newY >= 0 && newY < 10 && GridArray[newX][newY] == 1) {
                    return true;  // Object has an adjacent object
                }
            }
        }

        return false;  // Object not adjacent to another object
    }


    void InitializeRobots(int numBots) {
        srand(static_cast<unsigned int>(time(0)));

        for (int i = 0; i < numBots; ++i) {
            int x, y;

            do {
                x = rand() % 10;
                y = rand() % 10;
            } while (GridArray[x][y] != 0);

            GridArray[x][y] = 1;
        }
    }

    void InitializeBatteries(int numBatteries){
        srand(static_cast<unsigned int>(time(0)));

        for (int i = 0; i < numBatteries; ++i){
            int x = rand() % 10;
            int y = rand() % 10;
            if (GridArray[x][y] == 0){
                GridArray[x][y] = 2; // Battery placed in cel if empty
            } else {
                --i; // Places battery in other cel if space is occupied
            }
        }
    }

    int GetContent(int x, int y) const {
        if (x >= 0 && x < 10 && y >= 0 && y < 10) {
            return GridArray[x][y];
        } else {
            // Return a value indicating an out-of-bounds cell
            return -1;
        }
    }
};


class Robot {
private:
    // enum SensorStrength {NORMAL, DOUBLE}; // Meaning: Normal - Senors sees 1 space all around, DOUBLE - Sensors sees 2 spaces all around
    enum SensorReading {CLEAR, OBJECT, BATTERY, IGNORE};
    enum Movement {NORTH, SOUTH, EAST, WEST, RANDOM};
    int x;
    int y;
    int Position; // Holds the current location of the robot
    int Energy;
    int TotalEnergyHarvested;
    int TurnsSurvived;

    struct Gene {
        SensorReading sensorReading;
        SensorReading nextSensorReading;
        Movement direction;
    };

    Gene genes[16];
    int currentPosition;
    int energy;
    int currentGeneIndex;  // Added to keep track of the current gene being processed

public:
    Robot(){
        // Initialize genes randomly
        x = rand() % 10;
        y = rand() % 10;
        for (int i = 0; i < 16; ++i) {
            genes[i].sensorReading = static_cast<SensorReading>(rand() % 4);
            genes[i].nextSensorReading = static_cast<SensorReading>(rand() % 4);
            genes[i].direction = static_cast<Movement>(rand() % 5);
        }

        // Initialize other properties
        currentPosition = rand() % 10;
        energy = 5;
        currentGeneIndex = 0;  // Initialize currentGeneIndex to the first gene
    }

    // Method to update sensors based on the grid
    void UpdateSensors(const Grid& grid) {
        int northContent = grid.GetContent(x, y + 1);
        if (northContent == 1) {
            genes[currentGeneIndex].sensorReading = OBJECT;
        } else if (northContent == 2) {
            genes[currentGeneIndex].sensorReading = BATTERY;
        } else {
            genes[currentGeneIndex].sensorReading = CLEAR;
        }

        int southContent = grid.GetContent(x, y - 1);
        if (southContent == 1) {
            genes[currentGeneIndex].sensorReading = OBJECT;
        } else if (southContent == 2) {
            genes[currentGeneIndex].sensorReading = BATTERY;
        } else {
            genes[currentGeneIndex].sensorReading = CLEAR;
        }

        int eastContent = grid.GetContent(x + 1, y);
        if (eastContent == 1) {
            genes[currentGeneIndex].sensorReading = OBJECT;
        } else if (eastContent == 2) {
            genes[currentGeneIndex].sensorReading = BATTERY;
        } else {
            genes[currentGeneIndex].sensorReading = CLEAR;
        }

        int westContent = grid.GetContent(x - 1, y);
        if (westContent == 1) {
            genes[currentGeneIndex].sensorReading = OBJECT;
        } else if (westContent == 2) {
            genes[currentGeneIndex].sensorReading = BATTERY;
        } else {
            genes[currentGeneIndex].sensorReading = CLEAR;
        }

    }

    // Method to make decisions based on sensors and genes
    void MakeDecisions() {
        if (genes[currentGeneIndex].sensorReading == OBJECT) {
            // Turn left
            genes[currentGeneIndex].direction = static_cast<Movement>((genes[currentGeneIndex].direction - 1 + 4) % 4);
        } else if (genes[currentGeneIndex].sensorReading == CLEAR) {
            // Move forward
            Move();
        } else if (genes[currentGeneIndex].sensorReading == BATTERY) {
            // Turn right
            genes[currentGeneIndex].direction = static_cast<Movement>((genes[currentGeneIndex].direction + 1) % 4);
        }
    }

    // Method to move the robot and update position and energy
    void Move() {
        // Update x and y based on the direction
        if (genes[currentGeneIndex].direction == NORTH && y < 9) {
            y++;
        } else if (genes[currentGeneIndex].direction == SOUTH && y > 0) {
            y--;
        } else if (genes[currentGeneIndex].direction == EAST && x < 9) {
            x++;
        } else if (genes[currentGeneIndex].direction == WEST && x > 0) {
            x--;
        }

        energy -+ 1; // 1 energy depleted with every move
    }

    // Method to handle energy depletion and restoration
    void HandleEnergy(const Grid& grid) {
        int cellContent = grid.GetContent(x, y);

        if (cellContent == 2) {
            // battery found
            energy += 5;
            TotalEnergyHarvested += 5;
        }
    }

    void ResetEnergy() {
        energy = 5;
    }

    int RunRound(const Grid& grid) {
        ResetEnergy(); // Reset energy at the start of the round
        int maxTurns = 500; // Example maximum turns limit

        if (maxTurns > 499) {
            cout << "ERROR" << endl;
            return 0;
        }

        for (int turn = 0; turn < maxTurns && energy > 0; ++turn) {
            UpdateSensors(grid);
            MakeDecisions();
            Move();
            HandleEnergy(grid);
            TurnsSurvived++;
    // //Debugging
    // std::cout << "Robot at (" << x << ", " << y << ") | Energy: " << energy << " | Total Harvested: " << TotalEnergyHarvested << " | Turns Survived: " << TurnsSurvived << std::endl;
        }
    }

    int GetTotalEnergyHarvested() const {
        return TotalEnergyHarvested;
    }

    int GetTurnsSurvived() const {
        return TurnsSurvived;
    }    

    int GetEnergy() const {
        return energy;
    }
};


class Simulation {
private:
    Robot robot;
    Robot population[10];  // Array stores the robot population
    int generation;

public:
    Simulation(int populationSize) : generation(0) {
        // Initialize population with random robots
        for (int i = 0; i < populationSize; ++i) {
            population[i] = Robot();  // Add random robot to population
        }
    }


    void GenerationStep(const Grid& grid) {
        robot.UpdateSensors(grid);
        robot.MakeDecisions();
        robot.Move();
        robot.HandleEnergy(grid);

        for (int i = 0; i < 10; ++i) {
            population[i].UpdateSensors(grid);
            population[i].MakeDecisions();
            population[i].Move();
            population[i].HandleEnergy(grid);
        }
    }

    void RunSimulation(const Grid& grid, int numGenerations, int turnsPerGeneration) {
        for (int gen = 0; gen < numGenerations; ++gen) {
            RunGeneration(grid);
            PrintGenerationStats();
        }
    }

    void RunGeneration(const Grid& grid) {
        for (int i = 0; i < 10; ++i) {
            population[i].RunRound(grid);
        }
        ++generation;
    }


    void PrintGenerationStats() const {
        long long totalEnergyHarvested = 0;
        long long totalTurns = 0;

        for (int i = 0; i < 10; ++i) {
            totalEnergyHarvested += population[i].GetTotalEnergyHarvested();
            totalTurns += population[i].GetTurnsSurvived();
        }

        double averageFitness;
        if (totalTurns == 0) {
            averageFitness = 0.0;
        } else {
            averageFitness = static_cast<double>(totalEnergyHarvested) / totalTurns;
        }

        std::cout << "Generation: " << generation << "\n";
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "Average Fitness: " << averageFitness << "\n";
        std::cout << "------------------------\n";
    }


};


int main() {
    Grid grid;
    grid.InitializeObjects(20);
    grid.InitializeRobots(10);
    grid.InitializeBatteries(40);

    Simulation simulation(10);
    simulation.RunSimulation(grid, 10, 200);

    return 0;
}