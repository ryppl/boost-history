// map.hpp
//
// Copyright (c) 2008
// Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_SWITCH_MAP_HPP_INCLUDED
#define BOOST_SWITCH_MAP_HPP_INCLUDED

#include <map>
//#include <boost/thread/once.hpp>
#include <boost/mpl/for_each.hpp>

namespace boost {

namespace map_switch {

template<class R, class I, class F>
struct switch_impl {
    static std::map<I, R(*)(const F&)>* impl;
    //static boost::once_flag init_flag;
    struct init_impl {
        init_impl() {
            init();
        }
    };
    static const std::map<I, R(*)(const F&)>& get() {
        //boost::call_once(&init, init_flag);
        static init_impl initialize;
        return(*impl);
    }
    static void init() {
        impl = new std::map<I, R(*)(const F&)>();
        boost::mpl::for_each<typename F::labels>(do_init());
    }
    struct do_init {
        template<class T>
        void operator()(T) {
            impl->insert(std::make_pair(static_cast<I>(T::value), &f_impl<T>::call));
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
std::map<I, R(*)(const F&)>* switch_impl<R, I, F>::impl;
//template<class R, class I, class F>
//boost::once_flag switch_impl<R, I, F>::init_flag = BOOST_ONCE_INIT;

template<class R, class I, class F, class Default>
R switch_(I i, const F& f, Default default_) {
    const std::map<I, R(*)(const F&)>& m = switch_impl<R, I, F>::get();
    typename std::map<I, R(*)(const F&)>::const_iterator iter = m.find(i);
    if(iter == m.end()) {
        return(default_(i));
    } else {
        return(iter->second(f));
    }
}


}

}

#endif
