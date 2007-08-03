// Copyright 2007 Stjepan Rajko.
// Copyright (c) 2001-2007 Joel de Guzman
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_PHOENIX_STATIC_FUNCTION_HPP
#define BOOST_DATAFLOW_PHOENIX_STATIC_FUNCTION_HPP

#include <boost/dataflow/phoenix/static_value.hpp>

#include <boost/spirit/phoenix/core/compose.hpp>
#include <boost/spirit/phoenix/core/detail/function_eval.hpp>

namespace boost { namespace phoenix
{
    template <typename F>
    struct static_function
    {
        actor<typename as_composite<detail::function_eval<0>, actor<static_value<F> > >::type>
        operator()() const
        {
            return compose<detail::function_eval<0> >(actor<static_value<F> >());
        }

        template <typename A0>
        actor<typename as_composite<detail::function_eval<1>, actor<static_value<F> >, A0>::type>
        operator()(A0 const& _0) const
        {
            return compose<detail::function_eval<1> >(actor<static_value<F> >(), _0);
        }

        template <typename A0, typename A1>
        actor<typename as_composite<detail::function_eval<2>, actor<static_value<F> >, A0, A1>::type>
        operator()(A0 const& _0, A1 const& _1) const
        {
            return compose<detail::function_eval<2> >(actor<static_value<F> >(), _0, _1);
        }

        //  Bring in the rest of the function call operators
        #include <boost/dataflow/phoenix/detail/static_function_call.hpp>
    };
}}

#endif