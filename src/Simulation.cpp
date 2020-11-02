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
    simulationT = parser.getSimulationType();
    energyMC = parser.getEnergy();
    simulationStepsMC = parser.getNumberOfSimulationSteps();
    board.setBoard(parser.getDimensions(), 
                   parser.getGrainsAmount(),
                   parser.getNeighbourhoodType(),
                   parser.getBoundaryCondition());
}

void Simulation::start()
{
    auto start = std::chrono::high_resolution_clock::now();
    board.startCASimulation();
    if(simulationT == monteCarlo)
        board.startMCSimulation(simulationStepsMC, energyMC);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    simulationTime = elapsed.count();
}

void Simulation::writeDataToFile()
{
    parser.writeTime(simulationTime);
    parser.writeBoard(board);
}
