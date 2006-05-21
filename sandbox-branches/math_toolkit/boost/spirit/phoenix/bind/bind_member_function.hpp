/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_BIND_BIND_MEMBER_FUNCTION_HPP
#define PHOENIX_BIND_BIND_MEMBER_FUNCTION_HPP

#include <boost/spirit/phoenix/core/compose.hpp>
#include <boost/spirit/phoenix/core/impl/macro_utils.hpp>
#include <boost/spirit/phoenix/function/function.hpp>

namespace boost { namespace phoenix
{
    //  Bring in the impl::member_function_ptr_0..N definitions
    #include <boost/spirit/phoenix/bind/impl/member_function_ptr.ipp>

    template <typename RT, typename ClassT, typename ClassA>
    inline actor<
        typename as_composite<
            impl::function_eval<1>
          , impl::member_function_ptr<0, RT, RT(ClassT::*)()>
          , ClassA
        >::type>
    bind(RT(ClassT::*f)(), ClassA const& obj)
    {
        typedef impl::member_function_ptr<0, RT, RT(ClassT::*)()> fp_type;
        return compose<impl::function_eval<1> >(fp_type(f), obj);
    }

    template <typename RT, typename ClassT
      , typename T0, typename ClassA, typename A0>
    inline actor<
        typename as_composite<
            impl::function_eval<2>
          , impl::member_function_ptr<1, RT, RT(ClassT::*)(T0)>
          , ClassA, A0
        >::type>
    bind(RT(ClassT::*f)(T0), ClassA const& obj, A0 const& _0)
    {
        typedef impl::member_function_ptr<1, RT, RT(ClassT::*)(T0)> fp_type;
        return compose<impl::function_eval<2> >(fp_type(f), obj, _0);
    }

    template <typename RT, typename ClassT
      , typename T0, typename T1, typename ClassA, typename A0, typename A1>
    inline actor<
        typename as_composite<
            impl::function_eval<3>
          , impl::member_function_ptr<2, RT, RT(ClassT::*)(T0, T1)>
          , ClassA, A0, A1
        >::type>
    bind(RT(ClassT::*f)(T0, T1), ClassA const& obj, A0 const& _0, A1 const& _1)
    {
        typedef impl::member_function_ptr<2, RT, RT(ClassT::*)(T0, T1)> fp_type;
        return compose<impl::function_eval<3> >(fp_type(f), obj, _0, _1);
    }

    //  Bring in the rest of the function binders
    #include <boost/spirit/phoenix/bind/impl/bind_member_function.ipp>
}}

#endif
