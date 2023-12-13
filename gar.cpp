#include<iostream>
#include<iomanip>
#include<cstdlib>
#include<ctime>
using namespace std;

// ENUM Declaration
enum GridReading { EMPTY = 0, ROBOT = 'R', BATTERY = 'B', WALL = 'W' };

// Forward declarations
class Robot;
class Genes;


// Genes Class
class Genes {
private:
    // Define the Gene structure as an array of 5 integers
    // First 4 integers for the states (N, E, S, W) and the last one for the action
    struct Gene {
        int data[5]; // 4 states and 1 action
    };

    // Array to store 16 genes
    Gene genes[16];

public:
    // Constructor to initialize the genes with random values
    Genes() {
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 4; ++j) {
                // Randomly initialize the state values (0, 1, or 2)
                genes[i].data[j] = rand() % 3;
            }
            // Randomly initialize the action (represented as integers 0-3 corresponding to N, E, S, W)
            genes[i].data[4] = rand() % 4;
        }
    }

    // Method to get the action for a gene
    char GetAction(int geneIndex) const {
        if (geneIndex < 0 || geneIndex >= 16) {
            return 'N'; // Default action
        }
        int actionCode = genes[geneIndex].data[4];
        switch (actionCode) {
            case 0: return 'N';
            case 1: return 'E';
            case 2: return 'S';
            case 3: return 'W';
            default: return 'N';
        }
    }

    // Method to find the best matching gene based on current sensor readings
int ChooseBestGene(int N, int E, int S, int W) const {
    int bestMatchIndex = 0; // Assume the first gene is the best initially
    int bestMatchScore = -1; // Start with a low score

    for (int i = 0; i < 16; ++i) {
        int score = 0;
        // Scoring for North
        if (genes[i].data[0] == N) {
            score += (N == 2) ? 2 : 1; // Higher score for battery
        }
        // Scoring for East
        if (genes[i].data[1] == E) {
            score += (E == 2) ? 2 : 1; // Higher score for battery
        }
        // Scoring for South
        if (genes[i].data[2] == S) {
            score += (S == 2) ? 2 : 1; // Higher score for battery
        }
        // Scoring for West
        if (genes[i].data[3] == W) {
            score += (W == 2) ? 2 : 1; // Higher score for battery
        }

        if (score > bestMatchScore) {
            bestMatchScore = score;
            bestMatchIndex = i;
        }
    }

    return bestMatchIndex;
}


    void printGenes() const {
        cout << setw(10) << "Gene" << setw(10) << "N" << setw(10) << "E" << setw(10) << "S" << setw(10) << "W" << setw(10) << "Action" << endl;
        for (int i = 0; i < 16; ++i) {
            cout << setw(10) << i;  // Gene number
            for (int j = 0; j < 4; ++j) {
                cout << setw(10) << genes[i].data[j];  // State values
            }
            // Print action
            char action;
            switch (genes[i].data[4]) {
                case 0: action = 'N'; break;
                case 1: action = 'E'; break;
                case 2: action = 'S'; break;
                case 3: action = 'W'; break;
                default: action = '?'; break; // In case of an unexpected value
            }
            cout << setw(10) << action << endl;
        }
    }
};


// Grid Class
class Grid {
private:
    GridReading GridArray[12][12];
    Robot* robot; // Pointer to Robot

public:
    // Initializes all grid spaces to empty
    Grid() : robot(nullptr) {
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


    void PlaceRobot(Robot& robot);


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

    void PrintReport() const {

    }

};


// Robot Class
class Robot {
private:
    int x, y; // Position
    int initialX, initialY; // Robot starting position
    int energy; // Energy level
    int batteriesCollected; // Running count of batteries collected
    struct SensorReadings {
        GridReading north;
        GridReading south;
        GridReading east;
        GridReading west;
    } sensors;

