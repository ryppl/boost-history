/*-----------------------------------------------------------------------------+    
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_CONCEPT_INTERVAL_SET_VALUE_HPP_JOFA_100924
#define BOOST_ITL_CONCEPT_INTERVAL_SET_VALUE_HPP_JOFA_100924

#include <boost/itl/type_traits/is_interval_container.hpp>

namespace boost{ namespace itl
{

//==============================================================================
//= AlgoUnifiers<Set>
//==============================================================================
template<class Type, class Iterator>
inline typename enable_if<is_interval_set<Type>, typename Type::codomain_type>::type
co_value(Iterator value_)
{ 
	typedef typename Type::codomain_type codomain_type;
	return itl::is_empty(*value_)? codomain_type() : (*value_).lower(); 
}

}} // namespace boost itl

#endif


