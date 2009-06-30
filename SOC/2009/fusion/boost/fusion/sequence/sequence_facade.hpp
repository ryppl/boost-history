/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_SEQUENCE_FACADE_HPP
#define BOOST_FUSION_SEQUENCE_SEQUENCE_FACADE_HPP

#include <boost/fusion/support/sequence_base.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion
{
    struct sequence_facade_tag;

    template <typename Derived, typename Category, typename IsView = mpl::false_>
    struct sequence_facade : sequence_base<Derived>
    {
        typedef sequence_facade_tag fusion_tag;
        typedef Derived derived_type;
        typedef Category category;
        typedef IsView is_view;
    };
}}

#endif
