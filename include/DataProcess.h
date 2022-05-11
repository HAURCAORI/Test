#pragma once

#include "Calculate.h"
#include "DataIO.h"

#include <iostream>
#include <time.h>

#include <chrono>
#define BEGIN_CHRONO std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
#define END_CHRONO std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count() << "[ms]" << std::endl;

DataIO::IOManager* m_IOManager();

void TLoad(DataStruct& ds, Signal *signal, int i, int j, int k);
void Load(DataStruct& ds, Signal *signal, int i, int j, int k);