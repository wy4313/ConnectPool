//
// Created by wangyue20 on 2017/2/9.
//

#ifndef CONNECTPOOL_MANAGER_H
#define CONNECTPOOL_MANAGER_H

#include <map>
#include "MultiQueue.h"
#include "ThreadJob.h"
#include <thread>

class ConnectManager {
public:
    ConnectManager();
    ~ConnectManager();
    int sendPreLoadRequest(void *args);
    int sendPlayRequest(int id);
    int sendDeleteRequest(int id);

private:
    void requestHandler(MultiQueue* multiQueue);
private:
    MultiQueue mRequestions;
    MultiQueue mPlayRequestions;
    std::map<int,ThreadJob*> mJobs;
    std::mutex mJobsMutex;
    std::thread* mHandler;
    bool mExit;
};

#endif //CONNECTPOOL_MANAGER_H
