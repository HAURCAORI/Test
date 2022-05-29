#include "rtthread.h"
#include <iostream>


RTThread::RTThread(QObject *paret) : ipc(DataIO::IPC_MODE::RECEIVER, 98765)
{

}

void RTThread::run()
{
    while(ipc.valid()) {
        DataIO::IPCData d(ipc.receiveData());
        if((d.getFlag() & IPC_DESTROY) == IPC_DESTROY) {
            std::cout << "destroy" << std::endl;
            break;
        }
        else if((d.getFlag() & SEND_DATA) == SEND_DATA) {
            if((d.getFlag() & SEND_SUCCESS) == SEND_SUCCESS) {
                std::vector<DataIO::IPCStruct::IPCDataStruct> ds = DataIO::IPCStruct::decodeIPCData(d,vec_container);
                std::vector<rtplot::DataStruct> rds(ds.begin(), ds.end());
                qp->updateDataSet(rds);
                emit Update_Image("view1");

                /*
                for(auto it = rds.begin(); it != rds.end(); ++it) {

                    //plot->updateImage();
                    //plot->updateDataSet();
                    //rtplot::DataStruct temp(*it);
                    std::cout << it->size() << std::endl;

                    //rds.push_back(temp);
                }
                */


            } else if((d.getFlag() & SEND_ERROR) == SEND_ERROR) {
                std::cout << "error" << std::endl;
            }
        } else {
            std::cout << "fail" << std::endl;
        }
    }
}
