#include <iostream>
#include <cstdlib>
#include <limits>
#include <vector>
#include <chrono>
#include <thread>
#include <random>
#include <conio.h>
#include <windows.h>

using namespace std;

const int tinggi = 10;
const int lebar = 30;

char maps[tinggi][lebar+1] = {
    "##############################",
    "#*********#**#***************#",
    "#**###**###**###***#######***#",
    "#**#****#****#***###*********#",
    "#*####**#**###***#*****###**##",
    "#****************###*********#",
    "#*##**#####***##***#****######",
    "#**#**#******#**#******##****#",
    "#*****#**********************#",
    "##############################"
};

struct player {
    int playerX, playerY;
    int money = 0;
    int mailAmount;
    int stamPlayer = 100;
    bool defend;
} player;

struct pencuri {
    int thiefX, thiefY;
    int amount;
    int stamThief = 20;
    bool active;
} pencuri;

struct mail {
    int mailX, mailY;
    int amount;
    bool taken;
} mailItem;

int mailSpawned = 0;
int maxMail;

int playerX = 1, playerY = 1;

void wait(){
    this_thread::sleep_for(1s);
}

void flushInput(){
    while (kbhit()) _getch();
}

void printMaps(){
    system("cls||clear");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    for (int i = 0; i < tinggi; i++){
        for (int j = 0; j < lebar; j++){
            char c = maps[i][j];
            bool printed = false;

            if (playerX == j && playerY == i){
                SetConsoleTextAttribute(hConsole, 10);
                cout << 'P';
                printed = true;
            }   
            else if (!mailItem.taken && mailItem.mailX == j && mailItem.mailY == i) {
                SetConsoleTextAttribute(hConsole, 14);
                cout << '$';
                printed = true;
            }
            else if (pencuri.active && pencuri.thiefX == j && pencuri.thiefY == i) {
                SetConsoleTextAttribute(hConsole, 4);
                cout << '!';
                printed = true;
            }
            if (!printed) {
                if (c == '#'){
                    SetConsoleTextAttribute(hConsole, 1);
                    cout << c;
                }
                else if (c == '*'){
                    SetConsoleTextAttribute(hConsole, 0);
                    cout << c;
                }
                else {
                    SetConsoleTextAttribute(hConsole, 7);
                    cout << c;
                }
            }
            
        }
        cout << endl;
    }
    SetConsoleTextAttribute(hConsole, 7);
}

void trySpawnThief(){
    int chance = rand() % 100;
    if (chance < 99) {
        int tries = 0;
        do{
            pencuri.thiefX = std::rand() % (lebar - 2) + 1;
            pencuri.thiefY = std::rand() % (tinggi - 2) + 1;
        } while (maps[pencuri.thiefY][pencuri.thiefX] != '*' && ++tries < 200);
        pencuri.active = true;
        cout << "\nAda maling disekitar mu. Hati hati dalam perjalanannya.\n";
        wait();
    }
    else {
        pencuri.active = false;
    }
}

void giliranThief(){
    int damage;
    if (!player.defend){
        damage = rand() % 6 + 2;
    } 
    else if (player.defend){
        damage = rand() % 3 + 1;
        cout << "Pertahanan habis\n";
        wait();
        player.defend = false;
    }
    player.stamPlayer -= damage;
    cout << "Maling mencoba untuk mengambil paketmu (-" << damage << " stamina)!\n";
    wait();
}

void bertahanThief(){
    int cost = 20;
    if (player.stamPlayer < cost){
        cout << "Kamu kelelahan, kamu tidak memiliki stamina untuk bertahan!\n";
        wait();
        return;
    }
    player.stamPlayer -= cost;
    cout << "Kamu bertahan, mengurangi damage selanjutnya.\n";
    wait();
    giliranThief();
}

void menyerangThief(){
    int cost = 15;
    if (player.stamPlayer < cost){
        cout << "Kamu kelelahan, kamu tidak memiliki stamina untuk menyerang!\n";
        wait();
        return;
    }
    player.stamPlayer -= cost;

    int damage = rand() % 6 + 5;
    pencuri.stamThief -= damage;
    cout << "Kamu menyerang maling (-" << damage << " stamina maling)\n";
    wait();

    if (pencuri.stamThief <= 0){
        int drop = pencuri.amount;
        drop = rand() % 30 + 1;
        cout << "Kamu berhasil mengelabui maling tersebut! Kamu mendapatkan " << drop << " rupiah!\n";
        wait();
        player.money += drop;
        pencuri.active = false;
        return;
    }
    giliranThief();
}   

