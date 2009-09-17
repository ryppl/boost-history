/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/mpl/int.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct size_impl;

    template <>
    struct size_impl<BOOST_FUSION_ADAPTED_TUPLE_TAG>
    {
        template <typename Seq>
        struct apply
          : mpl::int_<
#ifdef BOOST_FUSION_ADAPTED_STD_TUPLE
                BOOST_FUSION_ADAPTED_TUPLE_NAMESPACE(size)<
#else
                tuples::length<
#endif
                    typename detail::identity<Seq>::type
                >::value
            >
        {};
    };
}}}

