#  C/C++的共享内存教程 | A tutorial about shraed memory in Linux using cpp

I have wrote all the things that should be noted in my code 

## 编译和使用

IDE : Clion

编译器 ： g++ > 3.10 && < 3.16

使用

```shell
cd /cmake-build-debug
cmake .
make
//分别执行
//1.
./shm_write
//2.
./shm_read
```



## 知识点 | some tips

* `shmdt() `与`shmctl()`
  * 当对共享存储段的操作已经结束时，则调用`shmdt`脱接该段。注意，这并不从系统中删除其标识符以及其数据结构。该标识符仍然存在，直至某个进程（一般是服务器）调用`shmctl`（带命令`IPC_RMID`， ***in shm_write.cpp line 51*** : `shmctl(shm_id, IPC_RMID, 0);`）特地删除它。
  * （连接是进程将共享内存的物理页加入进程页表，脱离是从页表中撤销该物理页的信息，并不改变实际的物理页）