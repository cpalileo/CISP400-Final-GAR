#include<iostream>
#include<iomanip>
#include<cstdlib>
#include<ctime>
using namespace std;

enum GridReading { EMPTY = 0, ROBOT = 'R', BATTERY = 'B', WALL = 'W' };

class Robot;

class Grid {
private:
    GridReading GridArray[12][12];

public:
    // Initializes all grid spaces to empty
    Grid() {
        for (int i = 0; i < 12; ++i) {
            for (int j = 0; j < 12; ++j) {
                GridArray[i][j] = EMPTY;
            }
        }
        // Place walls around the grid
        PlaceWalls();
        
        // Randomly place batteries on the grid
        PlaceBatteries(40);  // Assuming 40 batteries to be placed
    }

    void PlaceWalls(){
        // Place walls on top and bottom rows
        for (int i = 0; i < 12; ++i) {
            GridArray[0][i] = WALL; // Top row
            GridArray[11][i] = WALL; // Bottom row
        }

        // Place walls on left and right columns
        for (int i = 1; i < 11; ++i) { // Start and end at 1 and 11 to avoid overwriting corners
            GridArray[i][0] = WALL; // Left column
            GridArray[i][11] = WALL; // Right column
        }
    }

    void PlaceBatteries(int numBatteries) {
        int placed = 0;
        while (placed < numBatteries) {
            int x = rand() % 10 + 1;
            int y = rand() % 10 + 1;
            if (GridArray[x][y] == EMPTY) {
                GridArray[x][y] = BATTERY;
                placed++;
            }
        }
    }


    // BUILD UPON LATER POSSIBLY FOR SCANNING AND SUCH
    GridReading QueryCell(int x, int y) const {
        if (x >= 0 && x < 12 && y >= 0 && y < 12) {
            return GridArray[x][y];
            // Add more logic as needed...
        } else {
            return WALL;  // For out-of-bounds
        }
    }



    void PlaceRobot() {
        int x, y;

        do {
            // Generate random coordinates within the inner grid (excluding walls)
            x = rand() % 10 + 1;
            y = rand() % 10 + 1;
        } while (GridArray[x][y] != EMPTY); // Repeat until an empty cell is found

        // Place the robot in the found empty cell
        GridArray[x][y] = ROBOT;
    }


    void RemoveRobot(int x, int y) {
        if (x >= 1 && x <= 10 && y >= 1 && y <= 10) { // Searches inner grid only
            GridArray[x][y] = EMPTY;  
        }
    }


    void PlaceBattery() {
        int x, y;

        do {
            // Generate random coordinates within the inner grid (excluding walls)
            x = rand() % 10 + 1;
            y = rand() % 10 + 1;
        } while (GridArray[x][y] != EMPTY); // Repeat until an empty cell is found

        // Place the battery in the found empty cell
        GridArray[x][y] = BATTERY;
    }


    void RemoveBattery(int x, int y) {
        if (x >= 1 && x <= 10 && y >= 1 && y <= 10) { // Searches inner grid only
            GridArray[x][y] = EMPTY;  
        }
    }

    void PrintGrid() const {
        cout << "Current Grid State:\n";
        for (int i = 0; i < 12; ++i) {
            for (int j = 0; j < 12; ++j) {
                char displayChar = ' ';
                switch (GridArray[i][j]) {
                    case EMPTY: displayChar = '.'; break;
                    case ROBOT: displayChar = 'R'; break;
                    case BATTERY: displayChar = 'B'; break;
                    case WALL: displayChar = '#'; break;
                }
                cout << displayChar << " ";
            }
            cout << endl;
        }
    }

    //ADDED FOR TESTING PURPOSES
    void PlaceBatteryAt(int x, int y) {
    if (x >= 1 && x <= 10 && y >= 1 && y <= 10) {
        GridArray[x][y] = BATTERY;
    }
}


};


class Robot {
private:
    int x, y; // Position
    int energy; // Energy level
    struct SensorReadings {
        GridReading north;
        GridReading south;
        GridReading east;
        GridReading west;
    } sensors;

