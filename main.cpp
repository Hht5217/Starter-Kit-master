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
void PlayGame();

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
    void initAlien();
    void boardUpdate(int BoardX, int BoardY);
    int getBoardX() const;
    int getBoardY() const;
    // int posAlien() const;
    // void moveUp();
    // void moveDown();
    // void moveLeft();
    // void moveRight();
    void moveAlien(char movedir);
};

class GameObj
{
};

void Lines()
{
    cout << string(40, '#') << endl;
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

void Board::boardUpdate(int NewX, int NewY)
{
    BoardX_ = NewX;
    BoardY_ = NewY;
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
}

void Board::initAlien()
{
    map_[BoardY_ / 2][BoardX_ / 2] = 'A';
}

// int Board::posAlien() const
// {

// }

// void Board::moveUp()
// {
// }

// void Board::moveDown()
// {
// }

// void Board::moveLeft()
// {
// }

// void Board::moveRight()
// {
// }

void Board::moveAlien(char movedir)
{
    int posX, posY;
    for (int i = 0; i < BoardY_; ++i)
    {
        for (int j = 0; j < BoardX_; ++j)
        {
            if (map_[i][j] == 'A')
            {
                posX = j;
                posY = i;
                goto move;
            }
        }
    }
move:
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
// ################################################################

Board game;

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
                PlayGame();
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

void PlayGame()
{
    char commanddir;
    char movedir;
    game.ShowBoard();
    while (true)
    {
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
        if (commanddir == '1' || commanddir == '2' || commanddir == '3' || commanddir == '4')
        {
            if (commanddir == '1')
            {
                movedir = '^';
            }
            else if (commanddir == '2')
            {
                movedir = 'v';
            }
            else if (commanddir == '3')
            {
                movedir = '<';
            }
            else if (commanddir == '4')
            {
                movedir = '>';
            }
            game.moveAlien(movedir);
            ClearScreen();
            PlayGame();
            break;
        }
        else if (commanddir == '5')
        {
            MenuDisplay();
            break;
        }
        else
        {
            cout << "Command not found, please try again." << endl;
        }
    }
}

void BoardSettings()
{
    int numX;
    int numY;
    Lines();
    cout << "Current board dimension: " << game.getBoardX() << "x" << game.getBoardY() << endl;
    Lines();
    cout << "1. Change board size"
         << "\n"
         << "2. Back" << endl;

    char menu2;
    while (true)
    {
        cout << "Please input a command(number): ";
        cin >> menu2;
        if (menu2 == '1' || menu2 == '2')
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
                    cout << "Enter column/width => ";
                    cin >> numX;
                    cout << "Enter row/height => ";
                    cin >> numY;
                    if (numX % 2 == 0 || numY % 2 == 0)
                    {
                        cout << "Columns or rows entered is not an odd number, please try again." << endl;
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
                                game.boardUpdate(numX, numY);
                                ClearScreen();
                                BoardSettings();
                                break;
                            }
                            else if (confirmation == 'n')
                            {
                                ClearScreen();
                                BoardSettings();
                                break;
                            }
                            else
                            {
                                cout << "Command not found, please try again." << endl;
                                Lines();
                            }
                        }
                    }
                    break;
                }
            }
            else if (menu2 == '2')
            {
                MenuDisplay();
                break;
            }
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