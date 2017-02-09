//
// Created by wangyue20 on 2017/2/9.
//

#ifndef CONNECTPOOL_DEBUG_H
#define CONNECTPOOL_DEBUG_H

#include <iostream>

#define debug(fmt, ...) printf("[debug][%s %d]" fmt, __FUNCTION__,__LINE__, ##__VA_ARGS__)
#define error(fmt, ...) printf("[error][%s %d]" fmt, __FUNCTION__,__LINE__,##__VA_ARGS__)
#define info(fmt, ...) printf("[info][%s %d]" fmt, __FUNCTION__,__LINE__, ##__VA_ARGS__)

#endif //CONNECTPOOL_DEBUG_H
