#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
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

template <class T, T val>
struct member_wrapper{};

template <class T>
class has_native_handle
{
	struct two {char x; char y;};
	template <class U> static two  test(...);
	template <class U> static char test(member_wrapper<typename U::native_handle_type (U::*)(), &U::native_handle>*);
public:
	static const bool value = sizeof(test<T>(0)) == 1;
};

template <bool>
struct choose
{
    std::string operator()() const {return std::string("not ");}
};

template <>
struct choose<true>
{
    std::string operator()() const {return std::string();}
};

int main()
{
    std::cout << "Demonstration of automatic detection of native_handle_type and native_handle.\n";
    std::cout << "For this implementation of the threading API:\n\n";

    std::cout << "std::mutex does "
              << choose<has_native_handle_type<std::mutex>::value>()()
              << "have native_handle_type.\n";
    std::cout << "std::mutex does "
              << choose<has_native_handle<std::mutex>::value>()()
              << "have native_handle.\n\n";

    std::cout << "std::recursive_mutex does "
              << choose<has_native_handle_type<std::recursive_mutex>::value>()()
              << "have native_handle_type.\n";
    std::cout << "std::recursive_mutex does "
              << choose<has_native_handle<std::recursive_mutex>::value>()()
              << "have native_handle.\n\n";

    std::cout << "std::timed_mutex does "
              << choose<has_native_handle_type<std::timed_mutex>::value>()()
              << "have native_handle_type.\n";
    std::cout << "std::timed_mutex does "
              << choose<has_native_handle<std::timed_mutex>::value>()()
              << "have native_handle.\n\n";

    std::cout << "std::recursive_timed_mutex does "
              << choose<has_native_handle_type<std::recursive_timed_mutex>::value>()()
              << "have native_handle_type.\n";
    std::cout << "std::recursive_timed_mutex does "
              << choose<has_native_handle<std::recursive_timed_mutex>::value>()()
              << "have native_handle.\n\n";

    std::cout << "std::condition_variable does "
              << choose<has_native_handle_type<std::condition_variable>::value>()()
              << "have native_handle_type.\n";
    std::cout << "std::condition_variable does "
              << choose<has_native_handle<std::condition_variable>::value>()()
              << "have native_handle.\n\n";

    std::cout << "std::condition_variable_any does "
              << choose<has_native_handle_type<std::condition_variable_any>::value>()()
              << "have native_handle_type.\n";
    std::cout << "std::condition_variable_any does "
              << choose<has_native_handle<std::condition_variable_any>::value>()()
              << "have native_handle.\n\n";

    std::cout << "std::thread does "
              << choose<has_native_handle_type<std::thread>::value>()()
              << "have native_handle_type.\n";
    std::cout << "std::thread does "
              << choose<has_native_handle<std::thread>::value>()()
              << "have native_handle.\n";
}

/*
Output:

Demonstration of automatic detection of native_handle_type and native_handle.
For this implementation of the threading API:

std::mutex does have native_handle_type.
std::mutex does have native_handle.

std::recursive_mutex does have native_handle_type.
std::recursive_mutex does have native_handle.

std::timed_mutex does not have native_handle_type.
std::timed_mutex does not have native_handle.

std::recursive_timed_mutex does not have native_handle_type.
std::recursive_timed_mutex does not have native_handle.

std::condition_variable does have native_handle_type.
std::condition_variable does have native_handle.

std::condition_variable_any does not have native_handle_type.
std::condition_variable_any does not have native_handle.

std::thread does have native_handle_type.
std::thread does have native_handle.
*/
