/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_IS_ITERATOR_HPP
#define BOOST_FUSION_SUPPORT_IS_ITERATOR_HPP

#include <boost/fusion/support/ref.hpp>

#include <boost/type_traits/is_base_of.hpp>

namespace boost { namespace fusion
{
    struct iterator_root;

    template <typename T>
    struct is_fusion_iterator
      : is_base_of<iterator_root, typename detail::remove_reference<T>::type>
    {};
}}

#endif
