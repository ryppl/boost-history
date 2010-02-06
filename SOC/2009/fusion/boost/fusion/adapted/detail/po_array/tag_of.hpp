/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_PO_ARRAY_TAG_OF_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_PO_ARRAY_TAG_OF_HPP

#include <boost/fusion/support/tag_of_fwd.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <cstddef>

namespace boost
{
    namespace fusion
    {
        struct po_array_tag;
        struct po_array_iterator_tag;
        struct random_access_traversal_tag;
        struct fusion_sequence_tag;

        namespace traits
        {
#ifdef BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS
#   define BOOST_FUSION_ARRAY_TAG_OF_SPECIALIZATION(TYPE)\
            template<typename T, std::size_t N>\
            struct tag_of<TYPE, void>\
            {\
                typedef po_array_tag type;\
            };
#else
#   define BOOST_FUSION_ARRAY_TAG_OF_SPECIALIZATION(TYPE)\
            template<typename T, std::size_t N>\
            struct tag_of<TYPE>\
            {\
                typedef po_array_tag type;\
            };
#endif

        BOOST_FUSION_ARRAY_TAG_OF_SPECIALIZATION(T[N]);
        BOOST_FUSION_ARRAY_TAG_OF_SPECIALIZATION(T const[N]);
        BOOST_FUSION_ARRAY_TAG_OF_SPECIALIZATION(T(&)[N]);
        BOOST_FUSION_ARRAY_TAG_OF_SPECIALIZATION(T const(&)[N]);
#ifndef BOOST_NO_RVALUE_REFERENCES
        BOOST_FUSION_ARRAY_TAG_OF_SPECIALIZATION(T(&&)[N]);
        BOOST_FUSION_ARRAY_TAG_OF_SPECIALIZATION(T const(&&)[N]);
#endif

#undef BOOST_FUSION_ARRAY_TAG_OF_SPECIALIZATION
        }
    }

    namespace mpl
    {
        template<typename>
        struct sequence_tag;

        template<typename T, std::size_t N>
        struct sequence_tag<T[N]>
        {
            typedef fusion::po_array_tag type;
        };

        template<typename T, std::size_t N>
        struct sequence_tag<T const[N] >
        {
            typedef fusion::po_array_tag type;
        };
    }
}

#endif