    // Method to handle interaction with a grid cell (like collecting a battery)
    void InteractWithGridCell(Grid& grid, int newX, int newY) {
        GridReading cellContent = grid.QueryCell(newX, newY);
        if (cellContent == BATTERY) {
            energy += 5; // Gain energy for battery
            grid.RemoveBattery(newX, newY);
        }
    }

public:
Genes genes;
    // Initialize sensors to default values
    Robot() : x(0), y(0), initialX(0), initialY(0), energy(5), batteriesCollected(0) {
        sensors.north = EMPTY;
        sensors.south = EMPTY;
        sensors.east = EMPTY;
        sensors.west = EMPTY;
    }

    void SetPosition(int newX, int newY) {
        x = newX;
        y = newY;
        if (initialX == 0 && initialY == 0) { // Check if it's the first time setting the position
            initialX = newX;
            initialY = newY;
        }
    }

    void UpdateSensors(const Grid& grid) {
        sensors.north = (y > 0) ? grid.QueryCell(x, y - 1) : WALL;
        sensors.east = (x < 11) ? grid.QueryCell(x + 1, y) : WALL;
        sensors.south = (y < 11) ? grid.QueryCell(x, y + 1) : WALL;
        sensors.west = (x > 0) ? grid.QueryCell(x - 1, y) : WALL;
    }


    void MoveNorth(Grid& grid) {
        if (y > 0 && grid.QueryCell(x, y - 1) != WALL) {
            y--;
            InteractWithGridCell(grid, x, y);
        }
        energy --;
    }

    void MoveSouth(Grid& grid) {
        if (y < 11 && grid.QueryCell(x, y + 1) != WALL) {
            y++;
            InteractWithGridCell(grid, x, y);
        }
        energy --;
    }

    void MoveEast(Grid& grid) {
        if (x < 11 && grid.QueryCell(x + 1, y) != WALL) {
            x++;
            InteractWithGridCell(grid, x, y);
        }
        energy --;
    }

    void MoveWest(Grid& grid) {
        if (x > 0 && grid.QueryCell(x - 1, y) != WALL) {
            x--;
            InteractWithGridCell(grid, x, y);
        }
        energy --;
    }

    // Fitness function to calculate the score based on batteries collected
    int CalculateFitness() const {
        return batteriesCollected; // Assuming batteriesCollected is maintained correctly during the robot's run
    }

    // Getter for batteriesCollected (if not already implemented)
    int GetBatteriesCollected() const {
        return batteriesCollected;
    }

    void UpdateState() {
        CollectBattery();
    }

    void MoveBasedOnGene() {
        // Choose the best gene and move accordingly
    }

    void CollectBattery() {
        // Increase power if on a battery and remove the battery from the grid
    }


    // TEST MAKEDECISION METHOD
    void MakeDecision(Grid& grid) {
        // Log current sensor readings
        cout << "Current Sensor Readings: N=" << sensors.north << ", E=" << sensors.east 
            << ", S=" << sensors.south << ", W=" << sensors.west << endl;

        UpdateSensors(grid);
        // Retrieve sensor readings and convert them to gene format
        int N = sensors.north == BATTERY ? 2 : sensors.north == EMPTY ? 1 : 0;
        int E = sensors.east == BATTERY ? 2 : sensors.east == EMPTY ? 1 : 0;
        int S = sensors.south == BATTERY ? 2 : sensors.south == EMPTY ? 1 : 0;
        int W = sensors.west == BATTERY ? 2 : sensors.west == EMPTY ? 1 : 0;

        int bestGeneIndex = genes.ChooseBestGene(N, E, S, W);
        char action = genes.GetAction(bestGeneIndex);

        // Battery prioritization
        if (N == 2) action = 'N';
        else if (E == 2) action = 'E';
        else if (S == 2) action = 'S';
        else if (W == 2) action = 'W';

        // Execute the action
        switch (action) {
            case 'N': if(sensors.north != WALL) MoveNorth(grid); break;
            case 'E': if(sensors.east != WALL) MoveEast(grid); break;
            case 'S': if(sensors.south != WALL) MoveSouth(grid); break;
            case 'W': if(sensors.west != WALL) MoveWest(grid); break;
        }
        // Log chosen gene index and action
        cout << "Chosen Gene Index: " << bestGeneIndex << ", Action: " << action << endl;

        // Log the robot's new position after the move
        cout << "New Position: (" << x << ", " << y << ")" << endl;
    }

