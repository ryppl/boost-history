//  (C) Copyright 2008 Anthony Williams 
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include "boost/thread/thread.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition.hpp"
#include "boost/task/future.hpp"
#include <assert.h>
#include <utility>
#include <memory>
#include <string>

#ifdef BOOST_HAS_RVALUE_REFS
    template<typename T>
    typename boost::remove_reference<T>::type&& move(T&& t)
    {
        return t;
    }
#else
    template<typename T>
    boost::detail::thread_move_t<T> move(T& t)
    {
        return boost::move(t);
    }
#endif

struct X
{
private:
    
    X(X& other);
    
public:
    
    int i;
    
    X():
        i(42)
    {}
#ifdef BOOST_HAS_RVALUE_REFS
    X(X&& other):
        i(other.i)
    {
        other.i=0;
    }
#else
    X(boost::detail::thread_move_t<X> other):
        i(other->i)
    {
        other->i=0;
    }
    operator boost::detail::thread_move_t<X>()
    {
        return boost::detail::thread_move_t<X>(*this);
    }
#endif
    ~X()
    {}
};

int make_int()
{
    return 42;
}

int throw_runtime_error()
{
    throw std::runtime_error("42");
}

void set_promise_thread(boost::promise<int>* p)
{
    p->set_value(42);
}

struct my_exception
{};

void set_promise_exception_thread(boost::promise<int>* p)
{
    p->set_exception(boost::copy_exception(my_exception()));
}


void test_store_value_from_thread()
{
    boost::promise<int> pi2;
    boost::unique_future<int> fi2=pi2.get_future();
    boost::thread(set_promise_thread,&pi2);
    int j=fi2.get();
    assert(j==42);
    assert(fi2.is_ready());
    assert(fi2.has_value());
    assert(!fi2.has_exception());
    assert(fi2.get_state()==boost::future_state::ready);
}


void test_store_exception()
{
    boost::promise<int> pi3;
    boost::unique_future<int> fi3=pi3.get_future();
    boost::thread(set_promise_exception_thread,&pi3);
    try
    {
        fi3.get();
        assert(false);
    }
    catch(my_exception)
    {
        assert(true);
    }
    
    assert(fi3.is_ready());
    assert(!fi3.has_value());
    assert(fi3.has_exception());
    assert(fi3.get_state()==boost::future_state::ready);
}

void test_initial_state()
{
    boost::unique_future<int> fi;
    assert(!fi.is_ready());
    assert(!fi.has_value());
    assert(!fi.has_exception());
    assert(fi.get_state()==boost::future_state::uninitialized);
    int i;
    try
    {
        i=fi.get();
        assert(false);
    }
    catch(boost::future_uninitialized)
    {
        assert(true);
    }
}

void test_waiting_future()
{
    boost::promise<int> pi;
    boost::unique_future<int> fi;
    fi=pi.get_future();

    int i=0;
    assert(!fi.is_ready());
    assert(!fi.has_value());
    assert(!fi.has_exception());
    assert(fi.get_state()==boost::future_state::waiting);
    assert(i==0);
}

void test_cannot_get_future_twice()
{
    boost::promise<int> pi;
    pi.get_future();

    try
    {
        pi.get_future();
        assert(false);
    }
    catch(boost::future_already_retrieved&)
    {
        assert(true);
    }
}

void test_set_value_updates_future_state()
{
    boost::promise<int> pi;
    boost::unique_future<int> fi;
    fi=pi.get_future();

    pi.set_value(42);
    
    assert(fi.is_ready());
    assert(fi.has_value());
    assert(!fi.has_exception());
    assert(fi.get_state()==boost::future_state::ready);
}

void test_set_value_can_be_retrieved()
{
    boost::promise<int> pi;
    boost::unique_future<int> fi;
    fi=pi.get_future();

    pi.set_value(42);
    
    int i=0;
    assert(i=fi.get());
    assert(i==42);
    assert(fi.is_ready());
    assert(fi.has_value());
    assert(!fi.has_exception());
    assert(fi.get_state()==boost::future_state::ready);
}

void test_set_value_can_be_moved()
{
//     boost::promise<int> pi;
//     boost::unique_future<int> fi;
//     fi=pi.get_future();

//     pi.set_value(42);
    
//     int i=0;
//     assert(i=fi.get());
//     assert(i==42);
//     assert(fi.is_ready());
//     assert(fi.has_value());
//     assert(!fi.has_exception());
//     assert(fi.get_state()==boost::future_state::ready);
}

