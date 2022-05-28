#pragma once
#include <stdio.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <cstring>

#include <sys/time.h>

#include "IPCStruct.h"

#ifndef _DATA_IPC_H
#define _DATA_IPC_H


#define SequenceNumber unsigned int
#define SharedFlag char

//SharedFlag
#define SEND_NULL 0x00
#define SEND_DATA 0x01
#define SEND_SUCCESS 0x02
#define SEND_ERROR 0x04
#define SEND_TIMEOUT 0x08
#define IPC_DESTROY 0X80

#define IPC_BUFFER_SIZE 6

namespace DataIO {
//DataIPC
class IPCData {
    private:
    char* m_data = nullptr;
    unsigned int m_size = 0;
    SharedFlag m_flag = 0x00;

    public:
    IPCData() {}
    IPCData(unsigned int size) : m_size(size) {
        alloc(size);
    }
    IPCData(const IPCData& rhs) {
        if(rhs.m_data != nullptr) {
            m_data = new char[rhs.m_size];
            memcpy(m_data,rhs.m_data,rhs.m_size);
            m_size = rhs.m_size;
            m_flag = rhs.m_flag;
        }
    }
    IPCData& operator=(const IPCData& rhs) {
        if(rhs.m_data != nullptr) {
            m_data = new char[rhs.m_size];
            memcpy(m_data,rhs.m_data,rhs.m_size);
            m_size = rhs.m_size;
            m_flag = rhs.m_flag;
        }
        return *this;
    }
    IPCData(IPCData&& rhs) : m_data(rhs.m_data), m_size(rhs.m_size) {
        rhs.m_data = nullptr;
        rhs.m_size = 0;
    }

    IPCData& operator=(IPCData&& rhs) noexcept {
        if(this != &rhs) {
            free();
            m_data = rhs.m_data;
            m_size = rhs.m_size;
            m_flag = rhs.m_flag;
            rhs.m_data = nullptr;
            rhs.m_size = 0;
        }
        return *this;
    }

    ~IPCData() {
        free();
    }
    
    void alloc(unsigned int sz) {
        free();
        m_data = new char[sz];
        m_size = sz;
    }

    void free() {
        if(m_data != nullptr) { delete[] m_data; }
        m_data = nullptr;
        m_size = 0;
    }

    inline char* getData() { return m_data; }
    inline unsigned int getSize() { return m_size;}
    void setData(char* rhs, unsigned int sz) {
        if(sz > m_size) sz = m_size;
        memcpy(m_data,rhs,sz);
    }

    void setFlag(SharedFlag flag) { m_flag = flag; }
    SharedFlag getFlag() { return m_flag; }
};

enum class IPC_MODE {
    SENDER,
    RECEIVER    
};

struct shared_data {
    pthread_mutex_t shm_mutex;
    pthread_cond_t shm_cond;
    SequenceNumber sequence_number;
    unsigned int buffer;
    unsigned int size = 0;
    unsigned int index = 0;
    SharedFlag flag;
    char data[IPC_BUFFER_SIZE];
};
class DataIPC {
    private:
    bool ipc_success = false;
    bool ipc_destroy = false;
    IPC_MODE ipc_mode;
    int shmid;
    key_t key;
    void *memory_segment = nullptr; //shared memory data
    SequenceNumber sequence_number;

    public:

    inline struct shared_data* sharedData() { return (struct shared_data *)memory_segment; }

    DataIPC(IPC_MODE ipc_mode, key_t key) : ipc_mode(ipc_mode), key(key) {
        if ((shmid = shmget(key, sizeof(shared_data), IPC_CREAT | 0666)) < 0) {
            printf("shmget failed\n");
            ipc_success = false;
            return;
        }

        if ((memory_segment = shmat(shmid, NULL, 0)) == (void *)-1) {
            printf("shmat failed\n");
            ipc_success = false;
            return;
        }
        if (ipc_mode == IPC_MODE::SENDER)
        {
            shared_data* sd = sharedData();
            pthread_mutexattr_t attrmutex;
            pthread_mutexattr_init(&attrmutex);
            pthread_mutexattr_setpshared(&attrmutex, PTHREAD_PROCESS_SHARED);
            pthread_mutex_init(&(sd->shm_mutex), &attrmutex);

            pthread_condattr_t attrcond;
            pthread_condattr_init(&attrcond);
            pthread_condattr_setpshared(&attrcond, PTHREAD_PROCESS_SHARED);
            pthread_cond_init(&(sd->shm_cond), &attrcond);
            
            sequence_number = ( (double) (rand()/32767) * (__UINT32_MAX__));
            sd->sequence_number = sequence_number;
            sd->buffer = IPC_BUFFER_SIZE;
        }
        ipc_success = true;
    }

