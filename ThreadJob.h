//
// Created by wangyue20 on 2017/2/9.
//

#ifndef CONNECTPOOL_THREADJOB_H
#define CONNECTPOOL_THREADJOB_H


class ThreadJob {
public:
    void run(void* args)=0;
    virtual int start()=0;
    virtual int stop()=0;

    virtual bool isBusy()=0;
    virtual int getStatus()=0;

    virtual int cancal()=0;
};


#endif //CONNECTPOOL_THREADJOB_H
