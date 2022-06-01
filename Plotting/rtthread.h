#include <QThread>
#include "../include/DataIPC.h"
#include "plotstruct.h"
#include "qplot.h"
#include "vector"
#ifndef RTTHREAD_H
#define RTTHREAD_H

struct ThreadObject {
    qplot* qp;
    QString objectname;
    ThreadObject(qplot* q,QString s) : qp(q), objectname(s) {}
};

struct findThreadObject {
    qplot* id;
    findThreadObject(qplot* qp) : id(qp) {}
    bool operator() (const ThreadObject& obj) const {
        return obj.qp == id;
    }
};

class RTThread : public QThread{
    Q_OBJECT
private:
    DataIO::IPCSharedMemory ipc;
    DataIO::IPCStruct::VectorContainer vec_container;

    std::vector<ThreadObject> vec_qplot;
    void run();
public:
    explicit RTThread(QObject *parent = 0);
    inline void EnableThread(qplot* qp) { if(std::find_if(vec_qplot.begin(), vec_qplot.end(), findThreadObject(qp)) == vec_qplot.end()) vec_qplot.push_back(ThreadObject(qp,qp->objectName())); }
    inline void DisableThread(qplot* qp) { vec_qplot.erase(std::remove_if(vec_qplot.begin(),vec_qplot.end(), findThreadObject(qp)),vec_qplot.end()); }

signals:
    void Update_Image(QString);
};

#endif // RTTHREAD_H
