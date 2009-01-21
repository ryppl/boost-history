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
struct is_combinable<Type, typename Type::domain_mapping_type>
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




//------------------------------------------------------------------------------
// is_interval_set_derivative
//------------------------------------------------------------------------------
template<class Type, class AssociateT>
struct is_interval_set_derivative;

template<class Type>
struct is_interval_set_derivative<Type, typename Type::domain_type>
{ enum{ value = is_interval_container<Type>::value }; };

template<class Type>
struct is_interval_set_derivative<Type, typename Type::interval_type>
{ enum{ value = is_interval_container<Type>::value }; };

template<class Type, class AssociateT>
struct is_interval_set_derivative
{
	enum{ value = false };
};

//------------------------------------------------------------------------------
// is_interval_map_derivative
//------------------------------------------------------------------------------
template<class Type, class AssociateT>
struct is_interval_map_derivative;

template<class Type>
struct is_interval_map_derivative<Type, typename Type::domain_mapping_type>
{ enum{ value = is_interval_container<Type>::value }; };

template<class Type>
struct is_interval_map_derivative<Type, typename Type::interval_mapping_type>
{ enum{ value = is_interval_container<Type>::value }; };

template<class Type, class AssociateT>
struct is_interval_map_derivative
{
	enum{ value = false };
};



//------------------------------------------------------------------------------
// is_interval_set_companion
//------------------------------------------------------------------------------
template<class Type, class CompanionT> struct is_interval_set_companion;

template
<
	class Dom, ITL_COMPARE Cmp, template<class,ITL_COMPARE>class Itv, ITL_ALLOC Alc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet1,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet2
>
struct is_interval_set_companion<IntervalSet1<Dom,Cmp,Itv,Alc>, 
	                             IntervalSet2<Dom,Cmp,Itv,Alc> >
{
	enum{ value =   IntervalSet2<Dom,Cmp,Itv,Alc>::fineness 
		          < IntervalSet1<Dom,Cmp,Itv,Alc>::fineness };
};

// Every IntervalSet can be a companion of every IntervalMap for 
// operations intersection and erase.
template
<
	class Dom, class Cod, class Trt, ITL_COMPARE Cmp, ITL_COMBINE Cmb, ITL_SECTION Sec, 
	template<class,ITL_COMPARE>class Itv, ITL_ALLOC Alc,
    template<class, class, class, ITL_COMPARE, ITL_COMBINE, ITL_SECTION,
	         template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalMap,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
struct is_interval_set_companion<IntervalMap<Dom,Cod,Trt,Cmp,Cmb,Sec,Itv,Alc>, 
	                             IntervalSet<Dom,        Cmp,        Itv,Alc> >
{ enum{ value = true }; };

template<class Type, class CompanionT> struct is_interval_set_companion
{ enum{ value = is_interval_set_derivative<Type,CompanionT>::value }; };


//------------------------------------------------------------------------------
// is_interval_map_companion
//------------------------------------------------------------------------------
template<class Type, class CompanionT> struct is_interval_map_companion;

template
<
	class Dom, class Cod, class Trt, ITL_COMPARE Cmp, ITL_COMBINE Cmb, ITL_SECTION Sec, 
	template<class,ITL_COMPARE>class Itv, ITL_ALLOC Alc,
    template<class, class, class, ITL_COMPARE, ITL_COMBINE, ITL_SECTION,
	         template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalMap1,
    template<class, class, class, ITL_COMPARE, ITL_COMBINE, ITL_SECTION,
	         template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalMap2
>
struct is_interval_map_companion<IntervalMap1<Dom,Cod,Trt,Cmp,Cmb,Sec,Itv,Alc>, 
	                             IntervalMap2<Dom,Cod,Trt,Cmp,Cmb,Sec,Itv,Alc> >
{
	enum{ value =   IntervalMap2<Dom,Cod,Trt,Cmp,Cmb,Sec,Itv,Alc>::fineness 
		          < IntervalMap1<Dom,Cod,Trt,Cmp,Cmb,Sec,Itv,Alc>::fineness };
};

template<class Type, class CompanionT> struct is_interval_map_companion
{ enum{ value = is_interval_map_derivative<Type,CompanionT>::value }; };



}} // namespace itl boost

#endif


