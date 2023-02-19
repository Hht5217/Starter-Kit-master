// *********************************************************
// Course: TCP1101 PROGRAMMING FUNDAMENTALS
// Year: Trimester 1, 2022/23 (T2215)
// Lab: TT8L
// Names: TAN HONG HAN | ANADANI MUAAZ
// IDs: 1211105217 | 1221300723
// Emails: 1211105217@student.mmu.edu.my | 1221300723@student.mmu.edu.my
// Phones: +60166659416 | +60182040789
// *********************************************************

#include "pf/helper.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <cctype>
#include <cmath>
using namespace std;
using namespace pf;

// This class is defined first as its objects are being passed into many functions
class Zombie
{
public:
    Zombie(int life, int attack, int range);
    void ZombieStats(const int &i, const vector<Zombie> &Zombies);
    int getlifeZombie() const;
    int getattackZombie() const;
    int getrangeZombie() const;
    void updatelifeZombie(int value_attack);
    void loadupdateZombie(int load_life, int load_attack, int load_range);

private:
    int life_;
    int attack_;
    int range_;
};

// Declare functions here
void MenuDisplay();
void BoardSettings();
void PlayConfirmation();
void turnAlien(vector<Zombie> &Zombies);
void turnObject(vector<Zombie> &Zombies);
int setLife(char AorZ);
int setAttack();
int setRange();

class Board
{
private:
    vector<vector<char>> map_;
    int BoardX_, BoardY_;
    int initailizeAlienLife_;
    int lifeAlien_;
    int attackAlien_;

public:
    Board(int BoardX = 7, int BoardY = 7);
    void initializeboard(int BoardX, int BoardY);
    void ShowBoard(vector<Zombie> &Zombies) const;
    void initializeObjects(int &initialNumberOfZombies);
    void boardUpdate(int BoardY, int BoardX);
    int getBoardX() const;
    int getBoardY() const;
    int getlifeAlien() const;
    int getattackAlien() const;
    char getObject(int BoardY, int BoardX) const;
    vector<int> posAlien();
    vector<int> posZombie(char zombie_digit);
    void moveObject(int posY, int posX, char move_arrow, char AorZ, char zombie_digit);
    void resetTrail();
    void hitRock(const int posY_A, const int posX_A, const char location_rock);
    void updatelifeAlien(int value_life, char health_damage);
    void updateattackAlien(char add_reset);
    void changeArrow(int arrowY, int arrowX, char arrowChange);
    void loadupdateAlien(int load_life, int load_attack);
    void loadupdateBoard(const vector<vector<char>> &load_board);
};
// ####################### some functions ######################
void Lines()
{
    cout << string(50, '#') << endl;
}

void CleanInput()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ####################### Board class functions #######################
Board::Board(int BoardX, int BoardY)
{
    initializeboard(BoardX, BoardY);
}

void Board::initializeboard(int BoardX, int BoardY)
{
    char objects[] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'h', 'p', 'r', '^', 'v', '<', '>'};
    int numberOfObjects = 14;
    BoardX_ = BoardX;
    BoardY_ = BoardY;
    map_.resize(BoardY_);
    for (int i = 0; i < BoardY_; ++i)
    {
        map_[i].resize(BoardX_);
    }
    for (int i = 0; i < BoardY_; ++i)
    {
        for (int j = 0; j < BoardX_; ++j)
        {
            int objectNumber = rand() % numberOfObjects;
            map_[i][j] = objects[objectNumber];
        }
    }
}

void Board::boardUpdate(int NewY, int NewX)
{
    BoardY_ = NewY;
    BoardX_ = NewX;
}

int Board::getBoardX() const
{
    return BoardX_;
}

int Board::getBoardY() const
{
    return BoardY_;
}

int Board::getlifeAlien() const
{
    return lifeAlien_;
}

int Board::getattackAlien() const
{
    return attackAlien_;
}

void Board::ShowBoard(vector<Zombie> &Zombies) const
{
    for (int i = 0; i < BoardY_; ++i)
    {
        cout << "  ";
        for (int j = 0; j < BoardX_; ++j)
        {
            cout << "+-";
        }
        cout << "+" << endl;
        cout << setw(2) << (BoardY_ - i);
        for (int j = 0; j < BoardX_; ++j)
        {
            cout << "|" << map_[i][j];
        }
        cout << "|" << endl;
    }
    cout << "  ";
    for (int j = 0; j < BoardX_; ++j)
    {
        cout << "+-";
    }
    cout << "+" << endl;
    cout << "  ";
    for (int j = 0; j < BoardX_; ++j)
    {
        int digit = (j + 1) / 10;
        cout << " ";
        if (digit == 0)
            cout << " ";
        else
            cout << digit;
    }
    cout << endl;
    cout << "  ";
    for (int j = 0; j < BoardX_; ++j)
    {
        cout << " " << (j + 1) % 10;
    }

    cout << endl
         << endl;
    cout << "Alien";
    cout.width(15);
    cout << "=>  Life " << lifeAlien_ << " | ";
    cout << "Attack ";
    cout.width(2);
    cout << attackAlien_;
    cout << endl;
    for (int i = 0; i < size(Zombies); ++i)
    {
        Zombies[i].ZombieStats(i, Zombies);
    }
    cout << endl;
}

void Board::initializeObjects(int &initialNumberOfZombies)
{
    map_[BoardY_ / 2][BoardX_ / 2] = 'A';
    lifeAlien_ = initailizeAlienLife_ = setLife('a');
    attackAlien_ = 0;

    for (int z = 0; z < initialNumberOfZombies; ++z)
    {
        while (true)
        {
            int randomY = rand() % BoardY_;
            int randomX = rand() % BoardX_;
            if (map_[randomY][randomX] == ' ')
            {
                // adding 49 to become digits in terms of ascii
                map_[randomY][randomX] = z + 49;
                break;
            }
        }
    }
}

char Board::getObject(int BoardY, int BoardX) const
{
    return map_[BoardY][BoardX];
}

vector<int> Board::posAlien()
{
    vector<int> posA{-1, -1};
    for (int i = 0; i < BoardY_; ++i)
    {
        for (int j = 0; j < BoardX_; ++j)
        {
            if (map_[i][j] == 'A')
            {
                posA[0] = j;
                posA[1] = i;
                return posA;
            }
        }
    }
    return posA;
}

