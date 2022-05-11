#pragma once
#include <thread>

#ifdef __linux__
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/types.h"
#include "sys/sysinfo.h"
#endif

namespace Monitoring
{
class Monitoring
{
private:
    unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle; //CPU
    long long physMemUsed, totalPhysMem;                                             //메모리
    struct sysinfo memInfo;

    double cpuUsage, memoryUsage;
    bool running;
    const int interval = 100; //ms
    std::thread local_thread;

public:
    Monitoring();
    ~Monitoring();

    void Update();
    double calCurrentCpu();
    double calCurrentMemory();
    inline double getCpuUsage() { return cpuUsage; }
    inline double getMemoryUsage() { return memoryUsage; }
};
} // namespace Monitoring

Monitoring::Monitoring* m_Monitoring();