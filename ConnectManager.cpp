//
// Created by wangyue20 on 2017/2/9.
//

#include <thread>
#include "ConnectManager.h"
#include "Debug.h"
#include "IdCreater.h"


class PreLoadJob : ThreadJob {
    typedef enum {
        PRELOAD_JOB_STATUS_DEFAULT=0,
        PRELOAD_JOB_STATUS_CREATING,
        PRELOAD_JOB_STATUS_PREPARING,
        PRELOAD_JOB_STATUS_READING,
        PRELOAD_JOB_STATUS_READLY,
        PRELOAD_JOB_STATUS_PLAYING,
    } PreLoadJobStatus;
public:


    PreLoadJob(int id) : mId(id) {}

    ~PreLoadJob() {
        debug("Pre load exit");

    };

    virtual int start() {
        debug("Preload job start\n");
        mIsBusy=true;
        return 0;
    }

    virtual int stop() {
        debug("Preload job stop\n");
        mIsBusy=false;
        return 0;
    }

    virtual bool isBusy() {
        debug("isBusy :%d\n", mIsBusy);
        return mIsBusy;
    }

    PreLoadJob::PreLoadJobStatus getStatus() {
        std::unique_lock<std::mutex> lock(mMutex);
        return mStatus;
    }

    virtual int cancal() {
        mCancel=true;
        mWaitAction.notify_one();
        return 0;
    }

    int play() {
        debug("set playing...\n");
        std::unique_lock<std::mutex> lock(mMutex);
        mStatus=PRELOAD_JOB_STATUS_PLAYING;
        debug("set playing notify...\n");
        mWaitAction.notify_one();
        return 0;
    }

    virtual void run(void *args) {
        //only test code;
        for (;;) {

            if (mCancel) {
                debug("do cancal works\n");
                break;
            }

            if (mStatus == PRELOAD_JOB_STATUS_DEFAULT) {
                changeStatus(PRELOAD_JOB_STATUS_CREATING);
                debug("do creating works\n");
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                //goto next status;
                changeStatus(PRELOAD_JOB_STATUS_PREPARING);

            } else if (mStatus == PRELOAD_JOB_STATUS_PREPARING) {
                debug("do prepareing works\n");
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                changeStatus(PRELOAD_JOB_STATUS_READING);

            } else if (mStatus == PRELOAD_JOB_STATUS_READING) {
                debug("do Reading works\n");
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                changeStatus(PRELOAD_JOB_STATUS_READLY);
            } else if (mStatus == PRELOAD_JOB_STATUS_READLY) {
                debug("pre load is readly wait for play or exit\n");
                std::unique_lock<std::mutex> lock(mMutex);
                mWaitAction.wait(lock);

            } else if (mStatus == PRELOAD_JOB_STATUS_PLAYING) {
                debug("do playing...\n");
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
    }

    int getId() const {
        return mId;
    }

private:
    void changeStatus(PreLoadJobStatus status) {
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



class RequestTask:public Task{
public:
    typedef enum{
        TASK_TYPE_DEFAULT,
        TASK_TYPE_PRELOAD,
        TASK_TYPE_CANCEL,
        TASK_TYPE_PLAY,
    }TaskType;

    RequestTask(int id, RequestTask::TaskType type):
           mType(type){
        setId(id);
    }
    TaskType getType() const {
        return mType;
    }
    void setType(TaskType type) {
        mType=type;
    }
private:
    RequestTask::TaskType mType;
};

void ConnectManager::requestHandler(MultiQueue *multiQueue) {
    for (;false==mExit;) {
        RequestTask *pTask=(RequestTask*)multiQueue->pop(500);
        if(pTask==NULL) continue;
        debug("get task id:%d type:%d\n", pTask->getId(), pTask->getType());
        if(RequestTask::TASK_TYPE_PRELOAD==pTask->getType()) {
            ThreadJob *pTJob=NULL;
            {
                std::unique_lock<std::mutex> lock(mJobsMutex);
                pTJob=(ThreadJob *) new PreLoadJob(pTask->getId());
                mJobs[pTask->getId()]=pTJob;
            }
            std::thread t(
                    [this](ThreadJob *job, void *args) { job->run(args); },
                    pTJob,
                    pTask);

            t.detach();
        }else if(RequestTask::TASK_TYPE_PLAY==pTask->getType()){
            std::unique_lock<std::mutex> lock(mJobsMutex);
            PreLoadJob* pPreLoadJob=(PreLoadJob*) mJobs[pTask->getId()];
            if(pPreLoadJob!=NULL){
                pPreLoadJob->play();
            }
        }else if(RequestTask::TASK_TYPE_CANCEL==pTask->getType()){
            std::unique_lock<std::mutex> lock(mJobsMutex);
            PreLoadJob* pPreLoadJob=(PreLoadJob*) mJobs[pTask->getId()];
            if(pPreLoadJob!=NULL){
                pPreLoadJob->cancal();
            }
        }
    }
    debug("pthread exit");
}


int ConnectManager::sendPreLoadRequest(void *args) {
    RequestTask *task=new RequestTask(
            IdCreater::createId(), RequestTask::TASK_TYPE_PRELOAD);
    mRequestions.push((Task*)task);
    return task->getId();
}


int ConnectManager::sendPlayRequest(int id) {
    //todo
    debug("args id:%d", id);
    RequestTask *task=new RequestTask(id, RequestTask::TASK_TYPE_PLAY);
    mRequestions.push((Task*)task);
    return 0;
}

int ConnectManager::sendDeleteRequest(int id) {
    RequestTask *task=new RequestTask(id, RequestTask::TASK_TYPE_CANCEL);
    mRequestions.push((Task*)task);
    return 0;
}


ConnectManager::ConnectManager() {
    mHandler=new std::thread(
            [this](MultiQueue *multiQueue) { this->requestHandler(multiQueue); },
            &mRequestions);
}


ConnectManager::~ConnectManager() {
    mExit=true;
    mHandler->join();
    debug("request handler join...");
}