vector<int> Board::posZombie(char zombie_digit)
{
    vector<int> posZ{-1, -1};
    for (int i = 0; i < BoardY_; ++i)
    {
        for (int j = 0; j < BoardX_; ++j)
        {
            if (map_[i][j] == zombie_digit)
            {
                posZ[0] = j;
                posZ[1] = i;
                return posZ;
            }
        }
    }
    return posZ;
}

void Board::moveObject(int posY, int posX, char move_arrow, char AorZ, char zombie_digit = 'z')
{
    if (AorZ == 'A')
    {
        switch (move_arrow)
        {
        case '^':
            map_[posY - 1][posX] = 'A';
            map_[posY][posX] = '.';
            break;
        case 'v':
            map_[posY + 1][posX] = 'A';
            map_[posY][posX] = '.';
            break;
        case '<':
            map_[posY][posX - 1] = 'A';
            map_[posY][posX] = '.';
            break;
        case '>':
            map_[posY][posX + 1] = 'A';
            map_[posY][posX] = '.';
            break;
        }
    }
    else if (AorZ == 'Z')
    {
        switch (move_arrow)
        {
        case '^':
            map_[posY - 1][posX] = zombie_digit;
            map_[posY][posX] = ' ';
            break;
        case 'v':
            map_[posY + 1][posX] = zombie_digit;
            map_[posY][posX] = ' ';
            break;
        case '<':
            map_[posY][posX - 1] = zombie_digit;
            map_[posY][posX] = ' ';
            break;
        case '>':
            map_[posY][posX + 1] = zombie_digit;
            map_[posY][posX] = ' ';
            break;
        }
    }
}

void Board::resetTrail()
{
    char objects[] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'h', 'p', 'r', '^', 'v', '<', '>'};
    int numberOfObjects = 15;
    for (int i = 0; i < BoardY_; ++i)
    {
        for (int j = 0; j < BoardX_; ++j)
        {
            if (map_[i][j] == '.')
            {
                int objectNumber = rand() % numberOfObjects;
                map_[i][j] = objects[objectNumber];
            }
        }
    }
}

void Board::hitRock(const int posY_A, const int posX_A, const char location_rock)
{
    const int noofhidden = 2;
    char hiddenObject[noofhidden] = {'p', 'h'};
    int revealObject = rand() % noofhidden;

    int posY_Rock;
    int posX_Rock;

    switch (location_rock)
    {
    case '^':
        posY_Rock = posY_A - 1;
        posX_Rock = posX_A;
        break;
    case 'v':
        posY_Rock = posY_A + 1;
        posX_Rock = posX_A;
        break;
    case '<':
        posX_Rock = posX_A - 1;
        posY_Rock = posY_A;
        break;
    case '>':
        posX_Rock = posX_A + 1;
        posY_Rock = posY_A;
        break;
    }

    if (map_[posY_Rock][posX_Rock] == 'r')
    {
        map_[posY_Rock][posX_Rock] = hiddenObject[revealObject];
    }
    else
    {
    }
}

void Board::updatelifeAlien(int value_life, char health_damage)
{
    if (health_damage == 'h')
    {
        if (lifeAlien_ == initailizeAlienLife_)
            lifeAlien_ += 0;
        else
            lifeAlien_ += value_life;
    }
    else if (health_damage == 'd')
    {
        if (value_life >= lifeAlien_)
            lifeAlien_ = 0;
        else
            lifeAlien_ -= value_life;
    }
}

void Board::updateattackAlien(char add_reset)
{
    if (add_reset == 'a')
        attackAlien_ += 20;
    else if (add_reset == 'r')
        attackAlien_ = 0;
}

void Board::changeArrow(int arrowY, int arrowX, char arrowChange)
{
    map_[arrowY][arrowX] = arrowChange;
}

void Board::loadupdateAlien(int load_life, int load_attack)
{
    lifeAlien_ = load_life;
    attackAlien_ = load_attack;
}

void Board::loadupdateBoard(const vector<vector<char>> &load_board)
{
    int loadboardY = size(load_board);
    int loadboardX = size(load_board[0]);
    BoardY_ = loadboardY;
    BoardX_ = loadboardX;
    map_.resize(loadboardY);
    for (int r = 0; r < loadboardY; ++r)
    {
        map_[r].resize(loadboardX);
    }
    for (int ly = 0; ly < loadboardY; ++ly)
    {
        for (int lx = 0; lx < loadboardX; ++lx)
        {
            map_[ly][lx] = load_board[ly][lx];
        }
    }
}

// ################# Zombie class functions #####################
Zombie::Zombie(int life, int attack, int range)
{
    life_ = life;
    attack_ = attack;
    range_ = range;
}

void Zombie::ZombieStats(const int &i, const vector<Zombie> &Zombies)
{
    cout << "Zombie " << i + 1;
    cout.width(12);
    cout << "=>  Life " << Zombies[i].life_ << " | ";
    cout << "Attack ";
    cout.width(2);
    cout << Zombies[i].attack_ << " | ";
    cout << "Range " << Zombies[i].range_ << endl;
}

int Zombie::getlifeZombie() const
{
    return life_;
}

int Zombie::getattackZombie() const
{
    return attack_;
}

int Zombie::getrangeZombie() const
{
    return range_;
}

void Zombie::updatelifeZombie(int value_attack)
{
    if (life_ <= value_attack)
        life_ = 0;
    else
        life_ -= value_attack;
}

void Zombie::loadupdateZombie(int load_life, int load_attack, int load_range)
{
    life_ = load_life;
    attack_ = load_attack;
    range_ = load_range;
}

// ######################### other functions ###########################

Board game;
int initialNumberOfZombies = 2;
int setLife(char AorZ)
{
    // upper and lower limits for setting life value
    int lower_life, upper_life;
    // used switch in case of error, so life will still be set with default limits
    switch (AorZ)
    {
    case 'a':
        lower_life = 150;
        upper_life = 200;
        break;
    case 'z':
        lower_life = 100;
        upper_life = 150;
        break;

    default:
        lower_life = 130;
        upper_life = 160;
        break;
    }
    int life;
    while (true)
    {
        life = (rand() % (upper_life - lower_life + 1)) + lower_life;
        if (life % 10 == 0)
        {
            break;
        }
    }
    return life;
}

int setAttack()
{
    int lower_attack = 5, upper_attack = 15;
    int attack;
    while (true)
    {
        attack = (rand() % (upper_attack - lower_attack + 1)) + lower_attack;
        if (attack % 5 == 0)
        {
            break;
        }
    }
    return attack;
}

