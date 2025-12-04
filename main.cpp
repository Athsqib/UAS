#include <iostream>
#include <cstdlib>
#include <limits>
#include <vector>
#include <chrono>
#include <thread>
#include <random>
#include <conio.h>
#include <windows.h>
#include <array>

using namespace std;

const int tinggi = 10;
const int lebar = 30;

using Map = array<string, tinggi>;

vector<Map> allMaps = {
    Map{
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
    },
    Map{
        "##############################",
        "#***##********#****###*******#",
        "#**#***###***##***#***##*****#",
        "#*****##***#****##***#*******#",
        "##***###****#***##**##***#***#",
        "#******#****##*******#****#**#",
        "#*###****###***##****###***#*#",
        "#**#****#***#*****#***#*****##",
        "#*****##******##********##***#",
        "##############################"
    },
    Map{
        "##############################",
        "#**********#****###**********#",
        "#**###**#*#**#**#***##*******#",
        "#**#****#****#****##****###**#",
        "#*####**#**###***#*****###**##",
        "#*******#*************#******#",
        "#*##**#*###***##***###****####",
        "#**#**#******#**#******##****#",
        "#*****#**********************#",
        "##############################"
    },
    Map{
        "##############################",
        "#****##****#***###*****##****#",
        "#**#****#****##***##***#*****#",
        "#***###****#******#***###****#",
        "##*###***####**##***###****###",
        "#****#*******##*****#****#***#",
        "#*##***#****###***##***###**##",
        "#***#*****#****#********#****#",
        "#******###************###****#",
        "##############################"
    },
    Map{
        "##############################",
        "#**#**##******###***#*****#**#",
        "#***#******###***#****###****#",
        "##***##****#***####***#***##*#",
        "#****###***#****#****##***#**#",
        "#**#****#***********####***#*#",
        "#*###***###****##***#*****#**#",
        "#***#*******#******#***#*****#",
        "#****###***************###***#",
        "##############################"
    }
};

Map* currentMap = nullptr;

char cell(int y, int x){
    if (y < 0 || y >= tinggi || x < 0 || x >= lebar) return '#';
    return (*currentMap)[y][x];
}

struct player {
    int playerX, playerY;
    int money = 8000;
    int mailAmount;
    int stamPlayer = 30;
    int maxStamPlayer = 100;
    bool defend;
} player;

struct pencuri {
    int thiefX, thiefY;
    int amount;
    int stamThief = 20;
    int maxStamThief = 20;
    int chance = 20;
    int fullAmount = 50;
    int effectiveness = 5;
    bool active;
} pencuri;

struct mail {
    int mailX, mailY;
    int amount;
    int fullAmount = 100;
    int effectiveness = 10;
    bool taken;
} mailItem;

struct staminaItems {
    string name;
    int price;
    string description;
    int staminaRestore;
    int stockAmount;
    int stockEffectiveness;
} staminaItem;

struct shopUpgrade {
    string name;
    int price;
    string description;
    int level;
    int stock;
} shopUpgradeItem;

vector<staminaItems> shopStaminaItems = {
    {"Air Kelapa", 50, "Kaya akan elektrolit alami seperti kalium untuk mengganti cairan tubuh yang hilang saat beraktivitas fisik yang dapat menambahkan stamina mu sebanyak 30 poin.", 30, 30, 10},
    {"Kopi", 150, "Minuman hitam pait yang penuh kafein, dapat menambah stamina mu sebanyak 60 poin.", 60, rand() % 20 + 5},
    {"Smoothie Piscang", 300, "Smoothie dengan rasa campuran dari pisang dan selai kacang yang dapat menambah stamina mu sebanyak 100 poin.", 100, 10, 1},
    {"Blue Bull", 500, "Minuman energi yang mengandung kafein, taurine, dan vitamin B yang dapat menambah stamina mu sebanyak 150 poin.", 150, 5, 1},
    {"Expresso 10 Shot", 1000, "Minuman kopi yang sangat kuat dengan 10 shot espresso yang dapat menambah stamina mu sebanyak 300 poin.", 300, 3, 1}
};