    // Method to breed with another robot and produce an offspring
    Robot Breed(const Robot& partner) const {
        Robot offspring;
        // Mix genes of 'this' robot and 'partner' to fill offspring's genes
        // Implement the logic of gene mixing here

        return offspring;
    }


    // ACTUAL MAKEDECISION METHOD
        // void MakeDecision(Grid& grid) {
        //     UpdateSensors(grid);

        //     // Retrieve sensor readings
        //     int N, E, S, W;

        //     if (sensors.north == BATTERY) {
        //         N = 2;
        //     } else if (sensors.north == EMPTY) {
        //         N = 1;
        //     } else {
        //         N = 0;
        //     }

        //     if (sensors.east == BATTERY) {
        //         E = 2;
        //     } else if (sensors.east == EMPTY) {
        //         E = 1;
        //     } else {
        //         E = 0;
        //     }

        //     if (sensors.south == BATTERY) {
        //         S = 2;
        //     } else if (sensors.south == EMPTY) {
        //         S = 1;
        //     } else {
        //         S = 0;
        //     }

        //     if (sensors.west == BATTERY) {
        //         W = 2;
        //     } else if (sensors.west == EMPTY) {
        //         W = 1;
        //     } else {
        //         W = 0;
        //     }

        //     int bestGeneIndex = genes.ChooseBestGene(N, E, S, W);
        //     char action = genes.GetAction(bestGeneIndex);

        //     switch (action) {
        //         case 'N': MoveNorth(grid); break;
        //         case 'E': MoveEast(grid); break;
        //         case 'S': MoveSouth(grid); break;
        //         case 'W': MoveWest(grid); break;
        //     }
        // }

    void PrintReport() const {

    }


    void generateReport(int turns) const {
        cout << "Robot Performance Report:" << endl;
        cout << "Starting Position: (" << initialX << ", " << initialY << ")" << endl;
        cout << "Ending Position: (" << x << ", " << y << ")" << endl;
        cout << "Turns before death: " << turns << endl;
        cout << "Batteries Collected: " << batteriesCollected << endl;
    }

