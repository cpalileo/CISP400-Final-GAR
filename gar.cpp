#include<iostream>
#include<iomanip>
#include<cstdlib>
#include<ctime>
using namespace std;


struct FitnessData {
    double averageFitness;
};

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
            } while (GridArray[x][y] != 0 && IsAdjacent(x, y, 3));

            GridArray[x][y] = 3;  // Object/Wall is placed in the cell
        }
    }


    // Check adjacent cel other objects to prevent possible robot encapsulation
    bool IsAdjacent(int x, int y, int valueToCheck) const {
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) continue; // Skip the current cell

                int newX = x + i;
                int newY = y + j;

                if (newX >= 0 && newX < 10 && newY >= 0 && newY < 10 && GridArray[newX][newY] == valueToCheck) {
                    return true; // Adjacent cell has the specified value
                }
            }
        }
        return false;
    }



    void InitializeRobots() {
        srand(static_cast<unsigned int>(time(0)));
        int x, y;

        // Find an empty cell
        do {
            x = rand() % 10;
            y = rand() % 10;
        } while (GridArray[x][y] != 0);  // Ensure the cell is empty

        GridArray[x][y] = 1;  // Place the robot in the empty cell
    }

    void InitializeBatteries(int numBatteries){
        srand(static_cast<unsigned int>(time(0)));

        for (int i = 0; i < numBatteries; ++i){
            int x = rand() % 10;
            int y = rand() % 10;
            // DEBUG
            cout << "Trying to place battery at: (" << x << ", " << y << ")" << endl;
            if (GridArray[x][y] == 0){
                GridArray[x][y] = 2; // Battery placed in cel if empty
            } else {
                --i; // Places battery in other cel if space is occupied
            }
        }
    }

    void PlaceRobot() {
        srand(static_cast<unsigned int>(time(0)));
        int x, y;
        do {
            x = rand() % 10;
            y = rand() % 10;
        } while (GridArray[x][y] != 0);

        GridArray[x][y] = 1;  // Place robot in cell
    }

    void RemoveRobot(int x, int y) {
        if (x >= 0 && x < 10 && y >= 0 && y < 10) {
            GridArray[x][y] = 0;
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


    // In the Grid class:
    void ConsumeBattery(int x, int y) {
        if (x >= 0 && x < 10 && y >= 0 && y < 10) {
            GridArray[x][y] = 0; // Remove the battery from the grid
        //DEBUG
        cout << "Battery consumed at: (" << x << ", " << y << ")" << endl;
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
    int TurnsSurvived;
    int TotalEnergyHarvested = 0;

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

        energy -= 1; // 1 energy depleted with every move

    // //DEBUG
    // cout << "Robot moved to (" << x << ", " << y << ") | Energy after move: " << energy << endl;

    }

    // Method to handle energy depletion and restoration
    void HandleEnergy(Grid& grid) {
        int cellContent = grid.GetContent(x, y);

        if (cellContent == 2) {
            // DEBUG
            cout << "Robot found battery at (" << x << ", " << y << ")" << endl;
            energy += 5;
            TotalEnergyHarvested += 5;
            grid.ConsumeBattery(x, y); // Consume the battery from the map
        }
    // //DEBUG
    //     cout << "Handled energy at (" << x << ", " << y << ") | Energy now: " << energy << endl;

    }

    void ResetEnergy() {
        energy = 5;
    }

    void RunRound(Grid& grid, int robotNumber) {
        //DEBUG
        cout << "Robot " << robotNumber << " starting at (" << x << ", " << y << ") with energy " << energy << endl;
        ResetEnergy();  // Reset energy at the start of the round
        while (energy > 0) {
            UpdateSensors(grid);
            MakeDecisions();
            Move();
            HandleEnergy(grid);
            TurnsSurvived++;
        }
            cout << "Robot " << robotNumber << " | Total Harvested: " << TotalEnergyHarvested << " | Turns Survived: " << TurnsSurvived << endl;
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

    int GetX() const {
        return x;
    }

    int GetY() const {
        return y;
    }
};


class Simulation {
private:
    Robot population[200];
    int generation;
    FitnessData generationFitness[10]; // Using the FitnessData struct to store fitness data

public:
    Simulation() : generation(0) {
        for (auto &robot : population) {
            robot = Robot();
        }
    }

    void RunGeneration(Grid& grid) {
        double totalFitness = 0;

        for (int i = 0; i < 200; ++i) {
            grid.PlaceRobot();
            population[i].RunRound(grid, i + 1);
            
            // Get the robot's position for removal
            int x = population[i].GetX();
            int y = population[i].GetY();
            grid.RemoveRobot(x, y);

            totalFitness += static_cast<double>(population[i].GetTotalEnergyHarvested()) / population[i].GetTurnsSurvived();
        }

        generationFitness[generation].averageFitness = totalFitness / 200;
        ++generation;
    }

    void PrintAllFitnessData() const {
        cout << "\nGeneration Fitness Data:\n";
        for (int i = 0; i < generation; ++i) {
            cout << "Generation " << (i + 1) << ": Average Fitness = " << generationFitness[i].averageFitness << endl;
        }
    }

    void RunSimulation(Grid& grid, int numGenerations) {
        for (int gen = 0; gen < numGenerations; ++gen) {
            RunGeneration(grid);
        }
        PrintAllFitnessData();
    }
};


int main() {
    Grid grid;
    grid.InitializeObjects(20);
    grid.InitializeRobots();
    grid.InitializeBatteries(40);

    Simulation simulation;
    simulation.RunSimulation(grid, 10);

    return 0;
}