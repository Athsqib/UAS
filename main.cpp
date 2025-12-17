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
#include <fstream>

using namespace std;

void start();
void outside();
void shop();
void map();
void house();
void menu();
void wait();
void waitLong();
void playerPlacement(); 
int waitNewKey();
void collectionPage();
void gachaShop();

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

const Map* currentMap = nullptr;

struct player {
    int playerX, playerY;
    int money = 0;
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

struct gachaItems {
    string name;
    int cost;
    string description;
} gachaItemBox;

struct Item {
    string name;
    int code;
    string description;
    bool owned;
} item;

struct day {
    int currentDay = 1;
    int currentTimeHour = 7;
    int currentTimeMinute = 0;
} days;

constexpr int UPGRADE_COUNT = 7;
constexpr int STAMINA_ITEM_COUNT = 5;
constexpr int HOLO_GACHA_COUNT = 15;
constexpr int INDIE_GACHA_COUNT = 5;
constexpr int MINECRAFT_GACHA_COUNT = 6;
constexpr int MATH_GACHA_COUNT = 5;
constexpr int CARD_GACHA_COUNT = 5;
constexpr uint32_t SAVE_VERSION = 1;

struct saveData {
    uint32_t version = SAVE_VERSION;
    int px, py;
    int money;
    int stam, maxStam;

    int day;
    int hour, minute;
    int mapIndex;
    int mailSpawned;

    int upgradeLevels[UPGRADE_COUNT];
    int staminaStock[STAMINA_ITEM_COUNT];
    bool holoOwned[HOLO_GACHA_COUNT];
    bool indieOwned[INDIE_GACHA_COUNT];
    bool minecraftOwned[MINECRAFT_GACHA_COUNT];
    bool mathOwned[MATH_GACHA_COUNT];
    bool cardOwned[CARD_GACHA_COUNT];

