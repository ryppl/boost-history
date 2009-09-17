/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/fusion/iterator/basic_iterator.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct end_impl;

    template <>
    struct end_impl<BOOST_FUSION_ADAPTED_TUPLE_TAG>
    {
        template <typename Seq>
        struct apply
        {
            typedef
                basic_iterator<
                    BOOST_FUSION_ADAPTED_TUPLE_ITERATOR_TAG
                  , random_access_traversal_tag
                  , typename detail::add_lref<Seq>::type
#ifdef BOOST_FUSION_ADAPTED_STD_TUPLE
                  , BOOST_FUSION_ADAPTED_TUPLE_NAMESPACE(size)<
#else
                  , tuples::length<
#endif
                        typename detail::identity<Seq>::type
                    >::value
                >
            type;

            static type
            call(Seq seq)
            {
                return type(seq,0);
            }
        };
    };
}}}

