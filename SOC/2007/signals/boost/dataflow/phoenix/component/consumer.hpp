// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_PHOENIX_COMPONENT_CONSUMER_HPP
#define BOOST_DATAFLOW_PHOENIX_COMPONENT_CONSUMER_HPP

#include <boost/utility/result_of.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/spirit/phoenix/core/actor.hpp>

namespace boost { namespace phoenix {

    template <typename T>
    struct consumer
    {
        typedef boost::dataflow::phoenix_consumer consumer_category;

        typedef mpl::false_ no_nullary;
        
        template <typename Env>
        struct result
        {
            typedef const T& type;
        };
        
        consumer(T& arg)
            : ptr(&arg) {}
        consumer()
            : ptr(NULL) {}
        
        template <typename Env>
        const T& eval(Env const&) const
        {
            return *ptr;
        }
        
        mutable const T *ptr;
    };

    template <typename T>
    inline boost::phoenix::actor<consumer<T> > const
    make_consumer(T& v)
    {
        return consumer<T>(v);
    }

} } // namespace boost::phoenix

#endif // BOOST_DATAFLOW_PHOENIX_COMPONENT_CONSUMER_HPP
