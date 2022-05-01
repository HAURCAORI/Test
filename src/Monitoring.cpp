#include "Monitoring.h"
namespace Monitoring {
    void Monitoring::Update() {

    }
    Monitoring::Monitoring() : local_thread(Update) {
        #ifdef __linux__
        FILE* file = fopen("/proc/stat", "r");
        fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow, &lastTotalSys, &lastTotalIdle);
        fclose(file);
        #endif
    }
    Monitoring::~Monitoring() {
        local_thread.join();
    }
}