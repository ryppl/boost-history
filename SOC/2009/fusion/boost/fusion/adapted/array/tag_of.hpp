/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_ARRAY_TAG_OF_HPP
#define BOOST_FUSION_ADAPTED_ARRAY_TAG_OF_HPP

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
        template<typename T, std::size_t N>
#if defined(BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS)
        struct tag_of<boost::array<T,N>, void >
#else
        struct tag_of<boost::array<T,N> >
#endif
        {
            typedef array_tag type;
        };
    }
}}

#endif
