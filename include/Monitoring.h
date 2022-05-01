#pragma once

#include <thread>

#ifdef __linux__
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/types.h"
#include "sys/sysinfo.h"
#endif

namespace Monitoring {
    double calUsageCPU();
    double calUsageRAM();
    class Monitoring {
        private:
        unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle; //CPU
        long long physMemUsed, totalPhysMem; //메모리

        double memoryUsage, cpuUsage;
        bool running = false;
        std::thread local_thread;
        
        public:
        Monitoring();
        ~Monitoring();
        
        void Update();
        double calCurrentCpu();
        double calCurrentMemory();
        inline double getMemoryUsage() {return memoryUsage;}
        inline double getCpuUsage() {return cpuUsage;}
    };
}

