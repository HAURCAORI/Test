#include <QThread>
#include "../include/DataIPC.h"
#include "plotstruct.h"
#ifndef RTTHREAD_H
#define RTTHREAD_H

class RTThread : public QThread{
    Q_OBJECT
private:
    DataIO::IPCSharedMemory ipc;
    DataIO::IPCStruct::VectorContainer vec_container;
    void run();
public:
    explicit RTThread(QObject *parent = 0);

signals:
    void Update_Image(std::vector<rtplot::DataStruct>&,QString);
};

#endif // RTTHREAD_H
