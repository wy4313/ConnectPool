//
// Created by wangyue20 on 2017/2/9.
//

#ifndef CONNECTPOOL_MULTIQUEUE_H
#define CONNECTPOOL_MULTIQUEUE_H

class Task {
public:
    Task(){};
    Task(int id):mId(id){}

    int getId() const {
        return mId;
    }

    void setId(int id) {
        mId=id;
    }

private:
    int mId;
};
#include <mutex>

#include <list>

class MultiQueue{
public:
    int push(Task* task);
    Task* pop();
    Task* pop(int timeout_ms);
    void remove(Task* task);
    void remove(int id);
    void dump();
private:
    std::mutex mMutex;
    std::condition_variable mNotEmpty;
    std::list<Task*> mTaskList;

};


#endif //CONNECTPOOL_MULTIQUEUE_H
