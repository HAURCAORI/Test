#include "Logging.h"
#include <thread>
#include <iostream>
Logging::~Logging() {
    is_run = false;
}

void Logging::createSet(std::string name, IPCType type) {
    switch(type) {
        case IPCType::SINGLE_FLOAT : {
        std::vector<FLOAT> vec;
        auto p = vector_container.add(name, std::move(vec));
        m_container.push_back(IPCStruct(name,type, p, p->size(),sizeof(FLOAT)));
        }
        break;

        default:
        break;
    }
}
void Logging::addData(unsigned int index, FLOAT value) {
    if(!exist(index)) return;
    std::vector<FLOAT>* vec = static_cast<std::vector<FLOAT>*>(m_container[index].getData());
    vec->emplace_back(value);
    ++(*(m_container[index].sizePtr()));
}
void Logging::clear(unsigned int index) {
    if(!exist(index)) return;
    std::vector<FLOAT>* vec = static_cast<std::vector<FLOAT>*>(m_container[index].getData());
    vec->clear();
    *(m_container[index].sizePtr()) = 0;
}

void Logging::sendIPC() {
    while(is_run){
        DataIO::IPCData d = DataIO::IPCStruct::encodeIPCData(m_container);
        //ipc.sendData(d,0);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void Logging::run() {
    if(ipc.valid() && !is_run) {
        is_run = true;
        std::thread( [this]() { sendIPC(); }).detach();
    }
}
