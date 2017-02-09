#include <iostream>
#include "MultiQueue.h"
#include <vector>
#include <array>
#include "Debug.h"
#include "IdCreater.h"

int multiQueueTest() {
    std::array<Task*, 10> taskArry;
    MultiQueue multiQueue;
    std::cout << "Hello, World!" << std::endl;
    for (int i=0; i < taskArry.max_size(); ++i) {
        taskArry[i]=new Task(i);
        multiQueue.push(taskArry[i]);
    }
    multiQueue.dump();
    for (int i=0; i <3 ; ++i) {
        Task* pTask=multiQueue.pop();
        std::cout<< "pop task id:"<< pTask->getId()<<std::endl;
    }

    multiQueue.dump();
    for (int i=3; i <3+3; ++i) {
        multiQueue.remove(taskArry[i]);
    }


    multiQueue.dump();
    for (int i=6; i < 6+3; ++i) {
        multiQueue.remove(taskArry[i]->getId());
    }

    multiQueue.dump();
    return 0;
}

int main(int argc, char* argv[]){
    debug("111");
    error("222");
    info("333");
    for (int i=0; i <10 ; ++i) {
        debug("get id:%d\n", IdCreater::createId());
    }

    multiQueueTest();
    return 0;
}