/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Järvi
    Copyright (c) 2001-2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_DEPENDENCIES_HPP)
#define BOOST_FUSION_DEPENDENCIES_HPP

#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/cat.hpp>

#include <boost/mpl/int.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/count_if.hpp>

//  include mpl::vector0..N where N
//  is BOOST_FUSION_MAX_ARITY

#if (BOOST_FUSION_MAX_ARITY < 10)
# include <boost/mpl/vector/vector10.hpp>

#elif (BOOST_FUSION_MAX_ARITY < 20)
# include <boost/mpl/vector/vector20.hpp>
# if (BOOST_FUSION_MAX_ARITY == 11)
#   include <boost/mpl/vector/vector10.hpp>
# elif (BOOST_FUSION_MAX_ARITY < 30)
#   include <boost/mpl/vector/vector30.hpp>
#   if (BOOST_FUSION_MAX_ARITY == 21)
#     include <boost/mpl/vector/vector20.hpp>
#   elif (BOOST_FUSION_MAX_ARITY < 40)
#     include <boost/mpl/vector/vector40.hpp>
#     if (BOOST_FUSION_MAX_ARITY == 31)
#       include <boost/mpl/vector/vector30.hpp>
#     elif (BOOST_FUSION_MAX_ARITY < 50)
#       include <boost/mpl/vector/vector50.hpp>
#       if (BOOST_FUSION_MAX_ARITY == 41)
#       include <boost/mpl/vector/vector40.hpp>
#     endif
#   endif
# endif
#endif
#endif

#include <boost/ref.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/type_traits/is_convertible.hpp>

#endif // defined(BOOST_FUSION_DEPENDENCIES_HPP)
