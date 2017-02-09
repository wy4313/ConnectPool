//
// Created by wangyue20 on 2017/2/9.
//

#include <iostream>
#include "MultiQueue.h"


int MultiQueue::push(Task *task) {
    std::unique_lock<std::mutex> lock(mMutex);
    mTaskList.push_back(task);
    mNotEmpty.notify_all();
}

Task* MultiQueue::pop() {
    std::unique_lock<std::mutex> lock(mMutex);
    if(mTaskList.empty()){
        mNotEmpty.wait(lock);
    }
    Task* m=mTaskList.front();
    mTaskList.pop_front();
    return m;
}

void MultiQueue::remove(Task *task) {
    std::unique_lock<std::mutex> lock(mMutex);
    mTaskList.remove(task);
}

void MultiQueue::remove(int id) {
    std::unique_lock<std::mutex> lock(mMutex);
    for (auto iterator=mTaskList.begin(); iterator != mTaskList.end(); ++iterator) {
        if((*iterator)->getId()==id){
            mTaskList.remove(*iterator);
            return;
        }
    }
}

void MultiQueue::dump() {
    std::unique_lock<std::mutex> lock(mMutex);
    std::cout<<"+------dump MultiQueue-------------+"<<std::endl;
    int count=0;
    for (auto iterator=mTaskList.begin(); iterator != mTaskList.end(); ++iterator) {
        std::cout<< "task_id:"<<(*iterator)->getId()<<"("<<count++<<"/"<<mTaskList.size()<<")."<<std::endl;
    }
    std::cout<<'\n'<<std::endl;
}