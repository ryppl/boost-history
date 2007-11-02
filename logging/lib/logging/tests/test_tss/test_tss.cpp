// test_tss.cpp : Defines the entry point for the console application.
//

/* 
    Test : we use TSS (Thread Specific Storage). We check to see that there are no objects leaked.

    We create a number of threads.
    Each thread :
    - reads the current file - word by word
    - after each read, pauses a bit (like, 1 ms)
    - the words are concatenated, ignoring spaces
    - after the whole file is read, dumps to console : the thread index and the character at the thread's index from the concatenated string
    - we read as many words as there are threads (in order not to take too much time)

    The thread information is all kept using TSS


*/

#define BOOST_LOG_TSS_USE_INTERNAL
// this includes tss_value class
#include <boost/logging/logging.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/bind.hpp>
#include <boost/assert.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace boost;

// creating this (a log, or a filter), makes sure we initialize TSS
BOOST_DEFINE_LOG_FILTER_WITH_ARGS(g_log_filter, filter::use_tss_with_cache , 10) 


/* 
    counts the number of objects.
    When it's destroyed, there should be no objects left.
*/
struct object_count {
    typedef mutex::scoped_lock scoped_lock;

    object_count() : m_count(0) {}

    ~object_count() {
        scoped_lock lk(m_cs);
        BOOST_ASSERT(m_count == 0);
    }

    void increment() {
        scoped_lock lk(m_cs);
        ++m_count;
    }

    void decrement() {
        scoped_lock lk(m_cs);
        --m_count;
    }

    int count() const { 
        scoped_lock lk(m_cs);
        return m_count; 
    }

private:
    mutable mutex m_cs;
    int m_count;
};


object_count g_object_count;
object_count g_running_thread_count;

// the actual number of started threads
int g_thread_count = 50;


struct managed_object {
    managed_object(object_count & counter) : m_counter(counter) {
        m_counter.increment();
    }
    ~managed_object() {
        m_counter.decrement();
    }
private:
    object_count & m_counter;
};

struct read_file : private managed_object {
    // read this file itself
    read_file() : managed_object(g_object_count), m_in(new std::ifstream("test_tss.cpp")), m_word_idx(0) {
    }

    std::string read_word() {
        ++m_word_idx;
        if ( m_word_idx < g_thread_count) {
            std::string word;
            (*m_in) >> word;
            return word;
        }
        else
            return "";
    }

private:
    boost::shared_ptr<std::ifstream> m_in;
    int m_word_idx;
};

struct string_no_spaces : private managed_object {
    string_no_spaces() : managed_object(g_object_count) {}

    void add_word(const std::string & word) {
        m_str += word;
    }

    char at_idx(int idx) {
        if ( idx < (int)m_str.size())
            return m_str[idx];
        else
            return ' ';
    }

private:
    std::string m_str;
};

void do_sleep(int ms) {
    xtime next;
    xtime_get( &next, TIME_UTC);
    next.nsec += ms * 1000;

    int nano_per_sec = 1000000000;
    next.sec += next.nsec / nano_per_sec;
    next.nsec %= nano_per_sec;
    thread::sleep( next);
}

void process_file() {
    managed_object m(g_running_thread_count);
    int thread_idx = g_running_thread_count.count();
    std::cout << "started thread " << thread_idx << std::endl;

    using namespace logging;
    tss_value<read_file> file;
    tss_value<string_no_spaces> str;
    while ( true) {
        std::string word = file->read_word();
        str->add_word(word);
        if ( word.empty() )
            break;
        do_sleep(1);
    }

    std::cout << "char at idx " << thread_idx << ":" << str->at_idx(thread_idx) << std::endl ;

    std::cout << "ended thread " << thread_idx << std::endl;
}



int main(int argc, char* argv[])
{
    if ( argc > 1) {
        std::istringstream in(argv[1]);
        in >> g_thread_count;
    }
    std::cout << "running test with " << g_thread_count << " threads" << std::endl;

    for ( int idx = 0; idx < g_thread_count ; ++idx)
        thread t( &process_file);

    while ( g_running_thread_count.count() > 0 ) {
        do_sleep(100);
        std::cout << "remaining running threads " << g_running_thread_count.count() << std::endl;
    }
	return 0;
}


