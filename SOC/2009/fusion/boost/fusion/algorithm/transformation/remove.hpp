/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_REMOVE_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_REMOVE_HPP

#include <boost/fusion/algorithm/transformation/remove_if.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/fusion/support/internal/workaround.hpp>

#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename T>
        struct remove
          : remove_if<Seq, is_same<mpl::_1, T> >
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));
        };
    }

    template <typename T, typename Seq>
    inline typename result_of::remove<BOOST_FUSION_R_ELSE_CLREF(Seq), T>::type
    remove(BOOST_FUSION_R_ELSE_CLREF(Seq) seq)
    {
        return fusion::remove_if<is_same<mpl::_1, T> >(
                        BOOST_FUSION_FORWARD(Seq,seq));
    }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template <typename T, typename Seq>
    inline BOOST_FUSION_EXPLICIT_TEMPLATE_NON_CONST_ARG_OVERLOAD(
            result_of::remove<,Seq,&, T>)
    remove_if(Seq& seq)
    {
        return fusion::remove_if<is_same<mpl::_1, T> >(seq);
    }
#endif
}}

#endif
