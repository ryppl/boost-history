/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_ARRAY_TAG_OF_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_ARRAY_TAG_OF_HPP

#include <boost/fusion/support/tag_of_fwd.hpp>

#include <cstddef>

namespace boost 
{ 
    template<typename T, std::size_t N>
    class array;
}

namespace boost { namespace fusion 
{
    struct array_tag;

    namespace traits
    {
#define TAG_OF_SPECIALIZATION(COMBINATION,_)\
        template<typename T, std::size_t N>\
        struct tag_of<boost::array<T,N> COMBINATION>\
        {\
            typedef array_tag type;\
        };

        BOOST_FUSION_ALL_CV_REF_NON_REF_COMBINATIONS(TAG_OF_SPECIALIZATION,_)

#undef TAG_OF_SPECIALIZATION
    }
}}

#endif
