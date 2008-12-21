/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#ifndef __itl_interval_maps_h_JOFA_081008__
#define __itl_interval_maps_h_JOFA_081008__

#include <boost/itl/interval_base_map.hpp>
#include <boost/itl/interval_map_algo.hpp>

namespace boost{namespace itl
{

template
<    
    class, class, class, 
    ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
>
class interval_map;


//-----------------------------------------------------------------------------
// addition +=
//-----------------------------------------------------------------------------
/*CL
template 
<
    class SubType, class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& 
operator +=
(
          interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& operand
)
{
    typedef IntervalMap<DomainT,CodomainT,
                        Traits,Compare,Combine,Section,Interval,Alloc> operand_type;
    const_FORALL(typename operand_type, elem_, operand) 
        object.add(*elem_); 

    return object; 
}
*/
template 
<
	class ObjectT,
    class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
ObjectT& operator +=
(
          ObjectT& object,
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& operand
)
{
    typedef IntervalMap<DomainT,CodomainT,
                        Traits,Compare,Combine,Section,Interval,Alloc> operand_type;
    const_FORALL(typename operand_type, elem_, operand) 
        object.add(*elem_); 

    return object; 
}

template 
<
    class SubType, class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> 
operator +
(
    const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& operand
)
{
	typedef interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> ObjectT;
	return ObjectT(object) += operand; 
}
//-----------------------------------------------------------------------------

template 
<
    class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& 
operator +=
(
          IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& operand
)
{
    typedef IntervalMap<DomainT,CodomainT,
                        Traits,Compare,Combine,Section,Interval,Alloc> operand_type;
    const_FORALL(typename operand_type, elem_, operand) 
        object.add(*elem_); 

    return object; 
}

template 
<
    class DomainT, class CodomainT, class Traits,
 
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
operator +
(
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& operand
)
{
	typedef IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> ObjectT;
	return ObjectT(object) += operand; 
}
//-----------------------------------------------------------------------------

//--- value_type --------------------------------------------------------------
template 
<
    class DomainT, class CodomainT, class Traits,
 
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& 
operator +=
(
          IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const typename 
          IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>::value_type& operand
)
{
    return object.add(operand); 
}

template 
<
    class DomainT, class CodomainT, class Traits,
 
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> 
operator +
(
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const typename IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>::value_type& operand
)
{
	typedef IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> ObjectT;
	return ObjectT(object) += operand; 
}
//-----------------------------------------------------------------------------

//--- mapping_type ------------------------------------------------------------
// Addition (+=) of a base value pair.
/* Addition of an value pair <tt>x=(I,y)</tt>

    This adds (inserts) a value <tt>y</tt> for an interval <tt>I</tt> into the 
    map, identical member function add. 

    If no values are associated already within the range of <tt>I</tt>,
    <tt>y</tt> will be associated to that interval.

    If there are associated values, in the range of <tt>I</tt>, then all
    those values within the ranges of their intervals,
    are incremented by <tt>y</tt>. This is done via operator <tt>+=</tt>
    which has to be implemented for CodomainT. 
*/
template 
<
    class DomainT, class CodomainT, class Traits,
 
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& 
operator +=
(
          IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const typename IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>::mapping_pair_type& operand
)
{
    return object.add(operand); 
}

template 
<
    class DomainT, class CodomainT, class Traits,
 
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
operator +
(
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const typename IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>::mapping_pair_type& operand
)
{
	typedef IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> ObjectT;
	return ObjectT(object) += operand; 
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// subtraction -=
//-----------------------------------------------------------------------------
template 
<
    class SubType, class DomainT, class CodomainT, class Traits,
 
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& 
operator -=
(
          interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& operand
)
{
    typedef IntervalMap<DomainT,CodomainT,
                        Traits,Compare,Combine,Section,Interval,Alloc> operand_type;
    const_FORALL(typename operand_type, elem_, operand) 
        object.subtract(*elem_); 

    return object; 
}

template 
<
    class SubType, class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
operator -
(
    const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& operand
)
{
	typedef interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> ObjectT;
	return ObjectT(object) += operand; 
}
//-----------------------------------------------------------------------------

template 
<
    class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& 
operator -=
(
          IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& operand
)
{
    typedef IntervalMap<DomainT,CodomainT,
                        Traits,Compare,Combine,Section,Interval,Alloc> operand_type;
    const_FORALL(typename operand_type, elem_, operand) 
        object.subtract(*elem_); 

    return object; 
}

template 
<
    class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
operator -
(
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& operand
)
{
	typedef IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> ObjectT;
	return ObjectT(object) -= operand; 
}
//-----------------------------------------------------------------------------

//--- value_type --------------------------------------------------------------
// Subtraction of an interval value pair
/* Subtraction of an interval value pair  <tt>x=(I,y)</tt>.
    This subtracts a value <tt>y</tt> for an interval <tt>I</tt> from the map.

    If there are associated values, in the range of <tt>I</tt>, all
    those values within the ranges of their intervals,
    are decremented by <tt>y</tt>. This is done usign operator -=.

    If <tt>y</tt> becomes the neutral element CodomainT() <tt>k</tt> will
    also be removed from the map, if the Traits include the property 
    neutron_absorber. 
*/
template 
<
    class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& 
operator -=
(
          IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const typename IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>::value_type& operand
)
{
    return object.subtract(operand); 
}

template 
<
    class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
operator -
(
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const typename IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>::value_type& operand
)
{
	typedef IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> ObjectT;
	return ObjectT(object) -= operand; 
}
//-----------------------------------------------------------------------------


//--- mapping_type ---------------------------------------------------------
template 
<
    class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& 
operator -=
(
          IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const typename IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>::mapping_pair_type& operand
)
{
    return object.subtract(operand); 
}

template 
<
    class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
operator -
(
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const typename IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>::mapping_pair_type& operand
)
{
	typedef IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> ObjectT;
	return ObjectT(object) -= operand; 
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// erasure -= of elements given by an interval_set
//-----------------------------------------------------------------------------
template 
<
    class SubType, class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, template<class,ITL_COMPARE>class, 
        ITL_COMPARE, ITL_ALLOC
    >
    class IntervalSet
>
interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& 
operator -=
(
          interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const IntervalSet<DomainT,Interval,Compare,Alloc>& erasure
)
{
    typedef IntervalSet<DomainT,Interval,Compare,Alloc> set_type;
    const_FORALL(typename set_type, interval_, erasure) 
        object.erase(*interval_); 

    return object; 
}

template 
<
    class SubType, class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, template<class,ITL_COMPARE>class, 
        ITL_COMPARE, ITL_ALLOC
    >
    class IntervalSet
>
interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
operator -
(
    const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const IntervalSet<DomainT,Interval,Compare,Alloc>& operand
)
{
	typedef interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> ObjectT;
	return ObjectT(object) -= operand; 
}
//-----------------------------------------------------------------------------

//--- value_type --------------------------------------------------------------
template 
<
    class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& 
operator -=
(
          IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const typename IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>::interval_type& operand
)
{
    return object.erase(operand); 
}

template 
<
    class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> 
operator -
(
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const typename IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>::interval_type& operand
)
{
	typedef IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> ObjectT;
	return ObjectT(object) -= operand; 
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// insert  
//-----------------------------------------------------------------------------
template 
<
    class SubType, class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    class OperandT
>
interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& 
insert
(
          interval_base_map<SubType,DomainT,CodomainT,
                            Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const OperandT& operand
)
{
    const_FORALL(typename OperandT, elem_, operand) 
        object.insert(*elem_); 
    return object;
}

//-----------------------------------------------------------------------------
// erase  
//-----------------------------------------------------------------------------
template 
<
    class SubType, class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    class OperandT
>
interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& 
erase
(
          interval_base_map<SubType,DomainT,CodomainT,
                            Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const OperandT& operand
)
{
    const_FORALL(typename OperandT, elem_, operand) 
        object.erase(*elem_); 
    return object;
}

//-----------------------------------------------------------------------------
// intersection *=  
//-----------------------------------------------------------------------------
template 
<
    class SubType, class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    class SectanT
>
interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& 
operator *=
(
          interval_base_map<SubType,DomainT,CodomainT,
                            Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const SectanT& operand
)
{
    typedef interval_base_map<SubType,DomainT,CodomainT,
                              Traits,Compare,Combine,Section,Interval,Alloc> object_type;
    object_type intersection;
    object.add_intersection(intersection,operand);
    object.swap(intersection);
    return object;
}

template 
<
    class SubType, class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    class SectanT
>
interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>
operator *
(
    const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const SectanT& operand
)
{
	typedef interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> ObjectT;
	return ObjectT(object) *= operand; 
}
//-----------------------------------------------------------------------------

template 
<
    class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
	template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& 
operator *=
(
          IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& operand
)
{
    typedef IntervalMap<DomainT,CodomainT,
                        Traits,Compare,Combine,Section,Interval,Alloc> object_type;
    object_type intersection;
    object.add_intersection(intersection,operand);
    object.swap(intersection);
    return object;
}

template 
<
    class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
	template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> 
operator *
(
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& operand
)
{
	typedef IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> ObjectT;
	return ObjectT(object) *= operand; 
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// is_element_equal
//-----------------------------------------------------------------------------
template 
<
    class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class LeftIntervalMap,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class RightIntervalMap
>
bool is_element_equal
(
    const LeftIntervalMap <DomainT,CodomainT,
                           Traits,Compare,Combine,Section,Interval,Alloc>& left,
    const RightIntervalMap<DomainT,CodomainT,
                           Traits,Compare,Combine,Section,Interval,Alloc>& right
)
{
    return Interval_Set::is_element_equal(left, right);
}


//-----------------------------------------------------------------------------
// is_disjoint
//-----------------------------------------------------------------------------

//--- IntervalMap -------------------------------------------------------------
template 
<
    class SubType, class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
//JODO boost::enable_if
bool is_disjoint
(
          interval_base_map<SubType,DomainT,CodomainT,
                            Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const IntervalMap<DomainT,CodomainT,
                      Traits,Compare,Combine,Section,Interval,Alloc>& operand
)
{
    typedef interval_base_map<SubType,DomainT,CodomainT,
                              Traits,Compare,Combine,Section,Interval,Alloc> object_type;
    typedef IntervalMap<DomainT,CodomainT,
                        Traits,Compare,Combine,Section,Interval,Alloc> operand_type;
    object_type intersection;

    if(operand.empty())
        return true;

    typename operand_type::const_iterator common_lwb;
    typename operand_type::const_iterator common_upb;

    if(!Set::common_range(common_lwb, common_upb, operand, object))
        return true;

    typename operand_type::const_iterator it = common_lwb;
    while(it != common_upb)
    {
        object.add_intersection(intersection, (it++)->KEY_VALUE);
        if(!intersection.empty())
            return false;
    }

    return true; 
}

//--- IntervalSet -------------------------------------------------------------
/*CL
template 
<
    class SubType, class DomainT, class CodomainT, class Traits,
 
    ITL_COMPARE Compare, ITL_ALLOC Alloc,
    template
    <    
        class, template<class,ITL_COMPARE>class, 
        ITL_COMPARE, ITL_ALLOC
    >
    class IntervalSet
>
//JODO boost::enable_if
bool is_disjoint
(
          interval_base_map<SubType,DomainT,CodomainT,
                            Traits,Compare,Combine,Section,Interval,Alloc>& object,
    const IntervalSet<DomainT,Interval,Compare,Alloc>& operand
)
{
    typedef interval_base_map<SubType,DomainT,CodomainT,
                              Traits,Compare,Combine,Section,Interval,Alloc> object_type;
    typedef IntervalSet<DomainT,Interval,Compare,Alloc> operand_type;
    object_type intersection;

    if(operand.empty())
        return true;

    typename operand_type::const_iterator common_lwb;
    typename operand_type::const_iterator common_upb;

    if(!Set::common_range(common_lwb, common_upb, operand, object))
        return true;

    typename operand_type::const_iterator it = common_lwb;
    while(it != common_upb)
    {
        object.add_intersection(intersection, *it++);
        if(!intersection.empty())
            return false;
    }

    return true; 
}
*/

template 
<
    class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap,
    template
    <    
        class, template<class,ITL_COMPARE>class, 
        ITL_COMPARE, ITL_ALLOC
    >
    class IntervalSet
>
//JODO boost::enable_if
bool is_disjoint
(
    const IntervalMap<DomainT,CodomainT,
                      Traits,Compare,Combine,Section,Interval,Alloc>&  object,
    const IntervalSet<DomainT,Interval,Compare,Alloc>& operand
)
{
    typedef IntervalMap<DomainT,CodomainT,
                        Traits,Compare,Combine,Section,Interval,Alloc> object_type;
    typedef IntervalSet<DomainT,Interval,Compare,Alloc> operand_type;
    object_type intersection;

    if(operand.empty())
        return true;

    typename operand_type::const_iterator common_lwb;
    typename operand_type::const_iterator common_upb;

    if(!Set::common_range(common_lwb, common_upb, operand, object))
        return true;

    typename operand_type::const_iterator it = common_lwb;
    while(it != common_upb)
    {
        object.add_intersection(intersection, *it++);
        if(!intersection.empty())
            return false;
    }

    return true; 
}

template 
<
    class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap,
    template
    <    
        class, template<class,ITL_COMPARE>class, 
        ITL_COMPARE, ITL_ALLOC
    >
    class IntervalSet
>
//JODO boost::enable_if
bool is_disjoint
(
    const IntervalSet<DomainT,Interval,Compare,Alloc>& left,
    const IntervalMap<DomainT,CodomainT,
                      Traits,Compare,Combine,Section,Interval,Alloc>&  right
)
{
	return is_disjoint(right, left);
}

//-----------------------------------------------------------------------------
// enclosure
//-----------------------------------------------------------------------------
template 
<
    class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class IntervalMap
>
typename IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>::interval_type 
enclosure(const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object)
{
    typedef IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> IntervalMapT;
    typedef typename IntervalMapT::interval_type interval_type;
    return 
        object.empty() ? neutron<interval_type>::value()
        : (object.begin()->KEY_VALUE)
            .span(object.rbegin()->KEY_VALUE);
}

template 
<
    class SubType, class DomainT, class CodomainT, class Traits, 
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
typename interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>::interval_type 
enclosure(const interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& object)
{
    typedef interval_base_map<SubType,DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc> IntervalMapT;
    typedef typename IntervalMapT::interval_type interval_type;
    return 
        object.empty() ? neutron<interval_type>::value()
        : (object.begin()->KEY_VALUE)
            .span(object.rbegin()->KEY_VALUE);
}

}} // namespace itl boost

#endif


