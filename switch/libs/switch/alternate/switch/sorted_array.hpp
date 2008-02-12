// map.hpp
//
// Copyright (c) 2008
// Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_SWITCH_SORTED_ARRAY_HPP_INCLUDED
#define BOOST_SWITCH_SORTED_ARRAY_HPP_INCLUDED

#include <algorithm>
//#include <boost/thread/once.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>

namespace boost {

namespace sorted_array_switch {

// requires that K and V are POD
template<class K, class V>
struct map_element {
    K k;
    V v;
    bool operator<(const map_element& other) const {
        return(k < other.k);
    }
    bool operator<(const K& other) const {
        return(k < other);
    }
    friend bool operator<(const K& other, const map_element& self) {
        return(other < self.k);
    }
};

template<class R, class I, class F>
struct switch_impl {
    static map_element<I, R(*)(const F&)> impl[mpl::size<typename F::labels>::value];
    //static boost::once_flag init_flag;
    struct init_impl {
        init_impl() {
            init();
        }
    };
    static R (*get(I i))(const F&) {
        //boost::call_once(&init, init_flag);
        static init_impl initialize;
        map_element<I, R(*)(const F&)>* result = std::lower_bound(&impl[0], &impl[0] + mpl::size<typename F::labels>::value, i);
        if(result == &impl[0] + mpl::size<typename F::labels>::value || result->k != i) {
            return(0);
        } else {
            return(result->v);
        }
    }
    static void init() {
        boost::mpl::for_each<mpl::range_c<int, 0, mpl::size<typename F::labels>::value> >(do_init());
        std::sort(&impl[0], &impl[0] + mpl::size<typename F::labels>::value);
    }
    struct do_init {
        template<class T>
        void operator()(T) {
            impl[T::value].k = static_cast<I>(mpl::at_c<typename F::labels, T::value>::type::value);
            impl[T::value].v = &f_impl<typename mpl::at_c<typename F::labels, T::value>::type>::call;
        }
    };
    template<class T>
    struct f_impl {
        static R call(const F& f) {
            return(f.template apply<R>(T()));
        }
    };
};

template<class R, class I, class F>
map_element<I, R(*)(const F&)> switch_impl<R, I, F>::impl[mpl::size<typename F::labels>::value];
//template<class R, class I, class F>
//boost::once_flag switch_impl<R, I, F>::init_flag = BOOST_ONCE_INIT;

template<class R, class I, class F, class Default>
R switch_(I i, const F& f, Default default_) {
    R (*impl)(const F&) = switch_impl<R, I, F>::get(i);
    if(impl == 0) {
        return(default_(i));
    } else {
        return(impl(f));
    }
}


}

}

#endif