    void InteractWithGridCell(Grid& grid, int x, int y) {
        GridReading cellContent = grid.QueryCell(x, y);
        if (cellContent == BATTERY) {
            energy += 5; 
            grid.RemoveBattery(x, y);
        }
    }

public:
    // Initialize sensors to default values
    Robot() : x(0), y(0), energy(5) {
        sensors.north = EMPTY;
        sensors.south = EMPTY;
        sensors.east = EMPTY;
        sensors.west = EMPTY;
    }

    void SetPosition(int newX, int newY) {
        x = newX;
        y = newY;
    }

    void UpdateSensors(const Grid& grid) {
        // Assume the grid is 12x12 with indices ranging from 0 to 11.
        // Check north (y - 1), but ensure it's within the grid boundaries.

        if (y > 0) {
            sensors.north = grid.QueryCell(x, y - 1);
        } else {
            sensors.north = WALL;
        }

        // Check south (y + 1)
        if (y < 11) {
            sensors.south = grid.QueryCell(x, y + 1);
         } else { 
            sensors.south = WALL;
         }

        // Check east (x + 1)
        if (x < 11) {
            sensors.east = grid.QueryCell(x + 1, y);
        } else {
            sensors.east = WALL;
        }

        // Check west (x - 1)
        if (x > 0) {
            sensors.west = grid.QueryCell(x - 1, y);
        } else {
            sensors.west = WALL;
        }
    }

    void MakeDecision() {
        // Logic for making decisions based on sensor readings.
    }

    void MoveNorth(Grid& grid) {
        if (y > 0 && grid.QueryCell(x, y - 1) != WALL) {
            y--;
            InteractWithGridCell(grid, x, y);
        }
    }

    void MoveSouth(Grid& grid) {
        if (y < 11 && grid.QueryCell(x, y + 1) != WALL) {
            y++;
            InteractWithGridCell(grid, x, y);
        }
    }

    void MoveEast(Grid& grid) {
        if (x < 11 && grid.QueryCell(x + 1, y) != WALL) {
            x++;
            InteractWithGridCell(grid, x, y);
        }
    }

    void MoveWest(Grid& grid) {
        if (x > 0 && grid.QueryCell(x - 1, y) != WALL) {
            x--;
            InteractWithGridCell(grid, x, y);
        }
    }

    void GridReporting(const Grid& grid);
    // ... additional methods ...

    //ADDED FOR TESTING PURPOSES
    void SetEnergy(int newEnergy) {
        energy = newEnergy;
    }

    //ADDED FOR TESTING PURPOSES
    int GetEnergy() const {
        return energy;
    }

    //ADDED FOR TESTING PURPOSES
    void MoveRandom(Grid& grid) {
        int move = rand() % 4;
        switch (move) {
            case 0: MoveNorth(grid); break;
            case 1: MoveSouth(grid); break;
            case 2: MoveEast(grid); break;
            case 3: MoveWest(grid); break;
        }
        energy --;
    }


};

void Robot::GridReporting(const Grid& grid) {
    // Print initial position
    cout << "Robot initial position: (" << x << ", " << y << ")\n";
    
    // Move the robot (you may want to pass in a direction as a parameter)
    // Example: MoveNorth(grid); This would need to be implemented in your Robot class

    // Update sensors after the move
    UpdateSensors(grid);

    // Print new position and sensor readings
    cout << "Robot new position: (" << x << ", " << y << ")\n";
    cout << "Sensor Readings:\n";
    cout << "\tNorth: " << static_cast<char>(sensors.north) << "\n";
    cout << "\tSouth: " << static_cast<char>(sensors.south) << "\n";
    cout << "\tEast: " << static_cast<char>(sensors.east) << "\n";
    cout << "\tWest: " << static_cast<char>(sensors.west) << "\n";

    // Print the current state of the grid (assuming Grid has a PrintGrid method)
    grid.PrintGrid();
}


void TestEnclosedRobot() {
    Grid grid;
    Robot robot;

    // Place the robot in a cell completely surrounded by walls
    grid.PlaceWalls();
    robot.SetPosition(1, 1); // Assuming (1, 1) is surrounded by walls

    robot.UpdateSensors(grid);
    robot.GridReporting(grid);

    // Additional checks can be performed here
}


