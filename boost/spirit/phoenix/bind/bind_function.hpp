/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_BIND_BIND_FUNCTION_HPP
#define PHOENIX_BIND_BIND_FUNCTION_HPP

#include <boost/spirit/phoenix/core/compose.hpp>
#include <boost/spirit/phoenix/core/impl/macro_utils.hpp>
#include <boost/spirit/phoenix/function/function.hpp>

namespace boost { namespace phoenix
{
    //  Bring in the impl::function_ptr_0..N definitions
    #include <boost/spirit/phoenix/bind/impl/function_ptr.ipp>

    template <typename RT>
    inline actor<
        typename as_composite<
            impl::function_eval<0>
          , impl::function_ptr<0, RT, RT(*)()>
        >::type>
    bind(RT(*f)())
    {
        typedef impl::function_ptr<0, RT, RT(*)()> fp_type;
        return compose<impl::function_eval<0> >(fp_type(f));
    }

    template <typename RT, typename T0, typename A0>
    inline actor<
        typename as_composite<
            impl::function_eval<1>
          , impl::function_ptr<1, RT, RT(*)(T0)>
          , A0
        >::type>
    bind(RT(*f)(T0), A0 const& _0)
    {
        typedef impl::function_ptr<1, RT, RT(*)(T0)> fp_type;
        return compose<impl::function_eval<1> >(fp_type(f), _0);
    }

    template <typename RT, typename T0, typename T1, typename A0, typename A1>
    inline actor<
        typename as_composite<
            impl::function_eval<2>
          , impl::function_ptr<2, RT, RT(*)(T0, T1)>
          , A0, A1
        >::type>
    bind(RT(*f)(T0, T1), A0 const& _0, A1 const& _1)
    {
        typedef impl::function_ptr<2, RT, RT(*)(T0, T1)> fp_type;
        return compose<impl::function_eval<2> >(fp_type(f), _0, _1);
    }

    //  Bring in the rest of the function binders
    #include <boost/spirit/phoenix/bind/impl/bind_function.ipp>
}}

#endif
