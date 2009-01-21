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
#include <boost/mpl/and.hpp> 
#include <boost/type_traits/is_same.hpp>
#include <boost/itl/type_traits/is_interval_splitter.hpp> 

namespace boost{namespace itl
{

template<class Type>
struct is_overloadable
{
	enum { value = is_same<Type, typename Type::overloadable_type>::value };
};



template<class Type, class AssociateT>
struct is_combinable;

template<class Type>
struct is_combinable<Type, typename Type::value_type>       
{ enum{ value = is_overloadable<Type>::value }; };

template<class Type>
struct is_combinable<Type, typename Type::mapping_pair_type>
{ enum{ value = is_overloadable<Type>::value }; };

template<class Type>
struct is_combinable<Type, typename Type::base_type>        
{ enum{ value = is_overloadable<Type>::value }; };

template<class Type>
struct is_combinable<Type, typename Type::joint_type>       
{ enum{ value = is_overloadable<Type>::value }; };

template<class Type>
struct is_combinable<Type, typename Type::separate_type>    
{ 
	enum{ value = mpl::and_<is_interval_splitter<Type>, is_overloadable<Type> >::value }; 
};

template<class Type, class AssociateT>
struct is_combinable
{
	enum{ value = false };
};

/*
template<class Type, class AssociateT>
struct is_combinable2;

template<class Type>
struct is_combinable2<Type, typename Type::interval_type>       
{ enum{ value = is_overloadable<Type>::value }; };

template<class Type, class AssociateT>
struct is_combinable2
{
	enum{ value = is_combinable2<Type, AssociateT>::value };
};
*/

}} // namespace itl boost

#endif


