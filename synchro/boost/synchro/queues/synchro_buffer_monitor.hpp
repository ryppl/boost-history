//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_CONC_BUFFER__HPP
#define BOOST_SYNCHRO_CONC_BUFFER__HPP

#include <boost/synchro/monitor.hpp>

namespace boost { namespace synchro {

#if 0
//[sync_buffer_schema
class sync_buffer {
    boost::mutex mtx_; /*< explicit mutex declaration >*/
public:
    ...
    bool full() { return in_==out_; }
    bool empty() { return in_==(out_%size)+1; }
    void push(T& v) {
        // wait if buffer is full
        data_[in_]=v;
        in_ = (in_% size)+1;
    }
    T pull() {
        // wait if buffer is empty
        out_ = (out_% size)+1;
        return data_[out_];
    }
};
//]
#endif

//[sync_buffer_monitor
template <typename T, unsigned size>
class sync_buffer : protected exclusive_monitor<>
{
    condition not_full_;
    condition not_empty_;

    T data_[size+1];
    unsigned in_, out_;

    struct  not_full {
        explicit not_full(sync_buffer &b):that_(b){};
        bool operator()() const { return !that_.full(); }
        sync_buffer &that_;
    };
    struct  not_empty {
        explicit not_empty(sync_buffer &b):that_(b){};
        bool operator()() const { return !that_.empty(); }
        sync_buffer &that_;
    };
public:
    sync_buffer():in_(0), out_(0) {}

    bool full() { return out_==(in_+1)%(size+1); }
    bool empty() { return out_==in_; }

    unsigned get_in() {return in_;}
    unsigned get_out() {return out_;}

    void push(T v) {
        synchronizer _(*this->mutex(), not_full_, not_full(*this)); /*< waits until the condition not_full is satisfyed >*/
        data_[in_]=v;
        in_ = (in_+1)% (size+1);
        not_empty_.notify_one(); /*< notifies a not_empty condition >*/
    }

    T pull() {
        synchronizer _(*this->mutex(), not_empty_, not_empty(*this)); /*< waits until the condition not_empty is satisfyed >*/
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