int setRange()
{
    int lower_range = 1, upper_range = 4;
    int range = (rand() % (upper_range - lower_range + 1)) + lower_range;
    if (game.getBoardX() > 9 && game.getBoardY() > 9)
    {
        range += 3;
    }
    return range;
}

void MenuDisplay()
{
    ClearScreen();
    Lines();
    cout << "Welcome to Alien vs Zombie (Text Ver.)!" << endl;
    Lines();
    cout << "1. Play"
         << "\n"
         << "2. Board Settings"
         << "\n"
         << "3. Exit" << endl;
    string menu;
    while (true)
    {
        cout << "Please input a command(number): ";
        cin >> menu;
        if (menu == "1")
        {
            CleanInput();
            ClearScreen();
            PlayConfirmation();
            break;
        }
        else if (menu == "2")
        {
            CleanInput();
            ClearScreen();
            BoardSettings();
            break;
        }
        else if (menu == "3")
        {
            CleanInput();
            cout << "Good Bye!" << endl;
            break;
        }
        else
        {
            cout << "Command not found, please try again." << endl;
        }
    }
}

void PlayConfirmation()
{
    Lines();
    cout << "Play a game" << endl;
    Lines();
    cout << "1. Start the game"
         << "\n"
         << "2. View or change board settings"
         << "\n"
         << "3. Back" << endl;
    string playmenu;
    while (true)
    {
        cout << "Please input a command(number): ";
        cin >> playmenu;
        if (playmenu == "1")
        {
            CleanInput();
            ClearScreen();
            game.initializeboard(game.getBoardX(), game.getBoardY());
            game.initializeObjects(initialNumberOfZombies);
            vector<Zombie> Zombies;
            for (int i = 0; i < initialNumberOfZombies; ++i)
            {
                int life = setLife('z');
                int attack = setAttack();
                int range = setRange();
                Zombie npc(life, attack, range);
                Zombies.push_back(npc);
            }
            turnAlien(Zombies);
            break;
        }
        else if (playmenu == "2")
        {
            CleanInput();
            ClearScreen();
            BoardSettings();
            break;
        }
        else if (playmenu == "3")
        {
            CleanInput();
            MenuDisplay();
            break;
        }
        else
        {
            cout << "Command not found, please try again." << endl;
        }
    }
}

