/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#ifndef __itl_is_combinable_JOFA_090115_H__
#define __itl_is_combinable_JOFA_090115_H__

#include <boost/mpl/bool.hpp> 
#include <boost/mpl/if.hpp> 
#include <boost/itl/type_traits/is_interval_splitter.hpp> 

namespace boost{namespace itl
{

template<class Type, class AssociateT>
struct is_combinable;

template<class Type>
struct is_combinable<Type, typename Type::value_type>       { enum{ value = true }; };
template<class Type>
struct is_combinable<Type, typename Type::mapping_pair_type>{ enum{ value = true }; };
template<class Type>
struct is_combinable<Type, typename Type::base_type>        { enum{ value = true }; };
template<class Type>
struct is_combinable<Type, typename Type::joint_type>       { enum{ value = true }; };

template<class Type>
struct is_combinable<Type, typename Type::separate_type>    
{ 
	enum{ value = is_interval_splitter<Type>::value }; 
};

template<class Type, class AssociateT>
struct is_combinable
{
	enum{ value = false };
};

}} // namespace itl boost

#endif


