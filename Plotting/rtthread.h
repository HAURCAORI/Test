#include <QThread>
//#include "../include/DataIPC.h"

#ifndef RTTHREAD_H
#define RTTHREAD_H

class RTThread : public QThread{
    Q_OBJECT
public:
    explicit RTThread(QObject *parent = 0);
private:
    //DataIO::IPCSharedMemory ipc;
    //DataIO::IPCStruct::VectorContainer vec_container;
    void run();
signals:
    void Send();
};

#endif // RTTHREAD_H
