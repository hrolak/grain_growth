#ifndef GLOBALS_H
#define GLOBALS_H

#define DIMENSION 3
#define BORDER 2

using namespace std;

typedef enum e_neighbourhoodType
{
    neumann = 0,
    moore   = 1
} neighbourhoodType;

typedef enum e_boundaryCondition
{
    absorbing = 0,
    periodic  = 1
} boundaryCondition;

typedef enum e_simulationType
{
    cellularAutomate = 0,
    monteCarlo
} simulationType;

#endif