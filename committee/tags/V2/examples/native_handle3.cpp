// Example of native_handle2.cpp being done without the native_handle() facility
//   We've turned a compile time error into a run time error

#include <mutex>
#include <pthread.h>

template <class Mutex>
int
mutex_set_priority_ceiling(Mutex& mut, int priority)
{
    int old_priority;
    pthread_mutex_setprioceiling((pthread_mutex_t*)&mut, priority, &old_priority);
    return old_priority;
}

int main()
{
    std::mutex mut1;
    mutex_set_priority_ceiling(mut1, 3);  // ok on Mac OS X
    std::timed_mutex mut2;
    mutex_set_priority_ceiling(mut2, 3);  // run time error on Mac OS X
};