    saveData() : version(SAVE_VERSION), px(0), py(0), money(0), stam(0), maxStam(0), day(1), hour(7), minute(0), mapIndex(0) {
        memset(upgradeLevels, 0, sizeof(upgradeLevels));
        memset(staminaStock, 0, sizeof(staminaStock));
        memset(holoOwned, 0, sizeof(holoOwned));
        memset(indieOwned, 0, sizeof(indieOwned));
        memset(minecraftOwned, 0, sizeof(minecraftOwned));
        memset(mathOwned, 0, sizeof(mathOwned));
        memset(cardOwned, 0, sizeof(cardOwned));
    }
};

enum class menuSource {
    OUTSIDE,  HOUSE, SHOP, MAP
};

menuSource lastMenuSource = menuSource::HOUSE;

vector<staminaItems> shopStaminaItems = {
    {"Air Kelapa", 50, "Kaya akan elektrolit alami seperti kalium untuk mengganti cairan tubuh yang hilang saat beraktivitas fisik yang dapat menambahkan stamina mu sebanyak 30 poin.", 30, 30, 10},
    {"Kopi", 150, "Minuman hitam pait yang penuh kafein, dapat menambah stamina mu sebanyak 60 poin.", 60, 20, 5},
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

vector<gachaItems> gachaBox = {
    {"Holo Box", 250, "Sebuah perantara yang akan memberimu kebersamaan dengan memento idol virtual kesukaan mu."},
    {"Indie Box", 250, "Apa itu indie, game MASTERPIECE buatan seseorang yang tidak terafiliasi dengan corporat."},
    {"Minecraft Box", 250, "Menambang, membuat, dan berpetualang. Game masa kecil kalian semua."},
    {"Math Box", 250, "NERAKA"},
    {"Card Box", 250, "Gacha kartu....  Kalian ini kira apaan.."}
};

vector<Item> holoGachaPool {
    {"Teddy Bear", 1, "Boneka beruang yang menemani seorang idol pada masa masa berkarirnya.", false},
    {"Roboco Glasses", 2, "Kacamata yang mendampingi dari model 1.0 nya roboco-chan", false},
    {"AZKi Mic", 3, "Mic kepunyaan salah satu mantan member INoNaKa Music :3", false},
    {"Elite Cat", 4, "Boneka kucing yang mereferensikan dari Self-Proclaimed 'Elite' shrine maiden", false},
    {"Comet Axe", 5, "Kapak iconic yang dipakai seorang Comet Idol", false},
    {"Aki Pigtails", 6, "Benda ini terbang karena kekuatan supernatural", false},
    {"Fried Tarantula", 7, "Masakannya iseng buatan hati merah pada saat Australian Arc", false},
    {"The People's Tears", 8, "Tangisan para fans karena dipanggil teman oleh si rubah putih.", false},
    {"Bandaids ...", 9, "Cerita bizzare seorang idol suci...", false},
    {"Ojou-sama Long Lost Map", 10, "Kehilangan arah pada saat mencari rumahnya di Game Minecraft", false},
    {"Chocolate Teacher", 11, "Sebuah persona dari salah satu demon idol", false},
    {"Dancing Duck", 12, "Seorang idol yang di berikan label sebagai bebek", false},
    {"Wet Onion", 13, "'Bawang' yang menjadi maid nahkoda kapal", false},
    {"Hatotaurus", 14, "Stand seorang black-haired wolf girl", false},
    {"Onigiri", 15, "Mogu mogu!!", false}

};  

vector<Item> indieGachaPool {
    {"Red Heart", 79, "You are filled with determination", false}, 
    {"Flying Strawberry", 80, "It's a plane, It's a bird, no that is a FLYING STRAWBERRY", false},
    {"Needle", 81, "Senjata yang menemani sebuah kumbang dalam petualangannya", false},
    {"Green Toilet", 82, "Item End-Game dari sebuah game terkenal", false},
    {"Blue Soul", 83, "You need to die to progress the game forward", false}
};

vector<Item> minecraftGachaPool {
    {"Dirt Block", 90, "Block tanah logo iconic pada masanya", false},
    {"Iron Ingot", 91, "Biasanya speedrun suka bunuh golem buat dapet ini, kasian golemnya", false},
    {"Gold Ingot", 92, "Alat-alat dari sini jelek semua :[", false},
    {"Diamond", 93, "Kekayaan player", false},
    {"Emerald", 94, "Kasian anak orang kau jadikan budak", false},
    {"Ender Dragon Egg", 95, "Apa tuh teleport teleport pas disentuh", false}
};

vector<Item> mathGachaPool {
    {"+", 100, "Makin banyak", false},
    {"-", 101, "Makin sedikit", false},
    {"x", 102, "Makin banyak part 2", false},
    {":", 103, "Penjahat yang memumculkan angka desimal", false},
    {"Aljabar", 104, "VARIBEL..., apa itu varibel, pengen beli truck", false}
};

vector<Item> cardGachaPool {
    {"UNO", 121, "Game kartu yang punya aturan beda beda setiap tongkrongan", false},
    {"Poker", 122, "Judi dia", false},
    {"KTP", 123, "Gila KTP dapet dari gacha dia", false}, 
    {"SIM", 124, "Nyogok mahal, normal lama", false}, 
    {"Pot of Greed", 125, "This the GREED that bible talking about", false}
};

vector<int> actualStaminaStock;

int mailSpawned = 0;
int maxMail = 4;

int playerX, playerY;

char getBaseCell(int y, int x){
    if (y < 0 || y >= tinggi || x < 0 || x >= lebar) return '#';
    return (*currentMap)[y][x];
}

bool isWalkable(int y, int x){
    return getBaseCell(y, x) == '*';
}

char cell(int y, int x){
    if (y < 0 || y >= tinggi || x < 0 || x >= lebar) return '#';
    if (playerX == x && playerY == y) return 'P';
    if (!mailItem.taken && mailItem.mailX == x && mailItem.mailY == y) return '$';
    if (pencuri.active && pencuri.thiefX == x && pencuri.thiefY == y) return '!';
    return getBaseCell(y, x);
}

void saveGame(const string& filename = "save.dat"){
    ofstream out (filename, ios::binary);
    if (!out){
        cout << "Gagal menyimpan!\n";
        wait();
        return;
    }
    saveData s{};
    s.px = playerX;
    s.py = playerY;
    s.money = player.money;
    s.stam = player.stamPlayer;
    s.maxStam = player.maxStamPlayer;
    s.day = days.currentDay;
    s.hour = days.currentTimeHour;
    s.minute = days.currentTimeMinute;
    s.mailSpawned = mailSpawned;
    s.mapIndex = static_cast<int>(currentMap - &allMaps[0]);

    for (int i = 0; i < UPGRADE_COUNT && i < (int)shopUpgradeItems.size(); i++) s.upgradeLevels[i] = shopUpgradeItems[i].level;
    for (int i = 0; i < STAMINA_ITEM_COUNT && i < (int)actualStaminaStock.size(); i++) s.staminaStock[i] = actualStaminaStock[i];
    for (int i = 0; i < HOLO_GACHA_COUNT && i < (int)holoGachaPool.size(); i++) s.holoOwned[i] = holoGachaPool[i].owned;
    for (int i = 0; i < INDIE_GACHA_COUNT && i < (int)indieGachaPool.size(); i++) s.indieOwned[i] = indieGachaPool[i].owned;
    for (int i = 0; i < MINECRAFT_GACHA_COUNT && i < (int)minecraftGachaPool.size(); i++) s.minecraftOwned[i] = minecraftGachaPool[i].owned;
    for (int i = 0; i < MATH_GACHA_COUNT && i < (int)mathGachaPool.size(); i++) s.mathOwned[i] = mathGachaPool[i].owned;
    for (int i = 0; i < CARD_GACHA_COUNT && i < (int)cardGachaPool.size(); i++) s.cardOwned[i] = cardGachaPool[i].owned;

    out.write(reinterpret_cast<const char*>(&s), sizeof(s));
    out.close();

    cout << "\nPermainan berhasil disimpan!\n";
    wait();
}

bool loadGame(const string& filename = "save.dat"){
    ifstream in(filename, ios::binary);
    if (!in){
        cout << "File save tidak ditemukan atau rusak!\n";
        wait();
        return false;
        menu();
    }
    saveData s{};
    in.read(reinterpret_cast<char*>(&s), sizeof(s));
    if (s.version != SAVE_VERSION){
        cout << "Save file version tidak cocok!\n";
        wait();
        return false;
    }
    if (s.mapIndex < 0 || s.mapIndex >= (int)allMaps.size()){
        cout << "Map index invalid: " << s.mapIndex << "\n";
        wait();
        return false;
    }

    playerX = s.px;
    playerY = s.py;
    player.money = s.money;
    player.stamPlayer = s.stam;
    player.maxStamPlayer = s.maxStam;
    days.currentDay = s.day;
    days.currentTimeHour = s.hour;
    days.currentTimeMinute = s.minute;
    currentMap = &allMaps[s.mapIndex];
    mailSpawned = s.mailSpawned;

    if (shopUpgradeItems.size() != UPGRADE_COUNT) cout << "Error\n";
    if (actualStaminaStock.size() != STAMINA_ITEM_COUNT) cout << "Error\n";

    for (size_t i = 0; i < 7 && UPGRADE_COUNT < (int)shopStaminaItems.size(); i++) shopUpgradeItems[i].level = s.upgradeLevels[i];

    actualStaminaStock.clear();
    for (size_t i = 0; i < shopStaminaItems.size(); i++){
        int stock = (i < STAMINA_ITEM_COUNT) ? s.staminaStock[i] : rand() % shopStaminaItems[i].stockAmount + shopStaminaItems[i].stockEffectiveness;
        actualStaminaStock.push_back(stock);
    }

    for (int i = 0; i < HOLO_GACHA_COUNT && i < (int)holoGachaPool.size(); i++) holoGachaPool[i].owned = s.holoOwned[i];
    for (int i = 0; i < INDIE_GACHA_COUNT && i < (int)indieGachaPool.size(); i++) indieGachaPool[i].owned = s.indieOwned[i];
    for (int i = 0; i < MINECRAFT_GACHA_COUNT && i < (int)minecraftGachaPool.size(); i++) minecraftGachaPool[i].owned = s.minecraftOwned[i];
    for (int i = 0; i < MATH_GACHA_COUNT && i < (int)mathGachaPool.size(); i++) mathGachaPool[i].owned = s.mathOwned[i];
    for (int i = 0; i < CARD_GACHA_COUNT && i < (int)cardGachaPool.size(); i++) cardGachaPool[i].owned = s.cardOwned[i];

    mailItem.taken = true;
    pencuri.active = false;
    player.defend = false;

    cout << "\n=== DEBUG LOAD ===\n";
    cout << "Pos: " << playerX << "," << playerY << "\n";
    cout << "Map idx: " << s.mapIndex << "/" << allMaps.size() << "\n";
    cout << "Money: " << player.money << "\n";
    cout << "Stock count: " << actualStaminaStock.size() << "\n";
    cout << "Press Z to continue...\n";
    waitNewKey();

    cout << "\nPermainan berhasil dimuat!\n";
    wait();
    return true;
}

void startNewGame(){
    player = {};
    pencuri = {};
    mailItem = {};
    days = {};
    player.money = 8000;
    player.stamPlayer = 100;
    player.maxStamPlayer = 100;
    currentMap = &allMaps[rand() % allMaps.size()];
    playerPlacement();
    mailSpawned = 0;
    maxMail = 4;
    actualStaminaStock.clear();
    for (const auto& item : shopStaminaItems){
        int stock = rand() % item.stockAmount + item.stockEffectiveness;
        actualStaminaStock.push_back(stock);
    }
    for (auto& item : shopUpgradeItems) {
        item.level = 0;
    }
}

void printTime(){
    printf("Waktu: %02d:%02d\n", days.currentTimeHour, days.currentTimeMinute);
}

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
        bool num5 = GetAsyncKeyState('5') & 0x8000;
        bool num6 = GetAsyncKeyState('6') & 0x8000;
        bool num7 = GetAsyncKeyState('7') & 0x8000;
        bool num8 = GetAsyncKeyState('8') & 0x8000;
        bool num9 = GetAsyncKeyState('9') & 0x8000;
        

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
        else if (num5) key = '5';
        else if (num6) key = '6';
        else if (num7) key = '7';
        else if (num8) key = '8';
        else if (num9) key = '9';
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

void nextDay(){
    days.currentDay++;
    currentMap = &allMaps[rand() % allMaps.size()];
    playerPlacement();
    mailSpawned = 0;
    mailItem.taken = true;
    pencuri.active = false;
    waitLong();
}

void nextTime(int time){
    days.currentTimeMinute += time;
    if(days.currentTimeMinute >= 60){
        int extraHour = days.currentTimeMinute / 60;
        days.currentTimeHour+= extraHour;
        days.currentTimeMinute %= 60;
    }
    if(days.currentTimeHour >= 24){
        days.currentTimeHour = 0;
        nextDay();
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
        if (mailSpawned == maxMail) player.money -= randomLoss;
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

void checkPickup(){
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
    printMaps();
    cout << "\n===== Hari ke-" << days.currentDay << " =====\n";
    cout << "Bergerak (WASD) Tidak dapat di hold. ";
    cout << "Tekan (Q) untuk keluar dari peta.";
    if (mailSpawned == 0) {
        mailItem.taken = true;
        trySpawnMail();
    }

    cout << "\nUang = " << player.money << " rupiah\n";

    while (move){
        int key = waitNewKey();
        
        int newX = playerX;
        int newY = playerY;

        if (key == 'w') newY--;
        else if (key == 'a') newX--;
        else if (key == 's') newY++;
        else if (key == 'd') newX++;
        else if (key == 'q') {
            move = false;
            lastMenuSource = menuSource::MAP;
        }

        if (newX < 0 || newX >= lebar || newY < 0 || newY >= tinggi) {
            continue;
        }

        if (getBaseCell(newY, newX) != '*'){
            continue;
        }

        playerX = newX;
        playerY = newY;
        player.stamPlayer--;
        moveThiefTowardPlayer();
        nextTime(1);
        
        checkPickup();
        interactThief();
        flushInput();

        printMaps();
        cout << "\n===== Hari ke- " << days.currentDay << " =====\n";
        printTime();
        cout << "Bergerak (WASD) Tidak dapat di hold. ";
        cout << "Tekan (Q) untuk keluar dari peta.";
        cout << "\nUang = " << player.money << " rupiah\n";
        cout << "Stamina = " << player.stamPlayer << "/" << player.maxStamPlayer << endl;
        cout << "Lokasi paket = " << mailSpawned << "/" << maxMail << endl;

        if (player.stamPlayer <= 0){
            printMaps();
            cout << "\n===== Hari ke-" << days.currentDay << " =====\n";
            cout << "Kamu kelelahan dan tidak bisa melanjutkan perjalananmu.\n";
            waitLong();
            outside();
        }
    }
    outside();
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
    mailItem.effectiveness += ((mailItem.effectiveness * 15) / 100);
}

void upgradeThiefSpawnChance(int){
    pencuri.chance += 10;
}

void upgradeThiefAmount(int){
    pencuri.fullAmount += 20;
}

void upgradeThiefEffectiveness(int){
    pencuri.effectiveness += ((pencuri.effectiveness * 10) / 100);
}

void choiceShopStaminaItem(int index){
    int stock = actualStaminaStock[index];
    if (index < 0 || index >= (int)actualStaminaStock.size()) {
    cout << "Item tidak tersedia.\n";
    wait();
    return;
    }
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

bool upgradeShop = false;

void shop(){
    bool move = false;
    bool openshop = true;
    bool inUpgradedShop = false;
    while (!move && openshop){
        system("cls||clear");
        cout << "====== TOKO ======\n";
        cout << "Selamat datang di toko!\n";
        for (size_t i = 0; i < 3; i++){
            int stock = actualStaminaStock[i];
            cout << i + 1 << ". " << shopStaminaItems[i].name << " - " << shopStaminaItems[i].price << " rupiah" << " (Stok: " << stock << ")\n";
        }
        if (!upgradeShop) cout << "4. Upgrade Toko - 1000 rupiah";
        else if (upgradeShop) {
            cout << "4. Upgrade Toko (Sudah di-upgrade) - Pilih untuk masuk ke dalam toko yang sudah di upgrade";
        }
        cout << "\n5. Menu\n0. Keluar dari toko\n";
        cout << "\nUang mu = " << player.money << " rupiah\n";
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
                        cout << "\n0. Kembali ke toko utama\n\n";
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
        else if (choice == '5'){
            lastMenuSource = menuSource::SHOP;
            menu();
        }
        else if (choice == '0'){
            cout << "Keluar dari toko...\n";
            wait();
            move = true;
            openshop = false;
            outside();
            nextTime(20);
        }
        else {
            cout << "Pilihan tidak valid. Silakan coba lagi.\n";
            wait();
        
        }
    system("cls||clear");
    }
}

void showItemDetail(const Item& item){
    system ("cls||clear");
    cout << "====== DETAIL ======\n\n";
    cout << "Name : " << item.name << endl;
    cout << "Code : " << item.code << endl;
    cout << "Desc : " << item.description << endl;
    cout << "\n[Z]Back [Q]Quit\n";
}

void displayItemC(vector<Item>& collection, int currentPage, int perPage){
    int startItem = (currentPage - 1) * perPage;
    int endItem = startItem + perPage;
    if (endItem > collection.size()) endItem = collection.size();
    int totalPages = (collection.size() + perPage - 1) / perPage;

    system("cls||clear");
    cout << "Page " << currentPage << " of " << totalPages << endl;
    for (int i = startItem; i < endItem; i++){
        if (collection[i].owned){
            cout << i - startItem + 1 << ". " << collection[i].name << endl;
        }
        else {
            cout << i - startItem + 1 << ". " << "*****************************\n";
        }
    }
    cout << "[Z]Prev [X]Next [Q]Exit [1 - 5]See Detail \n";
    flushInput();
    int choice = waitNewKey();

    if (choice >= '1' && choice <= '5'){
        int index = (currentPage - 1) * perPage + (choice - '1');
        if (index < (int)collection.size()){
            if (collection[index].owned){
                showItemDetail(collection[index]);
                int pilihan = waitNewKey();
                switch(pilihan){
                    case 'z':
                    case 'Z':
                        break;
                    case 'q':
                    case 'Q':
                        return;
                }
            }
        }
    }
    else{
        switch(choice){
            case 'x':
                if (currentPage < totalPages) currentPage++;
                displayItemC(collection, currentPage, perPage);
                break;
            case 'z':
                if (currentPage > 1) currentPage--;
                displayItemC(collection, currentPage, perPage);
                break;
            case 'q':
                return;
            default:
                cout << "Pilihan invalid. Coba ulang kembali.\n";
                wait();
                break;
        }
    }
}

void HoloC(){
    int currentPage = 1;
    bool inHolo = true;
    while (inHolo){
        displayItemC(holoGachaPool, currentPage, 5);
        flushInput();
        int nextChoice = waitNewKey();
        if (nextChoice == 'q'){
            inHolo = false;
            break;
        }
    }
}

void IndieC(){
    int currentPage = 1;
    bool inIndie = true;
    while (inIndie){
        displayItemC(indieGachaPool, currentPage, 5);
        flushInput();
        int nextChoice = waitNewKey();
        if (nextChoice == 'q'){
            inIndie = false;
            break;
        }
    }
}

void MinecraftC(){
    int currentPage = 1;
    bool inMine = true;
    while (inMine){
        displayItemC(minecraftGachaPool, currentPage, 5);
        flushInput();
        int nextChoice = waitNewKey();
        if (nextChoice == 'q'){
            inMine = false;
            break;
        }
    }
}

void MathC(){
    int currentPage = 1;
    bool inMath = true;
    while (inMath){
        displayItemC(mathGachaPool, currentPage, 5);
        flushInput();
        int nextChoice = waitNewKey();
        if (nextChoice == 'q'){
            inMath = false;
            break;
        }
    }
}

void CardC(){
    int currentPage = 1;
    bool inCard = true;
    while (inCard){
        displayItemC(cardGachaPool, currentPage, 5);
        flushInput();
        int nextChoice = waitNewKey();
        if (nextChoice == 'q'){
            inCard = false;
            break;
        }
    }
}

void collectionPage(){
    bool inCollection = true;
    while (inCollection){
        system("cls||clear");
        cout << "======= RUANG KOLEKSI =======\n";
        cout << "1. Holo Collection\n2. Indie Collection\n3. Minecraft Collection\n4. Math Collection\n5. Card Collection\n0. Keluar\n";
        cout << "Pilihanmu = ";
        int choice = waitNewKey();
        switch (choice){
            case '1':
                HoloC();
                break;
            case '2':
                IndieC();
                break;
            case '3':
                MinecraftC();
                break;
            case '4':
                MathC();
                break;
            case '5':
                CardC();
                break;
            case '0':
                inCollection = false;
                house();
                break;
            default:
                cout << "Pilihan tidak valid. Coba ulangi lagi\n";
                break;
        }
    }
}

void start(){
    system("cls||clear");
    cout << "Selamat datang di permainan pengantar paket!\n";
    cout << "Mengumpulkan uang sebanyak banyaknya untuk khilaf dalam gacha game!\n";
    cout << "1. Mulai Permainan\n2. Load\n0. Keluar\n";
    cout << "Pilihanmu = ";
    flushInput();
    int choice = waitNewKey();
    switch (choice) {
        case '1':
            startNewGame();
            house();
            break;
        case '2':
            if (loadGame()){
                switch (lastMenuSource){
                    case menuSource::HOUSE:
                        house();
                        break;
                    case menuSource::OUTSIDE:
                        outside();
                        break;
                    case menuSource::MAP:
                        map();
                        break;
                    default:
                        house();
                        break;
                }
            }
            break;
        case '0':
            cout << "Terima kasih sudah bermain!\n";
            waitLong();
            exit(0);
            break;
        default:
            cout << "Pilihan tidak valid. Silakan coba lagi.\n";
            wait();
            start();
            break;
    }
}

void outside(){
    system("cls||clear");
    cout << "====== DI LUAR ======\n";
    cout << "===== Hari ke-" << days.currentDay << " =====\n";
    printTime();
    cout << "Kamu sedang di luar. Apa yang kamu ingin lakukan?";
    cout << "\n1. Toko\n2. Bekerja\n3. Rumah\n4. Toko Gacha\n5. Menu\n";
    cout << "Pilihanmu = ";
    flushInput();
    int choice = waitNewKey();
    switch (choice) {
        case '1':
            shop();
            nextTime(20);
            break;
        case '2':
            map();
            nextTime(5);
            break;
        case '3':
            house();
            nextTime(5);
            break;
        case '4':
            gachaShop();
            nextTime(5);
            break;
        case '5':
            lastMenuSource = menuSource::OUTSIDE;
            menu();
            break;
        default:
            cout << "Pilihan tidak valid. Silakan coba lagi.\n";
            wait();
            outside();
            break;
    }
}

void house(){
    bool inhouse = true;
    while (inhouse) {
        system("cls||clear");
        cout << "====== RUMAH ======\n";
        cout << "===== Hari ke-" << days.currentDay << " =====\n";
        printTime();
        cout << "Kamu berada di rumahmu. Apa yang ingin kamu lakukan?\n";
        cout << "1. Tidur\n2. Istirahat\n3. Mandi\n4. Collection\n5. Menu\n0. Keluar rumah\n";
        cout << "Pilihanmu = ";
        flushInput();
        int choice = waitNewKey();
        if (choice == '1'){
            player.stamPlayer = player.maxStamPlayer;
            cout << "Kamu tidur nyenyak dan stamina mu terisi penuh!\n";
            wait();
            nextDay();
            days.currentTimeHour = 7;
            days.currentTimeMinute = 0;
        }
        else if (choice == '2'){
            int restRecover = player.maxStamPlayer / 4;
            player.stamPlayer += restRecover;
            if (player.stamPlayer > player.maxStamPlayer){
                player.stamPlayer = player.maxStamPlayer;
            }
            nextTime(30);
            cout << "Kamu beristirahat sejenak dan stamina mu bertambah " << restRecover << " poin!\n";
            wait();
        }
        else if (choice == '3'){
            int bathRecover = player.maxStamPlayer / 10;
            player.stamPlayer += bathRecover;
            if (player.stamPlayer > player.maxStamPlayer){
                player.stamPlayer = player.maxStamPlayer;
            }
            nextTime(45);
            cout << "Kamu mandi dan merasa segar kembali! Stamina mu bertambah " << bathRecover << " poin!\n";
            wait();
        }
        else if (choice == '4'){
            collectionPage();
            inhouse = false;
        }
        else if (choice == '5'){
            lastMenuSource = menuSource::HOUSE;
            menu();
        }
        else if (choice == '0'){
            cout << "Kamu keluar dari rumahmu.\n";
            wait();
            inhouse = false;
            outside();
            nextTime(5);
        }
        else {
            cout << "Pilihan tidak valid. Silakan coba lagi.\n";
            wait();
        }
    }
}

void menu(){
    system("cls||clear");
    cout << "====== MENU ======\n";
    cout << "Penglihatanmu tiba tiba gelap dan tanpa sepengetahuanmu kamu berada di ruangan hitam!\n";
    cout << "Kamu sedang ada di menu. Apa yang ingin lakukan?\n";
    cout << "1. Kembali\n2. Save\n3. Load\n0. Kembali ke tampilan utama.\n";
    cout << "Tempat sebelum menu: ";
    switch (lastMenuSource) {
        case menuSource::HOUSE:
            cout << "House" << endl;
            break;
        case menuSource::OUTSIDE:
            cout << "Outside" << endl;
            break;
        case menuSource::SHOP:
            cout << "Shop" << endl;
            break;
        default:
            cout << "Unknown" << endl;
            break;
    }
    int choice = waitNewKey();
    switch (choice){
        case '1': 
            switch (lastMenuSource){
                case menuSource::HOUSE:
                    house();
                    break;
                case menuSource::OUTSIDE:
                    outside();
                    break;
                case menuSource::SHOP:
                    shop();
                    break;
                default:
                    start();
                    break;
            }
        case '2':
            saveGame();
            menu();
            break;
        case '3':
            if (loadGame()){
                switch (lastMenuSource){
                    case menuSource::HOUSE:
                        house();
                        break;
                    case menuSource::OUTSIDE:
                        outside();
                        break;
                    case menuSource::SHOP:
                        shop();
                        break;
                    default:
                        house();
                        break;
                }
            }
            break;
        case '0':
            start();
            break;
        default:
            cout << "Pilihan tidak valid. Silahkan coba lagi.\n";
            break;            
    }
}

void gachaBoxPurchase(int choice, int boxIndex, vector<Item>& gacha){
    int cost = gachaBox[boxIndex].cost;
    
    if (player.money < cost){
        notEnoughMoney(player.money, cost);
        return;
    }
    
    player.money -= cost;

    vector<int> weights(gacha.size(), 100);
    random_device rd;
    mt19937 gen(rd());
    discrete_distribution<int> dist(weights.begin(), weights.end());
    int index = dist(gen);
    
    gacha[index].owned = true;
    
    cout << "Anda mendapatkan: " << gacha[index].name << "!" << endl;
    flushInput();
    cout << "\nPress Z to continue...\n";
    waitNewKey();
}

void pengulanganGacha(int choice, vector<Item>& gachas){
    int banyak;
    bool moneyEnough = true;
    while (moneyEnough){
        cout << "\nIngin membuka berapa box? (1-9)\n";
        flushInput();
        banyak = waitNewKey();
        flushInput();

        if (banyak < '1' || banyak > '9'){
            cout << "Jumlah harus di antara 1 dan 9. Coba ulangi lagi.\n";
            wait();
            continue;
        }
        
        int totalCost = (banyak - '0') * gachaBox[choice - '1'].cost;
        if (player.money < totalCost){
            cout << "Kamu tidak punya uang yang cukup untuk pembelian ini.\n";
            wait();
            moneyEnough = false;
        } 
        else{
            cout << "Membeli sebanyak " << (banyak - '0') << " box\n\n";
            for (int i = '1'; i <= banyak; i++){
                gachaBoxPurchase(choice, choice - '1', gachas);
            }
            return;
        }
    }
}

void gachaShop(){
    bool inGacha = true;
    while (inGacha){
        system("cls||clear");
        cout << "====== TOKO GACHA ======\n";
        cout << "Selamat datang di toko Gacha, disini kamu bisa berkesempatan untuk mendapatkan item dari kategori kesukaanmu :]\n";
        for (size_t i = 0; i < gachaBox.size(); i++){
            cout << i + 1 << ". " << gachaBox[i].name << endl;
        }
        cout << "0. Keluar\n";
        cout << "Pilihanmu = ";
        flushInput();
        int choice = waitNewKey();
        switch (choice){
            case '1':
                pengulanganGacha(choice, holoGachaPool);
                break;
            case '2':
                pengulanganGacha(choice, indieGachaPool);
                break;
            case '3':
                pengulanganGacha(choice, minecraftGachaPool);
                break;
            case '4':
                pengulanganGacha(choice, mathGachaPool);
                break;
            case '5':
                pengulanganGacha(choice, cardGachaPool);
                break;
            case '0':
                inGacha = false;
                outside();
                break;
        }
    }
}

int main(){
    srand(time(0));
    start();
}