/*=============================================================================
    Copyright (c) 2005 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_TUPLE_TUPLE_HPP
#define BOOST_FUSION_TUPLE_TUPLE_HPP

#include <boost/fusion/tuple/tuple_fwd.hpp>
#include <boost/fusion/container/vector/vector.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/value_at.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/sequence/comparison.hpp>
#include <boost/fusion/sequence/io.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/support/ref.hpp>

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/container/detail/pp/forward_ctor.hpp>
#endif

namespace boost { namespace fusion
{
    VARIADIC_TEMPLATE(FUSION_MAX_VECTOR_SIZE)
    struct tuple
      : vector<EXPAND_TEMPLATE_ARGUMENTS(FUSION_MAX_VECTOR_SIZE)>
    {
        typedef
            vector<EXPAND_TEMPLATE_ARGUMENTS(FUSION_MAX_VECTOR_SIZE)>
        base_type;

        tuple()
        {}

#define TUPLE_CTOR(COMBINATION,_)\
        tuple(tuple COMBINATION other_tuple)\
          : base_type(static_cast<base_type COMBINATION>(other_tuple))\
        {}\
        \
        template <typename A1, typename A2>\
        tuple(std::pair<A1, A2> COMBINATION rhs)\
          : base_type(sequence_assign(\
                static_cast<std::pair<A1, A2> COMBINATION>(rhs)))\
        {}

        BOOST_FUSION_ALL_CV_REF_COMBINATIONS(TUPLE_CTOR,_)

#undef TUPLE_CTOR

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   define BOOST_FUSION_USE_BASE_TYPE
#   define BOOST_FUSION_SEQ_NAME tuple
#   define BOOST_FUSION_MAX_SEQ_SIZE FUSION_MAX_VECTOR_SIZE
#   include <boost/fusion/container/detail/pp/forward_ctor.hpp>
#   undef BOOST_FUSION_MAX_SEQ_SIZE
#   undef BOOST_FUSION_SEQ_NAME
#   undef BOOST_FUSION_USE_BASE_TYPE
#else
        template <typename... OtherArguments>
        tuple(BOOST_FUSION_R_ELSE_CLREF(OtherArguments)... other_arguments)
          : base_type(BOOST_FUSION_FORWARD(OtherArguments,other_arguments)...)
        {}
#endif

        template <typename Seq>
        tuple&
        operator=(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
        {
            *static_cast<base_type*>(this) = BOOST_FUSION_FORWARD(Seq, seq);
            return *this;
        }
    };

    template <typename Tuple>
    struct tuple_size
      : result_of::size<Tuple>
    {};

    template <int N, typename Tuple>
    struct tuple_element
      : result_of::value_at_c<Tuple, N>
    {};

    template <int N, typename Tuple>
    inline typename result_of::at_c<BOOST_FUSION_R_ELSE_CLREF(Tuple), N>::type
    get(BOOST_FUSION_R_ELSE_CLREF(Tuple) tuple)
    {
        return at_c<N>(tuple);
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <int N, typename Tuple>
    inline typename result_of::at_c<Tuple&, N>::type
    get(Tuple& tuple)
    {
        return at_c<N>(tuple);
    }
#endif
}}

#endif
