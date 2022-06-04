#pragma once
#include "DataIPC.h"
#include <mutex>

#ifndef _LOGGING_H_
#define _LOGGING_H_

typedef DataIO::IPCStruct::IPCDataType IPCType;
typedef DataIO::IPCStruct::IPCDataStruct IPCStruct;

class Logging{
private:
DataIO::IPCSharedMemory ipc;
DataIO::IPCStruct::VectorContainer vector_container;
std::mutex m_mutex;

std::vector<IPCStruct> m_container;
inline bool exist(unsigned int index) {return ( index < m_container.size() ) ? true : false; }
bool is_run = false;

public:
Logging() : ipc(DataIO::IPC_MODE::SENDER, DEFULAT_IPC_KEY) {}
~Logging();

void createSet(std::string name, IPCType type);
void addData(unsigned int index, FLOAT value);
void clear(unsigned int index);

void sendIPC();
void run();

};


#endif

