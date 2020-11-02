#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <string>
#include "Board.hpp"
#include "globals.hpp"

class FileParser
{
private:
    string inputFile;
    string outputFile;
    string simulationT;
    int dimensions[DIMENSION];
    int grainsAmount;
    string neighbourhoodT;
    string boundaryC;
    int numberOfSimulationSteps;
    double energy;
public:
    FileParser();
    FileParser(string _inputFile, string _outputFile);
    string getFilename();
    simulationType getSimulationType();
    int* getDimensions();
    int getGrainsAmount();
    neighbourhoodType getNeighbourhoodType();
    boundaryCondition getBoundaryCondition();
    int getNumberOfSimulationSteps();
    double getEnergy();
    void readFile();
    void writeTime(double time);
    void writeBoard(Board board);
    void setPaths(string _inputFile, string _outputFile);
};

#endif