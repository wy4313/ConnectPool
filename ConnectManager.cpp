//
// Created by wangyue20 on 2017/2/9.
//

#include <thread>
#include "ConnectManager.h"
#include "Debug.h"
#include "IdCreater.h"
#include "ThreadJob.h"


class PreLoadJob:ThreadJob{
    typedef enum {
        PRELOAD_JOB_STATUS_DEFAULT=0,
        PRELOAD_JOB_STATUS_CREATING,
        PRELOAD_JOB_STATUS_PREPARING,
        PRELOAD_JOB_STATUS_READING,
        PRELOAD_JOB_STATUS_READLY,
        PRELOAD_JOB_STATUS_PLAYING,
    }PreLoadJobStatus;
public:


    PreLoadJob(int id):mId(id){}
    ~PreLoadJob(){
        debug("Pre load exit");

    };
    virtual int start(){
        debug("Preload job start\n");
        mIsBusy=true;
        return 0;
    }
    virtual int stop(){
        debug("Preload job stop\n");
        mIsBusy=false;
        return 0;
    }

    virtual bool isBusy(){
        debug("isBusy :%d\n", mIsBusy);
        return mIsBusy;
    }
    PreLoadJob::PreLoadJobStatus getStatus(){
        std::unique_lock<std::mutex> lock(mMutex);
        return mStatus;
    }

    virtual int cancal(){
        mCancel=true;
        mWaitAction.notify_one();
        return 0;
    }

    int play(){
        debug("set playing...\n");
        std::unique_lock<std::mutex> lock(mMutex);
        mStatus=PRELOAD_JOB_STATUS_PLAYING;
        debug("set playing notify...\n");
        mWaitAction.notify_one();
        return 0;
    }

    virtual void run(void* args){
        //only test code;
        for(;;) {

            if(mCancel){
                debug("do cancal works\n");
                break;
            }

            if(mStatus==PRELOAD_JOB_STATUS_DEFAULT) {
                changeStatus(PRELOAD_JOB_STATUS_CREATING);
                debug("do creating works\n");
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                //goto next status;
                changeStatus(PRELOAD_JOB_STATUS_PREPARING);

            } else if(mStatus==PRELOAD_JOB_STATUS_PREPARING){
                debug("do prepareing works\n");
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                changeStatus(PRELOAD_JOB_STATUS_READING);

            }else if(mStatus==PRELOAD_JOB_STATUS_READING){
                debug("do Reading works\n");
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                changeStatus(PRELOAD_JOB_STATUS_READLY);
            }else if(mStatus==PRELOAD_JOB_STATUS_READLY) {
                debug("pre load is readly wait for play or exit\n");
                std::unique_lock<std::mutex> lock(mMutex);
                mWaitAction.wait(lock);

            }else if(mStatus==PRELOAD_JOB_STATUS_PLAYING){
                debug("do playing...\n");
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
    }

    int getId() const {
        return mId;
    }
private:
    void changeStatus(PreLoadJobStatus status){
        std::unique_lock<std::mutex> lock(mMutex);
        debug("status change %d -> %d\n", mStatus, status);
        mStatus=status;
    }
private:
    int mId;
    bool mIsBusy;
    PreLoadJobStatus mStatus;
    bool mCancel;
    std::mutex mMutex;
    std::condition_variable mWaitAction;
};



void ConnectManager::requestHandler(MultiQueue* multiQueue){
    for(;;) {
        Task *pTask=multiQueue->pop();
        debug("get task id:%d\n", pTask->getId());
        std::unique_lock<std::mutex> lock(mJobsMutex);
        ThreadJob *pTJob=(ThreadJob *) new PreLoadJob(pTask->getId());
        mJobs[pTask->getId()]=pTJob;

        std::thread t(
                [this](ThreadJob *job, void *args) { job->run(args); },
                pTJob,
                pTask);

        t.detach();
    }
}


int ConnectManager::sendPreLoadRequest(void *args){
    Task* task=new Task(IdCreater::createId());
    mRequestions.push(task);
    return task->getId();
}


int ConnectManager::sendPlayRequest(int id) {
    //todo
    debug("args id:%d", id);
    std::unique_lock<std::mutex> lock(mJobsMutex);
    ((PreLoadJob*)mJobs[id])->play();
    return 0;
}

int ConnectManager::sendDeleteRequest(int id) {
//    Task* task=new Task(IdCreater::createId());
//    mRequestions.push(task);
    std::unique_lock<std::mutex> lock(mJobsMutex);
    ((PreLoadJob*)mJobs[id])->cancal();
    return 0;
}


ConnectManager::ConnectManager(){
    mHandler=new std::thread(
            [this](MultiQueue* multiQueue){this->requestHandler(multiQueue);},
            &mRequestions);
}


ConnectManager::~ConnectManager(){
    mHandler->detach();
}
