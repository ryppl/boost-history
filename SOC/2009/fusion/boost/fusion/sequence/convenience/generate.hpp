/*==============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_CONVENIENCE_GENERATE_HPP
#define BOOST_FUSION_SEQUENCE_CONVENIENCE_GENERATE_HPP

#include <boost/fusion/support/internal/base.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template<class F>
        struct generator
        {
            F f;

            generator(F f)
#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
              : f(f)
#else
              : f(std::move(f))
#endif
            {}

            template<class Element>
            void
            operator()(Element& element)const
            {
                element=f();
            }
        };
    }

    namespace result_of
    {
        template<typename Seq, typename F>
        struct generate
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>))
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>))

            typedef void type;
        };
    }

    template<typename Seq, typename F>
    void
    generate(BOOST_FUSION_R_ELSE_LREF(Seq) seq,BOOST_FUSION_RREF_ELSE_OBJ(F) f)
    {
        fusion::for_each(
            seq,
            detail::generator<BOOST_FUSION_RREF_ELSE_OBJ(F)>(
                BOOST_FUSION_FORWARD(F,f)));
    }
}}

#endif
