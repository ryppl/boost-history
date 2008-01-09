#include <thread>

template <class Thread>
struct has_native_handle_type
{
	struct two {char x; char y;};
	template <class U> static two  test(...);
	template <class U> static char test(typename U::native_handle_type*);
public:
	static const bool value = sizeof(test<Thread>(0)) == 1;
};

template <class Thread>
void kill_thread(Thread t, std::false_type)
{
    static_assert(sizeof(Thread) == 0, "Having trouble porting kill_thread");
}

template <class Thread>
void kill_thread(Thread t, std::true_type, std::false_type)
{
    // handle Windows?
    windows_kill_thread(t.native_handle());
}

template <class Thread>
void kill_thread(Thread t, std::true_type, std::true_type)
{
    pthread_kill(t.native_handle(), SIGKILL);
}

void kill_thread(std::thread t, std::true_type a)
{
    kill_thread(move(t), a, std::integral_constant<bool, std::is_same<std::thread::native_handle_type, pthread_t>::value>());
}

void kill_thread(std::thread t)
{
    kill_thread(move(t), std::integral_constant<bool, has_native_handle_type<std::thread>::value>());
}

void f()
{
    while (true)
        ;
}

int main()
{
    std::thread t(f);
    kill_thread(move(t));
}