void turnAlien(vector<Zombie> &Zombies)
{
    int checkaliveAlien = game.getlifeAlien();
    if (checkaliveAlien == 0)
    {
        ClearScreen();
        game.ShowBoard(Zombies);
        cout << "Alien has been defeated, game over." << endl;
        cout << "Returning to menu." << endl;
        Pause();
        MenuDisplay();
    }
    else
    {
    nonturn:
        string main_command;
        char move_arrow;
        while (true)
        {
            game.ShowBoard(Zombies);
            cout << "Please input a command(word): " << endl;
            cout << "1. up"
                 << "\n"
                 << "2. down"
                 << "\n"
                 << "3. left"
                 << "\n"
                 << "4. right"
                 << "\n"
                 << "5. arrow"
                 << "\n"
                 << "6. help"
                 << "\n"
                 << "7. save"
                 << "\n"
                 << "8. load"
                 << "\n"
                 << "9. quit (to menu)"
                 << endl;

            getline(cin, main_command);
            size_t pos = main_command.find(' ');
            if (pos != string::npos)
            {
                main_command = main_command.substr(0, pos);
            }
            for (int l = 0; l < size(main_command); ++l)
            {
                main_command[l] = tolower(main_command[l]);
            }

        turn:
            const vector<int> &currentPos = game.posAlien();
            int posX = currentPos[0];
            int posY = currentPos[1];
            char objectCheck;
            int attack_Alien = game.getattackAlien();
            int numofZombies = size(Zombies);
            int pod_attack;

            bool saveongoinggame = true;

            // alien move up
            if (main_command == "up")
            {
                cin.clear();
                move_arrow = '^';
                if (posY == 0)
                {
                    ClearScreen();
                    game.resetTrail();
                    game.ShowBoard(Zombies);
                    cout << "Alien has reached border." << endl;
                    Pause();
                    game.updateattackAlien('r');
                    turnObject(Zombies);
                    break;
                }
                else
                {
                    objectCheck = game.getObject((posY - 1), (posX));
                    if (isdigit(objectCheck))
                    {
                        int zombieIndex = objectCheck - 49;
                        if (Zombies[zombieIndex].getlifeZombie() <= attack_Alien)
                        {
                            game.moveObject(posY, posX, move_arrow, 'A');
                            ClearScreen();
                            game.ShowBoard(Zombies);
                            cout << "Zombie " << zombieIndex + 1 << " defeated." << endl;
                            Pause();
                            goto turn;
                        }
                        else
                        {
                            ClearScreen();
                            game.resetTrail();
                            Zombies[zombieIndex].updatelifeZombie(attack_Alien);
                            game.ShowBoard(Zombies);
                            cout << "Alien hits zombie with attack of " << attack_Alien << endl;
                            Pause();
                            game.updateattackAlien('r');
                            turnObject(Zombies);
                            break;
                        }
                    }
                    else if (objectCheck == 'r')
                    {
                        ClearScreen();
                        game.hitRock(posY, posX, move_arrow);
                        game.resetTrail();
                        game.ShowBoard(Zombies);
                        cout << "Alien hits rock, hidden object revealed." << endl;
                        Pause();
                        game.updateattackAlien('r');
                        turnObject(Zombies);
                        break;
                    }
                    else if (objectCheck == 'h')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        game.updatelifeAlien(10, 'h');
                        game.ShowBoard(Zombies);
                        cout << "Alien found health pack, +10 life." << endl;
                        Pause();
                        goto turn;
                    }
                    else if (objectCheck == 'p')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        while (true)
                        {
                            pod_attack = rand() % numofZombies;
                            if (Zombies[pod_attack].getlifeZombie() != 0)
                            {
                                Zombies[pod_attack].updatelifeZombie(10);
                                break;
                            }
                        }
                        game.ShowBoard(Zombies);
                        cout << "Alien found pod, zombie " << pod_attack + 1 << " is attacked, -10 life." << endl;
                        Pause();
                        goto turn;
                    }
                    else if (objectCheck == '^')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        game.updateattackAlien('a');
                        game.ShowBoard(Zombies);
                        cout << "Alien found arrow object, attack +20." << endl;
                        Pause();
                        goto turn;
                    }
                    else if (objectCheck == 'v')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        game.updateattackAlien('a');
                        game.ShowBoard(Zombies);
                        main_command = "down";
                        cout << "Alien found arrow object, attack +20, changed to moving downwards." << endl;
                        Pause();
                        goto turn;
                    }
                    else if (objectCheck == '<')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        game.updateattackAlien('a');
                        game.ShowBoard(Zombies);
                        main_command = "left";
                        cout << "Alien found arrow object, attack +20, changed to moving left" << endl;
                        Pause();
                        goto turn;
                    }
                    else if (objectCheck == '>')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        game.updateattackAlien('a');
                        game.ShowBoard(Zombies);
                        main_command = "right";
                        cout << "Alien found arrow object, attack +20, changed to moving right" << endl;
                        Pause();
                        goto turn;
                    }
                    else
                    {
                        game.moveObject(posY, posX, move_arrow, 'A');
                        ClearScreen();
                        game.ShowBoard(Zombies);
                        Pause();
                        goto turn;
                    }
                }
            }

            // alien move down
            else if (main_command == "down")
            {
                cin.clear();
                move_arrow = 'v';
                if (posY == (game.getBoardY() - 1))
                {
                    ClearScreen();
                    game.resetTrail();
                    game.ShowBoard(Zombies);
                    cout << "Alien has reached border." << endl;
                    Pause();
                    game.updateattackAlien('r');
                    turnObject(Zombies);
                    break;
                }
                else
                {
                    objectCheck = game.getObject((posY + 1), (posX));
                    if (isdigit(objectCheck))
                    {
                        int zombieIndex = objectCheck - 49;
                        if (Zombies[zombieIndex].getlifeZombie() <= attack_Alien)
                        {
                            game.moveObject(posY, posX, move_arrow, 'A');
                            ClearScreen();
                            game.ShowBoard(Zombies);
                            cout << "Zombie " << zombieIndex + 1 << " defeated." << endl;
                            Pause();
                            goto turn;
                        }
                        else
                        {
                            ClearScreen();
                            game.resetTrail();
                            Zombies[zombieIndex].updatelifeZombie(attack_Alien);
                            game.ShowBoard(Zombies);
                            cout << "Alien hits zombie with attack of " << attack_Alien << endl;
                            Pause();
                            game.updateattackAlien('r');
                            turnObject(Zombies);
                            break;
                        }
                    }
                    else if (objectCheck == 'r')
                    {
                        ClearScreen();
                        game.hitRock(posY, posX, move_arrow);
                        game.resetTrail();
                        game.ShowBoard(Zombies);
                        cout << "Alien hits rock, hidden object revealed." << endl;
                        Pause();
                        game.updateattackAlien('r');
                        turnObject(Zombies);
                        break;
                    }
                    else if (objectCheck == 'h')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        game.updatelifeAlien(10, 'h');
                        game.ShowBoard(Zombies);
                        cout << "Alien found health pack, +10 life." << endl;
                        Pause();
                        goto turn;
                    }
                    else if (objectCheck == 'p')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        while (true)
                        {
                            pod_attack = rand() % numofZombies;
                            if (Zombies[pod_attack].getlifeZombie() != 0)
                            {
                                Zombies[pod_attack].updatelifeZombie(10);
                                break;
                            }
                        }
                        game.ShowBoard(Zombies);
                        cout << "Alien found pod, zombie " << pod_attack + 1 << " is attacked, -10 life." << endl;
                        Pause();
                        goto turn;
                    }
                    else if (objectCheck == 'v')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        game.updateattackAlien('a');
                        game.ShowBoard(Zombies);
                        cout << "Alien found arrow object, attack +20." << endl;
                        Pause();
                        goto turn;
                    }
                    else if (objectCheck == '^')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        game.updateattackAlien('a');
                        game.ShowBoard(Zombies);
                        main_command = "up";
                        cout << "Alien found arrow object, attack +20, changed to moving upwards." << endl;
                        Pause();
                        goto turn;
                    }
                    else if (objectCheck == '<')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        game.updateattackAlien('a');
                        game.ShowBoard(Zombies);
                        main_command = "left";
                        cout << "Alien found arrow object, attack +20, changed to moving left" << endl;
                        Pause();
                        goto turn;
                    }
                    else if (objectCheck == '>')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        game.updateattackAlien('a');
                        game.ShowBoard(Zombies);
                        main_command = "right";
                        cout << "Alien found arrow object, attack +20, changed to moving right" << endl;
                        Pause();
                        goto turn;
                    }
                    else
                    {

                        game.moveObject(posY, posX, move_arrow, 'A');
                        ClearScreen();
                        game.ShowBoard(Zombies);
                        Pause();
                        goto turn;
                    }
                }
            }
            // alien move left
            else if (main_command == "left")
            {
                cin.clear();
                move_arrow = '<';
                if (posX == 0)
                {
                    ClearScreen();
                    game.resetTrail();
                    game.ShowBoard(Zombies);
                    cout << "Alien has reached border." << endl;
                    Pause();
                    game.updateattackAlien('r');
                    turnObject(Zombies);
                    break;
                }
                else
                {
                    objectCheck = game.getObject((posY), (posX - 1));
                    if (isdigit(objectCheck))
                    {
                        int zombieIndex = objectCheck - 49;
                        if (Zombies[zombieIndex].getlifeZombie() <= attack_Alien)
                        {
                            game.moveObject(posY, posX, move_arrow, 'A');
                            ClearScreen();
                            game.ShowBoard(Zombies);
                            cout << "Zombie " << zombieIndex + 1 << " defeated." << endl;
                            Pause();
                            goto turn;
                        }
                        else
                        {
                            ClearScreen();
                            game.resetTrail();
                            Zombies[zombieIndex].updatelifeZombie(attack_Alien);
                            game.ShowBoard(Zombies);
                            cout << "Alien hits zombie with attack of " << attack_Alien << endl;
                            Pause();
                            game.updateattackAlien('r');
                            turnObject(Zombies);
                            break;
                        }
                    }
                    else if (objectCheck == 'r')
                    {
                        ClearScreen();
                        game.hitRock(posY, posX, move_arrow);
                        game.resetTrail();
                        game.ShowBoard(Zombies);
                        cout << "Alien hits rock, hidden object revealed." << endl;
                        Pause();
                        game.updateattackAlien('r');
                        turnObject(Zombies);
                        break;
                    }
                    else if (objectCheck == 'h')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        game.updatelifeAlien(10, 'h');
                        game.ShowBoard(Zombies);
                        cout << "Alien found health pack, +10 life." << endl;
                        Pause();
                        goto turn;
                    }
                    else if (objectCheck == 'p')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        while (true)
                        {
                            pod_attack = rand() % numofZombies;
                            if (Zombies[pod_attack].getlifeZombie() != 0)
                            {
                                Zombies[pod_attack].updatelifeZombie(10);
                                break;
                            }
                        }
                        game.ShowBoard(Zombies);
                        cout << "Alien found pod, zombie " << pod_attack + 1 << " is attacked, -10 life." << endl;
                        Pause();
                        goto turn;
                    }
                    else if (objectCheck == '<')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        game.updateattackAlien('a');
                        game.ShowBoard(Zombies);
                        cout << "Alien found arrow object, attack +20." << endl;
                        Pause();
                        goto turn;
                    }
                    else if (objectCheck == '^')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        game.updateattackAlien('a');
                        game.ShowBoard(Zombies);
                        main_command = "up";
                        cout << "Alien found arrow object, attack +20, changed to moving upwards." << endl;
                        Pause();
                        goto turn;
                    }
                    else if (objectCheck == 'v')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        game.updateattackAlien('a');
                        game.ShowBoard(Zombies);
                        main_command = "down";
                        cout << "Alien found arrow object, attack +20, changed to moving downwards." << endl;
                        Pause();
                        goto turn;
                    }
                    else if (objectCheck == '>')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        game.updateattackAlien('a');
                        game.ShowBoard(Zombies);
                        main_command = "right";
                        cout << "Alien found arrow object, attack +20, changed to moving right." << endl;
                        Pause();
                        goto turn;
                    }
                    else
                    {
                        game.moveObject(posY, posX, move_arrow, 'A');
                        ClearScreen();
                        game.ShowBoard(Zombies);
                        Pause();
                        goto turn;
                    }
                }
            }
            // alien move right
            else if (main_command == "right")
            {
                cin.clear();
                move_arrow = '>';
                if (posX == (game.getBoardX() - 1))
                {
                    ClearScreen();
                    game.resetTrail();
                    game.ShowBoard(Zombies);
                    cout << "Alien has reached border." << endl;
                    Pause();
                    game.updateattackAlien('r');
                    turnObject(Zombies);
                    break;
                }
                else
                {
                    objectCheck = game.getObject((posY), (posX + 1));
                    if (isdigit(objectCheck))
                    {
                        int zombieIndex = objectCheck - 49;
                        if (Zombies[zombieIndex].getlifeZombie() <= attack_Alien)
                        {
                            game.moveObject(posY, posX, move_arrow, 'A');
                            ClearScreen();
                            game.ShowBoard(Zombies);
                            cout << "Zombie " << zombieIndex + 1 << " defeated." << endl;
                            Pause();
                            goto turn;
                        }
                        else
                        {
                            ClearScreen();
                            game.resetTrail();
                            Zombies[zombieIndex].updatelifeZombie(attack_Alien);
                            game.ShowBoard(Zombies);
                            cout << "Alien hits zombie with attack of " << attack_Alien << endl;
                            Pause();
                            game.updateattackAlien('r');
                            turnObject(Zombies);
                            break;
                        }
                    }
                    else if (objectCheck == 'r')
                    {
                        ClearScreen();
                        game.resetTrail();
                        game.hitRock(posY, posX, move_arrow);
                        game.ShowBoard(Zombies);
                        cout << "Alien hits rock, hidden object revealed." << endl;
                        Pause();
                        game.updateattackAlien('r');
                        turnObject(Zombies);
                        break;
                    }
                    else if (objectCheck == 'h')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        game.updatelifeAlien(10, 'h');
                        game.ShowBoard(Zombies);
                        cout << "Alien found health pack, +10 life." << endl;
                        Pause();
                        goto turn;
                    }
                    else if (objectCheck == 'p')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        while (true)
                        {
                            pod_attack = rand() % numofZombies;
                            if (Zombies[pod_attack].getlifeZombie() != 0)
                            {
                                Zombies[pod_attack].updatelifeZombie(10);
                                break;
                            }
                        }
                        game.ShowBoard(Zombies);
                        cout << "Alien found pod, zombie " << pod_attack + 1 << " is attacked, -10 life." << endl;
                        Pause();
                        goto turn;
                    }
                    else if (objectCheck == '>')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        game.updateattackAlien('a');
                        game.ShowBoard(Zombies);
                        cout << "Alien found arrow object, attack +20." << endl;
                        Pause();
                        goto turn;
                    }
                    else if (objectCheck == '^')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        game.updateattackAlien('a');
                        game.ShowBoard(Zombies);
                        main_command = "up";
                        cout << "Alien found arrow object, attack +20, changed to moving upwards." << endl;
                        Pause();
                        goto turn;
                    }
                    else if (objectCheck == 'v')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        game.updateattackAlien('a');
                        game.ShowBoard(Zombies);
                        main_command = "down";
                        cout << "Alien found arrow object, attack +20, changed to moving downwards." << endl;
                        Pause();
                        goto turn;
                    }
                    else if (objectCheck == '<')
                    {
                        ClearScreen();
                        game.moveObject(posY, posX, move_arrow, 'A');
                        game.updateattackAlien('a');
                        game.ShowBoard(Zombies);
                        main_command = "left";
                        cout << "Alien found arrow object, attack +20, changed to moving left." << endl;
                        Pause();
                        goto turn;
                    }
                    else
                    {

                        game.moveObject(posY, posX, move_arrow, 'A');
                        ClearScreen();
                        game.ShowBoard(Zombies);
                        Pause();
                        goto turn;
                    }
                }
            }
            else if (main_command == "arrow")
            {
                cin.clear();
                int oriY = game.getBoardY();
                int oriX = game.getBoardX();
                int arrowY, arrowX;
                char changeArrow;
                while (true)
                {
                    while (cout << "Enter row of arrow: " && (!(cin >> arrowY) || (arrowY < 1 || arrowY > oriY)))
                    {
                        CleanInput();
                        cout << "Invalid input: input is not an integer / number not in range." << endl;
                    }
                    CleanInput();
                    while (cout << "Enter column of arrow: " && (!(cin >> arrowX) || (arrowX < 1 || arrowX > oriX)))
                    {
                        CleanInput();
                        cout << "Invalid input: input is not an integer / number not in range." << endl;
                    }
                    CleanInput();
                    char checkArrow = game.getObject((oriY - arrowY), (arrowX - 1));
                    if (checkArrow == '^' || checkArrow == 'v' || checkArrow == '<' || checkArrow == '>')
                    {
                        break;
                    }
                    else
                    {
                        cout << "No arrow in this cell, please try again." << endl;
                    }
                }

                while (cout << "Enter direction you want to changed ('^', 'v', '<', '>'): " && (!(cin >> changeArrow) || ((changeArrow != '^') && (changeArrow != 'v') && (changeArrow != '<') && (changeArrow != '>'))))
                {
                    CleanInput();
                    cout << "Invalid input: input is not a single character / not defined." << endl;
                }
                CleanInput();

                game.changeArrow(oriY - arrowY, arrowX - 1, changeArrow);
                ClearScreen();
                game.ShowBoard(Zombies);
                cout << "Arrow at " << arrowY << "," << arrowX << " changed." << endl;
                Pause();
                ClearScreen();
                goto nonturn;
            }
            else if (main_command == "help")
            {
                cin.clear();
                Lines();
                cout << "Command  --  Description" << endl;
                Lines();
                cout << "up  -> Alien to move up. \n"
                     << "down  -> Alien to move down. \n"
                     << "left  -> Alien to move left. \n"
                     << "right -> Alien to move right. \n"
                     << "arrow -> Switch the direction of an arrow object in the game board. \n"
                     << "help -> List and describe the commands that the player can use in the game. \n"
                     << "save -> Save the current game to a file. \n"
                     << "load -> Load a saved game from a file. \n"
                     << "quit -> Quit the game while still in play. \n";
                Pause();
                ClearScreen();
                goto nonturn;
            }
            else if (main_command == "save")
            {
                saveongoinggame = false;
            saveongoing:
                cin.clear();
                bool file_savenew = true;
                string savefolder = "game_saves";
                string savefile_name;

                cout << "Name the save file(without format): ";
                cin >> savefile_name;
                // example: game_saves/name.txt
                string savepath = savefolder + "/" + savefile_name + ".txt";

                filesystem::create_directory(savefolder);

                bool file_exists = false;
                for (const auto &entry : filesystem::directory_iterator(savefolder))
                {
                    if (entry.is_regular_file())
                    {
                        if (entry.path().filename() == (savefile_name + ".txt"))
                        {
                            file_exists = true;
                            break;
                        }
                    }
                }

                bool file_overwrite = false;
                if (file_exists)
                {
                    string overwrite;
                    cout << "Save file named " << savefile_name << " exists. Do you want to overwrite it? (number)" << endl;
                    cout << "1. Yes" << endl
                         << "2. No" << endl;
                    while (true)
                    {
                        cin >> overwrite;
                        if (overwrite == "1")
                        {
                            file_overwrite = true;
                            break;
                        }
                        else if (overwrite == "2")
                        {
                            break;
                        }
                        else
                        {
                            cin.clear();
                            cout << endl;
                            cout << "Command not found, please try again." << endl;
                            Pause();
                        }
                    }
                }
                else
                {
                    string saveconfirmation;
                    if (saveongoinggame = false)
                    {
                        cout << "Confirm save new file? (number)" << endl;
                        cout << "1. Yes" << endl
                             << "2. No" << endl;
                        while (true)
                        {
                            cin >> saveconfirmation;
                            if (saveconfirmation == "1")
                            {
                                break;
                            }
                            else if (saveconfirmation == "2")
                            {
                                file_savenew = false;
                                break;
                            }
                            else
                            {
                                cin.clear();
                                cout << endl;
                                cout << "Command not found, please try again." << endl;
                                Pause();
                            }
                        }
                    }
                    else
                    {
                        cout << "Confirm save ongoing game? (number)" << endl;
                        cout << "1. Yes" << endl
                             << "2. No" << endl;
                        while (true)
                        {
                            cin >> saveconfirmation;
                            if (saveconfirmation == "1")
                            {
                                break;
                            }
                            else if (saveconfirmation == "2")
                            {
                                saveongoinggame = false;
                                goto loadongoinggame;
                            }
                            else
                            {
                                cin.clear();
                                cout << endl;
                                cout << "Command not found, please try again." << endl;
                                Pause();
                            }
                        }
                    }
                }

                if ((file_overwrite == true) && (file_savenew == true))
                {
                    ofstream savegame(savepath);
                    if (!savegame)
                    {
                        cout << "There is an error creating the save file" << endl;
                    }
                    else
                    {
                        savegame << "Zombies: " << endl;
                        for (int saveZ = 0; saveZ < size(Zombies); ++saveZ)
                        {
                            savegame << Zombies[saveZ].getlifeZombie() << " " << Zombies[saveZ].getattackZombie() << " " << Zombies[saveZ].getrangeZombie() << endl;
                        }
                        savegame << endl;

                        savegame << "Saved alien: " << endl;
                        savegame << game.getlifeAlien() << " " << game.getattackAlien() << endl;
                        savegame << endl;
                        savegame << "Board: " << endl;
                        savegame << game.getBoardY() << " " << game.getBoardX() << endl;
                        for (int saveY = 0; saveY < game.getBoardY(); ++saveY)
                        {
                            for (int saveX = 0; saveX < game.getBoardX(); ++saveX)
                            {
                                savegame << game.getObject(saveY, saveX) << " ";
                            }
                            savegame << endl;
                        }
                        cout << "Save file created successfully." << endl;
                        savegame.close();
                    }
                    Pause();
                    ClearScreen();
                    goto nonturn;
                }
                else
                {
                    cout << "File is not created." << endl;
                    Pause();
                    ClearScreen();
                    goto nonturn;
                }
            }
            // load command
            else if (main_command == "load")
            {
                if (saveongoinggame)
                {
                    string saveconfirmation;
                    cout << "Save ongoing game? (number)" << endl;
                    cout << "1. Yes" << endl
                         << "2. No" << endl;
                    while (true)
                    {
                        cin >> saveconfirmation;
                        if (saveconfirmation == "1")
                        {
                            saveongoinggame = true;
                            goto saveongoing;
                        }
                        else if (saveconfirmation == "2")
                        {
                            break;
                        }
                        else
                        {
                            cin.clear();
                            cout << endl;
                            cout << "Command not found, please try again." << endl;
                            Pause();
                        }
                    }
                }
            loadongoinggame:
                string loadfolder = "game_saves";
                string loadfile_name;

                cout << "Choose which file you want to load (name, without format):" << endl;
                for (const auto &entry : filesystem::directory_iterator(loadfolder))
                {
                    if (entry.is_regular_file())
                    {
                        cout << entry.path().filename() << endl;
                    }
                }
                cout << "=> ";
                cin >> loadfile_name;

                ifstream loadfile("game_saves/" + loadfile_name + ".txt");
                if (!loadfile)
                {
                    cout << "Failed to load file." << endl;
                    Pause();
                    ClearScreen();
                    goto nonturn;
                }
                else
                {
                    int num_Zombie_loaded = 0;
                    vector<int> load_zombie;
                    load_zombie.resize(1);
                    vector<int> load_alien;
                    load_alien.resize(2);
                    vector<vector<char>> load_board;
                    int numY, numX;
                    int loadY = 0;
                    bool can_loadobjects = false;
                    string line;
                    while (getline(loadfile, line))
                    {
                        if (line.empty())
                        {
                            continue;
                        }
                        else
                        {
                            if (line[0] == 'Z')
                            {
                                int num1, num2, num3;
                                while (loadfile >> num1 >> num2 >> num3)
                                {
                                    load_zombie.push_back(num1);
                                    load_zombie.push_back(num2);
                                    load_zombie.push_back(num3);
                                    num_Zombie_loaded++;
                                }
                            }
                            else if (line[0] == 'S')
                            {
                                int num1, num2;
                                while (loadfile >> num1 >> num2)
                                {
                                    load_alien[0] = num1;
                                    load_alien[1] = num2;
                                }
                            }
                            else if (line[0] == 'B')
                            {
                                loadfile >> numY >> numX;
                                load_board.resize(numY);
                                for (int r = 0; r < numY; ++r)
                                {
                                    load_board[r].resize(numX);
                                }
                                can_loadobjects = true;
                            }
                            else if (can_loadobjects == true)
                            {
                                for (int loadX = 0; loadX < numX; ++loadX)
                                    load_board[loadY][loadX] = line[loadX * 2];
                                ++loadY;
                            }
                        }
                        loadfile.clear();
                    }
                    loadfile.close();
                    load_zombie.erase(load_zombie.begin());

                    // Load and assign zombies
                    int differenceload = abs(int(num_Zombie_loaded - size(Zombies)));
                    if (num_Zombie_loaded > size(Zombies))
                    {
                        for (int i = 0, j = 0; i < size(load_zombie), j < (num_Zombie_loaded - differenceload); i += 3, j++)
                        {
                            Zombies[j].loadupdateZombie(load_zombie[0 + i], load_zombie[1 + i], load_zombie[2 + i]);
                        }
                        for (int c = 0, d = 0; d < differenceload; c += 3, ++d)
                        {
                            int loadindex = size(load_zombie) - (differenceload * 3);
                            Zombie npc(load_zombie[loadindex + c + 0], load_zombie[loadindex + c + 1], load_zombie[loadindex + c + 2]);
                            Zombies.push_back(npc);
                        }
                    }
                    else if (num_Zombie_loaded < size(Zombies))
                    {
                        for (int d = 0; d < differenceload; ++d)
                        {
                            Zombies.pop_back();
                        }
                        for (int i = 0, j = 0; i < size(load_zombie), j < num_Zombie_loaded; i += 3, j++)
                        {
                            Zombies[j].loadupdateZombie(load_zombie[0 + i], load_zombie[1 + i], load_zombie[2 + i]);
                        }
                    }
                    else if (num_Zombie_loaded == size(Zombies))
                    {
                        for (int i = 0, j = 0; i < size(load_zombie), j < num_Zombie_loaded; i += 3, j++)
                        {
                            Zombies[j].loadupdateZombie(load_zombie[0 + i], load_zombie[1 + i], load_zombie[2 + i]);
                        }
                    }

                    // Load alien
                    game.loadupdateAlien(load_alien[0], load_alien[1]);

                    // Load board
                    game.loadupdateBoard(load_board);

                    ClearScreen();
                    game.ShowBoard(Zombies);
                    cout << "Game loaded successfully." << endl;
                    Pause();
                    ClearScreen();
                    goto nonturn;
                }
            }
            else if (main_command == "quit")
            {
                cin.clear();
                MenuDisplay();
                break;
            }
            else
            {
                cin.clear();
                cout << endl;
                cout << "Command not found, please try again." << endl;
                Pause();
                ClearScreen();
            }
        }
    }
}

