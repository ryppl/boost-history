/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/fusion/support/tag_of_fwd.hpp>

#include <cstddef>

namespace BOOST_FUSION_ADAPTED_ARRAY_NAMESPACE
{ 
    template<typename T, std::size_t N>
    BOOST_FUSION_ADAPTED_ARRAY_TYPE array;
}

namespace boost { namespace fusion 
{
    struct array_tag;
    struct array_iterator_tag;
    struct random_access_traversal_tag;

    namespace traits
    {
#define BOOST_FUSION_TAG_OF_SPECIALIZATION(COMBINATION,_)\
        template<typename T, std::size_t N>\
        struct tag_of<\
            BOOST_FUSION_ADAPTED_ARRAY_NAMESPACE::array<T,N> COMBINATION>\
        {\
            typedef array_tag type;\
        };

        BOOST_FUSION_ALL_CV_REF_NON_REF_COMBINATIONS(
                BOOST_FUSION_TAG_OF_SPECIALIZATION,_)

#undef BOOST_FUSION_TAG_OF_SPECIALIZATION
    }
}}
