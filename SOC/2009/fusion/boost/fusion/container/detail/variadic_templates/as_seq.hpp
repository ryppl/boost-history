// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fusion/mpl/begin.hpp>
#include <boost/fusion/mpl/end.hpp>
#include <boost/fusion/support/internal/variadic_templates/variadic_quote.hpp>
#include <boost/fusion/support/internal/to_random_access_mpl_seq.hpp>

#include <boost/mpl/apply.hpp>
#include <boost/mpl/unpack_args.hpp>

namespace boost { namespace fusion { namespace detail
{
    template<typename SeqRef>
    struct BOOST_PP_CAT(BOOST_PP_CAT(as_,BOOST_FUSION_SEQ_NAME),_impl)
    {
        struct apply
        {
            typedef typename
                mpl::apply<
                    mpl::unpack_args<variadic_quote<BOOST_FUSION_SEQ_NAME> >
                  , typename to_random_access_mpl_seq<
                        typename remove_reference<SeqRef>::type
                    >::type
                >::type
            type;
        };
    };
}}}
