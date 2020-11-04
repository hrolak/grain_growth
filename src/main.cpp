#include "Simulation.hpp"

bool printGraph = true;

int main(int argc, char *argv[])
{
    Simulation simulation("inputData", "outputData");
    simulation.prepare();
    simulation.start();
    simulation.writeDataToFile();
    cout << "Simulation has finished" << endl;

    return 0;
}
