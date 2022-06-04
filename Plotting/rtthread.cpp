#include "rtthread.h"
#include <iostream>


RTThread::RTThread(QObject *paret) : ipc(DataIO::IPC_MODE::RECEIVER, DEFULAT_IPC_KEY)
{

}

void RTThread::run()
{
    is_running = true;
    while(is_running) {
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
                    Target target = d.getTarget();
                    if(target < vec_qplot.size()) {
                        vec_qplot[target].qp->updateDataSet(&rds);
                        emit Update_Image(vec_qplot[target].objectname);
                    }
                    std::cout << "signal" << std::endl;
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
        std::cout << "[IPC] Try to Connect IPC..." << std::endl;
        this->sleep(5);
        ipc.connect();
    }
}
