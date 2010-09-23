/*-----------------------------------------------------------------------------+    
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_CONCEPT_ABSTRACT_COMPARABLE_HPP_JOFA_100921
#define BOOST_ITL_CONCEPT_ABSTRACT_COMPARABLE_HPP_JOFA_100921

#include <boost/itl/type_traits/is_icl_container.hpp>

namespace boost{ namespace itl
{

//==============================================================================
//= Equivalences and Orderings<Comparable>
//==============================================================================
template<class Type>
inline typename enable_if<is_icl_container<Type>, bool>::type
operator != (const Type& left, const Type& right)
{ return !(left == right); }

template<class Type>
inline typename enable_if<is_icl_container<Type>, bool>::type
operator > (const Type& left, const Type& right)
{ return right < left; }

/** Partial ordering which is induced by Compare */
template<class Type>
inline typename enable_if<is_icl_container<Type>, bool>::type
operator <= (const Type& left, const Type& right)
{ return !(left > right); }

template<class Type>
inline typename enable_if<is_icl_container<Type>, bool>::type
operator >= (const Type& left, const Type& right)
{ return !(left < right); }

}} // namespace boost itl

#endif


