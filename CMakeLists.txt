cmake_minimum_required(VERSION 3.6)
project(ConnectPool)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp MultiQueue.cpp MultiQueue.h ThreadPool.cpp ThreadPool.h ConnectManager.cpp ConnectManager.h Debug.h IdCreater.cpp IdCreater.h ThreadJob.cpp ThreadJob.h)
add_executable(ConnectPool ${SOURCE_FILES})