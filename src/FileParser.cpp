#include <string>
#include <iostream>
#include <fstream>

#include "FileParser.hpp"

FileParser::FileParser()
{

}

FileParser::FileParser(string _inputFile, string _outputFile)
{
    inputFile =  _inputFile;
    outputFile = _outputFile;
}

simulationType FileParser::getSimulationType()
{
    if(simulationT == "CA")
        return cellularAutomate;
    return monteCarlo;
}

int* FileParser::getDimensions()
{
    return dimensions;
}

int FileParser::getGrainsAmount()
{
    return grainsAmount;
}

neighbourhoodType FileParser::getNeighbourhoodType()
{
    if(neighbourhoodT == "moore")
        return moore;
    return neumann;
}

boundaryCondition FileParser::getBoundaryCondition()
{
    if(boundaryC == "periodic")
        return periodic;
    return absorbing;
}

int FileParser::getNumberOfSimulationSteps()
{
    return numberOfSimulationSteps;
}

double FileParser::getEnergy()
{
    return energy;
}

void FileParser::readFile()
{
    ifstream inFile(inputFile);

    if( inFile.is_open())
    {
        inFile >> simulationT;
        inFile >> dimensions[0] >> dimensions[1] >> dimensions[2];
        inFile >> grainsAmount;
        inFile >> neighbourhoodT;
        inFile >> boundaryC;
        inFile >> numberOfSimulationSteps;
        inFile >> energy;
        inFile.close();
    }
}

void FileParser::writeTime(double time)
{
    ofstream outFile(outputFile);

    if( outFile.is_open())
    {
        outFile << "Czas: " << time << "s" << endl << endl;
        outFile.close();
    }
}

void FileParser::writeBoard(Board board)
{
    ofstream outFile(outputFile, ios::app);

    if( outFile.is_open())
    {
        outFile << "Struktura:\nidx\tidy\tidz\tidCell" << endl;
        outFile << board;
        outFile.close();
    }
}

void FileParser::setPaths(string _inputFile, string _outputFile)
{
    inputFile =  _inputFile;
    outputFile = _outputFile;
}
