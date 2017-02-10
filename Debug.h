//
// Created by wangyue20 on 2017/2/9.
//

#ifndef CONNECTPOOL_DEBUG_H
#define CONNECTPOOL_DEBUG_H

#include <iostream>

#define debug(fmt, ...) printf("[debug][%s %d][tid:%d]" fmt "\n", __FUNCTION__,__LINE__, std::this_thread::get_id(),##__VA_ARGS__)
#define error(fmt, ...) printf("[error][%s %d][tid:%d]" fmt "\n", __FUNCTION__,__LINE__, std::this_thread::get_id(),##__VA_ARGS__)
#define info(fmt, ...) printf("[info][%s %d][tid:%d]" fmt "\n", __FUNCTION__,__LINE__, std::this_thread::get_id(), ##__VA_ARGS__)

#endif //CONNECTPOOL_DEBUG_H
