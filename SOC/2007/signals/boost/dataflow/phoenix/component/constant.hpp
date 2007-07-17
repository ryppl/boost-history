// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_PHOENIX_COMPONENT_CONSTANT_HPP
#define BOOST_DATAFLOW_PHOENIX_COMPONENT_CONSTANT_HPP

#include <boost/spirit/phoenix/core/actor.hpp>

namespace boost { namespace phoenix {

template <int N>
struct constant
{
    typedef mpl::false_ no_nullary;
    
    template <typename Env>
    struct result
    {
        typedef int type;
    };
    
    template <typename Env>
    int eval(Env const&) const
    {
        return N;
    }
};

} } // namespace boost::phoenix

#endif // BOOST_DATAFLOW_PHOENIX_COMPONENT_CONSTANT_HPP