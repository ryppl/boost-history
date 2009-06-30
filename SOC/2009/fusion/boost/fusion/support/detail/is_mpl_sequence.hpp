/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_DETAIL_IS_MPL_SEQUENCE_HPP
#define BOOST_FUSION_SUPPORT_DETAIL_IS_MPL_SEQUENCE_HPP

#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/ref.hpp>

#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace boost { namespace fusion { namespace detail
{
    template <typename T>
    struct is_mpl_sequence
      : mpl::and_<
            mpl::not_<
                is_base_of<
                    sequence_root
                  , typename identity<T>::type
                >
            >
          , mpl::is_sequence<typename identity<T>::type>
        >
    {};
}}}

#endif
