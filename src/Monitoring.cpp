#include "Monitoring.h"
namespace Monitoring
{
Monitoring::Monitoring() : running(true)
{
#ifdef __linux__
    FILE *file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow, &lastTotalSys, &lastTotalIdle);
    fclose(file);
#endif
    local_thread = std::thread([&](){Update();});
}

Monitoring::~Monitoring()
{
    running = false;
    local_thread.join();
}

void Monitoring::Update()
{
    while (running)
    {
        memoryUsage = calCurrentMemory();
        cpuUsage = calCurrentCpu();
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
    }
}

double Monitoring::calCurrentCpu()
{
    double percent;

    #ifdef __linux__
    FILE *file;
    unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

    file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow, &totalSys, &totalIdle);
    fclose(file);

    if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow || totalSys < lastTotalSys || totalIdle < lastTotalIdle)
    {
        //오버플로우 detection
        percent = -1.0;
    }
    else
    {
        total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) + (totalSys - lastTotalSys);
        percent = total;
        total += (totalIdle - lastTotalIdle);
        percent /= total;
        percent *= 100;
    }

    lastTotalUser = totalUser;
    lastTotalUserLow = totalUserLow;
    lastTotalSys = totalSys;
    lastTotalIdle = totalIdle;
    #endif
    return percent;
}
double Monitoring::calCurrentMemory()
{
    double percent;

    #ifdef __linux__
    sysinfo (&memInfo);
    physMemUsed = (memInfo.totalram - memInfo.freeram);
    totalPhysMem = memInfo.totalram;
    percent = (double) physMemUsed / (double) totalPhysMem;
    #endif

    return percent;
}
} // namespace Monitoring

static Monitoring::Monitoring monitoring;
Monitoring::Monitoring* m_Monitoring() {
    return &monitoring;
}

