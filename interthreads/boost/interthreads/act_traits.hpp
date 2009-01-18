#ifndef BOOST_INTERTHREADS_ACT_TRAITS__HPP
#define BOOST_INTERTHREADS_ACT_TRAITS__HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Based on the threader/joiner design from of Kevlin Henney (n1883)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/bind.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/futures/future.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/config/abi_prefix.hpp>


namespace boost {
namespace interthreads {

template <typename ACT>
struct act_value;

template <typename ACT>
struct is_movable : mpl::false_{};

template <typename ACT>
struct has_future_if : mpl::false_{};

template <typename ACT>
struct has_thread_if : mpl::false_{};

template <typename AE, typename T>
struct asynchronous_completion_token {
    typedef typename AE::template handle<T>::type type;
};    

    
template <typename AE>
struct get_future {
    template <typename T>
    shared_future<T>& operator()(typename asynchronous_completion_token<AE,T>::type& act) { return act.get_future(); }
};
    

}
}


#include <boost/config/abi_suffix.hpp>

#endif

