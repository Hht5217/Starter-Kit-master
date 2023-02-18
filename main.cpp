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
    int initlifeAlien_;
    int lifeAlien_;
    int attackAlien_;

public:
    Board(int BoardX = 7, int BoardY = 7);
    void init(int BoardX, int BoardY);
    void ShowBoard(vector<Zombie> &Zombies) const;
    void initObjects(int &init_numZombie);
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
};

void Lines()
{
    cout << string(50, '#') << endl;
}

// ####################### Board class functions #######################
Board::Board(int BoardX, int BoardY)
{
    init(BoardX, BoardY);
}

void Board::init(int BoardX, int BoardY)
{
    char objects[] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'h', 'p', 'r', '^', 'v', '<', '>'};
    int noofObjects = 14;
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
            int objNo = rand() % noofObjects;
            map_[i][j] = objects[objNo];
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

void Board::initObjects(int &init_numZombie)
{
    map_[BoardY_ / 2][BoardX_ / 2] = 'A';
    lifeAlien_ = initlifeAlien_ = setLife('a');
    attackAlien_ = 0;

    for (int z = 0; z < init_numZombie; ++z)
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
    int noofObjects = 15;
    for (int i = 0; i < BoardY_; ++i)
    {
        for (int j = 0; j < BoardX_; ++j)
        {
            if (map_[i][j] == '.')
            {
                int objNo = rand() % noofObjects;
                map_[i][j] = objects[objNo];
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
        if (lifeAlien_ == initlifeAlien_)
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

// ######################### other functions ###########################

Board game;
int init_numZombie = 2;
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
    char menu;
    while (true)
    {
        cout << "Please input a command(number): ";
        cin >> menu;
        if (menu == '1' || menu == '2' || menu == '3')
        {
            if (menu == '1')
            {
                ClearScreen();
                PlayConfirmation();
                break;
            }
            else if (menu == '2')
            {
                ClearScreen();
                BoardSettings();
                break;
            }
            else if (menu == '3')
            {
                cout << "Good Bye!" << endl;
                break;
            }
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
    char playmenu;
    while (true)
    {
        cout << "Please input a command(number): ";
        cin >> playmenu;
        if (playmenu == '1' || playmenu == '2' || playmenu == '3')
        {
            if (playmenu == '1')
            {
                ClearScreen();
                game.init(game.getBoardX(), game.getBoardY());
                game.initObjects(init_numZombie);
                vector<Zombie> Zombies;
                for (int i = 0; i < init_numZombie; ++i)
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
            else if (playmenu == '2')
            {
                ClearScreen();
                BoardSettings();
                break;
            }
            else if (playmenu == '3')
            {
                MenuDisplay();
                break;
            }
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
        char command_arrow;
        char move_arrow;
        while (true)
        {
            game.ShowBoard(Zombies);
            cout << "Please input a command: " << endl;
            cout << "1. up"
                 << "\n"
                 << "2. down"
                 << "\n"
                 << "3. left"
                 << "\n"
                 << "4. right"
                 << "\n"
                 << "5. exit" << endl;
            cin >> command_arrow;
        turn:
            const vector<int> &currentPos = game.posAlien();
            int posX = currentPos[0];
            int posY = currentPos[1];
            char objectCheck;
            int attack_Alien = game.getattackAlien();
            int numofZombies = size(Zombies);
            int pod_attack;

            // alien move up
            if (command_arrow == '1')
            {
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
                        command_arrow = '2';
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
                        command_arrow = '3';
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
                        command_arrow = '4';
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
            else if (command_arrow == '2')
            {
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
                        command_arrow = '1';
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
                        command_arrow = '3';
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
                        command_arrow = '4';
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
            else if (command_arrow == '3')
            {
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
                        command_arrow = '1';
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
                        command_arrow = '2';
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
                        command_arrow = '4';
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
            else if (command_arrow == '4')
            {
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
                        command_arrow = '1';
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
                        command_arrow = '2';
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
                        command_arrow = '3';
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
            else if (command_arrow == '5')
            {
                MenuDisplay();
                break;
            }
            else
            {
                ClearScreen();
                cout << "Command not found, please try again." << endl;
                Lines();
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
         << "2. Number of zombies: " << init_numZombie
         << "\n"
         << "3. Back" << endl;

    char menu2;
    while (true)
    {
        cout << "Please input a command(number): ";
        cin >> menu2;
        if (menu2 == '1' || menu2 == '2' || menu2 == '3')
        {
            char confirmation;
            if (menu2 == '1')
            {
                ClearScreen();
                Lines();
                cout << "Changing board size" << endl;
                Lines();
                while (true)
                {
                    cout << "Enter row/height => ";
                    cin >> numY;
                    cout << "Enter column/width => ";
                    cin >> numX;
                    if (numX % 2 == 0 || numY % 2 == 0)
                    {
                        cout << "Rows or columns entered is not an odd number, please try again." << endl;
                        Lines();
                    }
                    else
                    {
                        while (true)
                        {
                            cout << "Are you sure? y/n: ";
                            cin >> confirmation;
                            if (confirmation == 'y')
                            {
                                game.boardUpdate(numY, numX);
                                ClearScreen();
                                goto first;
                            }
                            else if (confirmation == 'n')
                            {
                                ClearScreen();
                                goto first;
                            }
                            else
                            {
                                cout << "Command not found, please try again." << endl;
                                Lines();
                            }
                        }
                    }
                }
            }
            else if (menu2 == '2')
            {
                int updateZombie;
                ClearScreen();
                Lines();
                cout << "Update number of zombies" << endl;
                Lines();
                while (true)
                {
                    cout << "Enter number of zombies(minimum 2, maximum 9): ";
                    cin >> updateZombie;
                    if (updateZombie < 2 || updateZombie > 9)
                    {
                        cout << "Number entered not in / out of range, please try again." << endl;
                    }
                    else
                    {
                        while (true)
                        {
                            cout << "Are you sure? y/n: ";
                            cin >> confirmation;
                            if (confirmation == 'y')
                            {
                                init_numZombie = updateZombie;
                                ClearScreen();
                                goto first;
                            }
                            else if (confirmation == 'n')
                            {
                                ClearScreen();
                                goto first;
                            }
                            else
                            {
                                cout << "Command not found, please try again." << endl;
                                Lines();
                            }
                        }
                    }
                }
            }
            else if (menu2 == '3')
            {
                MenuDisplay();
                break;
            }
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