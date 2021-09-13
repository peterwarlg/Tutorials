cmake_minimum_required(VERSION 3.10)
project(shared_memory)

set(CMAKE_CXX_STANDARD 14)

add_executable(shm_read shm_read.cpp shm_data_struct.h)
add_executable(shm_write shm_write.cpp shm_data_struct.h)