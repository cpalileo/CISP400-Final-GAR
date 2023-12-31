#include<iostream>
#include<iomanip>
#include<cstdlib>
#include <string>
#include<ctime>
using namespace std;


// ENUM Declaration
enum GridReading { EMPTY, ROBOT, BATTERY, WALL};

// Forward declarations
class Robot;

class Genes {
private:
    // GenePool structure made up of an array of 5 integers
    // 1 through 4 represent (N, E, S, W), last one for teh direction
    struct GenePool {
        int data[5]; // 4 states, 1 direction
    };

    // Array stores 16 gene pools
    GenePool genes[16];

public:
    // Constructor initialize genes with random values
    Genes() {
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 4; ++j) {
                // Initizlize random state value
                genes[i].data[j] = rand() % 3;
            }
            // Initialize direction (N, E, S, W) randomly
            genes[i].data[4] = rand() % 4;
        }
    }

    // Method gets gene's Direction
    char GetDirection(int geneIndex) const {
        if (geneIndex < 0 || geneIndex >= 16) {
            return 'N'; // Default direction
        }
        int directionCode = genes[geneIndex].data[4];
        switch (directionCode) {
            case 0: return 'N';
            case 1: return 'E';
            case 2: return 'S';
            case 3: return 'W';
            default: return 'N';
        }
    }

    // Find best matching genepool based on sensor readings
    // Added to keep robot from moving back and forth
    int ChooseBestGene(int N, int E, int S, int W) const {
        int bestMatchIndex = 0; // Assume first gene pool is best initially
        int bestMatchScore = -1; // Start with low score

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

    const GenePool& getGenePool(int index) const{
        // Keep index inbound
        if (index < 0 || index >= 16) {
            throw std::out_of_range("Index out of bounds");
        }
        return genes[index];    
    }

    // Merge genes to create a new genes
    static Genes Merge(const Genes& parent1, const Genes& parent2) {
        Genes offspring;
        int crossoverPoint = rand() % 16; // For teh 16 genes
        for (int i = 0; i < 16; ++i) {
            offspring.genes[i] = (i < crossoverPoint) ? parent1.genes[i] : parent2.genes[i];
        }
        return offspring;
    }
};


class Grid {
private:
    GridReading GridArray[12][12];
    Robot* robot; // Pointer to Robot

public:
    // Initializes empty grid spaces 
    Grid() : robot(nullptr) {
        for (int i = 0; i < 12; ++i) {
            for (int j = 0; j < 12; ++j) {
                GridArray[i][j] = EMPTY;
            }
        }
        // Call to place wall around grid
        PlaceWalls();
        
        // Randomly place batteries on grid
        PlaceBatteries(40);  // 40 batteries
    }