void test_future_from_packaged_task_is_waiting()
{
    boost::packaged_task<int> pt(make_int);
    boost::unique_future<int> fi=pt.get_future();
    int i=0;
    assert(!fi.is_ready());
    assert(!fi.has_value());
    assert(!fi.has_exception());
    assert(fi.get_state()==boost::future_state::waiting);
    assert(i==0);
}

void test_invoking_a_packaged_task_populates_future()
{
    boost::packaged_task<int> pt(make_int);
    boost::unique_future<int> fi=pt.get_future();

    pt();

    int i=0;
    assert(fi.is_ready());
    assert(fi.has_value());
    assert(!fi.has_exception());
    assert(fi.get_state()==boost::future_state::ready);
    assert(i=fi.get());
    assert(i==42);
}

void test_invoking_a_packaged_task_twice_throws()
{
    boost::packaged_task<int> pt(make_int);

    pt();
    try
    {
        pt();
        assert(false);
    }
    catch(boost::task_already_started)
    {
        assert(true);
    }
}


void test_cannot_get_future_twice_from_task()
{
    boost::packaged_task<int> pt(make_int);
    pt.get_future();
    try
    {
        pt.get_future();
        assert(false);
    }
    catch(boost::future_already_retrieved)
    {
        assert(true);
    }
}

void test_task_stores_exception_if_function_throws()
{
    boost::packaged_task<int> pt(throw_runtime_error);
    boost::unique_future<int> fi=pt.get_future();

    pt();

    int i=0;
    assert(fi.is_ready());
    assert(!fi.has_value());
    assert(fi.has_exception());
    assert(fi.get_state()==boost::future_state::ready);
    try
    {
        fi.get();
        assert(false);
    }
    catch(std::exception&)
    {
        assert(true);
    }
    catch(...)
    {
        assert(!"Unknown exception thrown");
    }
    
}

void test_void_promise()
{
    boost::promise<void> p;
    boost::unique_future<void> f=p.get_future();
    p.set_value();
    assert(f.is_ready());
    assert(f.has_value());
    assert(!f.has_exception());
    assert(f.get_state()==boost::future_state::ready);
    f.get();
}

void test_reference_promise()
{
    boost::promise<int&> p;
    boost::unique_future<int&> f=p.get_future();
    int i=42;
    p.set_value(i);
    assert(f.is_ready());
    assert(f.has_value());
    assert(!f.has_exception());
    assert(f.get_state()==boost::future_state::ready);
    assert(&f.get()==&i);
}

void do_nothing()
{}

void test_task_returning_void()
{
    boost::packaged_task<void> pt(do_nothing);
    boost::unique_future<void> fi=pt.get_future();

    pt();

    assert(fi.is_ready());
    assert(fi.has_value());
    assert(!fi.has_exception());
    assert(fi.get_state()==boost::future_state::ready);
}

int global_ref_target=0;

int& return_ref()
{
    return global_ref_target;
}

void test_task_returning_reference()
{
    boost::packaged_task<int&> pt(return_ref);
    boost::unique_future<int&> fi=pt.get_future();

    pt();

    assert(fi.is_ready());
    assert(fi.has_value());
    assert(!fi.has_exception());
    assert(fi.get_state()==boost::future_state::ready);
    int& i=fi.get();
    assert(&i==&global_ref_target);
}

void test_shared_future()
{
    boost::packaged_task<int> pt(make_int);
    boost::unique_future<int> fi=pt.get_future();

    boost::shared_future<int> sf(boost::move(fi));
    assert(fi.get_state()==boost::future_state::uninitialized);

    pt();

    int i=0;
    assert(sf.is_ready());
    assert(sf.has_value());
    assert(!sf.has_exception());
    assert(sf.get_state()==boost::future_state::ready);
    assert(i=sf.get());
    assert(i==42);
}

void test_copies_of_shared_future_become_ready_together()
{
    boost::packaged_task<int> pt(make_int);
    boost::unique_future<int> fi=pt.get_future();

    boost::shared_future<int> sf(boost::move(fi));
    boost::shared_future<int> sf2(sf);
    boost::shared_future<int> sf3;
    sf3=sf;
    assert(sf.get_state()==boost::future_state::waiting);
    assert(sf2.get_state()==boost::future_state::waiting);
    assert(sf3.get_state()==boost::future_state::waiting);

    pt();

    int i=0;
    assert(sf.is_ready());
    assert(sf.has_value());
    assert(!sf.has_exception());
    assert(sf.get_state()==boost::future_state::ready);
    assert(i=sf.get());
    assert(i==42);
    i=0;
    assert(sf2.is_ready());
    assert(sf2.has_value());
    assert(!sf2.has_exception());
    assert(sf2.get_state()==boost::future_state::ready);
    assert(i=sf2.get());
    assert(i==42);
    i=0;
    assert(sf3.is_ready());
    assert(sf3.has_value());
    assert(!sf3.has_exception());
    assert(sf3.get_state()==boost::future_state::ready);
    assert(i=sf3.get());
    assert(i==42);
}