    ~DataIPC() {
        ipc_destroy = true;
        if(!ipc_success) { return; }
        shared_data* sd = sharedData();
        pthread_mutex_lock(&(sd->shm_mutex));
        sd->flag = IPC_DESTROY;
        pthread_cond_signal(&(sd->shm_cond));
        pthread_mutex_unlock(&(sd->shm_mutex));

        if(ipc_mode == IPC_MODE::SENDER) { 
            if (-1 == shmctl(shmid, IPC_RMID, 0)){
                printf("Failed to Shared Memory Detach.\n");
            }
            printf("Detach Shared Memory.\n");
        }
    }

    bool sendData(IPCData& ipc_data, unsigned int size) {
        if(!ipc_success) { return false; }
        shared_data* sd = sharedData();
        sd->size = size;
        unsigned int i = 0;
        for(; i <= (size - sd->buffer); i+= sd->buffer) {
            pthread_mutex_lock(&(sd->shm_mutex));
            sd->flag = SEND_DATA;
            sd->sequence_number = sequence_number;
            ++sequence_number;
            if(memcpy(sd->data+i, (ipc_data.getData()+i), sd->buffer) == NULL) {
                printf("IPC Send Error\r\n");
                sd->flag |= SEND_ERROR;
                pthread_cond_signal(&(sd->shm_cond));
                pthread_mutex_unlock(&(sd->shm_mutex));
                return false;
            };
            sd->index = i;
            pthread_mutex_unlock(&(sd->shm_mutex));
            pthread_cond_signal(&(sd->shm_cond));
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        sd->flag |= SEND_SUCCESS;
        if(i < size) {
            pthread_mutex_lock(&(sd->shm_mutex));
            sd->flag |= SEND_DATA;
            sd->sequence_number = sequence_number;
            sd->buffer = (size-i);
            ++sequence_number;
            if(memcpy(sd->data+i, (ipc_data.getData()+i), (size-i)) == NULL) {
                printf("IPC Send Error\r\n");
                sd->flag |= SEND_ERROR;
                pthread_cond_signal(&(sd->shm_cond));
                pthread_mutex_unlock(&(sd->shm_mutex));
                return false;
            };
            sd->index = i;
            pthread_mutex_unlock(&(sd->shm_mutex));
            pthread_cond_signal(&(sd->shm_cond));
        }
        
        
        printf("IPC Send Success\r\n");
        return true;
    }

    IPCData receiveData() {
        IPCData result;
        if(!ipc_success) { return result; }
        if(ipc_mode != IPC_MODE::RECEIVER) { return result; }
        shared_data* sd = sharedData();
        result.setFlag(sd->flag);
        if((result.getFlag() & SEND_DATA) == SEND_DATA) {
            pthread_mutex_lock(&(sd->shm_mutex));
            unsigned int total_packet = sd->size / sd->buffer;
            if(sd->size % sd->buffer != 0) ++total_packet;
            result.alloc(sd->size);
            
            unsigned int packet = 0;
            
            bool valid = true;
            struct timespec wait_time = {0, 0};
            while(packet < total_packet) {
                //std::cout << packet << "/" << total_packet << std::endl;
                wait_time.tv_sec = time(NULL) + 2;

                if(pthread_cond_timedwait(&(sd->shm_cond),&(sd->shm_mutex),&wait_time) != 0) {
                    if((sd->flag & IPC_DESTROY) == IPC_DESTROY) { ipc_destroy = true; valid = false; break; }
                    continue;
                }
                if((sd->flag & SEND_ERROR) == SEND_ERROR) { valid = false; break; }

                if(sd->index < packet) {
                    total_packet = sd->size / sd->buffer;
                    if(sd->size % sd->buffer != 0) ++total_packet;
                    result.alloc(sd->size);
                    packet = 0;
                }
                
                if(memcpy((result.getData() + sd->index), sd->data+sd->index, sd->buffer) == NULL) { valid = false; break; }
                result.setFlag(sd->flag);

                ++packet;
                if((sd->flag & SEND_SUCCESS) == SEND_SUCCESS) {
                    pthread_mutex_unlock(&(sd->shm_mutex));
                    break;
                }
                pthread_mutex_unlock(&(sd->shm_mutex));
            }
            
            if(!valid) {
                result.setFlag(sd->flag);
                pthread_mutex_unlock(&(sd->shm_mutex));
                result.free();
                return result;
            }
            result.setFlag(result.getFlag() | SEND_SUCCESS);
        }
        return result;
    }

    inline bool valid() {
        return !ipc_destroy && ipc_success;
    }

};

namespace IPCStruct
{

//총 구조
//IPC Data 개수

//IPC Data 출력 type & id & size & byte & data
template <typename T>
void mwrite(char *dest, T *src, size_t *index)
{
    memcpy(dest + (*index), src, sizeof(T));
    *index += sizeof(T);
}

void mwrite(char *dest, char *src, size_t n, size_t *index)
{
    memcpy(dest + (*index), src, n);
    *index += n;
}

template <typename T>
T mread(char *src, size_t *index)
{
    T ret;
    memcpy(&ret,src+(*index), sizeof(T));
    *index += sizeof(T);
    return ret;
}

inline int getIPCDataSize(DataStruct data)
{
    return (1 + 20 + 4 + 1 + data.size() * data.typeSize());
}

template<typename T>
inline void vectorWrite(std::vector<T>* vec, char *dest, size_t *index) {
    int size = vec->size();
    char byte = sizeof(T);
    mwrite(dest, &size, index);
    mwrite(dest, &byte, index);
    for(auto it = vec->begin(); it != vec->end(); ++it) {
        T value = *it;
        mwrite(dest, &value, index);
    }
}

inline void vectorDecompos(DataStruct &data_struct, std::string s_id, char *dest, size_t *index)
{
    char data_type = static_cast<char>(data_struct.getType());
    mwrite(dest, &data_type, index);
    char id[20];
    strcpy(id, s_id.c_str());
    mwrite(dest, id, 20, index);

    if(data_struct.getType() == DataType::SINGLE_FLOAT) {
        std::vector<float>* vec = (std::vector<float>*) data_struct.getData();
        vectorWrite(vec,dest,index);
    } else if(data_struct.getType() == DataType::SINGLE_INT) {
        std::vector<int>* vec = (std::vector<int>*) data_struct.getData();
        vectorWrite(vec,dest,index);
    }
}

IPCData encodeIPCData(std::vector<DataStruct>& vecs)
{
    IPCData ipc_data;
    int number = vecs.size();
    int ipc_data_size = 4;
    for (int i = 0; i < number; i++)
    {
        ipc_data_size += getIPCDataSize(vecs[i]);
    }
    std::cout << "total size : " << ipc_data_size << std::endl;
    char *data = new char[ipc_data_size];
    
    //헤더 부분
    size_t index = 0;
    mwrite(data, &number, &index);    //4 number
    //vector 대입
    for (auto it = vecs.begin(); it != vecs.end(); ++it)
    {
        vectorDecompos(*it, "single", data, &index);
    }
    //std::cout << index;
    ipc_data.alloc(ipc_data_size);
    ipc_data.setData(data,ipc_data_size);
    delete[] data;
    return ipc_data;
}

std::vector<DataStruct> decodeIPCData(IPCData& ipc_data)
{
    std::vector<DataStruct> ret;
    char* data = ipc_data.getData();
    size_t index = 0;
    int number = mread<int>(data, &index);
    std::cout << number << std::endl;

    char *data = new char[ipc_data.getSize()];

    for (auto it = vecs.begin(); it != vecs.end(); ++it)
    {
        vectorDecompos(*it, "single", data, &index);
    }
    std::cout << index;
    ipc_data.alloc(ipc_data_size);
    ipc_data.setData(data,ipc_data_size);
    delete[] data;
    */
    return ret;
}
} // namespace IPCStruct

}
#endif