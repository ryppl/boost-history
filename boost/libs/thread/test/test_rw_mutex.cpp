#include <iostream>

#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/thread/rw_mutex.hpp>
#include <boost/test/test_tools.hpp>

namespace {

int shared_val = 0;

boost::xtime xsecs(int secs)
{
    boost::xtime ret;
	BOOST_TEST(boost::TIME_UTC == boost::xtime_get(&ret, boost::TIME_UTC));
    ret.sec += secs;
    return ret;
}

template <typename RW>
class thread_adapter
{
public:
    thread_adapter(void (*func)(void*,RW &), void* param1,RW &param2) 
        : _func(func), _param1(param1) ,_param2(param2){ }
    void operator()() const { _func(_param1, _param2); }

private:
    void (*_func)(void*, RW &);
    void* _param1;
    RW& _param2;
};

template <typename RW>
struct data
{
    data(int id, RW &m, int secs=0) : m_id(id), m_value(-1), m_secs(secs), m_rw_mutex(m) { }
    int m_value;
    int m_id;
    int m_secs;

    RW& m_rw_mutex;           // Reader/Writer mutex
};

// plain_writer excercises the "infinite" lock for each
//   RW_mutex type.

template<typename RW>
void plain_writer(void *arg,RW &rw)
{
    data<RW> *pdata = (data<RW> *) arg;
   // std::cout << "-->W" << pdata->m_id << "\n";
    
    RW::scoped_rw_lock l(rw,boost::EXCL_LOCK);

    boost::thread::sleep(xsecs(3));
    shared_val += 10;

    pdata->m_value = shared_val;
}

template<typename RW>
void plain_reader(void *arg,RW &rw)
{
    data<RW> *pdata = (data<RW> *) arg;
    RW::scoped_rw_lock l(rw,boost::SHARED_LOCK);

    pdata->m_value = shared_val;
}

template<typename RW>
void try_writer(void *arg,RW &rw)
{
    data<RW> *pdata = (data<RW> *) arg;
   // std::cout << "-->W" << pdata->m_id << "\n";
    
    RW::scoped_try_rw_lock l(rw,boost::NO_LOCK);

    if(l.try_wrlock())
    {

        boost::thread::sleep(xsecs(3));
        shared_val += 10;

        pdata->m_value = shared_val;
    }

}

template<typename RW>
void try_reader(void *arg,RW &rw)
{
    data<RW> *pdata = (data<RW> *) arg;
    RW::scoped_try_rw_lock l(rw);

    if(l.try_rdlock())
    {
        pdata->m_value = shared_val;
    }
}

template<typename RW>
void timed_writer(void *arg,RW &rw)
{
    data<RW> *pdata = (data<RW> *) arg;
    
    boost::xtime xt;
    xt = xsecs(pdata->m_secs);
    RW::scoped_timed_rw_lock l(rw,boost::NO_LOCK);

    if(l.timed_wrlock(xt))
    {
        boost::thread::sleep(xsecs(3));
        shared_val += 10;

        pdata->m_value = shared_val;
    }
}

template<typename RW>
void timed_reader(void *arg,RW &rw)
{
    data<RW> *pdata = (data<RW> *) arg;
    boost::xtime xt;
    xt = xsecs(pdata->m_secs);

    RW::scoped_timed_rw_lock l(rw,boost::NO_LOCK);

    if(l.timed_rdlock(xt))
    {
        pdata->m_value = shared_val;
    }
}

template<typename RW>
void dump_times(const char *prefix,data<RW> *pdata)
{
    std::cout << " " << prefix << pdata->m_id << 
        "   In:" << pdata->m_start.LowPart <<
        "   Holding:" << pdata->m_holding.LowPart <<
        "   Out: " << pdata->m_end.LowPart << std::endl;
}

template<typename RW>
void test_plain_rw_mutex(RW &rw_mutex)
{
    shared_val = 0;
	data<RW> r1(1,rw_mutex);
    data<RW> r2(2,rw_mutex);
    data<RW> w1(1,rw_mutex);
    data<RW> w2(2,rw_mutex);
    
    // Writer one launches, holds the lock for 3 seconds.
    boost::thread tw1(thread_adapter<RW>(plain_writer,&w1,rw_mutex));

    // Writer two launches, tries to grab the lock, "clearly"
    //  after Writer one will already be holding it.
    boost::thread::sleep(xsecs(1));
    boost::thread tw2(thread_adapter<RW>(plain_writer,&w2,rw_mutex));

    // Reader one launches, "clearly" after writer two, and "clearly"
    //   while writer 1 still holds the lock
    boost::thread::sleep(xsecs(1));
    boost::thread tr1(thread_adapter<RW>(plain_reader,&r1,rw_mutex));
    boost::thread tr2(thread_adapter<RW>(plain_reader,&r2,rw_mutex));

    tr2.join();
    tr1.join();
    tw2.join();
    tw1.join();

    if(rw_mutex.policy() == boost::sp_writer_priority)
    {
        BOOST_TEST(w1.m_value == 10);
        BOOST_TEST(w2.m_value == 20);
        BOOST_TEST(r1.m_value == 20);   // Readers get in after 2nd writer
        BOOST_TEST(r2.m_value == 20);
    }
    else if(rw_mutex.policy() == boost::sp_reader_priority)
    {
        BOOST_TEST(w1.m_value == 10);
        BOOST_TEST(w2.m_value == 20);
        BOOST_TEST(r1.m_value == 10);   // Readers get in before 2nd writer
        BOOST_TEST(r2.m_value == 10); 
    }
    else if(rw_mutex.policy() == boost::sp_alternating_many_reads)
    {
        BOOST_TEST(w1.m_value == 10);
        BOOST_TEST(w2.m_value == 20);
        BOOST_TEST(r1.m_value == 10);   // Readers get in before 2nd writer
        BOOST_TEST(r2.m_value == 10); 
    }
    else if(rw_mutex.policy() == boost::sp_alternating_single_reads)
    {
        BOOST_TEST(w1.m_value == 10);
        BOOST_TEST(w2.m_value == 20);

        // One Reader gets in before 2nd writer, but we can't tell
        // which reader will "win", so just check their sum.
        BOOST_TEST((r1.m_value + r2.m_value == 30));  
    }
}

template<typename RW>
void test_try_rw_mutex(RW &rw_mutex)
{
	data<RW> r1(1,rw_mutex);
    data<RW> w1(2,rw_mutex);
    data<RW> w2(3,rw_mutex);

    // We start with some specialized tests for "try" behavior

    shared_val = 0;

    // Writer one launches, holds the lock for 3 seconds.

    boost::thread tw1(thread_adapter<RW>(try_writer,&w1,rw_mutex));
    
    // Reader one launches, "clearly" after writer #1 holds the lock
    //   and before it releases the lock.
    boost::thread::sleep(xsecs(1));
    boost::thread tr1(thread_adapter<RW>(try_reader,&r1,rw_mutex));
    
    // Writer two launches in the same timeframe.
    boost::thread tw2(thread_adapter<RW>(try_writer,&w2,rw_mutex));

    tw2.join();
    tr1.join();
    tw1.join();

    BOOST_TEST(w1.m_value == 10);
    BOOST_TEST(r1.m_value == -1);        // Try would return w/o waiting
    BOOST_TEST(w2.m_value == -1);        // Try would return w/o waiting

    // We finish by repeating the plain tests with the try lock
    //  This is important to verify that try locks are proper rw_mutexes as
    //    well.
    test_plain_rw_mutex(rw_mutex);

}

template<typename RW>
void test_timed_rw_mutex(RW &rw_mutex)
{
    data<RW> r1(1,rw_mutex,1);
    data<RW> r2(2,rw_mutex,3);
    data<RW> w1(3,rw_mutex,3);
    data<RW> w2(4,rw_mutex,1);

    // We begin with some specialized tests for "timed" behavior

    shared_val = 0;

    // Writer one will hold the lock for 3 seconds.
    boost::thread tw1(thread_adapter<RW>(timed_writer,&w1,rw_mutex));

    boost::thread::sleep(xsecs(1));
    // Writer two will "clearly" try for the lock after the readers 
    //  have tried for it.  Writer will wait up 1 second for the lock.  This write will fail.
    boost::thread tw2(thread_adapter<RW>(timed_writer,&w2,rw_mutex));

    // Readers one and two will "clearly" try for the lock after writer
    //   one already holds it.  1st reader will wait 1 second, and will fail
    //   to get the lock.  2nd reader will wait 3 seconds, and will get 
    //   the lock.

    boost::thread tr1(thread_adapter<RW>(timed_reader,&r1,rw_mutex));
    boost::thread tr2(thread_adapter<RW>(timed_reader,&r2,rw_mutex));


    tw1.join();
    tr1.join();
    tr2.join();
    tw2.join();


    BOOST_TEST(w1.m_value == 10);
    BOOST_TEST(r1.m_value == -1);
    BOOST_TEST(r2.m_value == 10);
    BOOST_TEST(w2.m_value == -1);

    // We follow by repeating the try tests with the timed lock.
    //  This is important to verify that timed locks are proper try locks as well
    test_try_rw_mutex(rw_mutex);
}

} // namespace

void test_rw_mutex()
{
    int i;
    for(i = (int) boost::sp_writer_priority; 
    i <= (int) boost::sp_alternating_single_reads;
        i++)
    {
        boost::rw_mutex         plain_rw((boost::rw_scheduling_policy) i);
        boost::try_rw_mutex     try_rw((boost::rw_scheduling_policy) i);
        boost::timed_rw_mutex   timed_rw((boost::rw_scheduling_policy) i);

        std::cout << "plain test, sp=" << i << "\n";
        test_plain_rw_mutex(plain_rw);

        std::cout << "try test, sp=" << i << "\n";
        test_try_rw_mutex(try_rw);

        std::cout << "timed test, sp=" << i << "\n";
        test_timed_rw_mutex(timed_rw);  
    }
}
