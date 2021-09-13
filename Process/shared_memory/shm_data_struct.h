//
// Created by perterwarlg on 2021/9/13.
//

#ifndef SHARED_MEMORY_SHM_DATA_STRUCT_H
#define SHARED_MEMORY_SHM_DATA_STRUCT_H

#define TEXT_SIZE 1024

struct ShmDataStruct {
    int readable_;
    char text_[TEXT_SIZE];
};

#endif //SHARED_MEMORY_SHM_DATA_STRUCT_H
