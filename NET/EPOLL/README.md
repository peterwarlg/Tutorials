# 以下参考自epoll的man page
# 1. epoll
The  central  concept  of the epoll API is the epoll instance, an in-kernel data structure which, from a user- space perspective, can be considered as a container for two lists:
epoll API的核心内容在于内核中的一个epoll实例(epoll instance)，它是一种内核数据结构（RB Tree），从用户空间来看，主要是有两个列表构成

1.  The interest list (sometimes also called the epoll set): the set of file descriptors that the process  has registered an interest in monitoring.
    
    **interest list**，也叫epoll set：所有在内核中已经注册的文件描述符的集合 (比如socket的文件描述符)。所指就是`epoll_create()`创建的epoll fd。<在很多教程中都说过epoll底层是一棵红黑树，epoll fd可以视为指向这棵树>
    
2. The  **ready list**: the set of file descriptors that are "ready" for I/O.  The ready list is a subset of (or, more precisely, a set of references to) the file descriptors in the interest list that is dynamically populated by the kernel as a result of I/O activity on those file descriptors.

    就绪列表：为 I/O“准备好”的一组文件描述符。 就绪列表是感兴趣列表中文件描述符的子集（或更准确地说，是他们的一组引用），由于这些文件描述符上的 I/O 活动而由内核动态填充。 （`epoll_wait()`返回一个列表，是所有有事件发生的文件描述符的集合）

epoll中的系统调用

1. `epoll_create()` : 具体请参考man epoll_wait
   1. `epoll_create(int size)`： Since Linux 2.6.8, the size argument is ignored, but must be greater than zero; see NOTES below.
      * 最初的`epoll_create()`使用size参数初始化申请内存空间，现在内核可以动态的size所需要的空间大小，
   2. `epoll_create1(int flags):`传入0 : `epoll_create1(0)`，并且没有其他参数时，与epoll_create()相同
2. `epoll_ctl()`：
   * Interest in particular file descriptors is then registered via epoll_ctl(2), which adds items to the interest list of the epoll instance.
   * 将需要注册的文件描述符注册到内核的epoll instance上。(*通俗一点的话，把要监听的文件描述符挂到epoll的树上*)
3. `epoll_wait()`:
   * `epoll_wait()`  waits  for  I/O  events,  blocking  the calling thread if no events are currently available. (This system call can be thought of as fetching items from the ready list of the epoll instance.)
   * `epoll_wait()` 等待 I/O 事件，如果当前没有可用的事件，则阻塞调用线程。 （这个系统调用可以被认为是从 epoll 实例的就绪列表中获取(*有事件发生的*)项目。） 

# Level-triggered and edge-triggered

挑重点

## ET

使用ET需要注意什么？考虑以下情况：

1. The file descriptor that represents the read side of a pipe (rfd) is registered on the epoll instance.
2. A pipe writer writes 2 kB of data on the write side of the pipe.
3. A call to `epoll_wait(2)` is done that will return rfd as a ready file descriptor.
4. The pipe reader reads 1 kB of data from rfd.
5. A call to `epoll_wait(2)` is done.

If the rfd file descriptor has been added to the epoll interface using the EPOLLET (edge-triggered) flag,  the call  to  `epoll_wait(2)` done in step 5 will probably hang despite the available data still present in the file input buffer; meanwhile the remote peer might be expecting a response based on the data it already sent.   The reason  for this is that edge-triggered mode delivers events only when changes occur on the monitored file descriptor.  So, in step 5 the caller might end up waiting for some data that is already present inside the  input  buffer.   In  the above example, an event on rfd will be generated because of the write done in 2 and the event is consumed in 3.  Since the read operation done in 4 does not consume the whole buffer data,  the  call to `epoll_wait(2)` done in step 5 might block indefinitely.

如果已经使用 EPOLLET（边缘触发）标志将 rfd 文件描述符添加到 epoll 接口，则尽管文件输入缓冲区中仍然存在可用数据，但步骤 5 中对 `epoll_wait(2)` 的调用可能会挂起； 同时，数据的发送方可能会根据它已经发送的数据等待服务端的响应。 这样做的原因是边缘触发模式仅在受监视的文件描述符发生更改时才传递事件(rfd上收到2KB文件)。 因此，在第 5 步中，调用者可能会等待一些已经存在于输入缓冲区中的数据。 在上面的例子中，rfd上的事件会因为在2中发生写入，出发了`epoll_wait()`的返回，从而在3中被消耗。由于4中完成的读操作并没有消耗整个缓冲区数据，而客户端又在等在回复，不会继续给服务端发送消息所以调用`epoll_wait(2)`完成 在第 5 步中可能会无限期地阻塞。 

1.  An  application that employs the EPOLLET flag should use nonblocking file descriptors to avoid having a blocking read or write starve a task that is handling multiple file descriptors.  The suggested way to use epoll as an edge-triggered (EPOLLET) interface is as follows:
   1.  with nonblocking file descriptors; and
   2.  by waiting for an event only after `read(2)` or `write(2)` return `EAGAIN`.

2.  使用 `EPOLLET` 标志的应用程序应该使用非阻塞的文件描述符，以避免阻塞读或写导致其他文件描述符的任务starve。 使用 epoll 作为边缘触发（`EPOLLET`）接口的建议方法如下： 
   1. 使用非阻塞的文件描述符，并且
   2. 仅在`read()`或者`write()`调用返回`EAGAIN`之后等待事件

## LT

在使用LT模式的情况下，epoll是一个更简单的、更快的poll。因为他们共享相同的语义（semantics）

## EPOLLONESHOT

* If multiple threads (or processes, if child processes have inherited the epoll file descriptor across `fork(2)`) are blocked in  `epoll_wait(2)` waiting on the same epoll file descriptor and a file descriptor in  the  interest list  that is marked for edge-triggered (EPOLLET) notification becomes ready, just one of the threads (or processes) is awoken from `epoll_wait(2)`.  This provides a useful  optimization  for  avoiding  **"thundering  herd"** wake-ups in some scenarios.

* 如果多个线程（或进程，如果子进程通过 `fork()` 继承了 epoll 文件描述符）并在 `epoll_wait(2)` 中被阻塞，等待相同的 epoll 文件描述符和兴趣列表中标记为ET的文件描述符通知准备就绪，只有一个线程（或进程）从 `epoll_wait(2)` 中唤醒。 这为在某些情况下避免“惊群效应”提供了有用的优化。 

* 使用场合：

     一个线程在读取完某个socket上的数据后开始处理这些数据，而数据的处理过程中该socket又有新数据可读，此时另外一个线程被唤醒来读取这些新的数据。于是，就出现了两个线程同时操作一个socket的局面。可以使用epoll的`EPOLLONESHOT`事件实现一个socket连接在任一时刻都被一个线程处理。

* EPOLL的`EPOLLONESHOT`事件，使一个socket连接任何时刻都只被一个线程所处理。对于注册了`EPOLLONESHOT`事件的socket，操作系统最多触发其上注册的一个可读、可写、或者异常事件，**且只触发一次**，这样，当一个线程在处理某个socket时，其他线程是不可能有机会操作该socket的，但反过来，注册了`EPOLLONESHOT`事件的socket一旦被某个线程处理完毕，**该线程就应该立即重置这个socket上的EPOLLONESHOT事件**，以确保这个socket下一次可读，其`EPOLLIN`事件能被触发，其他工作线程有机会继续处理这个socket