vector<shopUpgrade> shopUpgradeItems = {
    {"Stamina Upgrade", 1000, "Meningkatkan kapasitas stamina mu sebanyak 50 poin.", 0, 5},
    {"Mail Spawn Upgrade", 1500, "Meningkatkan jumlah maksimal paket yang dapat diambil sebanyak 2.", 0, 5},
    {"Mail Amount Upgrade", 2000, "Meningkatkan jumlah uang yang didapat dari setiap paket sebanyak 50 rupiah.", 0, 10},
    {"Mail Effectiveness Upgrade", 2000, "Meningkatkan peluang mendapatkan paket bernilai tinggi sebanyak 15%.", 0, 5},
    {"Thief Spawn Upgrade", 2500, "Meningkatkan peluang munculnya maling sebanyak 10%.", 0, 5},
    {"Thief Amount Upgrade", 3000, "Meningkatkan jumlah uang yang didapat dari maling sebanyak 20 rupiah.", 0, 10},
    {"Thief Effectiveness Upgrade", 3000, "Meningkatkan efektivitas maling dalam mencuri paketmu sebanyak 10%.", 0, 5}
};

vector<int> actualStaminaStock;

int mailSpawned = 0;
int maxMail = 4;

int playerX, playerY;

void wait(){
    this_thread::sleep_for(1s);
}

void waitLong(){
    this_thread::sleep_for(3s);
}

void flushInput(){
    while (kbhit()) _getch();
}

