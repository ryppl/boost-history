/*-----------------------------------------------------------------------------+    
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_CONCEPT_DOMAIN_COMPARABLE_HPP_JOFA_100925
#define BOOST_ITL_CONCEPT_DOMAIN_COMPARABLE_HPP_JOFA_100925

#include <boost/itl/type_traits/is_domain_comparable.hpp>

namespace boost{ namespace itl
{

template<class Type> 
inline typename enable_if<is_domain_comparable<Type>, bool>::type
domain_less(const typename Type::domain_param left, const typename Type::domain_param right)
{
	return typename Type::domain_compare()(left, right);
}

template<class Type> 
inline typename enable_if<is_domain_comparable<Type>, bool>::type
domain_less_equal(const typename Type::domain_param left, const typename Type::domain_param right)
{
	return !(typename Type::domain_compare()(right, left));
}

template<class Type> 
inline typename enable_if<is_domain_comparable<Type>, bool>::type
domain_equal(const typename Type::domain_param left, const typename Type::domain_param right)
{
	typedef typename Type::domain_compare domain_compare;
    return !domain_compare()(left, right) && !domain_compare()(right, left);
}


}} // namespace boost itl

#endif


