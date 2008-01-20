// profiler.hpp

// Boost Logging library
//
// Author: John Torjo, www.torjo.com
//
// Copyright (C) 2007 John Torjo (see www.torjo.com for email)
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.
// See http://www.torjo.com/log2/ for more details

/** 
    Allows you to profile your application' logging

    That is, see how much CPU time is taken for logging
*/

#ifndef JT28092007_profiler_HPP_DEFINED
#define JT28092007_profiler_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>

#include <boost/date_time/microsec_time_clock.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/function.hpp>
#include <string>
#include <sstream>

namespace boost { namespace logging { namespace profile {


/** 
    @brief Computes profiling information, and can show it to the user

*/
struct compute {
    enum type {
        gather = 0,
        writer,
        filter,
        on_other_thread,

        last_ = on_other_thread
    };

    typedef ::boost::function<void(const std::string&)> log_function;
    static void nothing(const std::string&) {}

    static compute & inst() {
        static compute c;
        return c;
    }

    void add_period(const ::boost::int64_t & mcs, type t) {
        // note : this will work even after this object has been destroyed (since it's a global)
        m_cpu_times[t] += mcs;
    }

    void log_results(log_function l) {
        m_log = l;
    }
protected:
    compute() {
        m_log = &nothing;
    }
    ~compute() {
        dump_results();
    }
private:
    void dump_results() {
        std::ostringstream out;
        out << "gather time:      " 
            << (m_cpu_times[gather] / 1000000) << "." << (m_cpu_times[gather] % 1000000) << " seconds " << std::endl;
        out << "write time:       " 
            << (m_cpu_times[writer] / 1000000) << "." << (m_cpu_times[writer] % 1000000) << " seconds " << std::endl;
        out << "filter time:      " 
            << (m_cpu_times[filter] / 1000000) << "." << (m_cpu_times[filter] % 1000000) << " seconds " << std::endl;
        out << "otherthread time: " 
            << (m_cpu_times[on_other_thread] / 1000000) << "." << (m_cpu_times[on_other_thread] % 1000000) << " seconds " << std::endl;
        m_log( out.str() );
    }

private:
    compute(const compute&);
    void operator=(const compute&);

private:
    // the CPU time taken for each activity
    //
    // note: we don't use std::vector; we want this to be available even after destruction
    // (if logs are used after they've been destroyed
    boost::int64_t m_cpu_times[ last_ + 1];

    // where should we dump the results?
    log_function m_log;
};



struct scoped_compute {
    compute& m_comp;
    compute::type m_type;

    ::boost::posix_time::ptime m_start, m_end;

    scoped_compute(compute& comp, compute::type type) : m_comp(comp), m_type(type) {
        m_start = ::boost::posix_time::microsec_clock::local_time();
    }
    ~scoped_compute() {
        m_end = ::boost::posix_time::microsec_clock::local_time();
        m_comp.add_period( (m_end - m_start).total_microseconds() , m_type); 
    }

};


template<class msg_type, class base_type> struct on_dedicated_thread ;

template<class gather_msg> struct compute_gather : gather_msg {
    compute_gather() : m_lk( get_compute(), compute::gather) {}
    compute_gather(const compute_gather& other) : : m_lk( get_compute(), compute::gather), gather_msg(other) {}

    compute& get_compute() const { return compute::inst(); }
private:
    scoped_compute m_lk;
};

template<class writer_msg> struct compute_write : writer_msg {
    compute& get_compute() const { return compute::inst(); }

    template<class msg_type> void operator()(const msg_type& msg) const {
        scoped_compute lk( get_compute(), compute::writer );
        writer_msg::operator()(msg);
    }

    // just in case you do write on another thread
    virtual void write_array() const {
        scoped_compute lk( get_compute(), compute::on_other_thread );
        write_array_impl( *this);
    }
private:
    template<class msg_type, class base_type> void write_array_impl(const on_dedicated_thread<msg_type,base_type> &) const {
        // call base class's implementation
        writer_msg::write_array();
    }

    // does not derive from on_dedicated_thread
    void write_array_impl(...) const {}
};

/** 
    In case you want to profile your filter, there's just one requirement:
    - your function must be called @c is_enabled() and be const
*/
template<class filter_msg> struct compute_filter : filter_msg {
    // is_enabled - for any up to 5 params - const function
    compute& get_compute() const { return compute::inst(); }

    bool is_enabled() const {
        scoped_compute lk( get_compute(), compute::filter );
        return filter_msg::is_enabled();
    }
    template<class p1> bool is_enabled(const p1 & v1) const {
        scoped_compute lk( get_compute(), compute::filter );
        return filter_msg::is_enabled(v1);
    }
    template<class p1, class p2> bool is_enabled(const p1 & v1, const p2 &v2) const {
        scoped_compute lk( get_compute(), compute::filter );
        return filter_msg::is_enabled(v1, v2);
    }
    template<class p1, class p2, class p3> bool is_enabled(const p1 & v1, const p2 &v2, const p3 & v3) const {
        scoped_compute lk( get_compute(), compute::filter );
        return filter_msg::is_enabled(v1, v2, v3);
    }
    template<class p1, class p2, class p3, class p4> bool is_enabled(const p1 & v1, const p2 &v2, const p3 & v3, const p4 & v4) const {
        scoped_compute lk( get_compute(), compute::filter );
        return filter_msg::is_enabled(v1, v2, v3, v4);
    }
    template<class p1, class p2, class p3, class p4, class p5> bool is_enabled(const p1 & v1, const p2 &v2, const p3 & v3, const p4 & v4, class p5 & v5) const {
        scoped_compute lk( get_compute(), compute::filter );
        return filter_msg::is_enabled(v1, v2, v3, v4, v5);
    }

};


}}}

#endif