int waitNewKey(){
    static bool keyWasDown = false;

    for (;;){
        bool w = GetAsyncKeyState('W') & 0x8000 || GetAsyncKeyState('w') & 0x8000;
        bool a = GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState('a') & 0x8000;
        bool s = GetAsyncKeyState('S') & 0x8000 || GetAsyncKeyState('s') & 0x8000;
        bool d = GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState('d') & 0x8000;
        bool q = GetAsyncKeyState('Q') & 0x8000 || GetAsyncKeyState('q') & 0x8000;
        bool z = GetAsyncKeyState('Z') & 0x8000 || GetAsyncKeyState('z') & 0x8000;
        bool x = GetAsyncKeyState('X') & 0x8000 || GetAsyncKeyState('x') & 0x8000;
        bool c = GetAsyncKeyState('C') & 0x8000 || GetAsyncKeyState('c') & 0x8000;
        bool y = GetAsyncKeyState('Y') & 0x8000 || GetAsyncKeyState('y') & 0x8000;
        bool n = GetAsyncKeyState('N') & 0x8000 || GetAsyncKeyState('n') & 0x8000;
        bool num0 = GetAsyncKeyState('0') & 0x8000;
        bool num1 = GetAsyncKeyState('1') & 0x8000;
        bool num2 = GetAsyncKeyState('2') & 0x8000;
        bool num3 = GetAsyncKeyState('3') & 0x8000;
        bool num4 = GetAsyncKeyState('4') & 0x8000;
        

        int key = 0;

        if (w) key = 'w';
        else if (a) key = 'a';
        else if (s) key = 's';
        else if (d) key = 'd';
        else if (q) key = 'q';
        else if (z) key = 'z';
        else if (x) key = 'x';
        else if (c) key = 'c';
        else if (y) key = 'y';
        else if (n) key = 'n';
        else if (num0) key = '0';
        else if (num1) key = '1';
        else if (num2) key = '2';
        else if (num3) key = '3';
        else if (num4) key = '4';
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

void playerPlacement(){
    bool placed = false;
    for (int y = 0; y < tinggi && !placed; y++) {
        for (int x = 0; x < lebar; x++) {
            if ((*currentMap)[y][x] == '*') {
                playerX = x;
                playerY = y;
                placed = true;
                break;
            }
        }
    }
}

void printMaps(){
    system("cls||clear");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    for (int i = 0; i < tinggi; i++){
        for (int j = 0; j < lebar; j++){
            char c = cell(i, j);
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
    if (chance < pencuri.chance){
        int tries = 0;
        do{
            pencuri.thiefX = std::rand() % (lebar - 2) + 1;
            pencuri.thiefY = std::rand() % (tinggi - 2) + 1;
        } while (cell(pencuri.thiefY, pencuri.thiefX) != '*' && tries++ < 200);
        pencuri.active = true;
        pencuri.stamThief = 20;
        cout << "\nAda maling disekitar mu. Hati hati dalam perjalanannya.\n";
        wait();
    }
    else{
        pencuri.active = false;
    }
}

void matiThief(){
    if (pencuri.stamThief <= 0){
        pencuri.amount = rand() % pencuri.fullAmount + pencuri.effectiveness;
        cout << "Kamu berhasil mengelabui maling tersebut! Kamu mendapatkan " << pencuri.amount << " rupiah!\n";
        wait();
        player.money += pencuri.amount;
        pencuri.active = false;
        return;
    }
}

void giliranThief(){
    int damage;
    if (!player.defend){
        damage = rand() % 6 + 2;
    } 
    else if (player.defend){
        damage = rand() % 3 + 1;
        player.defend = false;
    }
    player.stamPlayer -= damage;
    pencuri.stamThief -= 3;
    cout << "Maling mencoba untuk mengambil paketmu (-" << damage << " stamina)!\n";
    wait();
}

void bertahanThief(){
    int cost = 10;
    if (player.stamPlayer < cost){
        cout << "Kamu kelelahan, kamu tidak memiliki stamina untuk bertahan!\n";
        wait();
        return;
    }
    player.stamPlayer -= cost;
    cout << "Kamu bertahan, mengurangi damage selanjutnya.\n";
    wait();
    player.defend = true;
    giliranThief();
    cout << "Pertahanan habis\n";
    wait();
}

void menyerangThief(){
    int cost = 5;
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
    giliranThief();
}   

void kaburThief(){
    int chance = rand() % 100;
    if (chance < 50){
        cout << "Kamu berhasil kabur dari kejaran maling.\n";
        wait();
        pencuri.active = false;
    }
    else {
        cout << "Kamu gagal kabur :[\n";
        wait();
        giliranThief();
    }
}

void interactThief(){
    if (!pencuri.active) return;
    if (playerX != pencuri.thiefX || playerY != pencuri.thiefY) return;
    while (pencuri.active && pencuri.stamThief > 0 && player.stamPlayer > 0){
        system("cls||clear");
        printMaps();
        cout << "\nKamu bertemu dengan maling, bertahan sampai maling itu kelelahan!!\n";
        cout << "1. Menyerang (Z)\n2. Bertahan (X)\n3. Kabur (C)\n";
        cout << "Stamina mu = " << player.stamPlayer << "/" << player.maxStamPlayer << endl;
        cout << "Stamina maling = " << pencuri.stamThief << "/" << pencuri.maxStamThief << endl;
        cout << "Pilihamnu = (Z/X/C)\n";
        flushInput();
            
        switch (waitNewKey()) {
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
                cout << "Kami anggap anda diam ;].\n";
                giliranThief();
                break;
        }
    }
    system("cls||clear");
    printMaps();          
    matiThief();
    if (player.stamPlayer <= 10){
        cout << "Kamu kelelahan dan jatuh pingsan! Maling itu mengambil paketmu dan pergi...\n";
        waitLong();
        int randomLoss = rand() % mailItem.amount + 10;
        if (mailSpawned = maxMail) player.money -= randomLoss;
        else maxMail--;
        pencuri.active = false;
    }
}

void moveThiefTowardPlayer(){
    if (!pencuri.active) return;

    int dx = 0;
    int dy = 0;

    if(pencuri.thiefX < playerX) dx = 1;
    else if(pencuri.thiefX > playerX) dx = -1;

    if(pencuri.thiefY < playerY) dy = 1;
    else if(pencuri.thiefY > playerY) dy = -1;

    int newThiefX = pencuri.thiefX + dx;
    int newThiefY = pencuri.thiefY + dy;

    if (newThiefX >= 0 && newThiefX < lebar && newThiefY >= 0 && newThiefY < tinggi && cell(newThiefY, newThiefX) == '*') {
        pencuri.thiefX = newThiefX; 
        pencuri.thiefY = newThiefY;
    }
}

void trySpawnMail(){
    if (mailSpawned >= maxMail) return;
    if (!mailItem.taken) return;

    int percobaan = 0;
    do {
        mailItem.mailX = rand() % (lebar - 2) + 1;
        mailItem.mailY = rand() % (tinggi - 2) + 1;
        percobaan++;
    } while (cell(mailItem.mailY, mailItem.mailX) != '*' && percobaan < 200);

    mailItem.amount = rand() % mailItem.fullAmount + mailItem.effectiveness;
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

void map(){
    bool move = true;
    playerPlacement();
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

        if (move) player.stamPlayer--;

        if (player.stamPlayer <= 0){
            printMaps();
            cout << "\nKamu kelelahan dan tidak bisa melanjutkan perjalananmu.\n";
            waitLong();
            move = false;
        }

        if (newX >= 0 && newX < lebar && newY >= 0 && newY < tinggi && cell(newY, newX) == '*') {
            (*currentMap)[playerY][playerX] = '*';
            playerX = newX;
            playerY = newY;
            moveThiefTowardPlayer();
            (*currentMap)[playerY][playerX] = 'P';
            
            checkPickup();
            interactThief();
            flushInput();

            printMaps();
            cout << "Bergerak (WASD) Tidak dapat di hold.";
            cout << "\nUang = " << player.money << " rupiah\n";
            cout << "Stamina = " << player.stamPlayer << "/" << player.maxStamPlayer << endl;
            cout << "Lokasi paket = " << mailSpawned << "/" << maxMail << endl;
        };
    }
}

void notEnoughMoney(int punya, int butuh){
    cout << "Uang mu tidak cukup! Kamu memiliki " << punya << " rupiah, tetapi butuh " << butuh << " rupiah.\n";
    wait();
}

void upgradeStaminaCapacity(int){
    player.maxStamPlayer += 50;
} 

void upgradeMailSpawnAmount(int){
    maxMail += 2;
}

void upgradeMailAmount(int){
    mailItem.fullAmount += 50;
}

void upgradeMailEffectiveness(int){
    mailItem.effectiveness += ((15/100) * mailItem.effectiveness);
}

void upgradeThiefSpawnChance(int){
    pencuri.chance += 10;
}

void upgradeThiefAmount(int){
    pencuri.fullAmount += 20;
}

void upgradeThiefEffectiveness(int){
    pencuri.effectiveness += ((10/100) * pencuri.effectiveness);
}

void choiceShopStaminaItem(int index){
    int stock = rand() % shopStaminaItems[index].stockAmount + shopStaminaItems[index].stockEffectiveness;
    cout << shopStaminaItems[index].description << endl;
    cout << "Apakah kamu yakin ingin membelinya? (Y/N)";
    flushInput();
    int confirmUp = waitNewKey();
    if (confirmUp == 'y' || confirmUp == 'Y'){
        if (player.money >= shopStaminaItems[index].price){
            player.money -= shopStaminaItems[index].price;
            player.stamPlayer += shopStaminaItems[index].staminaRestore;
            cout << "\nKamu membeli " << shopStaminaItems[index].name << "!\nStamina mu bertambah " << shopStaminaItems[index].staminaRestore << " poin.\n";
            waitLong();
        }
        else if (stock <= 0){
            cout << "Maaf, stok " << shopStaminaItems[index].name << " sedang habis.\n";
            wait();
        }
        else {
            notEnoughMoney(player.money, shopStaminaItems[index].price);
        }
    }
    else if (confirmUp == 'n' || confirmUp == 'N'){
        cout << "Pembelian dibatalkan.\n";
        wait();
    }
}

void shopUpgradeRaisePrice(int index){
    shopUpgradeItems[index].price += shopUpgradeItems[index].price / 2;
}

void choiceShopUpgradeItem(int index, void (*upgradeFunction)(int)){
    cout << shopUpgradeItems[index].description << endl;
    cout << "Apakah kamu yakin ingin meng-upgrade? (Y/N)";
    flushInput();
    int confirmUp = waitNewKey();
    if (confirmUp == 'y' || confirmUp == 'Y'){
        if (player.money >= shopUpgradeItems[index].price){
            if (shopUpgradeItems[index].level >= shopUpgradeItems[index].stock){
                cout << "Upgrade sudah mencapai level maksimal.\n";
                wait();
            }
            else {
                player.money -= shopUpgradeItems[index].price;
                upgradeFunction(index);
                shopUpgradeItems[index].level++;
                cout << "\nKamu meng-upgrade " << shopUpgradeItems[index].name << "!";
                shopUpgradeRaisePrice(index);
                waitLong();
            }
        }
        else {
            notEnoughMoney(player.money, shopUpgradeItems[index].price);
        }
    }
    else if (confirmUp == 'n' || confirmUp == 'N'){
        cout << "Upgrade dibatalkan.\n";
        wait();
    }
}

void shop(){
    bool upgradeShop = false;
    bool move = false;
    bool openshop = true;
    bool inUpgradedShop = false;
    while (!move && openshop){
        system("cls||clear");
        cout << "Selamat datang di toko!\n";
        for (size_t i = 0; i < 3; i++){
            int stock = rand() % shopStaminaItems[i].stockAmount + shopStaminaItems[i].stockEffectiveness;
            cout << i + 1 << ". " << shopStaminaItems[i].name << " - " << shopStaminaItems[i].price << " rupiah" << " (Stok: " << stock << ")\n";
        }
        if (!upgradeShop) cout << "4. Upgrade Toko - 1000 rupiah";
        else if (upgradeShop) {
            cout << "4. Upgrade Toko (Sudah di-upgrade) - Pilih untuk masuk ke dalam toko yang sudah di upgrade";
        }
        cout << "\n0. Keluar dari toko\n";
        cout << "Uang mu = " << player.money << " rupiah\n";
        cout << "Stamina = " << player.stamPlayer << "/" << player.maxStamPlayer << endl;
        cout << "Pilihanmu = ";
        flushInput();
        int choice = waitNewKey();
        if (choice == '1'){
            choiceShopStaminaItem(0);
        }
        else if (choice == '2'){
            choiceShopStaminaItem(1);
        }
        else if (choice == '3'){
            choiceShopStaminaItem(2);
        }
        else if (choice == '4'){
            if (!upgradeShop){
                cout << "Apakah kamu yakin ingin meng-upgrade toko? (Y/N)";
                flushInput();
                int confirm = waitNewKey();
                if (confirm == 'y' || confirm == 'Y'){
                    if (player.money >= 1000){
                        player.money -= 1000;
                        upgradeShop = true;
                        cout << "\nToko berhasil di-upgrade! Lebih banyak item tersedia di toko!\n";
                        waitLong();
                    }
                    else {
                        notEnoughMoney(player.money, 1000);
                    }
                }
                else if (confirm == 'n' || confirm == 'N'){
                    cout << "Upgrade dibatalkan.\n";
                    wait();
                }
            }
            else {
                cout << "Masuk ke dalam toko yang sudah di-upgrade? (Y/N)";
                flushInput();
                int confirm = waitNewKey();
                if (confirm == 'y' || confirm == 'Y'){
                    inUpgradedShop = true;
                    while (inUpgradedShop){
                        system("cls||clear");
                        cout << "Toko yang sudah di-upgrade:";
                        for (size_t i = 3; i < shopStaminaItems.size(); i++){
                            cout << "\n" << i - 2 << ". " << shopStaminaItems[i].name << " - " << shopStaminaItems[i].price << " rupiah (Stok: " << actualStaminaStock[i] << ")";
                        }
                        for (size_t i = 0; i < shopUpgradeItems.size(); i++){
                            cout << "\n" << i + 3 << ". " << shopUpgradeItems[i].name << " - " << shopUpgradeItems[i].price << " rupiah (Level: " << shopUpgradeItems[i].level << ")";
                            if (shopUpgradeItems[i].level >= shopUpgradeItems[i].stock){
                                cout << " (Sudah max)";
                            }
                        }
                        cout << "0. Kembali ke toko utama\n";
                        cout << "Uang mu = " << player.money << " rupiah\n";
                        cout << "Stamina = " << player.stamPlayer << "/" << player.maxStamPlayer << endl;
                        cout << "Pilihanmu = ";
                        int choiceUp = waitNewKey();
                        switch (choiceUp) {
                            case '0':
                                cout << "Kembali ke toko utama...\n";
                                inUpgradedShop = false;
                                break;
                            case '1':
                                choiceShopStaminaItem(3);
                                break;
                            case '2':
                                choiceShopStaminaItem(4);
                                break;
                            case '3':
                                choiceShopUpgradeItem(0, upgradeStaminaCapacity);
                                break;
                            case '4':
                                choiceShopUpgradeItem(1, upgradeMailSpawnAmount);
                                break;
                            case '5':
                                choiceShopUpgradeItem(2, upgradeMailAmount);
                                break;
                            case '6':
                                choiceShopUpgradeItem(3, upgradeMailEffectiveness);
                                break;
                            case '7':
                                choiceShopUpgradeItem(4, upgradeThiefSpawnChance);
                                break;
                            case '8':
                                choiceShopUpgradeItem(5, upgradeThiefAmount);
                                break;
                            case '9':
                                choiceShopUpgradeItem(6, upgradeThiefEffectiveness);
                                break;
                            default:
                                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                                wait();
                                break;
                            }
                        }
                    }
                else if (confirm == 'n' || confirm == 'N'){
                    cout << "Masuk ke bagian toko dibatalkan.\n";
                    wait();
                }
                wait();
            }
        }
        else if (choice == '0'){
            cout << "Keluar dari toko...\n";
            wait();
            move = true;
            openshop = false;
            map();
        }
        else {
            cout << "Pilihan tidak valid. Silakan coba lagi.\n";
            wait();
        
        }
    system("cls||clear");
    }
}

int main(){
    srand(time(0));
    currentMap = &allMaps[rand() % allMaps.size()];
    for (const auto& item : shopStaminaItems){
        int stock = rand() % item.stockAmount + item.stockEffectiveness;
        actualStaminaStock.push_back(stock);
    }
    shop();
    
}