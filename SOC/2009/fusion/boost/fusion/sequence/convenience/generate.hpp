/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_CONVENIENCE_GENERATE_HPP
#define BOOST_FUSION_SEQUENCE_CONVENIENCE_GENERATE_HPP

namespace boost { namespace fusion
{
    namespace detail
    {
        template<class F>
        struct generator
        {
            F f;

            generator(F f)
#ifndef BOOST_NO_RVALUE_REFERENCES
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
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));

            typedef void type;
        };
    }

    template<typename Seq, typename F>
    typename result_of::generate<
        BOOST_FUSION_R_ELSE_LREF(Seq)
      , BOOST_FUSION_RREF_ELSE_OBJ(F)
    >::type
    generate(BOOST_FUSION_R_ELSE_LREF(Seq) seq,BOOST_FUSION_RREF_ELSE_OBJ(F) f)
    {
        fusion::for_each(
            seq,
            detail::generator<BOOST_FUSION_R_ELSE_OBJ(F)>(
                BOOST_FUSION_FORWARD(F,f)));
    }
}}

#endif
