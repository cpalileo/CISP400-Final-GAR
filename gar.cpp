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


int main(){
    Grid grid;
    grid.InitializeRobotPlacement(200);
    grid.InitializeBatteryPlacement(40);
};