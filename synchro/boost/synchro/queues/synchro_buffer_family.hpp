//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_QUEUES_SYNCHRO_BUFFER__HPP
#define BOOST_SYNCHRO_QUEUES_SYNCHRO_BUFFER__HPP

#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace boost { namespace synchro {
#if 0
//[sync_buffer_boost_thread_style
template <typename T, unsigned size>
class sync_buffer
{
	typedef boost::mutex mutex_type;
	typedef boost::condition_variable condition_type;
	typedef boost::unique_lock<mutex_type> unique_lock_type;
	mutex_type mtx_;
	condition_type not_full_;
	condition_type not_empty_;

    T data_[size+1];
    unsigned in_, out_;

public:
    sync_buffer():in_(0), out_(0) {}

    bool full() { return out_==(in_+1)%(size+1); }
    bool empty() { return out_==in_; }

    unsigned get_in() {return in_;}
    unsigned get_out() {return out_;}
    void push(T v) {
        unique_lock_type guard(mtx_); /*< ensure the mutex is locked!!! >*/
        while (full()) { /*< loop until not full!!! >*/
            not_full_.wait(guard);
        }
        data_[in_]=v;
        in_ = (in_+1)% (size+1);
        not_empty_.notify_one(); /*< notifies a not_empty condition >*/
    }

    T pull() {
        unique_lock_type guard(mtx_); /*< ensure the mutex is locked!!! >*/
        while (empty()) { /*< loop until not full!!! >*/
            not_empty_.wait(guard);
        }
        unsigned idx = out_;
        out_ = (out_+1)% (size+1);
        not_full_.notify_one(); /*< notifies a not_full condition >*/
        return data_[idx];
    }
};

//]
#endif
//[sync_buffer_family_style
template <typename T, unsigned size, typename Sync>
class sync_buffer
{
	typedef typename Sync::mutex_type mutex_type;
	typedef typename Sync::condition_type condition_type;
	typedef typename boost::unique_lock<mutex_type> unique_lock_type;
	mutex_type mtx_;
	condition_type not_full_;
	condition_type not_empty_;

    T data_[size+1];
    unsigned in_, out_;

public:
    sync_buffer():in_(0), out_(0) {}

    bool full() { return out_==(in_+1)%(size+1); }
    bool empty() { return out_==in_; }

    unsigned get_in() {return in_;}
    unsigned get_out() {return out_;}
    void push(T v) {
        unique_lock_type guard(mtx_); /*< ensure the mutex is locked!!! >*/
        while (full()) { /*< loop until not full!!! >*/
            not_full_.wait(guard);
        }
        data_[in_]=v;
        in_ = (in_+1)% (size+1);
        not_empty_.notify_one(); /*< notifies a not_empty condition >*/
    }

    T pull() {
        unique_lock_type guard(mtx_); /*< ensure the mutex is locked!!! >*/
        while (empty()) { /*< loop until not full!!! >*/
            not_empty_.wait(guard);
        }
        unsigned idx = out_;
        out_ = (out_+1)% (size+1);
        not_full_.notify_one(); /*< notifies a not_full condition >*/
        return data_[idx];
    }
};

//]

}
}
#endif
