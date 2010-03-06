/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef EXTENSION_STRUCT_DETAIL_AT_IMPL_HPP
#define EXTENSION_STRUCT_DETAIL_AT_IMPL_HPP

#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_const.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct at_impl;

    template<>
    struct at_impl<example::example_sequence_tag>
    {
        template<typename SeqRef, typename N>
        struct apply
          : apply<SeqRef, mpl::int_<N::value> >
        {};

        template<typename SeqRef>
        struct apply<SeqRef, mpl::int_<0> >
        {
            typedef typename
                mpl::if_<
                    is_const<typename detail::remove_reference<SeqRef>::type>
                  , std::string const&
                  , std::string&
                >::type
           type;

            static type
            call(SeqRef seq)
            {
                return seq.name;
            };
        };

        template<typename SeqRef>
        struct apply<SeqRef, mpl::int_<1> >
        {
            typedef typename
                mpl::if_<
                    is_const<typename detail::remove_reference<SeqRef>::type>
                  , int const&
                  , int&
                >::type
            type;

            static type
            call(SeqRef seq)
            {
                return seq.age;
            };
        };
    };
}}}

#endif
