/*==============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_WORKAROUND_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_WORKAROUND_HPP

#ifdef BOOST_FUSION_NO_RVALUE_REFERENCES
//cschmidt: see https://svn.boost.org/trac/boost/ticket/3305
#   if defined(BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS) ||\
        BOOST_WORKAROUND(__GNUC__,<4)
#       include <boost/type_traits/is_const.hpp>
#       include <boost/utility/enable_if.hpp>

#       define BOOST_FUSION_EXPLICIT_TEMPLATE_NON_CONST_ARG_OVERLOAD(\
            PRE,ARG,ARG_MODIFIER,POST)\
            typename lazy_disable_if<\
                is_const<ARG>\
              , PRE ARG ARG_MODIFIER, POST\
            >::type
#   else
#       define BOOST_FUSION_EXPLICIT_TEMPLATE_NON_CONST_ARG_OVERLOAD(\
            PRE,ARG,ARG_MODIFIER,POST)\
            typename PRE ARG ARG_MODIFIER, POST ::type
#   endif
#endif

#endif
