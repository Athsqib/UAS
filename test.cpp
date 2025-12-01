#include <iostream>
using namespace std;

const int ROWS = 10;
const int COLS = 30;

// Map — only environment
char mapData[ROWS][COLS + 1] = {
    "##############################",
    "#P********#**#***************#",
    "#**###**###**###***#######***#",
    "#**#****#****#***###*********#",
    "#*####**#**###***#*****###**##",
    "#****************###*********#",
    "#*##**#####***##***#****######",
    "#**#**#******#**#******##****#",
    "#*****#**********************#",
    "##############################"
};

// 🔥 Player entity
struct Player {
    int x, y;
    int gold;
} player;

// 🔥 Gold entity
struct Gold {
    int x, y;
    int amount;
    bool taken;
} goldItem;

// -------------------------

bool isWall(int y, int x) {
    return (mapData[y][x] == '#');
}

void drawMap() {
    system("clear || cls");

    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            
            // Draw player
            if (player.x == x && player.y == y) {
                cout << 'P';
            }
            // Draw gold (only if not taken)
            else if (!goldItem.taken && goldItem.x == x && goldItem.y == y) {
                cout << '$';
            }
            else {
                cout << mapData[y][x];
            }

        }
        cout << endl;
    }

    cout << "\n=== PLAYER STATS ===\n";
    cout << "Gold: " << player.gold << endl;
}

void checkPickup() {
    if (!goldItem.taken &&
        player.x == goldItem.x &&
        player.y == goldItem.y)
    {
        goldItem.taken = true;
        player.gold += goldItem.amount;
        cout << "\nYou picked up " << goldItem.amount << " gold!\n";
    }
}

int main() {
    // Initialize player
    player.x = 1;
    player.y = 1;
    player.gold = 0;

    // Place gold entity
    goldItem.x = 15;
    goldItem.y = 5;
    goldItem.amount = 50;
    goldItem.taken = false;

    char input;

    while (true) {
        drawMap();
        cout << "Move (WASD): ";
        cin >> input;

        int nx = player.x;
        int ny = player.y;

        if (input == 'w') ny--;
        if (input == 's') ny++;
        if (input == 'a') nx--;
        if (input == 'd') nx++;

        // Movement only if not wall
        if (!isWall(ny, nx)) {
            player.x = nx;
            player.y = ny;

            checkPickup(); // 🔥 check if touching gold
        }
    }

    return 0;
}