void test_shared_future_can_be_move_assigned_from_unique_future()
{
    boost::packaged_task<int> pt(make_int);
    boost::unique_future<int> fi=pt.get_future();

    boost::shared_future<int> sf;
    sf=boost::move(fi);
    assert(fi.get_state()==boost::future_state::uninitialized);

    int i=0;
    assert(!sf.is_ready());
    assert(!sf.has_value());
    assert(!sf.has_exception());
    assert(sf.get_state()==boost::future_state::waiting);
}

void test_shared_future_void()
{
    boost::packaged_task<void> pt(do_nothing);
    boost::unique_future<void> fi=pt.get_future();

    boost::shared_future<void> sf(boost::move(fi));
    assert(fi.get_state()==boost::future_state::uninitialized);

    pt();

    assert(sf.is_ready());
    assert(sf.has_value());
    assert(!sf.has_exception());
    assert(sf.get_state()==boost::future_state::ready);
    sf.get();
}

void test_shared_future_ref()
{
    boost::promise<int&> p;
    boost::shared_future<int&> f(p.get_future());
    int i=42;
    p.set_value(i);
    assert(f.is_ready());
    assert(f.has_value());
    assert(!f.has_exception());
    assert(f.get_state()==boost::future_state::ready);
    assert(&f.get()==&i);
}

void test_can_get_a_second_future_from_a_moved_promise()
{
    boost::promise<int> pi;
    boost::unique_future<int> fi=pi.get_future();
    
    boost::promise<int> pi2(boost::move(pi));
    boost::unique_future<int> fi2=pi.get_future();

    pi2.set_value(3);
    assert(fi.is_ready());
    assert(!fi2.is_ready());
    assert(fi.get()==3);
    pi.set_value(42);
    assert(fi2.is_ready());
    assert(fi2.get()==42);
}

void test_can_get_a_second_future_from_a_moved_void_promise()
{
    boost::promise<void> pi;
    boost::unique_future<void> fi=pi.get_future();
    
    boost::promise<void> pi2(boost::move(pi));
    boost::unique_future<void> fi2=pi.get_future();

    pi2.set_value();
    assert(fi.is_ready());
    assert(!fi2.is_ready());
    pi.set_value();
    assert(fi2.is_ready());
}

void test_unique_future_for_move_only_udt()
{
    boost::promise<X> pt;
    boost::unique_future<X> fi=pt.get_future();

    pt.set_value(X());
    X res(fi.get());
    assert(res.i==42);
}

void test_unique_future_for_string()
{
    boost::promise<std::string> pt;
    boost::unique_future<std::string> fi=pt.get_future();

    pt.set_value(std::string("hello"));
    std::string res(fi.get());
    assert(res=="hello");

    boost::promise<std::string> pt2;
    fi=pt2.get_future();

    std::string const s="goodbye";
    
    pt2.set_value(s);
    res=fi.get();
    assert(res=="goodbye");

    boost::promise<std::string> pt3;
    fi=pt3.get_future();

    std::string s2="foo";
    
    pt3.set_value(s2);
    res=fi.get();
    assert(res=="foo");
}

boost::mutex callback_mutex;
unsigned callback_called=0;

void wait_callback(boost::promise<int>& pi)
{
    boost::lock_guard<boost::mutex> lk(callback_mutex);
    ++callback_called;
    try
    {
        pi.set_value(42);
    }
    catch(...)
    {
    }
}

void do_nothing_callback(boost::promise<int>& pi)
{
    boost::lock_guard<boost::mutex> lk(callback_mutex);
    ++callback_called;
}

void test_wait_callback()
{
    callback_called=0;
    boost::promise<int> pi;
    boost::unique_future<int> fi=pi.get_future();
    pi.set_wait_callback(wait_callback);
    fi.wait();
    assert(callback_called);
    assert(fi.get()==42);
    fi.wait();
    fi.wait();
    assert(callback_called==1);
}

