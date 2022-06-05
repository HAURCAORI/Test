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
            Target target = d.getTarget();
            if((d.getFlag() & IPC_DESTROY) == IPC_DESTROY) {
                std::cout << "destroy" << std::endl;
                break;
            }
            else if((d.getFlag() & SEND_DATA) == SEND_DATA) {
                if((d.getFlag() & SEND_SUCCESS) == SEND_SUCCESS) {
                    if(vec_qplot[target].qp->update) {

                        std::vector<DataIO::IPCStruct::IPCDataStruct> ds = DataIO::IPCStruct::decodeIPCData(d,vec_container);
                        std::vector<rtplot::DataStruct> rds(ds.begin(), ds.end());

/*
                        for(auto it = rds.begin(); it != rds.end(); ++it) {
                            auto temp = static_cast<std::vector<FLOAT>*> (it->getData());
                            std::cout << "size : " << temp->size() << std::endl;

                            for(auto i = temp->begin(); i != temp->end(); ++i) {
                                std::cout << *i << " ";
                            }
                            std::cout << std::endl;

                        }
                    */

                        if(target < vec_qplot.size()) {
                            vec_qplot[target].qp->updateDataSet(&rds);
                            emit Update_Image(vec_qplot[target].objectname);
                        }

                    }
                    std::cout << "signal" << std::endl;
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
