//
// Created by perterwarlg on 2021/9/13.
//

#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <vector>
#include <unistd.h>
#include <cstring>
#include "shm_data_struct.h"

#define KEY_OF_SHM 8888

using namespace std;

int main() {
    //获取shm_id，在不同进程中这是唯一的，获取和KEY有关
    int shm_id = shmget(KEY_OF_SHM, sizeof(struct ShmDataStruct), 0666 | IPC_CREAT);

    //创建一个指针，用来指向共享内存
    void *addr_to_shm;

    //一个指针表明共享内存的结构体，应当和addr_to_shm互相转换来使用
    struct ShmDataStruct *shm_data;

    //将addr_to_shm连接到系统分配的共享内存，也就是将共享内存（物理地址）挂到当前进程的内存空间（虚拟地址）
    addr_to_shm = shmat(shm_id, (void*)0, 0);
    printf("\nMemory attached at %X\n", *(int*)addr_to_shm);

    //将获得的void*类型的转为我们需要的data struct
    shm_data = (struct ShmDataStruct*)addr_to_shm;

    char buffer[TEXT_SIZE];
    shm_data->readable_ = 1;

    while (1) {
        cin >> buffer;
        strncpy(shm_data->text_, buffer, TEXT_SIZE);

        if (strncmp(shm_data->text_, "end", 3) == 0) {
            cout << "Close Write SHM" << endl;
            break;
        }
    }

    //把共享内存从当前进程分离，也就是将其从进程内存空间的页表中除掉
    shmdt(addr_to_shm);

    //删除共享内存
    shmctl(shm_id, IPC_RMID, 0);
    exit(EXIT_SUCCESS);
}
