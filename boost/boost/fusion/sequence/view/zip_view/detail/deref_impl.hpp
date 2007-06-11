/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_DEREF_IMPL_20061024_1959)
#define FUSION_DEREF_IMPL_20061024_1959

#include <boost/fusion/sequence/container/vector.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>
#include <boost/fusion/sequence/conversion/as_vector.hpp>
#include <boost/fusion/support/unused.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion {

    struct zip_view_iterator_tag;

    namespace detail
    {
        struct poly_deref
        {
            template<typename T>
            struct result;

            template<typename It>
            struct result<poly_deref(It)>
                : mpl::eval_if<is_same<It, unused_type>,
                               mpl::identity<unused_type>,
                               result_of::deref<It> >
            {};

            template<typename It>
            typename result<poly_deref(It)>::type
            operator()(const It& it) const
            {
                return fusion::deref(it);
            }

            unused_type operator()(unused_type const&) const
            {
                return unused_type();
            }
        };
    }

    namespace extension
    {
        template<typename Tag>
        struct deref_impl;

        template<>
        struct deref_impl<zip_view_iterator_tag>
        {
            template<typename It>
            struct apply
            {
                typedef typename result_of::as_vector<
                    typename result_of::transform<typename It::iterators, detail::poly_deref>::type>::type type;

                static type
                call(It const& it)
                {
                    return type(
                        fusion::transform(it.iterators_, detail::poly_deref()));
                }
            };
        };
    }
}}

#endif
