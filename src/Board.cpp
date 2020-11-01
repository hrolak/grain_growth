#include <iostream>
#include <time.h>
#include <random>

#ifdef DEBUG
#include <chrono>
#include <thread>
#include <windows.h>
const WORD colors[] =
		{
		0x0F, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
		0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
		};
#endif

#include "Board.hpp"

Cell::Cell()
{
    id     = 0;
    color  = 0;
    state  = false;
    energy = 0;
}

Cell::Cell(int _id, int _color, bool _state, double _energy)
{
    id     = _id;
    color  = _color;
    state  = _state;
    energy = _energy;
}

int Cell::getId()
{
    return id;
}

bool Cell::getState()
{
    return state;
}

void Cell::printPretty()
{
    HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );
    SetConsoleTextAttribute( hstdout, colors[id] );
    cout << "  ";
    SetConsoleTextAttribute( hstdout, 0x0F );
}

Board::Board()
{

}

Board::Board(int *_dimensions, int _grainsAmount,
          neighbourhoodType _neighbourhood, boundaryCondition _boundary)
{
    freeFields = 1;
    for(int i=0; i<3 ; ++i)
    {
        dimensions[i] = _dimensions[i];
        freeFields *= dimensions[i];
    }
    grainsAmount  = _grainsAmount;
    neighbourhood = _neighbourhood;
    boundary      = _boundary;
    vector<vector<vector<Cell>>> _board = vector<vector<vector<Cell>>>(dimensions[0] + BORDER,
                                             vector<vector<Cell>>(dimensions[1] + BORDER,
                                             vector<Cell>(dimensions[2] + BORDER)));
    board = _board;
    vector<vector<vector<Cell>>> _backBoard = vector<vector<vector<Cell>>>(dimensions[0] + BORDER,
                                             vector<vector<Cell>>(dimensions[1] + BORDER,
                                             vector<Cell>(dimensions[2] + BORDER)));
    backBoard = _backBoard;
    
}

void Board::setBoard(int _dimensions[DIMENSION], int _grainsAmount,
          neighbourhoodType _neighbourhood, boundaryCondition _boundary)
{
    freeFields = 1;
    for(int i=0; i<3 ; ++i)
    {
        dimensions[i] = _dimensions[i];
        freeFields *= dimensions[i];
    }
    grainsAmount  = _grainsAmount;
    neighbourhood = _neighbourhood;
    boundary      = _boundary;
    vector<vector<vector<Cell>>> _board = vector<vector<vector<Cell>>>(dimensions[0] + BORDER,
                                             vector<vector<Cell>>(dimensions[1] + BORDER,
                                             vector<Cell>(dimensions[2] + BORDER)));
    board = _board;
    vector<vector<vector<Cell>>> _backBoard = vector<vector<vector<Cell>>>(dimensions[0] + BORDER,
                                             vector<vector<Cell>>(dimensions[1] + BORDER,
                                             vector<Cell>(dimensions[2] + BORDER)));
    backBoard = _backBoard;
}
void Board::printBoard()
{
    #ifdef DEBUG
    for(int j = 1; j <= dimensions[1]; ++j)
    {
        for(int i = 1; i <= dimensions[0]; ++i)
        {
            for(int k = 1; k <= dimensions[2]; ++k)
            {
                board[i][j][k].printPretty();
            }
            cout << '\t';
        }
        cout << '\n';
    }
    cout << "Free left: " << freeFields <<'\n' << '\n';
    std::chrono::milliseconds timespan(100);
    //std::this_thread::sleep_for(timespan);
    #endif
}

void Board::generate()
{
    srand( time(NULL) );
    int currentGrains = 0;
    while(currentGrains < grainsAmount)
    {
        int coor[3] = { (rand() % dimensions[0]) + 1,
                        (rand() % dimensions[1]) + 1,
                        (rand() % dimensions[2]) + 1};

        if(backBoard[coor[0]][coor[1]][coor[2]].getState() == false)
        {
            Cell cell(currentGrains+1, currentGrains+1, true, 0.0);
            backBoard[coor[0]][coor[1]][coor[2]] = cell;
            currentGrains++;
        }
    }
    freeFields -= grainsAmount;
    propagate();
}

void Board::propagate()
{
    freeFields = dimensions[0]*dimensions[1]*dimensions[2];
    for(int i = 1; i <= dimensions[0]; ++i)
        for(int j = 1; j <= dimensions[1]; ++j)
            for(int k = 1; k <= dimensions[2]; ++k)
            {
                board[i][j][k] = backBoard[i][j][k];
                if(board[i][j][k].getState()) freeFields--;
            }
}

vector<vector<int>> Board::getIndexes(int i, int j, int k)
{
    vector<vector<int>> indexes = { {i-1, j, k}, {i+1, j, k}, 
                                      {i, j-1, k}, {i, j+1, k}, 
                                      {i, j, k-1}, {i, j, k+1} };
    if(neighbourhood == moore)
    {
        vector<vector<int>> mooreIndexes = { {i-1, j-1, k}, {i+1, j-1, k},
                                               {i-1, j+1, k}, {i+1, j+1, k},
                                               {i-1, j, k-1}, {i+1, j, k-1},
                                               {i-1, j, k+1}, {i+1, j, k+1}, 
                                               {i, j-1, k-1}, {i, j+1, k-1}, 
                                               {i, j-1, k+1}, {i, j+1, k+1},
                                               {i-1, j-1, k-1}, {i-1, j-1, k+1},
                                               {i-1, j+1, k-1}, {i-1, j+1, k+1},
                                               {i+1, j-1, k-1}, {i+1, j-1, k+1},
                                               {i+1, j+1, k-1}, {i+1, j+1, k+1} };
        indexes.insert(indexes.end(), mooreIndexes.begin(), mooreIndexes.end());
    }
    if(boundary == periodic)
    {
        for(int i = 0; i<indexes.size(); ++i)
        {
            for(int j = 0 ; j<DIMENSION; ++j)
            {
                indexes[i][j] = (indexes[i][j]<1)             ? dimensions[j] : indexes[i][j];
                indexes[i][j] = (indexes[i][j]>dimensions[j]) ? 1             : indexes[i][j];
            }
        }
    }
    return indexes;
}

void Board::fillSurroundingCells(int i, int j, int k)
{
    if(board[i][j][k].getState())
    {
        vector<vector<int>> ind = getIndexes(i , j , k );
        for(int l = 0 ; l<ind.size(); ++l)
        {
            if(backBoard[ind[l][0]][ind[l][1]][ind[l][2]].getState() == false)
            {
                backBoard[ind[l][0]][ind[l][1]][ind[l][2]] = board[i][j][k];
            }
            else if(board[ind[l][0]][ind[l][1]][ind[l][2]].getState() == false)
            {
                if(backBoard[ind[l][0]][ind[l][1]][ind[l][2]].getId() < board[i][j][k].getId())
                {
                    backBoard[ind[l][0]][ind[l][1]][ind[l][2]] = board[i][j][k];
                }
            }
        }
    }
}

void Board::iterate()
{
    for(int i = 1; i <= dimensions[0]; ++i)
    {
        for(int j = 1; j <= dimensions[1]; ++j)
        {
            for(int k = 1; k <= dimensions[2]; ++k)
            {
                fillSurroundingCells(i, j, k);
            }
        }
    }
    propagate();
}

void Board::startSimulation()
{
    generate();
    printBoard();
    while(freeFields > 0)
    {
        iterate();
        system("cls");
        printBoard();
    }
}
