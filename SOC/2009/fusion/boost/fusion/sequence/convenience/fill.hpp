/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_CONVENIENCE_FILL_HPP
#define BOOST_FUSION_SEQUENCE_CONVENIENCE_FILL_HPP

namespace boost { namespace fusion
{
    namespace detail
    {
        template<class TRef>
        struct filler
        {
            TRef t;

            filler(TRef t)
              : t(t)
            {}

            template<class Element>
            void
            operator()(Element& element)const
            {
                element=t;
            }
        };
    }

    namespace result_of
    {
        template<typename Seq, typename T>
        struct fill
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));

            typedef void type;
        };
    }

    template<typename Seq, typename T>
    typename result_of::fill<BOOST_FUSION_R_ELSE_LREF(Seq),T const&>::type
    fill(BOOST_FUSION_R_ELSE_LREF(Seq) seq,T const& t)
    {
        fusion::for_each(seq,detail::filler<T const&>(t));
    }
}}

#endif
