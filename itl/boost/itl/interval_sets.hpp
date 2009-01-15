/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#ifndef __itl_interval_sets_hpp_JOFA_080930__
#define __itl_interval_sets_hpp_JOFA_080930__

#include <boost/itl/interval_base_set.hpp>
#include <boost/itl/interval_set_algo.hpp>

namespace boost{namespace itl
{

template<typename, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC> 
class interval_set;

//-----------------------------------------------------------------------------
// addition +=
//-----------------------------------------------------------------------------
template 
<
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& 
operator +=
(
          interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& object,
    const IntervalSet              <DomainT,Compare,Interval,Alloc>& operand
)
{
    typedef IntervalSet<DomainT,Compare,Interval,Alloc> set_type;
    const_FORALL(typename set_type, elem_, operand) 
        object.add(*elem_); 

    return object; 
}

template 
<
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
interval_base_set<SubType,DomainT,Compare,Interval,Alloc> 
operator +
(
    const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& object,
    const IntervalSet              <DomainT,Compare,Interval,Alloc>& operand
)
{
	typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> ObjectT;
	return ObjectT(object) += operand; 
}
//-----------------------------------------------------------------------------

//--- interval_type -----------------------------------------------------------
template 
<
    class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
IntervalSet<DomainT,Compare,Interval,Alloc>& 
operator +=
(
    IntervalSet<DomainT,Compare,Interval,Alloc>& object,
    const typename IntervalSet<DomainT,Compare,Interval,Alloc>::interval_type& interval
)
{
    return object.add(interval);
}

template 
<
    class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
IntervalSet<DomainT,Compare,Interval,Alloc>
operator +
(
    const IntervalSet<DomainT,Compare,Interval,Alloc>& object,
    const typename IntervalSet<DomainT,Compare,Interval,Alloc>::interval_type& operand
)
{
	typedef IntervalSet<DomainT,Compare,Interval,Alloc> ObjectT;
	return ObjectT(object) += operand; 
}
//-----------------------------------------------------------------------------

//--- domain_type -------------------------------------------------------------
template 
<
    class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
IntervalSet<DomainT,Compare,Interval,Alloc>& 
operator +=
(
    IntervalSet<DomainT,Compare,Interval,Alloc>& object,
    const typename IntervalSet<DomainT,Compare,Interval,Alloc>::domain_type& value
)
{
    return object.add(value);
}

template 
<
    class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
IntervalSet<DomainT,Compare,Interval,Alloc> 
operator +
(
    const IntervalSet<DomainT,Compare,Interval,Alloc>& object,
    const typename IntervalSet<DomainT,Compare,Interval,Alloc>::domain_type& operand
)
{
	typedef IntervalSet<DomainT,Compare,Interval,Alloc> ObjectT;
	return ObjectT(object) += operand; 
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// addition |=
//-----------------------------------------------------------------------------
template 
<
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& 
operator |=
(
          interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& object,
    const IntervalSet              <DomainT,Compare,Interval,Alloc>& operand
)
{ return object += operand; }

template 
<
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
interval_base_set<SubType,DomainT,Compare,Interval,Alloc> 
operator |
(
    const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& object,
    const IntervalSet              <DomainT,Compare,Interval,Alloc>& operand
)
{
	typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> ObjectT;
	return ObjectT(object) += operand; 
}
//-----------------------------------------------------------------------------

//--- interval_type -----------------------------------------------------------
template 
<
    class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
IntervalSet<DomainT,Compare,Interval,Alloc>& 
operator |=
(
    IntervalSet<DomainT,Compare,Interval,Alloc>& object,
    const typename IntervalSet<DomainT,Compare,Interval,Alloc>::interval_type& interval
)
{
    return object.add(interval);
}

template 
<
    class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
IntervalSet<DomainT,Compare,Interval,Alloc>
operator |
(
    const IntervalSet<DomainT,Compare,Interval,Alloc>& object,
    const typename IntervalSet<DomainT,Compare,Interval,Alloc>::interval_type& operand
)
{
	typedef IntervalSet<DomainT,Compare,Interval,Alloc> ObjectT;
	return ObjectT(object) += operand; 
}
//-----------------------------------------------------------------------------

//--- domain_type -------------------------------------------------------------
template 
<
    class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
IntervalSet<DomainT,Compare,Interval,Alloc>& 
operator |=
(
    IntervalSet<DomainT,Compare,Interval,Alloc>& object,
    const typename IntervalSet<DomainT,Compare,Interval,Alloc>::domain_type& value
)
{
    return object.add(value);
}

template 
<
    class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
IntervalSet<DomainT,Compare,Interval,Alloc> 
operator |
(
    const IntervalSet<DomainT,Compare,Interval,Alloc>& object,
    const typename IntervalSet<DomainT,Compare,Interval,Alloc>::domain_type& operand
)
{
	typedef IntervalSet<DomainT,Compare,Interval,Alloc> ObjectT;
	return ObjectT(object) += operand; 
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// difference -=
//-----------------------------------------------------------------------------
template 
<
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& 
operator -=
(
          interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& object,
    const IntervalSet              <DomainT,Compare,Interval,Alloc>& operand
)
{
        typedef IntervalSet<DomainT,Compare,Interval,Alloc> operand_type;
        const_FORALL(typename operand_type, elem_, operand) 
            object.subtract(*elem_); 

        return object; 
}

template 
<
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
interval_base_set<SubType,DomainT,Compare,Interval,Alloc>
operator -
(
    const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& object,
    const IntervalSet              <DomainT,Compare,Interval,Alloc>& operand
)
{
	typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> ObjectT;
	return ObjectT(object) -= operand; 
}
//-----------------------------------------------------------------------------


//--- interval_type -----------------------------------------------------------
template 
<
    class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
IntervalSet<DomainT,Compare,Interval,Alloc>& 
operator -=
(
    IntervalSet<DomainT,Compare,Interval,Alloc>& object,
    const typename IntervalSet<DomainT,Compare,Interval,Alloc>::interval_type& interval
)
{
    return object.subtract(interval);
}

template 
<
    class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
IntervalSet<DomainT,Compare,Interval,Alloc>
operator -
(
    const IntervalSet<DomainT,Compare,Interval,Alloc>& object,
    const typename IntervalSet<DomainT,Compare,Interval,Alloc>::interval_type& operand
)
{
	typedef IntervalSet<DomainT,Compare,Interval,Alloc> ObjectT;
	return ObjectT(object) -= operand; 
}
//-----------------------------------------------------------------------------

//--- domain_type -------------------------------------------------------------
template 
<
    class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
IntervalSet<DomainT,Compare,Interval,Alloc>& 
operator -=
(
    IntervalSet<DomainT,Compare,Interval,Alloc>& object,
    const typename IntervalSet<DomainT,Compare,Interval,Alloc>::domain_type& value
)
{
    return object.subtract(value);
}

template 
<
    class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
IntervalSet<DomainT,Compare,Interval,Alloc>
operator -
(
    const IntervalSet<DomainT,Compare,Interval,Alloc>& object,
    const typename IntervalSet<DomainT,Compare,Interval,Alloc>::domain_type& operand
)
{
	typedef IntervalSet<DomainT,Compare,Interval,Alloc> ObjectT;
	return ObjectT(object) -= operand; 
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// intersection &=
//-----------------------------------------------------------------------------
template 
<
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& 
operator &=
(
          interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& object,
    const IntervalSet              <DomainT,Compare,Interval,Alloc>& operand
)
{
    typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> object_type;
    typedef IntervalSet              <DomainT,Compare,Interval,Alloc> operand_type;
    object_type intersection;

    if(operand.empty())
    {
        object.clear();
        return object;
    }

    typename operand_type::const_iterator common_lwb;
    typename operand_type::const_iterator common_upb;

    if(!Set::common_range(common_lwb, common_upb, operand, object))
    {
        object.clear();
        return object;
    }

    typename operand_type::const_iterator it = common_lwb;
    while(it != common_upb)
        object.add_intersection(intersection, *it++);

    object.swap(intersection);

    return object; 
}

template 
<
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
interval_base_set<SubType,DomainT,Compare,Interval,Alloc>
operator &
(
    const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& object,
    const IntervalSet              <DomainT,Compare,Interval,Alloc>& operand
)
{
	typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> ObjectT;
	return ObjectT(object) &= operand; 
}
//-----------------------------------------------------------------------------

//--- interval_type -----------------------------------------------------------
template 
<
    class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
IntervalSet<DomainT,Compare,Interval,Alloc>& 
operator &=
(
    IntervalSet<DomainT,Compare,Interval,Alloc>& object,
    const typename IntervalSet<DomainT,Compare,Interval,Alloc>::interval_type& interval
)
{
    typedef IntervalSet<DomainT,Compare,Interval,Alloc> object_type;
    object_type intersection;

    if(interval.empty())
    {
        object.clear();
        return object;
    }

    object.add_intersection(intersection, interval);
    object.swap(intersection);
    return object; 
}

template 
<
    class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
IntervalSet<DomainT,Compare,Interval,Alloc> 
operator &
(
    const IntervalSet<DomainT,Compare,Interval,Alloc>& object,
    const typename IntervalSet<DomainT,Compare,Interval,Alloc>::interval_type& operand
)
{
	typedef IntervalSet<DomainT,Compare,Interval,Alloc> ObjectT;
	return ObjectT(object) &= operand; 
}
//-----------------------------------------------------------------------------

//--- domain_type -------------------------------------------------------------
template 
<
    class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
IntervalSet<DomainT,Compare,Interval,Alloc>& 
operator &=
(
    IntervalSet<DomainT,Compare,Interval,Alloc>& object,
    const DomainT& value
)
{
    typedef typename IntervalSet<DomainT,Compare,Interval,Alloc>
        ::interval_type interval_type;
    return object &= interval_type(value);
}

template 
<
    class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
IntervalSet<DomainT,Compare,Interval,Alloc>
operator &
(
    const IntervalSet<DomainT,Compare,Interval,Alloc>& object,
    const DomainT& operand
)
{
	typedef IntervalSet<DomainT,Compare,Interval,Alloc> ObjectT;
	return ObjectT(object) &= operand; 
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// is_element_equal
//-----------------------------------------------------------------------------
template 
<
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
bool is_element_equal
(
    const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& object,
    const IntervalSet              <DomainT,Compare,Interval,Alloc>& operand
)
{
	return Interval_Set::is_element_equal(object, operand);
}



//-----------------------------------------------------------------------------
// is_disjoint
//-----------------------------------------------------------------------------
template 
<
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
bool is_disjoint
(
          interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& object,
    const IntervalSet              <DomainT,Compare,Interval,Alloc>& operand
)
{
    typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> object_type;
    typedef IntervalSet              <DomainT,Compare,Interval,Alloc> operand_type;
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

/*CL used??? if so move it to interval_maps.hpp
template 
<
    class SubType, class DomainT, class CodomainT, class Traits,
    ITL_COMPARE Compare, ITL_COMBINE Combine, ITL_SECTION Section, 
	template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
bool is_disjoint
(
          interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& object,
    const IntervalMap<DomainT,CodomainT,
                      Traits,Compare,Combine,Section,Interval,Alloc>& operand
)
{
    typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> object_type;
    typedef IntervalMap<DomainT,CodomainT,
                        Traits,Compare,Interval,Alloc> operand_type;
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
        //JODO operand_type::key_value not working with gcc_3.4.4 (cygwin)
        //object.add_intersection(intersection, operand_type::key_value(it++));
        object.add_intersection(intersection, (it++)->KEY_VALUE);
        if(!intersection.empty())
            return false;
    }

    return true; 
}
*/

//-----------------------------------------------------------------------------
// insert
//-----------------------------------------------------------------------------
template 
<
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& 
insert
(
          interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& object,
    const IntervalSet              <DomainT,Compare,Interval,Alloc>& operand
)
{
    return object += operand; 
}
    
//-----------------------------------------------------------------------------
// erase
//-----------------------------------------------------------------------------
template 
<
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& 
erase
(
          interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& object,
    const IntervalSet              <DomainT,Compare,Interval,Alloc>& operand
)
{
    return object -= operand; 
}


//-----------------------------------------------------------------------------
// enclosure
//-----------------------------------------------------------------------------
template 
<
    class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
typename IntervalSet<DomainT,Compare,Interval,Alloc>::interval_type 
enclosure(const IntervalSet<DomainT,Compare,Interval,Alloc>& object)
{
    typedef IntervalSet<DomainT,Compare,Interval,Alloc> IntervalSetT;
    typedef typename IntervalSetT::interval_type interval_type;
    return 
        object.empty() ? neutron<interval_type>::value()
        : (*object.begin()).span(*object.rbegin());
}

template 
<
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
typename interval_base_set<SubType,DomainT,Compare,Interval,Alloc>::interval_type 
enclosure(const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& object)
{
    typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> IntervalSetT;
    typedef typename IntervalSetT::interval_type interval_type;
    return 
        object.empty() ? neutron<interval_type>::value()
        : (*object.begin()).span(*object.rbegin());
}



}} // namespace itl boost

#endif


