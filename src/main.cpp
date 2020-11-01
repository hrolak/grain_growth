#include "Simulation.hpp"

int main(int argc, char *argv[])
{
    Simulation simulation("inputData", "outputData");
    simulation.prepare();
    simulation.start();
    simulation.writeDataToFile();

    return 0;
}
