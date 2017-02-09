//
// Created by wangyue20 on 2017/2/9.
//

#include <thread>
#include "ConnectionManager.h"
#include "Debug.h"
#include "IdCreater.h"
#include "ThreadJob.h"


class PreLoadJob:ThreadJob{
    typedef enum {
        PRELOAD_JOB_STATUS_CREATING=0,
        JOB_STATUS_BUYS,
    }PreLoadJobStatus;
    virtual int start(){
        debug("Preload job start\n");
        mIsBusy=true;
    }
    virtual int stop(){
        debug("Preload job stop\n");
        mIsBusy=false;
    }

    virtual bool isBusy(){
        debug("isBusy :%d\n", mIsBusy);
        return mIsBusy;
    }
    virtual int getStatus(){
        return
    }

    virtual int cancal()=0;
private:
    bool mIsBusy;
    PreLoadJobStatus mStatus;
};

void RequestHandler(MultiQueue* multiQueue){
   Task* pTask=multiQueue->pop();
    debug("get task id:"+pTask->getId());
}


int ConnectionManager::sendPreLoadRequest(void *args){
    Task* task=new Task(IdCreater::createId());
    mRequestions.push(task);
    return 0;
}


int ConnectionManager::sendPlayRequest(void *args) {
    //todo something
    return 0;
}

int ConnectionManager::sendDeleteRequest(void *args) {
    Task* task=new Task(IdCreater::createId());
    mRequestions.push(task);
    return 0;
}
