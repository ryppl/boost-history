/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef EXTENSION_STRUCT_DETAIL_DEREF_IMPL_HPP
#define EXTENSION_STRUCT_DETAIL_DEREF_IMPL_HPP

#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_const.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct deref_impl;

    template<>
    struct deref_impl<example::example_struct_iterator_tag>
    {
        template<typename ItRef>
        struct apply
        {
            typedef typename detail::remove_reference<ItRef>::type it;
            typedef typename
                mpl::if_c<
                    !it::index::value
                  , std::string
                  , int
                >::type
            attr_type;

            typedef typename
                mpl::if_<
                    is_const<typename it::struct_type>
                  , attr_type const&
                  , attr_type&
                >::type
            type;

            static type
            call(ItRef it, mpl::true_)
            {
                return it.struct_.name;
            }

            static type
            call(ItRef it, mpl::false_)
            {
                return it.struct_.age;
            }

            static type
            call(ItRef it_)
            {
                return call(it_, mpl::bool_<!it::index::value>());
            }
        };
    };
}}}

#endif
