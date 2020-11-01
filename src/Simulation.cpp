#include <chrono>

#include "Simulation.hpp"

Simulation::Simulation()
{

}

Simulation::Simulation(string _inputFile, string _outputFile)
{
    inputFile = path + _inputFile;
    outputFile = path + _outputFile;
}

void Simulation::prepare()
{
    parser.setPaths(inputFile, outputFile);
    parser.readFile();
    board.setBoard(parser.getDimensions(), 
                   parser.getGrainsAmount(),
                   parser.getNeighbourhoodType(),
                   parser.getBoundaryCondition());
}

void Simulation::start()
{
    auto start = std::chrono::high_resolution_clock::now();
    board.startSimulation();
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    simulationTime = elapsed.count();
}

void Simulation::writeDataToFile()
{
    parser.writeTime(simulationTime);
    parser.writeBoard(board);
}
