#include <iostream>
#include "MultiQueue.h"
#include <vector>
#include <array>
#include <zconf.h>
#include "Debug.h"
#include "IdCreater.h"
#include "ConnectManager.h"

int multiQueueTest() {
    std::array<Task *, 10> taskArry;
    MultiQueue multiQueue;
    std::cout << "Hello, World!" << std::endl;
    for (int i=0; i < taskArry.max_size(); ++i) {
        taskArry[i]=new Task(i);
        multiQueue.push(taskArry[i]);
    }
    multiQueue.dump();
    for (int i=0; i < 3; ++i) {
        Task *pTask=multiQueue.pop();
        std::cout << "pop task id:" << pTask->getId() << std::endl;
    }

    multiQueue.dump();
    for (int i=3; i < 3 + 3; ++i) {
        multiQueue.remove(taskArry[i]);
    }


    multiQueue.dump();
    for (int i=6; i < 6 + 3; ++i) {
        multiQueue.remove(taskArry[i]->getId());
    }

    multiQueue.dump();
    return 0;
}


void user(ConnectManager* pManager){

    std::srand(unsigned(time(0)));
    int num=rand()%6;
   // int num=0;
    debug("rand num is:%d, usr:%d", num, std::this_thread::get_id());

    int taskId=pManager->sendPreLoadRequest(NULL);
    std::this_thread::sleep_for(std::chrono::seconds(num));
    pManager->sendPlayRequest(taskId);
    std::this_thread::sleep_for(std::chrono::seconds(num));
    pManager->sendDeleteRequest(taskId);
    std::this_thread::sleep_for(std::chrono::seconds(num));
    debug("use end, usr:%d", std::this_thread::get_id());
}

int main(int argc, char *argv[]) {
    for (int i=0; i < 10; ++i) {
        debug("get id:%d\n", IdCreater::createId());
    }

//    multiQueueTest();


    ConnectManager manager;
    std::thread threads[10];
    for (int j=0; j < sizeof(threads)/sizeof(std::thread); ++j) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        threads[j]=std::thread(user, &manager);
    }
    sleep(10);
    for(auto& t:threads){
        t.join();
    }
    return 0;
}