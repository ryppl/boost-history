/*-----------------------------------------------------------------------------+    
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------+
Function templates to call functions in object oriented or namespace glabal 
versions.
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TEST_CALLS_H_JOFA_100908
#define BOOST_ITL_TEST_CALLS_H_JOFA_100908

#include <boost/itl/detail/notate.hpp>
#include <boost/itl/detail/design_config.hpp>

namespace boost{namespace itl
{

namespace call{

template<class Type, class CoType>
bool contains(const Type& object, const CoType& co_object)
{
#ifdef ITL_PURE_CONCEPTUAL
    return object.contains(co_object);
#else
    return itl::contains(object, co_object);
#endif
}

template<class Type, class CoType>
bool within(const Type& object, const CoType& co_object)
{
#ifdef ITL_PURE_CONCEPTUAL
    return object.within(co_object);
#else
    return itl::within(object, co_object);
#endif
}

} // namespace call

}} // namespace itl boost

#endif // BOOST_ITL_TEST_CALLS_H_JOFA_100908

