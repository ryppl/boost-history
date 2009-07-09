/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_BOOST_TUPLE_END_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_BOOST_TUPLE_END_IMPL_HPP

#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_convertible.hpp>

namespace boost { namespace fusion
{
    struct boost_tuple_tag;

    namespace extension
    {
        template <typename Tag>
        struct end_impl;

        template <>
        struct end_impl<boost_tuple_tag>
        {
            template <typename>
            struct apply 
            {
                typedef
                    boost_tuple_iterator<tuples::null_type const volatile&>
                type;

                //TODO volatile!
                template<typename Seq>
                static type
                call(Seq const& seq, mpl::true_)
                {
                    return type(seq,0);
                }

                template<typename Seq>
                static type
                call(Seq const& seq, mpl::false_)
                {
                    return call(seq.get_tail());
                }

                template<typename Seq>
                static type
                call(Seq const& seq)
                {
                    return call(seq,
                            typename is_convertible<
                                Seq*
                              , tuples::null_type const volatile*
                            >::type());
                }
            };
        };
    }
}}

#endif
