/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

//cschmidt: to avoid cyclic references - as some of the (here unneeded) mpl
//sequence modification metafunctions might instantiate result_of::as_xxx - just
//include the relevant /fusion/mpl/ headers.
#include <boost/fusion/support/category_of.hpp>

#include <boost/fusion/mpl/at.hpp>
#include <boost/fusion/mpl/back.hpp>
#include <boost/fusion/mpl/begin.hpp>
#include <boost/fusion/mpl/empty.hpp>
#include <boost/fusion/mpl/end.hpp>
#include <boost/fusion/mpl/front.hpp>
#include <boost/fusion/mpl/size.hpp>

#include <boost/fusion/support/internal/variadic_templates/variadic_quote.hpp>

#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/unpack_args.hpp>

namespace boost { namespace fusion { namespace detail
{
    template<typename Seq>
    struct BOOST_PP_CAT(BOOST_PP_CAT(as_,BOOST_FUSION_SEQ_NAME),_impl)
    {
        typedef typename detail::remove_reference<Seq>::type seq;

        typedef typename
            mpl::apply<
                mpl::unpack_args<variadic_quote<BOOST_FUSION_SEQ_NAME> >
              , typename mpl::eval_if<
                    traits::is_random_access<Seq>
                  , mpl::identity<seq>
                  , mpl::fold<
                        seq
                      , mpl::vector0<>
                      , mpl::quote2<mpl::push_back>
                    >
                >::type
            >::type
        type;
    };
}}}

