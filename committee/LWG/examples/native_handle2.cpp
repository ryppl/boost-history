#include <mutex>
#include <pthread.h>

template <class Thread>
struct has_native_handle_type
{
	struct two {char x; char y;};
	template <class U> static two  test(...);
	template <class U> static char test(typename U::native_handle_type*);
public:
	static const bool value = sizeof(test<Thread>(0)) == 1;
};

// Set the priority ceiling of a POSIX mutex if possible

template <class Mutex>
typename stb::enable_if
<
    has_native_handle_type<Mutex>::value,
    int
>::type
mutex_set_priority_ceiling(Mutex& mut, int priority)
{
    static_assert(stb::is_same<typename Mutex::native_handle_type, pthread_mutex_t*>::value,
                  "Mutex::native_handle_type isn't a pthread_mutex_t*");
    int old_priority;
    pthread_mutex_setprioceiling(mut.native_handle(), priority, &old_priority);
    return old_priority;
}

int main()
{
    std::mutex mut1;
    mutex_set_priority_ceiling(mut1, 3);  // ok on Mac OS X
    std::timed_mutex mut2;
    mutex_set_priority_ceiling(mut2, 3);  // compile time error on Mac OS X
};

/*
main.cpp: In function ‘int main()’:
main.cpp:36: error: no matching function for call to ‘mutex_set_priority_ceiling(std::timed_mutex&, int)’
*/
