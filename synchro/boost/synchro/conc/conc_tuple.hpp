//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_CONC_TUPLE__HPP
#define BOOST_SYNCHRO_CONC_TUPLE__HPP

#include <boost/thread.hpp>
#include <boost/ref.hpp>
#include <boost/bind.hpp>

namespace boost { namespace synchro {
struct none{};
#if BOOST_HAS_VARIADIC_TMPl
template <
    typename S...
>
struct conc_tuple;

template <
    typename Head, Tail...
>
struct conc_tuple;
{
    Head& head_;
    boost::thread thr_head_;
    conc_tuple<Tail> tail_;
public:
    conc_tuple(Head& head, Tail... tail) : head_(head), tail_(...tail) {};
    void operator()() {
        head_();
        tail_();
    }
    void join() {
        thr_head_.join();
        tail_.join();
    }
    void interrupt() {
        thr_head_.interrupt();
        tail_.interrupt();
    }
    int size() const {return 1;}
};
#endif

//[conc_tuple
template <
    typename S1
>
struct conc_tuple_1 {
//    BOOST_CONCEPT_ASSERT((ConcurrentComponentConcept<S1>));
    S1& s1_;
    boost::thread thr1_;
public:
    conc_tuple_1(S1& s1) : s1_(s1) {};
    void operator()() {
        thr1_= boost::thread(boost::ref(s1_));
    }
    void join() {
        thr1_.join();
    }
    void interrupt() {
        thr1_.interrupt();
    }
    int size() const {return 1;}
};

template <
    typename S1,
    typename S2
>
struct conc_tuple_2 {
//    BOOST_CONCEPT_ASSERT((ConcurrentComponentConcept<S1>));
    conc_tuple_1<S1> s1_;
    conc_tuple_1<S2> s2_;
//    S1& s1_; S2& s2_;
//    boost::thread thr1_;
//    boost::thread thr2_;
public:
    conc_tuple_2(S1& s1, S2& s2) : s1_(s1), s2_(s2) {};
    void operator()() {
        s1_();
        s2_();
    }
    void join() {
        s1_.join();
        s2_.join();
    }
    void interrupt() {
        s1_.interrupt();
        s2_.interrupt();
    }
    int size() const {return 2;}
};
template <
    typename S1,
    typename S2,
    typename S3
>
struct conc_tuple_3 {
    conc_tuple_1<S1> s1_;
    conc_tuple_2<S2, S3> s23_;
public:
    conc_tuple_3(S1& s1, S2& s2, S3& s3) : s1_(s1), s23_(s2, s3) {};
    void operator()() {
        s1_();
        s23_();
    }
    void join() {
        s1_.join();
        s23_.join();
    }
    void interrupt() {
        s1_.interrupt();
        s23_.interrupt();
    }
    int size() const {return 3;}
};
template <
    typename S1,
    typename S2,
    typename S3,
    typename S4
>
struct conc_tuple_4 {
    conc_tuple_2<S1, S2> s12_;
    conc_tuple_2<S3, S4> s34_;
public:
    conc_tuple_4(S1& s1, S2& s2, S3& s3, S4& s4) : s12_(s1, s2), s34_(s3, s4) {};
    void operator()() {
        s12_();
        s34_();
    }
    void join() {
        s12_.join();
        s34_.join();
    }
    void interrupt() {
        s12_.interrupt();
        s34_.interrupt();
    }
    int size() const {return 4;}
};
//]

template <
    typename S1,
    typename S2,
    typename S3=none,
    typename S4=none,
    typename S5=none
>
struct conc_tuple;

template <
    typename S1,
    typename S2,
    typename S3,
    typename S4
>
struct conc_tuple<S1, S2, S3, S4, none> : conc_tuple_4<S1, S2, S3, S4> {
    conc_tuple(S1& s1, S2& s2, S3& s3, S4& s4) : conc_tuple_4<S1, S2, S3, S4>(s1, s2, s3, s4) {};
};

template <
    typename S1,
    typename S2,
    typename S3
>
struct conc_tuple<S1, S2, S3, none, none> : conc_tuple_3<S1, S2, S3>{
    conc_tuple(S1& s1, S2& s2, S3& s3) : conc_tuple_3<S1, S2, S3>(s1, s2, s3) {};
};

template <
    typename S1,
    typename S2
>
struct conc_tuple<S1, S2, none, none, none> : conc_tuple_2<S1, S2>{
    conc_tuple(S1& s1, S2& s2) : conc_tuple_2<S1, S2>(s1, s2) {};
};

}
}
#endif
