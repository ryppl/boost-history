/* example to demonstrate compile-in-place for threads

Compile with
  $CXX -I /path/to/boost_X_YY_Z test-threads.cpp -l $THREADLIBS

Notes:

 - The thread libs are e.g. "pthread" for Linux.

$Id$
*/

#define BOOST_THREAD_COMPILE_IN_PLACE
#include <boost/thread.hpp>
#include <iostream>
#include <ostream>
#include <boost/thread/compile_in_place.cpp>

void delay( unsigned sec)
{
    boost::xtime xt;
    boost::xtime_get( &xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

typedef boost::recursive_mutex mutex;
typedef mutex::scoped_lock scoped_lock;

mutex mtx;


void thread1()
{
    std::cerr << "thread 1 entering\n";

    delay(2);
    {
        std::cerr << "thread 1 acquiring mutex...\n";
        scoped_lock lock(mtx);
        std::cerr << "thread 1 acquired mutex\n";
        delay(2);
        std::cerr << "thread 1 releasing mutex\n";
    }
    delay(2);

    std::cerr << "thread 1 leaving\n";
}

void thread2()
{
    std::cerr << "thread 2 entering\n";

    delay(1);
    {
        std::cerr << "thread 2 acquiring mutex...\n";
        scoped_lock lock(mtx);
        std::cerr << "thread 2 acquired mutex\n";
        delay(1);
        std::cerr << "thread 2 releasing mutex\n";
    }
    delay(1);
    {
        std::cerr << "thread 2 acquiring mutex...\n";
        scoped_lock lock(mtx);
        std::cerr << "thread 2 acquired mutex\n";
        delay(1);
        std::cerr << "thread 2 releasing mutex\n";
    }
    delay(1);

    std::cerr << "thread 2 leaving\n";
}

int main()
{
    std::cerr << "main() entering\n";

    boost::thread th1(&thread1);
    std::cerr << "main() thread 1 started\n";
    boost::thread th2(&thread2);
    std::cerr << "main() thread 2 started\n";
    th1.join();
    th2.join();

    std::cerr << "main() leaving\n";
}