void TestOpenFieldWithBatteries() {
    Grid grid;
    Robot robot;

    // Set up the grid with several batteries
    grid.PlaceBatteries(10); // Place 10 batteries randomly

    // Place the robot at a random position
    robot.SetPosition(rand() % 10 + 1, rand() % 10 + 1);

    // Simulate robot movement and interaction
    for (int i = 0; i < 5; ++i) { // Simulate 5 moves
        int direction = rand() % 4;
        switch (direction) {
            case 0: robot.MoveNorth(grid); break;
            case 1: robot.MoveSouth(grid); break;
            case 2: robot.MoveEast(grid); break;
            case 3: robot.MoveWest(grid); break;
        }
        robot.UpdateSensors(grid);
        robot.GridReporting(grid);
    }

    // Additional checks can be performed here
}


void TestBatteryCollection() {
    Grid grid;
    Robot robot;

    // Place the robot at a known position
    robot.SetPosition(5, 5);

    // Place a battery at a position where the robot can reach
    grid.PlaceBatteryAt(5, 4); // North of the robot

    // Move the robot and check if it collects the battery
    robot.MoveNorth(grid);
    robot.UpdateSensors(grid);
    robot.GridReporting(grid);

    // Additional assertions can be added here
}


void TestWallAvoidance() {
    Grid grid;
    Robot robot;

    // Place the robot close to a wall
    robot.SetPosition(1, 1);

    // Attempt to move the robot into the wall
    robot.MoveNorth(grid); // North is a wall
    robot.UpdateSensors(grid);
    robot.GridReporting(grid);

    // Additional assertions can be added here
}


void TestRandomMovement() {
    Grid grid;
    Robot robot;

    // Place the robot at a random position
    robot.SetPosition(rand() % 10 + 1, rand() % 10 + 1);

    // Simulate random movements
    for (int i = 0; i < 10; ++i) {
        int move = rand() % 4;
        switch (move) {
            case 0: robot.MoveNorth(grid); break;
            case 1: robot.MoveSouth(grid); break;
            case 2: robot.MoveEast(grid); break;
            case 3: robot.MoveWest(grid); break;
        }
        robot.UpdateSensors(grid);
        robot.GridReporting(grid);
    }
}


void TestEnergyDepletion() {
    Grid grid;
    Robot robot;

    // Set initial position and energy
    robot.SetPosition(5, 5);
    robot.SetEnergy(10); // Assuming there's a SetEnergy method

    // Move the robot until energy depletes
    while (robot.GetEnergy() > 0) { // Assuming there's a GetEnergy method
        robot.MoveRandom(grid); // Assuming there's a MoveRandom method
        robot.UpdateSensors(grid);
        robot.GridReporting(grid);
    }

    // Additional assertions here
}


void TestGridBoundaries() {
    Grid grid;
    Robot robot;

    // Place the robot at the edge of the grid
    robot.SetPosition(10, 10);

    // Try moving the robot beyond the grid boundaries
    robot.MoveEast(grid); // East should be blocked by a wall
    robot.MoveSouth(grid); // South should be blocked by a wall
    robot.UpdateSensors(grid);
    robot.GridReporting(grid);

    // Additional assertions here
}

void PressEnterToContinue() {
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    Grid grid;
    Robot robot;

    // Test scenarios
    TestEnclosedRobot();
    cout << "TestEnclosedRobot()\n" << endl;
        PressEnterToContinue();

    TestOpenFieldWithBatteries();
        cout << "TestOpenFieldWithBatteries()\n" << endl;
        PressEnterToContinue();

    TestBatteryCollection();
            cout << "TestBatteryCollection()\n" << endl;
        PressEnterToContinue();

    TestWallAvoidance();
            cout << "TestWallAvoidance()\n" << endl;
        PressEnterToContinue();

    TestRandomMovement();
            cout << "TestRandomMovement()\n" << endl;
        PressEnterToContinue();

    TestEnergyDepletion();
            cout << "TestEnergyDepletion()\n" << endl;
        PressEnterToContinue();

    TestGridBoundaries();
            cout << "TestGridBoundries()\n" << endl;
        PressEnterToContinue();


    // Further testing and implementation...
    robot.GridReporting(grid);
    cout<< "GridReporting\n" << endl;

    return 0;
}
