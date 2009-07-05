/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_ZIP_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_ZIP_HPP

#if defined(BOOST_NO_VARIADIC_TEMPLATES) || !defined(BOOST_FUSION_CPP0X_NO_DEPRECEATED)
#   ifndef FUSION_MAX_ZIP_SEQUENCES
#       define FUSION_MAX_ZIP_SEQUENCES 10
#   endif
#endif

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#   include <boost/fusion/algorithm/transformation/detail/pp/zip.hpp>
#else
#   include <boost/fusion/algorithm/transformation/detail/variadic_templates/zip.hpp>
#endif

#endif
