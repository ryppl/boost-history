/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_CORE_ARG_LIST_HPP
#define PHOENIX_CORE_ARG_LIST_HPP

#include <boost/spirit/fusion/sequence/tuple.hpp>
#include <boost/spirit/fusion/sequence/tuple_element.hpp>
#include <boost/spirit/fusion/sequence/tuple_size.hpp>
#include <boost/spirit/phoenix/core/impl/macro_utils.hpp>
#include <boost/mpl/at.hpp>

#if !defined(PHOENIX_ARG_LIMIT)
# define PHOENIX_ARG_LIMIT PHOENIX_LIMIT
#endif

//  include mpl::vector<N> 0..N where N is PHOENIX_ARG_LIMIT
#include <boost/mpl/vector/vector10.hpp>
#if (PHOENIX_ARG_LIMIT > 10)
#include <boost/mpl/vector/vector20.hpp>
#endif
#if (PHOENIX_ARG_LIMIT > 20)
#include <boost/mpl/vector/vector30.hpp>
#endif
#if (PHOENIX_ARG_LIMIT > 30)
#include <boost/mpl/vector/vector40.hpp>
#endif
#if (PHOENIX_ARG_LIMIT > 40)
#include <boost/mpl/vector/vector50.hpp>
#endif

namespace boost { namespace phoenix
{
    using boost::fusion::void_t;
    using boost::fusion::tuple;

///////////////////////////////////////////////////////////////////////////////
//
//  arg_list class
//
//      Basically, an arg_list is a tuple of references (a tie). Why don't we
//      just use a tuple? Well, the elements of a tie are all references.
//      Example:
//
//          arg_list<int, char> ---> tuple<int&, char&>
//
//      On some occassions, we wish to know the actual type of an argument,
//      reference removed. We could strip the reference using boost::
//      remove_reference. However, there might be cases where we really want
//      an argument which is already a reference. In such a case, and since
//      we can't have a type which is a reference to a reference, we can't
//      really know if the actual type of an argument is a reference, given a
//      tuple of references.
//
//      What we need to do is to preserve the actual types of the arguments,
//      minus the reference. The arg_list has a typedef "types" which is an
//      mpl::vector of the actual argument types, reference stripped.
//
///////////////////////////////////////////////////////////////////////////////
    template <PHOENIX_TEMPLATE_WITH_DEFAULT(PHOENIX_ARG_LIMIT, T)>
    struct arg_list : tuple<PHOENIX_TIE_LIST(PHOENIX_ARG_LIMIT, T)>
    {
        typedef tuple<PHOENIX_TIE_LIST(PHOENIX_ARG_LIMIT, T)> tie_t;
        typedef BOOST_PP_CAT(mpl::vector, PHOENIX_ARG_LIMIT)
            <PHOENIX_TYPE_LIST(PHOENIX_ARG_LIMIT, T)> types;

        arg_list()
            : tie_t() {}

        template <typename TB0>
        arg_list(TB0& _0)
            : tie_t(_0) {}

        template <typename TB0, typename TB1>
        arg_list(TB0& _0, TB1& _1)
            : tie_t(_0, _1) {}

        //  Bring in the rest of the constructors
        #include <boost/spirit/phoenix/core/impl/arg_list_constructor.ipp>
    };

///////////////////////////////////////////////////////////////////////////////
//
//  arg class
//
//      Metafunction: given an int N and an arg_list, return the type of the
//      Nth argument.
//
///////////////////////////////////////////////////////////////////////////////
    template <int N, typename Args>
    struct arg
    {
        typedef typename
            mpl::at_c<typename Args::types, N>::type
        type;
    };

///////////////////////////////////////////////////////////////////////////////
//
//  arity class
//
//      Metafunction: given an arg_list, return the number of arguments.
//
///////////////////////////////////////////////////////////////////////////////
    template <typename Args>
    struct arity : fusion::tuple_size<Args> {};
}}

#endif

