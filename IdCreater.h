//
// Created by wangyue20 on 2017/2/9.
//

#ifndef CONNECTPOOL_IDCREATER_H
#define CONNECTPOOL_IDCREATER_H


#include <mutex>
static std::mutex mutex;

class IdCreater {
public:
    static int createId(){
        static int globalId=0;
        std::unique_lock<std::mutex> lock(mutex);
        return globalId++;
    }
private:
    IdCreater(){};
};


#endif //CONNECTPOOL_IDCREATER_H