void test_wait_callback_with_timed_wait()
{
    callback_called=0;
    boost::promise<int> pi;
    boost::unique_future<int> fi=pi.get_future();
    pi.set_wait_callback(do_nothing_callback);
    bool success=fi.timed_wait(boost::posix_time::milliseconds(10));
    assert(callback_called);
    assert(!success);
    success=fi.timed_wait(boost::posix_time::milliseconds(10));
    assert(!success);
    success=fi.timed_wait(boost::posix_time::milliseconds(10));
    assert(!success);
    assert(callback_called==3);
    pi.set_value(42);
    success=fi.timed_wait(boost::posix_time::milliseconds(10));
    assert(success);
    assert(callback_called==3);
    assert(fi.get()==42);
    assert(callback_called==3);
}

void test_wait_callback_with_timed_wait_until()
{
    callback_called=0;
    boost::promise<int> pi;
    boost::unique_future<int> fi=pi.get_future();
    pi.set_wait_callback(do_nothing_callback);
    bool success=fi.timed_wait_until(boost::get_system_time() + boost::posix_time::milliseconds(10));
    assert(callback_called);
    assert(!success);
    success=fi.timed_wait_until(boost::get_system_time() + boost::posix_time::milliseconds(10));
    assert(!success);
    success=fi.timed_wait_until(boost::get_system_time() + boost::posix_time::milliseconds(10));
    assert(!success);
    assert(callback_called==3);
    pi.set_value(42);
    success=fi.timed_wait_until(boost::get_system_time() + boost::posix_time::milliseconds(10));
    assert(success);
    assert(callback_called==3);
    assert(fi.get()==42);
    assert(callback_called==3);
}


void wait_callback_for_task(boost::packaged_task<int>& pt)
{
    boost::lock_guard<boost::mutex> lk(callback_mutex);
    ++callback_called;
    try
    {
        pt();
    }
    catch(...)
    {
    }
}


void test_wait_callback_for_packaged_task()
{
    callback_called=0;
    boost::packaged_task<int> pt(make_int);
    boost::unique_future<int> fi=pt.get_future();
    pt.set_wait_callback(wait_callback_for_task);
    fi.wait();
    assert(callback_called);
    assert(fi.get()==42);
    fi.wait();
    fi.wait();
    assert(callback_called==1);
}

void test_packaged_task_can_be_moved()
{
    boost::packaged_task<int> pt(make_int);

    boost::unique_future<int> fi=pt.get_future();

    assert(!fi.is_ready());
    
    boost::packaged_task<int> pt2=boost::move(pt);
    assert(!fi.is_ready());
    try
    {
        pt();
        assert(!"Can invoke moved task!");
    }
    catch(boost::task_moved&)
    {
    }

    assert(!fi.is_ready());

    pt2();
    
    assert(fi.is_ready());
}

void test_destroying_a_promise_stores_broken_promise()
{
    boost::unique_future<int> f;
    
    {
        boost::promise<int> p;
        f=p.get_future();
    }
    assert(f.is_ready());
    assert(f.has_exception());
    try
    {
        f.get();
    }
    catch(boost::broken_promise&)
    {
    }
}

void test_destroying_a_packaged_task_stores_broken_promise()
{
    boost::unique_future<int> f;
    
    {
        boost::packaged_task<int> p(make_int);
        f=p.get_future();
    }
    assert(f.is_ready());
    assert(f.has_exception());
    try
    {
        f.get();
    }
    catch(boost::broken_promise&)
    {
    }
}

int make_int_slowly()
{
    boost::this_thread::sleep(boost::posix_time::seconds(1));
    return 42;
}

int main()
{
    test_initial_state();
    test_waiting_future();
    test_cannot_get_future_twice();
   test_set_value_updates_future_state();
   test_set_value_can_be_retrieved();
   test_set_value_can_be_moved();
   test_store_value_from_thread();
   test_store_exception();
   test_future_from_packaged_task_is_waiting();
   test_invoking_a_packaged_task_populates_future();
   test_invoking_a_packaged_task_twice_throws();
   test_cannot_get_future_twice_from_task();
   test_task_stores_exception_if_function_throws();
   test_void_promise();
   test_reference_promise();
   test_task_returning_void();
   test_task_returning_reference();
   test_shared_future();
   test_copies_of_shared_future_become_ready_together();
  test_shared_future_can_be_move_assigned_from_unique_future();
  test_shared_future_void();
  test_shared_future_ref();
   test_can_get_a_second_future_from_a_moved_promise();
   test_can_get_a_second_future_from_a_moved_void_promise();
   test_unique_future_for_move_only_udt();
   test_unique_future_for_string();
   test_wait_callback();
   test_wait_callback_with_timed_wait();
   test_wait_callback_with_timed_wait_until();
   test_wait_callback_for_packaged_task();
   test_packaged_task_can_be_moved();
   test_destroying_a_promise_stores_broken_promise();
   test_destroying_a_packaged_task_stores_broken_promise();
}
