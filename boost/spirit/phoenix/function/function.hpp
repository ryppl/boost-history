/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_FUNCTION_FUNCTION_HPP
#define PHOENIX_FUNCTION_FUNCTION_HPP

#include <boost/spirit/phoenix/core/compose.hpp>

namespace boost { namespace phoenix
{
    //  Bring in the impl::function_eval<0..N> definitions
    #include <boost/spirit/phoenix/function/impl/function_eval.ipp>

    template <typename F>
    struct function
    {
        function() : f() {}
        function(F const& f) : f(f) {}

        actor<typename as_composite<impl::function_eval<0>, F>::type>
        operator()() const
        {
            return compose<impl::function_eval<0> >(f);
        }

        template <typename A0>
        actor<typename as_composite<impl::function_eval<1>, F, A0>::type>
        operator()(A0 const& _0) const
        {
            return compose<impl::function_eval<1> >(f, _0);
        }

        template <typename A0, typename A1>
        actor<typename as_composite<impl::function_eval<2>, F, A0, A1>::type>
        operator()(A0 const& _0, A1 const& _1) const
        {
            return compose<impl::function_eval<2> >(f, _0, _1);
        }

        //  Bring in the rest of the function call operators
        #include <boost/spirit/phoenix/function/impl/function_call.ipp>

        F f;
    };
}}

#endif
