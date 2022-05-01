#pragma once
#include "NStruct.h"
#include <iostream>
#include <time.h>
#include "Calculate.h"


//함수 처리부 (Process)
void Load(Neuron (*target)[SectorSize][SectorSize],int i, int j, int k,  Signal* signal);

//Threading
void TLoad(Neuron (*target)[SectorSize][SectorSize], int i, int j, int k, Signal *signal);