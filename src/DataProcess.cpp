#include "DataProcess.h"
#include <math.h>
#include <chrono>
#define BEGIN_CHRONO std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
#define END_CHRONO std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count() << "[ms]" << std::endl;

bool Load(Neuron (*target)[SectorSize][SectorSize], int i, int j, int k, Signal *signal)
{
    if (i >= SectorLayer || j >= SectorSize || k >= SectorSize || i < 0 || j < 0 || k < 0)
    {
        std::cout << "end" << std::endl;
        return false;
    }
    printf("Load[%i,%i,%i]", i, j, k);

    if (((target[i][j][k].direction) & 1) == 1)
    {
        Load(target, i, j, k + 1, signal);
    }
    if (((target[i][j][k].direction >> 1) & 1) == 1)
    {
        Load(target, i, j, k - 1, signal);
    }
    if (((target[i][j][k].direction >> 2) & 1) == 1)
    {
        Load(target, i, j + 1, k, signal);
    }
    if (((target[i][j][k].direction >> 3) & 1) == 1)
    {
        Load(target, i, j - 1, k, signal);
    }
    if (((target[i][j][k].direction >> 4) & 1) == 1)
    {
        Load(target, i + 1, j, k, signal);
    }
    if (((target[i][j][k].direction >> 5) & 1) == 1)
    {
        Load(target, i - 1, j, k, signal);
    }

    return true;
}