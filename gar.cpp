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
    int GridArray[12][12];

public:
    // Constructor to initialize all spaces to blank
    Grid(){

        // Inititialize the map (Blank = 0)
        for (int i = 0; i < 12; ++i) {
            for (int j = 0; j < 12; ++j) {
                GridArray[i][j] = 0;
            }
        }

        // Add the boarder to the map (Wall/Object = 3)
        for (int i = 0; i < 12; ++i) {
            GridArray[i][0] = 3; // Western wall
            GridArray[i][11] = 3; // Eastern wall
            GridArray[0][i] = 3; // Northern wall
            GridArray[11][i] = 3; // Southern wall
        }
    }

    // Optional Task: Place obstacles around map
    // void InitializeObjects(int numObjects) {
    //     srand(static_cast<unsigned int>(time(0)));

    //     for (int i = 0; i < numObjects; ++i) {
    //         int x, y;

    //         do {
    //             x = rand() % 10;
    //             y = rand() % 10;
    //         } while (GridArray[x][y] != 0 && IsAdjacent(x, y, 3));

    //         GridArray[x][y] = 3;  // Object/Wall is placed in the cell
    //     }
    // }


    // Check adjacent cel other objects to prevent possible robot encapsulation
    // bool IsAdjacent(int x, int y, int valueToCheck) const {
    //     for (int i = -1; i <= 1; ++i) {
    //         for (int j = -1; j <= 1; ++j) {
    //             if (i == 0 && j == 0) continue; // Skip the current cell

    //             int newX = x + i;
    //             int newY = y + j;

    //             if (newX >= 0 && newX < 10 && newY >= 0 && newY < 10 && GridArray[newX][newY] == valueToCheck) {
    //                 return true; // Adjacent cell has the specified value
    //             }
    //         }
    //     }
    //     return false;
    // }

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
        int x, y;

        for (int i = 0; i < numBatteries; i++) {
        // Find an empty cell
            do {
                x = rand() % 10;
                y = rand() % 10;
            } while (GridArray[x][y] != 0);  // Ensure the cell is empty

            GridArray[x][y] = 2;  // Place the robot in the empty cell
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

// TEST CASE
// void PlaceRobot() {
//     srand(static_cast<unsigned int>(time(0)));
//     int x, y;
//     do {
//         x = rand() % 10;
//         y = rand() % 10;
//     } while (GridArray[x][y] != 0);

//     GridArray[x][y] = 1;  // Place robot in cell

//     // Place a battery to the right of the robot, if within grid boundaries
//     if (x + 1 < 10) {
//         GridArray[x + 1][y] = 2;  // Place a battery
//     }
// }


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
    enum SensorReading { CLEAR = 0, OBJECT = 1, BATTERY = 2, IGNORE = 3 };    
    enum Movement {NORTH, SOUTH, EAST, WEST, RANDOM};
    int x;
    int y;
    int Position; // Holds the current location of the robot
    int Energy;
    int TurnsSurvived;
    int TotalEnergyHarvested = 0;
    Movement decisionDirection;
    
    struct Gene {
        int states[4]; // Represents N, S, E, W states. For example: {1, 0, 2, 1}
        Movement action; // Represents the action to take when the above state is observed
    };

    struct SensorData {
        SensorReading north;
        SensorReading south;
        SensorReading east;
        SensorReading west;
    };

    Gene genes[16];
    SensorData currentSensorData;
    int energy;
    int totalEnergyHarvested;
    int turnsSurvived;

public:
    Robot() : x(rand() % 10), y(rand() % 10), energy(5), totalEnergyHarvested(0), turnsSurvived(0) {
        // Initialize genes with random states and actions
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 4; ++j) {
                genes[i].states[j] = rand() % 3; // States can be 0, 1, or 2
            }
            genes[i].action = static_cast<Movement>(rand() % 5); // Actions can be NORTH, SOUTH, EAST, WEST, or RANDOM
        }
    }


    void UpdateSensors(const Grid& grid) {

    }

    // Method to make decisions based on sensors and genes
    void MakeDecisions() {
        // Convert sensor readings to a comparable form
        int sensorStates[4] = {
            currentSensorData.north == BATTERY ? 2 : (currentSensorData.north == OBJECT ? 1 : 0),
            currentSensorData.east == BATTERY ? 2 : (currentSensorData.east == OBJECT ? 1 : 0),
            currentSensorData.south == BATTERY ? 2 : (currentSensorData.south == OBJECT ? 1 : 0),
            currentSensorData.west == BATTERY ? 2 : (currentSensorData.west == OBJECT ? 1 : 0),
        };

        bool decisionMade = false;
        for (int i = 0; i < 16; ++i) {
            if (std::equal(std::begin(genes[i].states), std::end(genes[i].states), std::begin(sensorStates))) {
                decisionDirection = genes[i].action;
                decisionMade = true;
                break;
            }
        }

        if (!decisionMade) {
            // If no gene matches, choose a random direction
            decisionDirection = static_cast<Movement>(rand() % 4);
        }
    }





    // Method to move the robot and update position and energy
    void Move() {
        // Modify this method to use the direction decided in MakeDecisions
        // For example, if the decision is to move north:
        if (decisionDirection == NORTH) {
            y = (y < 9) ? y + 1 : y; // Ensuring the robot doesn't go out of bounds
        } else if (decisionDirection == SOUTH) {
            y = (y > 0) ? y - 1: y;
        } else if (decisionDirection == EAST) {
            x = (x < 9) ? x + 1 : x;
        } else if (decisionDirection == WEST) {
            x = (x > 0) ? x - 1 : x;
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
            TurnsSurvived += 5 * 5; // Update TurnsSurvived based on energy harvested
            grid.ConsumeBattery(x, y);
        }
        // DEBUG
        cout << "Handled energy at (" << x << ", " << y << ") | Energy now: " << energy << endl;
    }


    void ResetEnergy() {
        // energy = 5;
        energy = 50; //FIX ME TESTING PURPOSES ONLY
    }

    void RunRound(Grid& grid, int robotNumber) {
        ResetEnergy();
        TotalEnergyHarvested = 0; // Reset total energy harvested
        TurnsSurvived = 0;        // Reset turns survived
        // DEBUG
        cout << "Robot " << robotNumber << " starting at (" << x << ", " << y << ") with energy " << energy << endl;

        while (energy > 0) {
            UpdateSensors(grid);
            MakeDecisions();
            Move();
            HandleEnergy(grid); // Energy handling might change TurnsSurvived
            TurnsSurvived++;
        }

        cout << "Robot " << robotNumber << " | Total Harvested: " << TotalEnergyHarvested 
            << " | Turns Survived: " << TurnsSurvived << endl;
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
    // grid.InitializeObjects(20);
    grid.InitializeRobots();
    grid.InitializeBatteries(40);

    Simulation simulation;
    simulation.RunSimulation(grid, 10);

    return 0;
}