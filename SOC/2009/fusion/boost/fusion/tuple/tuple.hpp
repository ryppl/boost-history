/*=============================================================================
    Copyright (c) 2005 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

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
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/workaround.hpp>

#include <boost/fusion/container/detail/forward_interface.hpp>

namespace boost { namespace fusion
{
    VARIADIC_TEMPLATE(FUSION_MAX_VECTOR_SIZE)
    struct tuple
      : vector<EXPAND_TEMPLATE_ARGUMENTS(FUSION_MAX_VECTOR_SIZE)>
    {
        typedef
            vector<EXPAND_TEMPLATE_ARGUMENTS(FUSION_MAX_VECTOR_SIZE)>
        base_type;

#define TUPLE_CTOR(COMBINATION,_)\
        template <typename A1, typename A2>\
        tuple(std::pair<A1, A2> COMBINATION pair)\
          : base_type(static_cast<std::pair<A1, A2> COMBINATION>(pair))\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(TUPLE_CTOR,_)

#undef TUPLE_CTOR

#define BOOST_FUSION_USE_BASE_TYPE
#define BOOST_FUSION_SEQ_NAME tuple
#define BOOST_FUSION_MAX_SEQ_SIZE FUSION_MAX_VECTOR_SIZE
#include <boost/fusion/container/detail/forward_interface.hpp>
#undef BOOST_FUSION_MAX_SEQ_SIZE
#undef BOOST_FUSION_SEQ_NAME
#undef BOOST_FUSION_USE_BASE_TYPE
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
    inline BOOST_FUSION_EXPLICIT_TEMPLATE_NON_CONST_ARG_OVERLOAD(
            result_of::at_c<,Tuple,&, N>)
    get(Tuple& tuple)
    {
        return at_c<N>(tuple);
    }
#endif
}}

#endif
