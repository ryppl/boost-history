/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_BOOST_TUPLE_TAG_OF_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_BOOST_TUPLE_TAG_OF_HPP

#include <boost/fusion/support/tag_of_fwd.hpp>

namespace boost { namespace fusion 
{
    struct boost_tuple_tag;
    struct boost_tuple_iterator_tag;

    namespace traits
    {
#define BOOST_FUSION_TAG_OF_SPECIALIZATION(COMBINATION,_)\
        template <\
            class T0, class T1, class T2, class T3, class T4, \
            class T5, class T6, class T7, class T8, class T9\
        >\
        struct tag_of<\
            tuples::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> COMBINATION\
        >\
        {\
            typedef boost_tuple_tag type;\
        };\
        \
        template <class Head, class Tail>\
        struct tag_of<tuples::cons<Head, Tail> COMBINATION>\
        {\
            typedef boost_tuple_tag type;\
        };\
        \
        template <>\
        struct tag_of<tuples::null_type COMBINATION>\
        {\
            typedef boost_tuple_tag type;\
        };

        BOOST_FUSION_ALL_CV_REF_NON_REF_COMBINATIONS(
                BOOST_FUSION_TAG_OF_SPECIALIZATION,_)

#undef BOOST_FUSION_TAG_OF_SPECIALIZATION
    }
}}

#endif
