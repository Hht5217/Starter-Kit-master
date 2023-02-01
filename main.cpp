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
void MapSettings();
void PlayConfirmation();

class Board
{
private:
    vector<vector<char>> map_;
    int BoardX_, BoardY_;

public:
    Board()
    {
        int BoardX = 7;
        int BoardY = 7;
        init(BoardX, BoardY);
    }
    void init(int BoardX, int BoardY);
    void ShowBoard() const;
    void SetAlien();
    void mapUpdate(int BoardX, int BoardY);
    int getColumn() const;
    int getRow() const;
};

void Lines()
{
    cout << string(40, '#') << endl;
}

void Board::mapUpdate(int NewX, int NewY)
{
    NewX = BoardX_;
    NewY = BoardY_;
}

int Board::getColumn() const
{
    return BoardX_;
}
int Board::getRow() const
{
    return BoardY_;
}

void Board::init(int BoardX, int BoardY)
{
    char objects[] = {' ', ' '};
    int noofObjects = 2;
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
    cout << "(Return to menu)";
    Pause();
    MenuDisplay();
}

void Board::SetAlien()
{
    map_[BoardY_ / 2][BoardX_ / 2] = 'A';
}

void PlayConfirmation()
{
    Lines();
    cout << "Play a game" << endl;
    Lines();
    cout << "1. Start the game"
         << "\n"
         << "2. View or change map settings"
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
                Board play;
                ClearScreen();
                play.SetAlien();
                play.ShowBoard();
                break;
            }
            else if (playmenu == '2')
            {
                ClearScreen();
                MapSettings();
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

int zomnum = 1;

void MapSettings()
{
    Board mapset;
    int numRows = mapset.getRow();
    int numColumns = mapset.getColumn();
    Lines();
    cout << "Change the game Settings" << endl;
    Lines();
    cout << "1. Map size: "
         << numRows << "x" << numColumns
         << "\n"
         << "2. Number of Zombies: " << zomnum
         << "\n"
         << "3. Back" << endl;

    char menu2;
    while (true)
    {
        cout << "Please input a command(number): ";
        cin >> menu2;
        if (menu2 == '1' || menu2 == '2' || menu2 == '3')
        {
            if (menu2 == '1')
            {
                ClearScreen();
                Lines();
                cout << "Changing map size" << endl;
                Lines();
                while (true)
                {
                    cout << "Enter rows => ";
                    cin >> numRows;
                    cout << "Enter columns => ";
                    cin >> numColumns;
                    if (numRows % 2 == 0 || numColumns % 2 == 0)
                    {
                        cout << "Rows / Columns entered is not an odd number, please try again." << endl;
                        Lines();
                    }
                    else
                    {
                        break;
                    }
                }
                mapset.mapUpdate(numColumns, numRows);
                ClearScreen();
                MapSettings();
                break;
            }
            else if (menu2 == '2')
            {
                int numofzom;
                cout << "Enter number of Zombies:";
                cin >> numofzom;
                char confirmation;
                cout << "Are you sure? y/n: ";
                cin >> confirmation;
                if (confirmation == 'y')
                {
                    zomnum = numofzom;
                    ClearScreen();
                    MapSettings();
                    break;
                }
                else if (confirmation == 'n')
                {
                    ClearScreen();
                    MapSettings();
                    break;
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

void MenuDisplay()
{
    ClearScreen();
    Lines();
    cout << "Welcome to Alien Path Simplified!" << endl;
    Lines();
    cout << "1. Play"
         << "\n"
         << "2. Map Settings"
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
                // add back button
            }
            else if (menu == '2')
            {
                ClearScreen();
                MapSettings();
                break;
                // add back button
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

int main()
{
    MenuDisplay();

    return 0;
}