    void PlaceWalls(){
        // Place walls at top & bottom rows
        for (int i = 0; i < 12; ++i) {
            GridArray[0][i] = WALL; // Top row
            GridArray[11][i] = WALL; // Bottom row
        }

        // Place walls at left & right columns
        for (int i = 1; i < 11; ++i) { // Ours go to 11! (override corner protection)
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


    // BUILD UPON LATER POSSIBLY FOR SCANNING AND SUCH IF CONTINUE TO HAVE
    // PROBLEMS WITH ROBOT MOVEMENT AND BATTERY COLLECTION
    GridReading QueryCell(int x, int y) const {
        if (x >= 0 && x < 12 && y >= 0 && y < 12) {
            return GridArray[x][y];
        } else {
            return WALL;  // For out-of-bounds
        }
    }


    void PlaceRobot(Robot& robot);


    void RemoveRobot(int x, int y) {
        // Searches inner grid only
        // Avoids surrounding walls
        if (x >= 1 && x <= 10 && y >= 1 && y <= 10) { 
            GridArray[x][y] = EMPTY;  
        }
    }


    void PlaceBattery() {
        int x, y;

        do {
            // Random coordinates for inner grid (no walls)
            x = rand() % 10 + 1;
            y = rand() % 10 + 1;
        } while (GridArray[x][y] != EMPTY); // Loop until empty cel found

        // Place battery in empty cell
        GridArray[x][y] = BATTERY;
    }


    void RemoveBattery(int x, int y) {
        // Searches inner grid only
        // Avoids surrounding walls
        if (x >= 1 && x <= 10 && y >= 1 && y <= 10) { 
            GridArray[x][y] = EMPTY;  
        }
    }

    void PrintGrid() const {
        // DEBUG
        // cout << "Current Grid State:\n";
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

    //DEBUG METHOD
    void PlaceBatteryAt(int x, int y) {
    if (x >= 1 && x <= 10 && y >= 1 && y <= 10) {
        GridArray[x][y] = BATTERY;
    }
}

    void PrintReport() const {

    }

};


class Robot {
private:
    int x, y; 
    int initialX, initialY; // Robot starting coordinates
    int energy; 
    int batteriesCollected; 
    static const int GENE_COUNT = 16;
    char lastDirection = '\0';
    int directionStreak = 0;

    struct SensorReadings {
        GridReading north;
        GridReading south;
        GridReading east;
        GridReading west;
    } sensors;

    // TEST Robot Tune Up
    // Interdirection with grid cell (like collecting a battery)
    void InteractWithGridCell(Grid& grid, int newX, int newY) {
        GridReading cellContent = grid.QueryCell(newX, newY);
        if (cellContent == BATTERY) {
            energy += 5; // +6 = +1 reset step deduction (-1) and +5 for battery
            grid.RemoveBattery(newX, newY);
            batteriesCollected++; // Increment #batteries collected
        }
    }

public:
    Genes genes; 

    struct GenePool {
        int north, east, south, west;
        char direction; // 'N', 'E', 'S', 'W'
    };

    // Initialize sensors defalt vals
    Robot() : x(0), y(0), initialX(0), initialY(0), energy(5), batteriesCollected(0), lastDirection('\0'), directionStreak(0) {
        sensors.north = EMPTY;
        sensors.south = EMPTY;
        sensors.east = EMPTY;
        sensors.west = EMPTY;
    }

    void SetPosition(int newX, int newY) {
        x = newX;
        y = newY;
        if (initialX == 0 && initialY == 0) { // Check to see if initial setup
            initialX = newX;
            initialY = newY;
        }
    }

    void UpdateSensors(const Grid& grid) {
        sensors.north = (y > 1) ? grid.QueryCell(x, y - 1) : WALL;
        sensors.east = (x < 10) ? grid.QueryCell(x + 1, y) : WALL;
        sensors.south = (y < 10) ? grid.QueryCell(x, y + 1) : WALL;
        sensors.west = (x > 1) ? grid.QueryCell(x - 1, y) : WALL;
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


    // Pick genepool based on sensor readings
    char selectGene() {
        int index = rand() % 16; // Select random gene index
        return genes.GetDirection(index); // gets gene's direction
    }

    // Calculate fitness score
    int CalculateFitness() const {
        return batteriesCollected; // score = #batteries collected
    }

    // TEST MAKEDECISION METHOD
    // GETTING BIG MODULATE ME
    void MakeDecision(Grid& grid) {
        // DEBUG
        // Log current sensor readings
        // cout << "Grid State Before Decision:\n";
        // grid.PrintGrid();

        UpdateSensors(grid);
        // DEBUG
        // cout << "Sensor Readings: N=" << sensors.north << ", E=" << sensors.east 
        //     << ", S=" << sensors.south << ", W=" << sensors.west << endl;

        // Direction determined by sensor
        int N = sensors.north == BATTERY ? 2 : sensors.north == EMPTY ? 1 : 0;
        int E = sensors.east == BATTERY ? 2 : sensors.east == EMPTY ? 1 : 0;
        int S = sensors.south == BATTERY ? 2 : sensors.south == EMPTY ? 1 : 0;
        int W = sensors.west == BATTERY ? 2 : sensors.west == EMPTY ? 1 : 0;

        int bestGeneIndex = genes.ChooseBestGene(N, E, S, W);
        char direction = genes.GetDirection(bestGeneIndex);

        // prioritize batteires
        if (N == 2) direction = 'N';
        else if (E == 2) direction = 'E';
        else if (S == 2) direction = 'S';
        else if (W == 2) direction = 'W';

        // Check if robot stuck in a loop...again
        if (direction == lastDirection) {
            directionStreak++;
        } else {
            directionStreak = 0;
        }

        if (directionStreak > 2) {
            direction = FindAlternativeDirection(); // !!!!!
            directionStreak = 0;
        }

        // Execute chosen direction
        switch (direction) {
            case 'N': if(sensors.north != WALL) MoveNorth(grid); break;
            case 'E': if(sensors.east != WALL) MoveEast(grid); break;
            case 'S': if(sensors.south != WALL) MoveSouth(grid); break;
            case 'W': if(sensors.west != WALL) MoveWest(grid); break;
        }

        // DEBUG
        // Log chosen gene pool index and direction
        // cout << "Chosen Gene Pool Index: " << bestGeneIndex << ", Direction: " << direction << endl;

        // DEBUG
        // Log robot's new position after move
        // cout << "New Position: (" << x << ", " << y << ")" << endl;

        lastDirection = direction;

        // Check that robot energy is properly draining
        if (energy <= 0) {
            // DEBUG
            // cout << "Energy depleted. Robot stopped." << endl;
            return;
        }
    }

    // Find alternative direction (when robot is stuck!!!)
    char FindAlternativeDirection() {
        // Returns a random direction...just do something else!!!
        int randomDirection = rand() % 4;
        switch (randomDirection) {
            case 0: return 'N';
            case 1: return 'E';
            case 2: return 'S';
            case 3: return 'W';
            default: return 'N'; // Default direction
        }
    }


    // Move and spend energy
    void ExecuteDirection(char direction, Grid& grid) {
        switch (direction) {
            case 'N': MoveNorth(grid); break;
            case 'E': MoveEast(grid); break;
            case 'S': MoveSouth(grid); break;
            case 'W': MoveWest(grid); break;
        }
        // Deplete energy after direction
        energy--;
    }


    // "Breed" robots...more like clone really.
    Robot Breed(const Robot& partner) const {
        Robot offspring;
            offspring.genes = Genes::Merge(this->genes, partner.genes);
        return offspring;
    }

    void PrintReport() const {

    }

    // DEBUG
    // void generateReport(int turns) const {
        // cout << "Robot Performance Report:" << endl;
        // cout << "Starting Position: (" << initialX << ", " << initialY << ")" << endl;
        // cout << "Ending Position: (" << x << ", " << y << ")" << endl;
        // cout << "Turns before death: " << turns << endl;
        // cout << "Batteries Collected: " << batteriesCollected << endl;
    // }

    void run(Grid &grid) {
        int turns = 0; // Couns # of turns

        // Set starting position (if not done)
        initialX = x;
        initialY = y;

        // Sim loop
        while (energy > 0) {
            MakeDecision(grid); // Make move depeding on genePool
            turns++; // Increment count

            // Check for drained robots
            if (energy <= 0) {
                break;
            }
        }

        // DEBUG
        // Generate performance report
        // generateReport(turns);
    }

    void GridReporting(const Grid& grid);
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
        //  Bubble sort, thank you friends in discord for the reminder
        for (int i = 0; i < size - 1; ++i) {
            for (int j = 0; j < size - i - 1; ++j) {
                if (robots[j].CalculateFitness() < robots[j + 1].CalculateFitness()) {
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
            totalFitness += robots[i].CalculateFitness();
        }
        return totalFitness / size;
    }

    void BreedNewGeneration() {
        // Top half highest scores
        int halfSize = size / 2;

        for (int i = halfSize; i < size; ++i) {
            // Select two random robots from top 100
            int parent1Index = rand() % halfSize;
            int parent2Index = rand() % halfSize;

            // Assure different parents are used
            while (parent1Index == parent2Index) {
                parent2Index = rand() % halfSize;
            }

            // Breed and replace less weaker robots
            robots[i] = robots[parent1Index].Breed(robots[parent2Index]);
        }
    }
};


void Grid::PlaceRobot(Robot& r) {
    int x, y;
    this ->robot = &r;
    do {
        // Random coordinates for inner grid (no walls)
        x = rand() % 10 + 1;
        y = rand() % 10 + 1;
    } while (GridArray[x][y] != EMPTY); // Loop until empty cell found

    // Place robot empty cell
    GridArray[x][y] = ROBOT;
    r.SetPosition(x, y);
    this->robot = &r;
};

void programGreeting() {
cout << "    ____        __     ______          __                                                                            " << endl;
cout << "   / __ )____  / /_   /_  __/_______  / /__                                                                          " << endl;
cout << "  / __  / __ \\/ __/    / / / ___/ _ \\/ //_/                                                                          " << endl;
cout << " / /_/ / /_/ / /_     / / / /  /  __/ ,<                                                                             " << endl;
cout << "/_____/\\____/\\__/    /_/ /_/   \\___/_/|_|                                                                            " << endl;
cout << "       ________             ______            __               __   ______                           __  _           " << endl;
cout << "      /_  __/ /_   ___     / ____/   ______  / /   _____  ____/ /  / ____/__  ____  ___  _________ _/ /_(_)___  ____  " << endl;
cout << "        / / / __ \\/ _ \\   / __/ | | / / __ \\/ / | / / _ \\/ __  /  / / __/ _ \\/ __ \\/ _ \\/ ___/ __ `/ __/ / __ \\/ __ \\" << endl;
cout << "       / / / / / /  __/  / /___ | |/ / /_/ / /| |/ /  __/ /_/ /  / /_/ /  __/ / / /  __/ /  / /_/ / /_/ / /_/ / / / /" << endl;
cout << "      /_/ /_/ /_/\\___/  /_____/ |___/\\____/_/ |___/\\___/\\__,_/   \\____/\\___/_/ /_/\\___/_/   \\__,_/\\__/_/\\____/_/ /_/" << endl;

                                                                                                                     
};


int menu(int& choice, bool firstTime) {
    if (firstTime) {
        cout << "\nYou are now part of the collective. There is no escape!" << endl;
    } else {
        cout << "\nFauxcutus is regenerating. What will you do?" << endl;
    }
    cout << "1) Assimilate new bots." << endl;
    cout << "2) Warp out of here!" << endl;
    
    string input;
    while (true) {
        getline(cin, input);
        // Check if input is "1" or "2"
        if (input == "1" || input == "2") {
            // Convert string to integer
            choice = stoi(input);
            break;
        } else {
            cout << "Enter 1 or 2. Resistance is futile: ";
        }
    }
    return choice;
}

int main() {
    int choice;
    bool firstTime = true;
    srand(static_cast<unsigned int>(time(0)));

    const int MAX_ROBOTS = 200;
    const int MAX_GENERATIONS = 10;
    Robot robots[MAX_ROBOTS];
    double averageFitnessPerGeneration[MAX_GENERATIONS] = {0};

    programGreeting();
    menu(choice, firstTime);
    firstTime = false;
    while (choice != 2) {
        for (int gen = 0; gen < MAX_GENERATIONS; ++gen) {
            for (int i = 0; i < MAX_ROBOTS; ++i) {
                Grid grid;
                grid.PlaceRobot(robots[i]);
                robots[i].run(grid);  // Simulation

                // Uncomment line below for robot's simulation status fitness score
                // cout << "Robot " << i + 1 << " of Generation " << gen + 1 << " Fitness Score: " << robots[i].CalculateFitness() << endl;
            }
            Breeding breeding(robots, MAX_ROBOTS);
            breeding.SortRobotsByFitness();
            double avgFitness = breeding.CalculateAverageFitness();
            averageFitnessPerGeneration[gen] = avgFitness;
            breeding.BreedNewGeneration();

            // Uncomment line to see live fitness scores and average/generation
            // cout << "Generation " << gen + 1 << " complete. Average Fitness: " << avgFitness << endl;
        }
        // Print summary table
        cout << "\nBot Drone\tAverage Drone Fitness\n";
        cout << "Generation\tXilithium Collected\n";    
        for (int i = 0; i < MAX_GENERATIONS; ++i) {
            cout << setw(3) << setfill('0') << i * MAX_ROBOTS + 1
                << " - " << setw(3) << setfill('0') << (i + 1) * MAX_ROBOTS
                << "\t" << fixed << setprecision(3) << averageFitnessPerGeneration[i] << endl;
        }

        menu(choice, firstTime);
        cout << "1) To continue assimilating." << endl;
        cout << "2) Warp out of here!" << endl;
    }
    cout << "Helm...warp 9.6! Engage!\n" << endl;
    return 0;

};

