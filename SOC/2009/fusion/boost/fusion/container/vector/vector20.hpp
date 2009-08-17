// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_CONTAINER_VECTOR_VECTOR20_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_VECTOR20_HPP

#include <boost/config.hpp>
#include <boost/mpl/vector/vector20.hpp>

#if defined(BOOST_NO_VARIADIC_TEMPLATES) || !defined(BOOST_FUSION_CPP0X_NO_DEPRECEATED)
#   define BOOST_FUSION_FROM 11
#   define BOOST_FUSION_TO 20

#   ifdef BOOST_NO_VARIADIC_TEMPLATES
#       include<boost/fusion/container/vector/detail/pp/vector_n.hpp>
#   else
#       include<boost/fusion/container/vector/detail/variadic_templates/vector_n.hpp>
#   endif

#   undef BOOST_FUSION_FROM
#   undef BOOST_FUSION_TO
#else
#   include <boost/fusion/container/vector/vector.hpp>
#endif

#endif
