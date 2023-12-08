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
        int x, y;

        // Find an empty cell
        do {
            x = rand() % 10;
            y = rand() % 10;
        } while (GridArray[x][y] != 0);  // Ensure the cell is empty

        GridArray[x][y] = 1;  // Place the robot in the empty cell
    }

    void InitializeBatteries(int numBatteries){
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
    int x, y;
    do {
        x = rand() % 10;
        y = rand() % 10;
    } while (GridArray[x][y] != 0);

    GridArray[x][y] = 1;  // Place robot in cell
    cout << "Robot placed at: (" << x << ", " << y << ")" << endl;
}

// TEST CASE
// void PlaceRobot() {
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
        cout << "Robot removed from: (" << x << ", " << y << ")" << endl;
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

    // DEBUG METHOD
    void PrintGrid() const {
    cout << "Current Grid State:" << endl;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            cout << GridArray[i][j] << " ";
        }
        cout << endl;
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
    Movement decisionDirection;

    // struct Gene {
    //     SensorReading sensorReading;
    //     SensorReading nextSensorReading;
    //     Movement direction;
    // };
    
// TEST CASE
struct Gene {
    SensorReading sensorReading;
    SensorReading nextSensorReading;
    Movement direction;
    float randomnessFactor; // New gene trait for randomness
};



    struct SensorData {
        SensorReading north;
        SensorReading south;
        SensorReading east;
        SensorReading west;
    };

    Gene genes[16];
    int currentPosition;
    int energy;
    int currentGeneIndex; 

public:
    Robot(){
        x = rand() % 10;
        y = rand() % 10;
        for (int i = 0; i < 16; ++i) {
            genes[i].sensorReading = static_cast<SensorReading>(rand() % 4);
            genes[i].nextSensorReading = static_cast<SensorReading>(rand() % 4);

            // Generate a random value between 0 and 4 (inclusive) for direction, including RANDOM
            int directionIndex = std::rand() % 5;
            genes[i].direction = static_cast<Movement>(directionIndex);

            // TEST CASE
            genes[i].randomnessFactor = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        }

        currentPosition = rand() % 10;
        energy = 5;
        currentGeneIndex = 0;

        //DEBUG
        cout << "New Robot created at: (" << x << ", " << y << ")" << endl;

    }

    SensorData currentSensorData;

    // Method to update sensors based on the grid
void UpdateSensors(const Grid& grid) {
    currentSensorData.north = static_cast<SensorReading>(grid.GetContent(x, y + 1));
    currentSensorData.south = static_cast<SensorReading>(grid.GetContent(x, y - 1));
    currentSensorData.east = static_cast<SensorReading>(grid.GetContent(x + 1, y));
    currentSensorData.west = static_cast<SensorReading>(grid.GetContent(x - 1, y));
}


    // Method to make decisions based on sensors and genes
    void MakeDecisions() {

        if (currentSensorData.north == BATTERY) {decisionDirection = NORTH;}
            else if (currentSensorData.south == BATTERY) {decisionDirection = SOUTH;}
            else if (currentSensorData.east == BATTERY) {decisionDirection = EAST;}
            else if (currentSensorData.west == BATTERY) {decisionDirection = WEST;}
            else {
                // Check each direction and make a decision based on the gene's strategy
                if (currentSensorData.north == genes[currentGeneIndex].sensorReading) {
                    decisionDirection = genes[currentGeneIndex].direction; // Use gene's direction if sensor reading matches
                } else if (currentSensorData.south == genes[currentGeneIndex].sensorReading) {
                    decisionDirection = genes[currentGeneIndex].direction;
                } else if (currentSensorData.east == genes[currentGeneIndex].sensorReading) {
                    decisionDirection = genes[currentGeneIndex].direction;
                } else if (currentSensorData.west == genes[currentGeneIndex].sensorReading) {
                    decisionDirection = genes[currentGeneIndex].direction;
                } else {
                    // If no sensor data matches the current gene, choose a random direction
                    decisionDirection = static_cast<Movement>(rand() % 4);
                }
            }

        // TEST CASE
        float randomValue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        if (randomValue < genes[currentGeneIndex].randomnessFactor) {
            decisionDirection = static_cast<Movement>(rand() % 5); // Includes RANDOM direction
        }

        // Increment the gene index for the next decision
        currentGeneIndex = (currentGeneIndex + 1) % 16; 

        // DEBUG
        cout << "Decision made: " << decisionDirection << endl; // Add this line

    }



    // Method to move the robot and update position and energy
