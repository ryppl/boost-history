/*=============================================================================
    Copyright (c) 2005 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_LIST_HPP
#define BOOST_FUSION_CONTAINER_LIST_LIST_HPP

#include <boost/fusion/container/list/list_fwd.hpp>
#include <boost/fusion/container/list/cons.hpp>
#include <boost/fusion/container/generation/vector_tie.hpp>
#include <boost/fusion/container/detail/forward_interface.hpp>
#include <boost/fusion/container/list/detail/cons_gen.hpp>

namespace boost { namespace fusion
{
    VARIADIC_TEMPLATE(FUSION_MAX_LIST_SIZE)
    struct list
      : detail::cons_gen<EXPAND_TEMPLATE_ARGUMENTS(FUSION_MAX_LIST_SIZE)>::type
    {
        typedef
            detail::cons_gen<
                EXPAND_TEMPLATE_ARGUMENTS(FUSION_MAX_LIST_SIZE)
            >
        cons_gen;
        typedef typename cons_gen::type base_type;

        typedef list_tag fusion_tag;

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   define BOOST_FUSION_LIST_CTOR(MODIFIER,_)\
        list(list MODIFIER list_)\
          : base_type(static_cast<base_type MODIFIER>(list_))\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(BOOST_FUSION_LIST_CTOR,_)

#   undef BOOST_FUSION_LIST_CTOR

#   include <boost/fusion/container/list/detail/pp/list_forward_ctor.hpp>

        template<typename SeqAssign>
        list(SeqAssign const& seq_assign)
          : base_type(seq_assign)
        {}

        template <typename Seq>
        list&
        operator=(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
        {
            *static_cast<base_type*>(this) = BOOST_FUSION_FORWARD(Seq, seq);
            return *this;
        }
#else
#   define BOOST_FUSION_USE_BASE_TYPE
#   define BOOST_FUSION_SEQ_NAME list
#   define BOOST_FUSION_MAX_SEQ_SIZE FUSION_MAX_LIST_SIZE
#   include <boost/fusion/container/detail/forward_interface.hpp>
#   undef BOOST_FUSION_MAX_SEQ_SIZE
#   undef BOOST_FUSION_SEQ_NAME
#   undef BOOST_FUSION_USE_BASE_TYPE
#endif
    };
}}

#endif
