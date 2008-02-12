// if_else.hpp
//
// Copyright (c) 2008
// Steven Watanabe
//
// Distriuted under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost/org/LICENSE_1_0.txt)

#ifndef BOOST_SWITCH_IF_ELSE_HPP_INCLUDED
#define BOOST_SWITCH_IF_ELSE_HPP_INCLUDED

#include <boost/mpl/size.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/next.hpp>

namespace boost {

namespace if_else_switch {

template<int N>
struct switch_impl {
    template<class R, class Iter, class I, class F, class D>
    static R apply(I i, F& f, D& d) {
        if(mpl::deref<Iter>::type::value == i) {
            typename mpl::deref<Iter>::type x;
            return(f.template apply<R>(x));
        } else {
            return(switch_impl<N - 1>::template apply<R, typename mpl::next<Iter>::type>(i, f, d));
        }
    }
};

template<>
struct switch_impl<0> {
    template<class R, class Iter, class I, class F, class D>
    static R apply(I i, F& f, D& d) {
        return(d(i));
    }
};

template<class R, class I, class F, class D>
R switch_(I i, const F& f, D d) {
    typedef switch_impl<mpl::size<typename F::labels>::value> impl;
    return(impl::template apply<R, typename mpl::begin<typename F::labels>::type>(i, f, d));
};

}

}

#endif
