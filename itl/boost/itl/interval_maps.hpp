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
#include <boost/itl/operators.hpp>

namespace boost{namespace itl
{

template
<    
    class, class, class, 
    ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
>
class interval_map;


//==============================================================================
//= Emptieness, containment
//==============================================================================
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
bool is_contained_in
(
    const LeftIntervalMap <DomainT,CodomainT,
                           Traits,Compare,Combine,Section,Interval,Alloc>& left,
    const RightIntervalMap<DomainT,CodomainT,
                           Traits,Compare,Combine,Section,Interval,Alloc>& right
)
{
    return Interval_Set::is_contained_in(left, right);
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
    class LeftIntervalMap,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class RightIntervalMap
>
bool contains
(
    const LeftIntervalMap <DomainT,CodomainT,
                           Traits,Compare,Combine,Section,Interval,Alloc>& left,
    const RightIntervalMap<DomainT,CodomainT,
                           Traits,Compare,Combine,Section,Interval,Alloc>& right
)
{
    return Interval_Set::contains(left, right);
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
    template<class,  ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
//JODO boost::enable_if
bool is_disjoint
(
    const IntervalMap<DomainT,CodomainT,
                      Traits,Compare,Combine,Section,Interval,Alloc>&  object,
    const IntervalSet<DomainT,Compare,Interval,Alloc>& operand
)
{
    typedef IntervalMap<DomainT,CodomainT,
                        Traits,Compare,Combine,Section,Interval,Alloc> object_type;
    typedef IntervalSet<DomainT,Compare,Interval,Alloc> operand_type;
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
    template<class,  ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
//JODO boost::enable_if
bool is_disjoint
(
    const IntervalSet<DomainT,Compare,Interval,Alloc>& left,
    const IntervalMap<DomainT,CodomainT,
                      Traits,Compare,Combine,Section,Interval,Alloc>&  right
)
{
	return is_disjoint(right, left);
}


//==============================================================================
//= Equivalences and Orderings 
//==============================================================================
//------------------------------------------------------------------------------
// is_element_equal
//------------------------------------------------------------------------------
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
bool is_element_less
(
    const LeftIntervalMap <DomainT,CodomainT,
                           Traits,Compare,Combine,Section,Interval,Alloc>& left,
    const RightIntervalMap<DomainT,CodomainT,
                           Traits,Compare,Combine,Section,Interval,Alloc>& right
)
{
    return Interval_Set::is_element_less(left, right);
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
    class LeftIntervalMap,
    template
    <    
        class, class, class, 
        ITL_COMPARE, ITL_COMBINE, ITL_SECTION, template<class,ITL_COMPARE>class, ITL_ALLOC
    >
    class RightIntervalMap
>
bool is_element_greater
(
    const LeftIntervalMap <DomainT,CodomainT,
                           Traits,Compare,Combine,Section,Interval,Alloc>& left,
    const RightIntervalMap<DomainT,CodomainT,
                           Traits,Compare,Combine,Section,Interval,Alloc>& right
)
{
    return Interval_Set::is_element_greater(left, right);
}

//------------------------------------------------------------------------------
// is_inclusion_equal
//------------------------------------------------------------------------------
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
bool is_inclusion_equal
(
    const LeftIntervalMap <DomainT,CodomainT,
                           Traits,Compare,Combine,Section,Interval,Alloc>& left,
    const RightIntervalMap<DomainT,CodomainT,
                           Traits,Compare,Combine,Section,Interval,Alloc>& right
)
{
    return Interval_Set::is_inclusion_equal(left, right);
}

//-----------------------------------------------------------------------------
// is_protonic_equal
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
bool is_protonic_equal
(
    const IntervalMap <DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& left,
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& right
)
{
	return Map::lexicographical_protonic_equal(left, right);
}

//-----------------------------------------------------------------------------
// addition +=
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// subtraction -=
//-----------------------------------------------------------------------------
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
ObjectT& operator -=
(
          ObjectT& object,
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& operand
)
{
    typedef IntervalMap<DomainT,CodomainT,
                        Traits,Compare,Combine,Section,Interval,Alloc> operand_type;
    const_FORALL(typename operand_type, elem_, operand) 
        object.subtract(*elem_); 

    return object; 
}

//-----------------------------------------------------------------------------
// symmetric difference ^=
//-----------------------------------------------------------------------------
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
ObjectT& operator ^=
(
          ObjectT& object,
    const IntervalMap<DomainT,CodomainT,Traits,Compare,Combine,Section,Interval,Alloc>& operand
)
{
    return object.flip(operand); 
}

//-----------------------------------------------------------------------------
// erasure -= of elements given by an interval_set
//-----------------------------------------------------------------------------
template 
<
    class ObjectT, 
	class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class,  ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
ObjectT& 
operator -=
(
          ObjectT& object,
    const IntervalSet<DomainT,Compare,Interval,Alloc>& erasure
)
{
    typedef IntervalSet<DomainT,Compare,Interval,Alloc> set_type;
    const_FORALL(typename set_type, interval_, erasure) 
        object.erase(*interval_); 

    return object; 
}


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