void turnObject(vector<Zombie> &Zombies)
{
    for (int i = 0; i < size(Zombies); ++i)
    {
        ClearScreen();
        game.ShowBoard(Zombies);
        cout << "Zombie " << i + 1 << "'s turn" << endl;
        // check if zombie still alive
        if (Zombies[i].getlifeZombie() != 0)
        {
            Pause();
            vector<int> currentpos_Zombie = game.posZombie((i + 49));
            int posY_Zombie = currentpos_Zombie[1];
            int posX_Zombie = currentpos_Zombie[0];

            // Zombie's attack
            int attack_zombie = Zombies[i].getattackZombie();
            vector<int> currentpos_Alien = game.posAlien();
            int differenceY = abs(currentpos_Alien[1] - posY_Zombie);
            int differenceX = abs(currentpos_Alien[0] - posX_Zombie);
            if ((differenceY <= Zombies[i].getrangeZombie() && differenceY >= 0) && (differenceX <= Zombies[i].getrangeZombie() && differenceX >= 0))
            {
                if (game.getlifeAlien() > 0)
                {
                    game.updatelifeAlien(attack_zombie, 'd');
                    ClearScreen();
                    game.ShowBoard(Zombies);
                    cout << "Zombie " << i + 1 << " has attacked alien with damage of " << attack_zombie << endl;
                    Pause();
                }
                else
                {
                    turnAlien(Zombies);
                    break;
                }
            }
            else
            {
                ClearScreen();
                game.ShowBoard(Zombies);
                cout << "Alien out of range, zombie " << i + 1 << " does not attack." << endl;
                Pause();
            }

            // Zombie's move
            // initialize every char with something, in case char checking later of the direction is out of range of board
            char moveUp = 'm';
            char moveDown = 'm';
            char moveLeft = 'm';
            char moveRight = 'm';

            // Check 9 cases: 4 corner -> 4 border not corner -> not corner nor border
            if (posY_Zombie == 0 && posX_Zombie == 0) // top left corner
            {
                moveDown = game.getObject(posY_Zombie + 1, posX_Zombie);
                moveRight = game.getObject(posY_Zombie, posX_Zombie + 1);
            }
            else if (posY_Zombie == 0 && posX_Zombie == (game.getBoardX() - 1)) // top right corner
            {
                moveDown = game.getObject(posY_Zombie + 1, posX_Zombie);
                moveLeft = game.getObject(posY_Zombie, posX_Zombie - 1);
            }
            else if (posY_Zombie == (game.getBoardY() - 1) && posX_Zombie == 0) // bottom left corner
            {
                moveUp = game.getObject(posY_Zombie - 1, posX_Zombie);
                moveRight = game.getObject(posY_Zombie, posX_Zombie + 1);
            }
            else if (posY_Zombie == (game.getBoardY() - 1) && posX_Zombie == (game.getBoardX() - 1)) // bottom right corner
            {
                moveUp = game.getObject(posY_Zombie - 1, posX_Zombie);
                moveLeft = game.getObject(posY_Zombie, posX_Zombie - 1);
            }
            else if (posY_Zombie == 0) // top row border
            {
                moveDown = game.getObject(posY_Zombie + 1, posX_Zombie);
                moveLeft = game.getObject(posY_Zombie, posX_Zombie - 1);
                moveRight = game.getObject(posY_Zombie, posX_Zombie + 1);
            }
            else if (posY_Zombie == (game.getBoardY() - 1)) // bottom row border
            {
                moveUp = game.getObject(posY_Zombie - 1, posX_Zombie);
                moveLeft = game.getObject(posY_Zombie, posX_Zombie - 1);
                moveRight = game.getObject(posY_Zombie, posX_Zombie + 1);
            }
            else if (posX_Zombie == 0) // left column border
            {
                moveUp = game.getObject(posY_Zombie - 1, posX_Zombie);
                moveDown = game.getObject(posY_Zombie + 1, posX_Zombie);
                moveRight = game.getObject(posY_Zombie, posX_Zombie + 1);
            }
            else if (posX_Zombie == (game.getBoardX() - 1)) // right column border
            {
                moveUp = game.getObject(posY_Zombie - 1, posX_Zombie);
                moveDown = game.getObject(posY_Zombie + 1, posX_Zombie);
                moveLeft = game.getObject(posY_Zombie, posX_Zombie - 1);
            }
            else // not at any border
            {
                moveUp = game.getObject(posY_Zombie - 1, posX_Zombie);
                moveDown = game.getObject(posY_Zombie + 1, posX_Zombie);
                moveLeft = game.getObject(posY_Zombie, posX_Zombie - 1);
                moveRight = game.getObject(posY_Zombie, posX_Zombie + 1);
            }

            if (moveUp == ' ' || moveDown == ' ' || moveLeft == ' ' || moveRight == ' ')
            {
                while (true)
                {
                    int moveZombie = rand() % 4;
                    if (moveZombie == 0 && moveUp == ' ')
                    {
                        game.moveObject(posY_Zombie, posX_Zombie, '^', 'Z', (i + 49));
                        ClearScreen();
                        game.ShowBoard(Zombies);
                        cout << "Zombie " << i + 1 << " has moved upwards." << endl;
                        Pause();
                        break;
                    }
                    else if (moveZombie == 1 && moveDown == ' ')
                    {
                        game.moveObject(posY_Zombie, posX_Zombie, 'v', 'Z', (i + 49));
                        ClearScreen();
                        game.ShowBoard(Zombies);
                        cout << "Zombie " << i + 1 << " has moved downwards." << endl;
                        Pause();
                        break;
                    }
                    else if (moveZombie == 2 && moveLeft == ' ')
                    {
                        game.moveObject(posY_Zombie, posX_Zombie, '<', 'Z', (i + 49));
                        ClearScreen();
                        game.ShowBoard(Zombies);
                        cout << "Zombie " << i + 1 << " has moved to left." << endl;
                        Pause();
                        break;
                    }
                    else if (moveZombie == 3 && moveRight == ' ')
                    {
                        game.moveObject(posY_Zombie, posX_Zombie, '>', 'Z', (i + 49));
                        ClearScreen();
                        game.ShowBoard(Zombies);
                        cout << "Zombie " << i + 1 << " has moved to right." << endl;
                        Pause();
                        break;
                    }
                }
            }
            else
            {
                ClearScreen();
                game.ShowBoard(Zombies);
                cout << "Zombie " << i + 1 << " has no place to move." << endl;
                Pause();
            }
        }
        else
        {
            continue;
        }
    }
    cout << "Zombies' turn ends." << endl;
    Pause();
    ClearScreen();
    turnAlien(Zombies);
}

