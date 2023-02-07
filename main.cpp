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

// Declare functions here
void MenuDisplay();
void BoardSettings();
void PlayConfirmation();
void turnAlien();
void inGameStats();

// Classes and methods

class Board
{
private:
    vector<vector<char>> map_;
    int BoardX_, BoardY_;

public:
    Board(int BoardX = 7, int BoardY = 7);
    void init(int BoardX, int BoardY);
    void ShowBoard() const;
    void setAlien();
    int initAlien();
    void boardUpdate(int BoardY, int BoardX);
    int getBoardX() const;
    int getBoardY() const;
    vector<int> posAlien();
    void moveAlien(int posY, int posX, char movedir);
};

class GameObj
{
public:
    void turnObj();
    vector<vector<int>> initZombies(int numZomb);
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

void Board::ShowBoard() const
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
    cout << endl;
    inGameStats();
}

void Board::setAlien()
{
    map_[BoardY_ / 2][BoardX_ / 2] = 'A';
}

int Board::initAlien()
{
    int lb = 150, ub = 200;
    int rn;
    for (int i = 0; i < 1;)
    {
        rn = (rand() % (ub - lb + 1)) + lb;
        if (rn % 10 == 0)
        {
            i++;
        }
    }

    return rn;
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

void GameObj::turnObj()
{
    cout << "npc's turn start" << endl;
    Pause();
    cout << "npc's turn finished" << endl;
    Pause();
    ClearScreen();
    turnAlien();
}

vector<vector<int>> GameObj::initZombies(int numZomb)
{
    int lb_life = 150, ub_life = 200;
    int lb_atk = 5, ub_atk = 15;
    int lb_range = 1, ub_range = 4;
    vector<vector<int>> zombAttr;
    zombAttr.resize(numZomb);
    for (int r = 0; r < numZomb; ++r)
    {
        zombAttr[r].resize(3);
    }
    for (int i = 0; i < numZomb;)
    {
        for (int j = 0; j < 1;)
        {
            int rn_life = (rand() % (ub_life - lb_life + 1)) + lb_life;
            if (rn_life % 10 == 0)
            {
                zombAttr[i][0] = rn_life;
                j++;
            }
        }
        for (int k = 0; k < 1;)
        {
            int rn_atk = (rand() % (ub_atk - lb_atk + 1)) + lb_atk;
            if (rn_atk % 5 == 0)
            {
                zombAttr[i][1] = rn_atk;
                k++;
            }
        }
        for (int h = 0; h < 1; h++)
        {
            int rn_range = (rand() % (ub_range - lb_range + 1)) + lb_range;
            zombAttr[i][2] = rn_range;
        }
        i++;
    }
    return zombAttr;
}
// ######################### functions ###########################

Board game;
GameObj npc;
int numZomb = 2;
void inGameStats()
{
    vector<vector<int>> zombAttr = npc.initZombies(numZomb);
    cout << "Alien       :  Life " << game.initAlien() << ".  Attack 0" << endl;
    for (int i = 0; i < zombAttr.size(); i++)
    {
        cout << "Zombie" << i + 1 << "    :  Life " << zombAttr[i][0] << ".  Attack   " << zombAttr[i][1] << "Range :  " << zombAttr[i][2] << endl;
    }
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
                game.setAlien();
                turnAlien();
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

void turnAlien()
{
    char commanddir;
    char movedir;
    while (true)
    {
        ClearScreen();
        game.ShowBoard();
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
                    game.ShowBoard();
                    cout << "Alien reached border" << endl;
                    npc.turnObj();
                    break;
                }
                else if (posY == 0)
                {
                    ClearScreen();
                    game.ShowBoard();
                    cout << "Alien hits border, position unchanged" << endl;
                    npc.turnObj();
                    break;
                }
                else
                {
                    game.moveAlien(posY, posX, movedir);
                    ClearScreen();
                    turnAlien();
                    break;
                }
            }
            else if (commanddir == '2')
            {
                movedir = 'v';
                if (posY == (game.getBoardY() - 2))
                {
                    game.moveAlien(posY, posX, movedir);
                    ClearScreen();
                    game.ShowBoard();
                    cout << "Alien reached border" << endl;
                    npc.turnObj();
                    break;
                }
                else if (posY == (game.getBoardY() - 1))
                {
                    ClearScreen();
                    game.ShowBoard();
                    cout << "Alien hits border, position unchanged" << endl;
                    npc.turnObj();
                    break;
                }
                else
                {
                    game.moveAlien(posY, posX, movedir);
                    ClearScreen();
                    turnAlien();
                    break;
                }
            }
            else if (commanddir == '3')
            {
                movedir = '<';
                if (posX == 1)
                {
                    game.moveAlien(posY, posX, movedir);
                    ClearScreen();
                    game.ShowBoard();
                    cout << "Alien reached border" << endl;
                    npc.turnObj();
                    break;
                }
                else if (posX == 0)
                {
                    ClearScreen();
                    game.ShowBoard();
                    cout << "Alien hits border, position unchanged" << endl;
                    npc.turnObj();
                    break;
                }
                else
                {
                    game.moveAlien(posY, posX, movedir);
                    ClearScreen();
                    turnAlien();
                    break;
                }
            }
            else if (commanddir == '4')
            {
                movedir = '>';
                if (posX == (game.getBoardX() - 2))
                {
                    game.moveAlien(posY, posX, movedir);
                    ClearScreen();
                    game.ShowBoard();
                    cout << "Alien reached border" << endl;
                    npc.turnObj();
                    break;
                }
                else if (posX == (game.getBoardX() - 1))
                {
                    ClearScreen();
                    game.ShowBoard();
                    cout << "Alien hits border, position unchanged" << endl;
                    npc.turnObj();
                    break;
                }
                else
                {
                    game.moveAlien(posY, posX, movedir);
                    ClearScreen();
                    turnAlien();
                    break;
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
            Lines();
            cout << "Command not found, please try again." << endl;
        }
    }
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
         << "2. Number of zombies: " << numZomb
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
                cout << "Enter number: ";
                cin >> updateZomb;
                numZomb = updateZomb;
                ClearScreen();
                goto first;
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