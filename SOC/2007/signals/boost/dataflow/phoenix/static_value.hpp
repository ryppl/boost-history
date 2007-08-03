// Copyright 2007 Stjepan Rajko.
// Copyright (c) 2001-2007 Joel de Guzman
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_PHOENIX_STATIC_VALUE_HPP
#define BOOST_DATAFLOW_PHOENIX_STATIC_VALUE_HPP

#include <boost/mpl/int.hpp>
#include <boost/spirit/phoenix/core/actor.hpp>

namespace boost { namespace phoenix {
    
template <typename T>
struct static_value
{
    typedef mpl::false_ no_nullary;
    
    template <typename Env>
    struct result
    {
        typedef T type;
    };
    
    template <typename Env>
    T eval(Env const&) const
    {
        return T();
    }
};

template<int N>
actor<static_value<boost::mpl::int_<N> > > int_()
{
    return actor<static_value<boost::mpl::int_<N> > >();
}

} } // namespace boost::phoenix

#endif // BOOST_DATAFLOW_PHOENIX_STATIC_VALUE_HPP