void BoardSettings()
{
first:
    int numX;
    int numY;
    Lines();
    cout << "Current board dimension(row x column): " << game.getBoardY() << "x" << game.getBoardX() << endl;
    Lines();
    cout << "1. Change board size"
         << "\n"
         << "2. Number of zombies: " << initialNumberOfZombies
         << "\n"
         << "3. Back" << endl;

    string boardmenu;
    while (true)
    {
        cout << "Please input a command(number): ";
        cin >> boardmenu;
        if (boardmenu == "1")
        {
            CleanInput();
            ClearScreen();
            Lines();
            cout << "Change board size: min 5 and max 15 for both dimensions." << endl;
            Lines();
            while (cout << "Enter row / height: " && (!(cin >> numY) || (numY % 2 == 0) || (numY < 5 || numY > 15)))
            {
                CleanInput();
                cout << "Invalid input: input is not an integer / number is not an odd number / number not in range." << endl;
            }
            CleanInput();
            while (cout << "Enter column / width: " && (!(cin >> numX) || (numX % 2 == 0) || (numX < 5 || numX > 15)))
            {
                CleanInput();
                cout << "Invalid input: input is not an integer / number is not an odd number / number not in range." << endl;
            }
            CleanInput();
            game.boardUpdate(numY, numX);
            ClearScreen();
            BoardSettings();
            break;
        }
        else if (boardmenu == "2")
        {
            CleanInput();
            int updateZombie;
            ClearScreen();
            Lines();
            cout << "Update number of zombies" << endl;
            Lines();
            while (cout << "Enter a number between 2 and 9: " && (!(cin >> updateZombie) || (updateZombie < 2 || updateZombie > 9)))
            {
                CleanInput();
                cout << "Invalid input: input is not an integer / number entered is not in range.\n";
            }
            CleanInput();
            initialNumberOfZombies = updateZombie;
            ClearScreen();
            BoardSettings();
            break;
        }
        else if (boardmenu == "3")
        {
            CleanInput();
            MenuDisplay();
            break;
        }
        else
        {
            cout << "Command not found, please try again." << endl;
        }
    }
}

int main()
{
    srand(time(NULL));
    MenuDisplay();

    return 0;
}