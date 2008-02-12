// binary_search.hpp
//
// Copyright (c) 2008
// Steven Watanabe
//
// Distriuted under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost/org/LICENSE_1_0.txt)

#ifndef BOOST_SWITCH_BINARY_SEARCH_HPP_INCLUDED
#define BOOST_SWITCH_BINARY_SEARCH_HPP_INCLUDED

#include <boost/mpl/size.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/deref.hpp>

namespace boost {

namespace binary_search_switch {

template<int N>
struct list_at {
    template<class L>
    struct apply {
        typedef typename list_at<N - 1>::template apply<L>::type::tail type;
    };
};

template<>
struct list_at<0> {
    template<class L>
    struct apply {
        typedef L type;
    };
};

struct list_end {};

template<class T, class Tail>
struct list {
    typedef T element;
    typedef Tail tail;
};

template<class List, class T>
struct insert;

template<bool>
struct insert_impl;

template<>
struct insert_impl<true> {
    template<class List, class T>
    struct apply {
        typedef list<T, List> type;
    };
};

template<>
struct insert_impl<false> {
    template<class List, class T>
    struct apply {
        typedef list<typename List::element, typename insert<typename List::tail, T>::type> type;
    };
};

template<class List, class T>
struct insert {
    typedef typename insert_impl<(T::value) < (List::element::value)>::template apply<List, T>::type type;
};

template<class T>
struct insert<list_end, T> {
    typedef list<T, list_end> type;
};

template<int Low, int High, class S, int Diff = (High - Low)>
struct switch_impl {
    static const int mid = Low + Diff / 2;
    typedef typename list_at<mid>::template apply<S>::type::element mid_t;
    template<class R, class I, class F, class D>
    static R apply(I i, F& f, D& d) {
        if(mid_t::value < i) {
            return(switch_impl<mid, High, S>::template apply<R>(i, f, d));
        } else {
            return(switch_impl<Low, mid, S>::template apply<R>(i, f, d));
        }
    }
};

template<int Low, int High, class S>
struct switch_impl<Low, High, S, 0> {
    template<class R, class I, class F, class D>
    static R apply(I i, F& f, D& d) {
        return(d(i));
    }
};

template<int Low, int High, class S>
struct switch_impl<Low, High, S, 2> {
    typedef typename list_at<Low>::template apply<S>::type::element first_t;
    typedef typename list_at<Low + 1>::template apply<S>::type::element second_t;
    template<class R, class I, class F, class D>
    static R apply(I i, F& f, D& d) {
        if(first_t::value == i) {
            return(f.template apply<R>(first_t()));
        } else if(second_t::value == i) {
            return(f.template apply<R>(second_t()));
        } else {
            return(d(i));
        }
    }
};

template<int Low, int High, class S>
struct switch_impl<Low, High, S, 1> {
    typedef typename list_at<Low>::template apply<S>::type::element mid_t;
    template<class R, class I, class F, class D>
    static R apply(I i, F& f, D& d) {
        if(mid_t::value == i) {
            return(f.template apply<R>(mid_t()));
        } else {
            return(d(i));
        }
    }
};

template<int Low, int High, class S>
struct switch_impl<Low, High, S, 3> {
    static const int mid = Low + 1;
    typedef typename list_at<mid>::template apply<S>::type::element mid_t;
    template<class R, class I, class F, class D>
    static R apply(I i, F& f, D& d) {
        if(mid_t::value < i) {
            return(switch_impl<mid + 1, High, S>::template apply<R>(i, f, d));
        } else if(mid_t::value > i) {
            return(switch_impl<Low, mid, S>::template apply<R>(i, f, d));
        } else {
            return(f.template apply<R>(mid_t()));
        }
    }
};

template<int N>
struct sort {
    template<class Iter>
    struct apply {
        typedef typename mpl::next<Iter>::type next1;
        typedef typename mpl::next<next1>::type next2;
        typedef typename mpl::next<next2>::type next3;
        typedef typename mpl::next<next3>::type next4;
        typedef typename insert<
                typename insert<
                typename insert<
                typename insert<typename sort<N - 4>::template apply<next4>::type,
                typename mpl::deref<next3>::type>::type,
                typename mpl::deref<next2>::type>::type,
                typename mpl::deref<next1>::type>::type,
                typename mpl::deref<Iter>::type>::type
            type;
    };
};

template<>
struct sort<0> {
    template<class Iter>
    struct apply {
        typedef list_end type;
    };
};

template<>
struct sort<1> {
    template<class Iter>
    struct apply {
        typedef list<typename mpl::deref<Iter>::type, list_end> type;
    };
};

template<>
struct sort<2> {
    template<class Iter>
    struct apply {
        typedef typename mpl::next<Iter>::type next1;
        typedef typename insert<
                list<typename mpl::deref<next1>::type, list_end>,
                typename mpl::deref<Iter>::type>::type
            type;
    };
};

template<>
struct sort<3> {
    template<class Iter>
    struct apply {
        typedef typename mpl::next<Iter>::type next1;
        typedef typename mpl::next<next1>::type next2;
        typedef typename insert<
                typename insert<
                list<typename mpl::deref<next2>::type, list_end>,
                typename mpl::deref<next1>::type>::type,
                typename mpl::deref<Iter>::type>::type
            type;
    };
};

template<class R, class I, class F, class D>
R switch_(I i, const F& f, D d) {
    typedef switch_impl<0, mpl::size<typename F::labels>::value,
        typename sort<mpl::size<typename F::labels>::value>::template apply<typename mpl::begin<typename F::labels>::type>::type> impl;
    return(impl::template apply<R>(i, f, d));
}

}

}

#endif
