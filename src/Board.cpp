#include <algorithm>
#include <iostream>
#include <time.h>
#include <map>
#include <chrono>
#include <thread>
#include <windows.h>
const WORD colors[] =
		{
		0x0F, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
		0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
		};


#include "Board.hpp"

extern bool printGraph;

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

void Cell::setCell(int _id, int _color, bool _state, double _energy)
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
    numOfCells = dimensions[0]*dimensions[1]*dimensions[2];
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
    numOfCells = dimensions[0]*dimensions[1]*dimensions[2];
}

void Board::printBoard()
{
    if(printGraph)
    {
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
    }
}

void Board::generate()
{
    srand( 0 );
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
   srand( time(NULL) );
    for(int i = 0; i < numOfCells; ++i) indexesMC.push_back(i); // generujemy liczby od 0 do numOfCells
    random_shuffle(indexesMC.begin(), indexesMC.end()); // mieszamy te liczby (bedą w losowej kolejności)
    // liczby te odpowiadają losowym komórkom z tablicy
}

void Board::propagate()
{
    freeFields = numOfCells;
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

vector<int> Board::getMCIndex(int i) //konwersja indexu z tablicy indexesMC na współrzedne i j, k
{
    vector<int> indexes;
    indexes.push_back( (i / (dimensions[1]*dimensions[2])) + 1);
    indexes.push_back( ((i % (dimensions[1]*dimensions[2])) / dimensions[2]) + 1 );
    indexes.push_back( ((i % (dimensions[1]*dimensions[2])) % dimensions[2]) + 1 );
    return indexes;
}

void Board::fillSurroundingCells(int i, int j, int k)
{
    if(board[i][j][k].getState() == false)
    {
        vector<vector<int>> ind = getIndexes(i , j , k );
        map<int, int> neighbours;
        for(int l = 0 ; l<ind.size(); ++l)
        {
            Cell neighbour = board[ind[l][0]][ind[l][1]][ind[l][2]];
            if(neighbour.getState())
            {
                if(neighbours.find(neighbour.getId()) == neighbours.end())
                    neighbours[neighbour.getId()] = 1;
                else
                    neighbours[neighbour.getId()]++;
            }
        }
        if(!neighbours.empty())
        {
            vector<int> winners;
            map<int, int>::iterator it;
            int maxId = 0;
            int maxIdVal = 0;
            for ( it = neighbours.begin(); it != neighbours.end(); it++ )
            {
                if(it->second > maxIdVal)
                {
                    maxId = it->first;
                    maxIdVal = it->second;
                    winners.clear();
                    winners.push_back(it->first);
                }
                else if(it->second == maxIdVal)
                {
                    winners.push_back(it->first);
                }
            }
            int newId = winners[ rand() % winners.size() ];
            backBoard[i][j][k].setCell(newId, newId, true, 0.0);
        }
    }
}

void Board::iterateCA()
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

void Board::iterateMC(double energy)
{

    for(int i=0;i<numOfCells;++i)
    {
        vector<int> ind = getMCIndex(indexesMC[i]);
        vector<vector<int>> bor = getIndexes(ind[0], ind[1], ind[2]);
        int nextCellId = 0;
        Cell nextCell;
        int rndInx = rand() % bor.size();

        while(nextCellId == 0) //znalezienie niepustej sasiedniej komórki (pusta będzie w przypadku komórki na granicy i typu absrobic)
        {
            nextCell = board[ bor[rndInx][0] ][ bor[rndInx][1] ][ bor[rndInx][2] ];
            nextCellId = nextCell.getId();
            rndInx = (rndInx+1) % bor.size();
        }

        Cell currCell = board[ ind[0] ][ ind[1] ][ ind[2] ];
        int currCellId = currCell.getId();
        double currEnergy = 0;
        double nextEnergy = 0;
        for(int j = 0 ; j<bor.size(); ++j)
        {
            int brdCellId = board[ bor[j][0] ][ bor[j][1] ][ bor[j][2] ].getId();
            if(brdCellId != 0)
            {
                if(brdCellId != currCellId) currEnergy+=energy;
                if(brdCellId != nextCellId) nextEnergy+=energy;
            }

        }
        double deltaEnergy = nextEnergy - currEnergy;
        double prob = exp(-1.0*deltaEnergy/kt);
        bernoulli_distribution d(prob);
        if(deltaEnergy<=0 || d(rand_engine))
        {
            board[ ind[0] ][ ind[1] ][ ind[2] ] = nextCell;
        }
    }
    random_shuffle(indexesMC.begin(), indexesMC.end());
}

void Board::startCASimulation()
{
    generate();
    printBoard();
    while(freeFields > 0)
    {
        iterateCA();
        printBoard();
    }
}

void Board::startMCSimulation(int iterations, double energy)
{
    for(int i = 0; i<iterations; ++i)
    {
        iterateMC(energy);
        printBoard();
    }
}