    void run(Grid &grid) {
        int turns = 0; // Counter for the number of turns

        // Set initial position (if not already set)
        initialX = x;
        initialY = y;

        // Simulation loop
        while (energy > 0) {
            MakeDecision(grid); // Make a move based on the gene decision
            turns++; // Increment turn count

            // Check for robot's death
            if (energy <= 0) {
                break;
            }
        }

        // Generate performance report
        generateReport(turns);
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


class Breeding {
private:
    Robot* robots;
    int size;

public:
    Breeding(Robot* robots, int size) : robots(robots), size(size) {}

    void SortRobotsByFitness() {
        // Simple bubble sort
        for (int i = 0; i < size - 1; ++i) {
            for (int j = 0; j < size - i - 1; ++j) {
                if (robots[j].GetBatteriesCollected() < robots[j + 1].GetBatteriesCollected()) {
                    Robot temp = robots[j];
                    robots[j] = robots[j + 1];
                    robots[j + 1] = temp;
                }
            }
        }
    }

    double CalculateAverageFitness() const {
        double totalFitness = 0.0;
        for (int i = 0; i < size; ++i) {
            totalFitness += robots[i].GetBatteriesCollected();
        }
        return totalFitness / size;
    }

    // Add more methods if needed, like breeding logic
};



    void Grid::PlaceRobot(Robot& r) {
        int x, y;
        this ->robot = &r;
        do {
            // Generate random coordinates within the inner grid (excluding walls)
            x = rand() % 10 + 1;
            y = rand() % 10 + 1;
        } while (GridArray[x][y] != EMPTY); // Repeat until an empty cell is found

        // Place the robot in the found empty cell
        GridArray[x][y] = ROBOT;
        r.SetPosition(x, y);
        this->robot = &r;
    }

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

    // Simulate the robot trying to move in all directions
    robot.MoveNorth(grid);
    robot.MoveSouth(grid);
    robot.MoveEast(grid);
    robot.MoveWest(grid);
    robot.GridReporting(grid);
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

    // Simulate robot movement for multiple steps
    for (int step = 0; step < 5; ++step) {
        int direction = rand() % 4;  // Random direction
        switch (direction) {
            case 0: robot.MoveNorth(grid); break;
            case 1: robot.MoveSouth(grid); break;
            case 2: robot.MoveEast(grid); break;
            case 3: robot.MoveWest(grid); break;
        }
        robot.UpdateSensors(grid);
        robot.GridReporting(grid);
    }
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

    robot.MoveNorth(grid);  // Move towards the battery
    robot.GridReporting(grid);
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

    // Simulate the robot trying to move into the wall
    robot.MoveNorth(grid);  // North is a wall
    robot.GridReporting(grid);
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
    for (int i = 0; i < 10; ++i) {
        robot.MoveRandom(grid);  // Random movement
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

    while (robot.GetEnergy() > 0) {
        robot.MoveRandom(grid);  // Random movement with energy depletion
        robot.GridReporting(grid);
    }
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

    robot.MoveEast(grid);  // East is a wall
    robot.MoveSouth(grid);  // South is a wall
    robot.GridReporting(grid);
}

void TestGeneLogic() {
    Genes genes;

    // Print the genes for reference
    genes.printGenes();

    // Define sensor conditions
    int sensorConditions[][4] = {
        {0, 0, 0, 0},  // All sensors detect wall
        {1, 1, 1, 1},  // All sensors detect empty space
        {2, 0, 0, 0},  // North sensor detects battery
        // Add more sensor conditions as needed
    };

    // Number of conditions to test
    const int numConditions = 3;  // Update this as you add more conditions

    // Test gene logic for each condition
    for (int i = 0; i < numConditions; ++i) {
        int N = sensorConditions[i][0];
        int E = sensorConditions[i][1];
        int S = sensorConditions[i][2];
        int W = sensorConditions[i][3];

        int bestGeneIndex = genes.ChooseBestGene(N, E, S, W);
        char action = genes.GetAction(bestGeneIndex);

        cout << "Condition: N=" << N << ", E=" << E << ", S=" << S << ", W=" << W << endl;
        cout << "Chosen Gene Index: " << bestGeneIndex << ", Action: " << action << endl;
    }
}



void PressEnterToContinue() {
    cout << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    const int MAX_ROBOTS = 200;
    const int MAX_GENERATIONS = 1000;

    Robot robots[MAX_ROBOTS];
    Grid grid;

    cout << "Placing robots on the grid..." << endl;
    for (int i = 0; i < MAX_ROBOTS; ++i) {
        grid.PlaceRobot(robots[i]);
    }

    cout << "Running test functions..." << endl;
    // Call the test functions here
    TestEnclosedRobot();
    TestOpenFieldWithBatteries();
    TestBatteryCollection();
    TestWallAvoidance();
    TestRandomMovement();
    TestEnergyDepletion();
    TestGridBoundaries();
    TestGeneLogic();

    double averageFitnessPerGeneration[MAX_GENERATIONS] = {0};

    cout << "Starting simulation over generations..." << endl;
    for (int gen = 0; gen < MAX_GENERATIONS; ++gen) {
        cout << "Generation: " << gen + 1 << endl;

        for (int i = 0; i < MAX_ROBOTS; ++i) {
            robots[i].run(grid);
        }

        Breeding breeding(robots, MAX_ROBOTS);
        breeding.SortRobotsByFitness();

        double avgFitness = breeding.CalculateAverageFitness();
        averageFitnessPerGeneration[gen] = avgFitness;

        // Resetting for next generation (if necessary)
        // breeding.GenerateNextGeneration();  // This needs to be implemented

        cout << "Generation " << gen + 1 << " complete. Average Fitness: " << avgFitness << endl;
    }

    cout << "Printing average fitness per 100 generations..." << endl;
    for (int i = 0; i < MAX_GENERATIONS; i += 100) {
        double sum = 0.0;
        for (int j = i; j < i + 100 && j < MAX_GENERATIONS; ++j) {
            sum += averageFitnessPerGeneration[j];
        }
        cout << "Generations " << i + 1 << " - " << i + 100 << ": Average Fitness = " << (sum / 100.0) << endl;
    }

    return 0;
}
