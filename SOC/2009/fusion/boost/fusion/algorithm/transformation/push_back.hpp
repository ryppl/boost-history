/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_PUSH_BACK_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_PUSH_BACK_HPP

#include <boost/fusion/support/detail/as_fusion_element.hpp>
#include <boost/fusion/view/joint_view/joint_view.hpp>
#include <boost/fusion/view/single_view/single_view.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename T>
        struct push_back
        {
            typedef
                fusion::single_view<
                    typename detail::as_fusion_element<T>::type
                >
            single_view;

            typedef joint_view<Seq, single_view const> type;
        };
    }

    template <typename Seq, typename T>
    inline typename result_of::push_back<Seq const&, T const&>::type
    push_back(Seq const& seq, T const& x)
    {
        typedef result_of::push_back<Seq const&, T const&> gen;

        return typename gen::type(seq, x);
    }
}}

#endif
