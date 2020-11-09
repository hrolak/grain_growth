#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <iostream>
#include <random>

#include "globals.hpp"

class Cell
{
private:
    int id;
    int color;
    bool state;
    double energy;
public:
    Cell();
    Cell(int _id, int _color, bool _state, double _energy);
    void setCell(int _id, int _color, bool _state, double _energy);
    int getId();
    bool getState();
    void printPretty();
    friend ostream & operator<< (ostream &outputStream, Cell &cell);
};

class Board
{
public:
    double kt = -1.2;
    std::default_random_engine rand_engine;
    int dimensions[DIMENSION];
    int grainsAmount;
    neighbourhoodType neighbourhood;
    boundaryCondition boundary;
    vector<vector<vector<Cell>>> board;
    vector<vector<vector<Cell>>> backBoard;
    vector<int> indexesMC;
    int freeFields;
    int numOfCells;

    void generate();
    void printBoard();
    void fillSurroundingCells(int i, int j, int k);
    void iterateCA();
    void iterateMC(double energy);
    void propagate();
    vector<vector<int>> getIndexes(int i, int j, int k);
    vector<int> getMCIndex(int i);

public:
    Board();
    Board(int _dimensions[DIMENSION], int _grainsAmount,
          neighbourhoodType _neighbourhood, boundaryCondition _boundary);

    void startCASimulation();
    void startMCSimulation(int iterations, double energy);
    void setBoard(int _dimensions[DIMENSION], int _grainsAmount,
          neighbourhoodType _neighbourhood, boundaryCondition _boundary);
    friend ostream & operator<< (ostream &outputStream, Board &board);
};

inline ostream & operator<< (ostream &outputStream, Cell &cell)
{
    return outputStream << cell.id;
}

inline ostream & operator<< (ostream &outputStream, Board &board)
{
    for(int i = 1; i<=board.dimensions[0]; ++i)
    {
        for(int j = 1; j<=board.dimensions[1]; ++j)
        {
            for(int k = 1; k<=board.dimensions[2]; ++k)
            {
                outputStream << i << '\t' << j << '\t' << k << '\t' << board.board[i][j][k] << endl;
            }
        }
    }
    return outputStream;
}

#endif