void kaburThief(){
    int chance = rand() % 100;
    if (chance < 50){
        cout << "Kamu berhasil kabur dari kejaran maling";
        wait();
        pencuri.active = false;
    }
    else {
        cout << "Kamu gagal kabur :[";
        wait();
        giliranThief();
    }
}
void interactThief(){
    if (!pencuri.active) return;
    if (playerX != pencuri.thiefX || playerY != pencuri.thiefY) return;
    char opsi;
    while (pencuri.active && pencuri.stamThief > 0 && player.stamPlayer > 0){
        system("cls||clear");
        printMaps();
        cout << "\nKamu bertemu dengan maling, bertahan sampai maling itu kelelahan!!\n";
        cout << "1. Menyerang (Z)\n2. Bertahan (X)\n3. Kabur (C)\n";
        cout << "Stamina mu = " << player.stamPlayer << endl;
        cout << "Stamia maling = " << pencuri.stamThief << endl;
        cout << "Pilihamnu = ";
        flushInput();
        cin >> opsi;

        switch (opsi) {
            case 'z':
                menyerangThief();
                break;
            case 'x':
                bertahanThief();
                break;
            case 'c':
                kaburThief();
                break;
            default:
                cout << "Kami anggap plihanmu menyerang ;].\n";
                menyerangThief();
                break;
        }
    }
}

void trySpawnMail(){
    maxMail = rand() % 5 + 2;

    if (mailSpawned >= maxMail) return;
    if (!mailItem.taken) return;

    int percobaan = 0;
    do {
        mailItem.mailX = rand() % (lebar - 2) + 1;
        mailItem.mailY = rand() % (tinggi - 2) + 1;
        percobaan++;
    } while (maps[mailItem.mailY][mailItem.mailX] != '*' && percobaan < 200);

    mailItem.amount = rand() % 100 + 1;
    mailItem.taken = false;

    mailSpawned++;

}

void checkPickup() {
    if (!mailItem.taken && playerX == mailItem.mailX && playerY == mailItem.mailY){
        mailItem.taken = true;
        player.money += mailItem.amount;
        cout << "Kamu sudak menyelesaikan order dan mendapatkan " << mailItem.amount << " rupiah!\n";
        wait();
        trySpawnMail();
        trySpawnThief();
    }
}

int waitNewKey(){
    static bool keyWasDown = false;

    for (;;){
        bool w = GetAsyncKeyState('W') & 0x8000 || GetAsyncKeyState('w') & 0x8000;
        bool a = GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState('a') & 0x8000;
        bool s = GetAsyncKeyState('S') & 0x8000 || GetAsyncKeyState('s') & 0x8000;
        bool d = GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState('d') & 0x8000;
        bool q = GetAsyncKeyState('Q') & 0x8000 || GetAsyncKeyState('q') & 0x8000;

        int key = 0;

        if (w) key = 'w';
        else if (a) key = 'a';
        else if (s) key = 's';
        else if (d) key = 'd';
        else if (q) key = 'q';
        else key = 0;

        if (key && !keyWasDown){
            keyWasDown = true;
            return key;
        }
        if (!key) keyWasDown = false;

        this_thread::sleep_for(20ms);
    }
    return 0;
}

void map(){
    bool move = true;
    printMaps();
    cout << "Bergerak (WASD) Tidak dapat di hold.";
    mailItem.taken = true;
    trySpawnMail();

    cout << "\nUang = " << player.money << " rupiah\n";


    while (move){
        int key = waitNewKey();
        if (key == 'q') break;
        
        int newX = playerX;
        int newY = playerY;

        if (key == 'w') newY--;
        else if (key == 'a') newX--;
        else if (key == 's') newY++;
        else if (key == 'd') newX++;
        else if (key == 'q') move = false;

        if (newX >= 0 && newX < lebar && newY >= 0 && newY < tinggi && maps[newY][newX] == '*') {
            maps[playerY][playerX] = '*';
            playerX = newX;
            playerY = newY;
            maps[playerY][playerX] = 'P';
            
            checkPickup();
            interactThief();
            flushInput();

            printMaps();
            cout << "Bergerak (WASD) Tidak dapat di hold.";
            cout << "\nUang = " << player.money << " rupiah\n";
            cout << "Paket yang sudah di antarkan = " << (mailSpawned - 1) << endl;
        };
    }
}

int main(){
    srand(time(0));
    map();
}