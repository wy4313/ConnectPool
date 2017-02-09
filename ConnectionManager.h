//
// Created by wangyue20 on 2017/2/9.
//

#ifndef CONNECTPOOL_MANAGER_H
#define CONNECTPOOL_MANAGER_H

#include "MultiQueue.h"

class ConnectionManager {
public:
    int sendPreLoadRequest(void *args);
    int sendPlayRequest(void *args);
    int sendDeleteRequest(void *args);
private:
    MultiQueue mRequestions;
    MultiQueue mPlayRequestions;
};

#endif //CONNECTPOOL_MANAGER_H