// Method to move the robot and update position and energy
void Move(const Grid& grid) {
    bool hasValidMove = true;

    for (int step = 0; step < 3; ++step) {
        UpdateSensors(grid);

        // Break if a battery is in the current cell
        if (grid.GetContent(x, y) == BATTERY) break;

        // Check if the robot is surrounded by obstacles or batteries on all sides
        if ((currentSensorData.north == OBJECT || currentSensorData.north == BATTERY) &&
            (currentSensorData.south == OBJECT || currentSensorData.south == BATTERY) &&
            (currentSensorData.east == OBJECT || currentSensorData.east == BATTERY) &&
            (currentSensorData.west == OBJECT || currentSensorData.west == BATTERY)) {
            hasValidMove = false;
            cout << "No valid moves available. Surrounded by: ";
            cout << "North - " << currentSensorData.north << ", ";
            cout << "South - " << currentSensorData.south << ", ";
            cout << "East - " << currentSensorData.east << ", ";
            cout << "West - " << currentSensorData.west << endl;
            break;
        }

        // DEBUG: Show sensor readings and decision before moving
        cout << "Sensors: N=" << currentSensorData.north << ", S=" << currentSensorData.south
             << ", E=" << currentSensorData.east << ", W=" << currentSensorData.west << endl;
        cout << "Before Move Step " << step << ": Position: (" << x << ", " << y << "). Decision: " << decisionDirection << endl;

        switch (decisionDirection) {
            case NORTH:
                y = (y > 0) ? y - 1 : y;
                break;
            case SOUTH:
                y = (y < 9) ? y + 1 : y;
                break;
            case EAST:
                x = (x < 9) ? x + 1 : x;
                break;
            case WEST:
                x = (x > 0) ? x - 1 : x;
                break;
            default:
                cout << "Random or undefined decision direction. Not moving." << endl;
                break;  // RANDOM or other undefined cases
        }

        energy -= 1;  // Deduct energy per movement step

        // DEBUG: Show position and energy after moving
        cout << "After Move Step " << step << ": Position: (" << x << ", " << y << "). Energy: " << energy << endl;

        if (energy <= 0) {
            cout << "Energy depleted. Stopping movement." << endl;
            break;  // Stop if energy runs out
        }
    }

    if (!hasValidMove) {
        cout << "Move ended due to lack of valid moves." << endl;
    }
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
        energy = 5;
        // energy = 50; //FIX ME TESTING PURPOSES ONLY
    }

    void RunRound(Grid& grid, int robotNumber) {

        // DEBUG
        grid.PrintGrid(); // This will print the state of the grid after each movement

        ResetEnergy();
        TotalEnergyHarvested = 0; // Reset total energy harvested
        TurnsSurvived = 0;        // Reset turns survived
        // DEBUG
        cout << "Robot " << robotNumber << " starting at (" << x << ", " << y << ") with energy " << energy << endl;

        while (energy > 0) {
            UpdateSensors(grid);
            MakeDecisions();
            Move(grid);
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

    //DEBUG
    grid.PrintGrid(); // This will print the state of the grid before each robot's round

    double totalFitness = 0;

    for (int i = 0; i < 200; ++i) {
        grid.PlaceRobot();
        // After placing a robot, print the grid state
        grid.PrintGrid();

        population[i].RunRound(grid, i + 1);

        // Get the robot's position for removal
        int x = population[i].GetX();
        int y = population[i].GetY();
        grid.RemoveRobot(x, y);

        // After removing a robot, print the grid state
        grid.PrintGrid();

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
    srand(static_cast<unsigned int>(time(0))); // Initialize random seed once
    // DEBUG
    cout << "Begin Initializing Objects" << endl;
    grid.InitializeObjects(20);
    // DEBUG
    cout << "Objects Initialized."
         << "Begin Initializing Robots" << endl;
    grid.InitializeRobots();
    // DEBUG
    cout << "Robots Initialized." 
         << "Begin Initializing Batteries" << endl;
    grid.InitializeBatteries(40);
    // DEBUG
    cout << "Batteries Initialized" << endl;

    // DEBUG
    grid.PrintGrid(); // This will print the initial state of the grid


    Simulation simulation;
    // DEBUG 
    cout << "Begin RunSimulation from main" << endl;
    simulation.RunSimulation(grid, 2); // Change back to 10 (generations) on final

    return 0;
}