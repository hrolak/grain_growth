#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include "Board.hpp"
#include "FileParser.hpp"
#include "globals.hpp"

class Simulation
{
private:
    const string path = "../"; 
    string inputFile;
    string outputFile;
    Board board;
    FileParser parser;
    double simulationTime;
    simulationType simulationT;
    double energyMC;
    int simulationStepsMC;

public:
    Simulation();
    Simulation(string _inputFile, string _outputFile);
    void prepare();
    void start();
    void writeDataToFile();
};

#endif