// Copyright 2008 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW__UTILITY__BIND_FUNCTOR_HPP
#define BOOST_DATAFLOW__UTILITY__BIND_FUNCTOR_HPP


#include <boost/bind.hpp>
#include <boost/bind/placeholders.hpp>
#include <boost/function.hpp>


namespace boost { namespace dataflow {

namespace utility {

    struct bind_functor
    {
        typedef boost::function<void()> result_type;
        
        template<typename T0>
        result_type operator()(const T0 &t0)
        {
            return boost::bind(t0);
        }

        template<typename T0, typename T1>
        result_type operator()(const T0 &t0, const T1 &t1)
        {
            return boost::bind(t0, t1);
        }

        template<typename T0, typename T1, typename T2>
        result_type operator()(const T0 &t0, const T1 &t1, const T2 &t2)
        {
            return boost::bind(t0, t1, t2);
        }

        template<typename T0, typename T1, typename T2, typename T3>
        result_type operator()(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3)
        {
            return boost::bind(t0, t1, t2, t3);
        }
        
        //...
    };

} // namespace utility

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW__UTILITY__BIND_FUNCTOR_HPP
