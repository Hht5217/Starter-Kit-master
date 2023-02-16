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
using namespace std;
using namespace pf;

// This class is defined first as its objects are being passed into many functions
class GameObject
{
public:
    GameObject(int life, int attack, int range);
    void ZombieStats(const int &i, const vector<GameObject> &Zombies);

private:
    int life_;
    int attack_;
    int range_;
};

// Declare functions here
void MenuDisplay();
void BoardSettings();
void PlayConfirmation();
void turnAlien(vector<GameObject> &Zombies);
void turnObject(vector<GameObject> &Zombies);
int setLife();
int setAttack();
int setRange();

class Board
{
private:
    vector<vector<char>> map_;
    int BoardX_, BoardY_;
    int lifeAlien_;

public:
    Board(int BoardX = 7, int BoardY = 7);
    void init(int BoardX, int BoardY);
    void ShowBoard(vector<GameObject> &Zombies) const;
    void initAlien();
    void boardUpdate(int BoardY, int BoardX);
    int getBoardX() const;
    int getBoardY() const;
    int getlifeAlien() const;
    vector<int> posAlien();
    void moveAlien(int posY, int posX, char movedir);
    void resetTrail();
};

void Lines()
{
    cout << string(50, '#') << endl;
}

Board::Board(int BoardX, int BoardY)
{
    init(BoardX, BoardY);
}

void Board::init(int BoardX, int BoardY)
{
    char objects[] = {' ', ' ', ' ', ' ', ' ', ' ', 'z', 'h', 'p', 'r'};
    int noofObjects = 10;
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

void Board::ShowBoard(vector<GameObject> &Zombies) const
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
    cout << "0";
    cout << endl;
    for (int i = 0; i < size(Zombies); ++i)
    {
        Zombies[i].ZombieStats(i, Zombies);
    }
    cout << endl;
}

void Board::initAlien()
{
    map_[BoardY_ / 2][BoardX_ / 2] = 'A';
    int lb = 150, ub = 200;
    int lifeAlien;
    for (int i = 0; i < 1;)
    {
        lifeAlien = (rand() % (ub - lb + 1)) + lb;
        if (lifeAlien % 10 == 0)
        {
            i++;
        }
    }
    lifeAlien_ = lifeAlien;
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

void Board::moveAlien(int posY, int posX, char movedir)
{
    switch (movedir)
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

void Board::resetTrail()
{
    char objects[] = {' ', ' ', ' ', ' ', ' ', ' ', 'h', 'p', 'r'};
    int noofObjects = 9;
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

GameObject::GameObject(int life, int attack, int range)
{
    life_ = life;
    attack_ = attack;
    range_ = range;
}

void GameObject::ZombieStats(const int &i, const vector<GameObject> &Zombies)
{
    cout << "Zombie " << i + 1;
    cout.width(12);
    cout << "=>  Life " << Zombies[i].life_ << " | ";
    cout << "Attack ";
    cout.width(2);
    cout << Zombies[i].attack_ << " | ";
    cout << "Range " << Zombies[i].range_ << endl;
}
// ######################### functions ###########################

Board game;
int numZombie = 2;
int setLife()
{
    int lower_life = 150, upper_life = 200;
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
                game.initAlien();
                vector<GameObject> Zombies;
                for (int i = 0; i < numZombie; ++i)
                {
                    int life = setLife();
                    int attack = setAttack();
                    int range = setRange();
                    GameObject npc(life, attack, range);
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

void turnAlien(vector<GameObject> &Zombies)
{
    char commanddir;
    char movedir;
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
        cin >> commanddir;
    turn:
        vector<int> currentPos = game.posAlien();
        int posX = currentPos[0];
        int posY = currentPos[1];
        if (commanddir == '1' || commanddir == '2' || commanddir == '3' || commanddir == '4')
        {
            if (commanddir == '1')
            {
                movedir = '^';
                if (posY == 1)
                {
                    game.moveAlien(posY, posX, movedir);
                    ClearScreen();
                    game.resetTrail();
                    game.ShowBoard(Zombies);
                    cout << "Alien reached border, trail reset" << endl;
                    turnObject(Zombies);
                    break;
                }
                else if (posY == 0)
                {
                    ClearScreen();
                    game.resetTrail();
                    game.ShowBoard(Zombies);
                    cout << "Alien hits border, position unchanged" << endl;
                    turnObject(Zombies);
                    break;
                }
                else
                {
                    game.moveAlien(posY, posX, movedir);
                    ClearScreen();
                    game.ShowBoard(Zombies);
                    Pause();
                    goto turn;
                }
            }
            else if (commanddir == '2')
            {
                movedir = 'v';
                if (posY == (game.getBoardY() - 2))
                {
                    game.moveAlien(posY, posX, movedir);
                    ClearScreen();
                    game.resetTrail();
                    game.ShowBoard(Zombies);
                    cout << "Alien reached border, trail reset" << endl;
                    turnObject(Zombies);
                    break;
                }
                else if (posY == (game.getBoardY() - 1))
                {
                    ClearScreen();
                    game.resetTrail();
                    game.ShowBoard(Zombies);
                    cout << "Alien hits border, position unchanged" << endl;
                    turnObject(Zombies);
                    break;
                }
                else
                {
                    game.moveAlien(posY, posX, movedir);
                    ClearScreen();
                    game.ShowBoard(Zombies);
                    Pause();
                    goto turn;
                }
            }
            else if (commanddir == '3')
            {
                movedir = '<';
                if (posX == 1)
                {
                    game.moveAlien(posY, posX, movedir);
                    ClearScreen();
                    game.resetTrail();
                    game.ShowBoard(Zombies);
                    cout << "Alien reached border, trail reset" << endl;
                    turnObject(Zombies);
                    break;
                }
                else if (posX == 0)
                {
                    ClearScreen();
                    game.resetTrail();
                    game.ShowBoard(Zombies);
                    cout << "Alien hits border, position unchanged" << endl;
                    turnObject(Zombies);
                    break;
                }
                else
                {
                    game.moveAlien(posY, posX, movedir);
                    ClearScreen();
                    game.ShowBoard(Zombies);
                    Pause();
                    goto turn;
                }
            }
            else if (commanddir == '4')
            {
                movedir = '>';
                if (posX == (game.getBoardX() - 2))
                {
                    game.moveAlien(posY, posX, movedir);
                    ClearScreen();
                    game.resetTrail();
                    game.ShowBoard(Zombies);
                    cout << "Alien reached border, trail reset" << endl;
                    turnObject(Zombies);
                    break;
                }
                else if (posX == (game.getBoardX() - 1))
                {
                    ClearScreen();
                    game.resetTrail();
                    game.ShowBoard(Zombies);
                    cout << "Alien hits border, position unchanged" << endl;
                    turnObject(Zombies);
                    break;
                }
                else
                {
                    game.moveAlien(posY, posX, movedir);
                    ClearScreen();
                    game.ShowBoard(Zombies);
                    Pause();
                    goto turn;
                }
            }
        }
        else if (commanddir == '5')
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

void turnObject(vector<GameObject> &Zombies)
{
    cout << "npc's turn start" << endl;
    Pause();
    cout << "npc's turn finished" << endl;
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
         << "2. Number of zombies: " << numZombie
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
                int updateZomb;
                ClearScreen();
                Lines();
                cout << "Update number of zombies" << endl;
                Lines();
                while (true)
                {
                    cout << "Enter number of zombies(minimum 2, maximum 9): ";
                    cin >> updateZomb;
                    if (updateZomb < 2 || updateZomb > 9)
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
                                numZombie = updateZomb;
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