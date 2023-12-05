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
    void InitializeObjectPlacement(int numObjects) {
        srand(static_cast<unsigned int>(time(0)));

        for (int i = 0; i < numObjects; ++i) {
            int x, y;

            do {
                x = rand() % 10;
                y = rand() % 10;
            } while (GridArray[x][y] != 0 && hasAdjacentObject(x, y));

            GridArray[x][y] = 1;  // Random Object is placed in cel
        }
    }

    // Check adjacent cel other objects to prevent possible robot encapsulation
    bool hasAdjacentObject(int x, int y) const {
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


    void InitializeRobotPlacement(int numBots){
        srand(static_cast<unsigned int>(time(0)));

        for (int i = 0; i < numBots; ++i){
            int x = rand() % 10;
            int y = rand() % 10;
            if (GridArray[x][y] = 0){
                GridArray[x][y] = 1;  // Bot is placed in grid cel
            } else {
                --i; // Bot is placed in another grid cel if already occupied 
            }
        }
    }

    void InitializeBatteryPlacement(int numBatteries){
        srand(static_cast<unsigned int>(time(0)));

        for (int i = 0; i < numBatteries; ++i){
            int x = rand() % 10;
            int y = rand() % 10;
            if (GridArray[x][y] = 0){
                GridArray[x][y] = 2; // Battery placed in cel if empty
            } else {
                --i; // Places battery in other cel if space is occupied
            }
        }
    }
};


class Robot {
private:
    enum SensorStrength {NORMAL, DOUBLE}; // Meaning: Normal - Senors sees 1 space all around, DOUBLE - Sensors sees 2 spaces all around
    enum SensorReading {CLEAR, OBJECT, BATTERY, IGNORE};
    enum Movement {NORTH, SOUTH, EAST, WEST, RANDOM};
    int Position;
    int Energy = 5;

    struct Gene {
        SensorReading sensorReading;
        SensorReading nextSensorReading;
        Movement direction;
    };

    Gene genes[16];
    int position;
    int energy;

public:
    Robot(){
        // Initialize genes randomly (you can modify this part based on your understanding)
        for (int i = 0; i < 16; ++i) {
            genes[i].sensorReading = static_cast<SensorReading>(rand() % 4);
            genes[i].nextSensorReading = static_cast<SensorReading>(rand() % 4);
            genes[i].direction = static_cast<Movement>(rand() % 5);
        }

        // Initialize other properties
        position = -1;  // You can initialize it to a specific value or leave it for later
        energy = 5;
    }

    // Method to update sensors based on the grid
    void updateSensors(const Grid& grid) {
        // Implement this method based on your requirements
    }

    // Method to make decisions based on sensors and genes
    void makeDecisions() {
        // Implement this method based on your requirements
    }

    // Method to move the robot and update position and energy
    void move() {
        // Implement this method based on your requirements
    }

    // Method to handle energy depletion and restoration
    void handleEnergy(const Grid& grid) {
        // Implement this method based on your requirements
    }
};


int main(){
    Grid grid;
    grid.InitializeObjectPlacement(20);
    grid.InitializeRobotPlacement(1);
    grid.InitializeBatteryPlacement(40);
};