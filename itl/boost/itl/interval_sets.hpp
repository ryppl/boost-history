/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __itl_interval_sets_hpp_JOFA_080930__
#define __itl_interval_sets_hpp_JOFA_080930__

#include <boost/itl/interval_base_set.hpp>
#include <boost/itl/interval_set_algo.hpp>
#include <boost/itl/operators.hpp>

namespace boost{namespace itl
{

template<typename, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC> 
class interval_set;

//==============================================================================
//= Equivalences and Orderings
//==============================================================================
//------------------------------------------------------------------------------
// is_element_equal
//------------------------------------------------------------------------------
template 
<
    class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    class LeftIntervalSet, class RightIntervalSet
>
inline bool is_element_equal
(
    const interval_base_set<LeftIntervalSet, DomainT,Compare,Interval,Alloc>& left,
    const interval_base_set<RightIntervalSet,DomainT,Compare,Interval,Alloc>& right
)
{
    return Interval_Set::is_element_equal(left, right);
}

template 
<
    class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    class LeftIntervalSet, class RightIntervalSet
>
inline bool is_element_less
(
    const interval_base_set<LeftIntervalSet, DomainT,Compare,Interval,Alloc>& left,
    const interval_base_set<RightIntervalSet,DomainT,Compare,Interval,Alloc>& right
)
{
    return Interval_Set::is_element_less(left, right);
}

template 
<
    class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    class LeftIntervalSet, class RightIntervalSet
>
inline bool is_element_greater
(
    const interval_base_set<LeftIntervalSet, DomainT,Compare,Interval,Alloc>& left,
    const interval_base_set<RightIntervalSet,DomainT,Compare,Interval,Alloc>& right
)
{
    return Interval_Set::is_element_greater(left, right);
}

//==============================================================================
//= Addition
//==============================================================================
/** \c operator \c += adds an interval_base_set \c operand to an interval set
    \c object. 

	\b Returns: A reference to \c object. 

	\b Complexity: loglinear.
*/
template 
<
    class ObjectT,
    class SubType, class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
inline
typename boost::enable_if<is_interval_set<ObjectT>, 
                          ObjectT>::type&
operator +=
(
          ObjectT& object,
    const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& operand
)
{
    typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> operand_type;
	typename ObjectT::iterator prior_ = object.end();
    const_FORALL(typename operand_type, elem_, operand) 
        prior_ = object.add(prior_, *elem_); 

    return object; 
}

/*CL?
//==============================================================================
//= Subtraction
//==============================================================================
template 
<
    class ObjectT,
    class SubType, class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
inline
typename boost::enable_if<is_interval_set_companion<ObjectT, SubType>,
                          ObjectT>::type&
operator -=
(
          ObjectT& object,
    const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& operand
)
{
    typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> operand_type;

    if(operand.empty())
        return object;

    typename operand_type::const_iterator common_lwb;
    typename operand_type::const_iterator common_upb;

    if(!Set::common_range(common_lwb, common_upb, operand, object))
        return object;

    typename operand_type::const_iterator it_ = common_lwb;
    while(it_ != common_upb)
		object.erase(*it_++);

    return object; 
}
*/

//-----------------------------------------------------------------------------
// symmetric difference ^=
//-----------------------------------------------------------------------------
template 
<
    class ObjectT,
    class SubType, class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
inline 
typename boost::enable_if<is_interval_set<ObjectT>, 
                          ObjectT>::type&
operator ^=
(
          ObjectT& object,
    const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& operand
)
{ 
    return object.flip(operand); 
}

//-----------------------------------------------------------------------------
// is_disjoint
//-----------------------------------------------------------------------------
template 
<
    class DomainT, ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    class LeftIntervalSet, class RightIntervalSet
>
bool is_disjoint
(
    const interval_base_set<LeftIntervalSet, DomainT,Compare,Interval,Alloc>& object,
    const interval_base_set<RightIntervalSet,DomainT,Compare,Interval,Alloc>& operand
)
{
    typedef interval_base_set<LeftIntervalSet, DomainT,Compare,Interval,Alloc> object_type;
    typedef interval_base_set<RightIntervalSet,DomainT,Compare,Interval,Alloc> operand_type;
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

//-----------------------------------------------------------------------------
// insert
//-----------------------------------------------------------------------------
template 
<
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
inline interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& 
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
inline interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& 
erase
(
          interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& object,
    const IntervalSet              <DomainT,Compare,Interval,Alloc>& operand
)
{
    typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> operand_type;

    if(operand.empty())
        return object;

    typename operand_type::const_iterator common_lwb;
    typename operand_type::const_iterator common_upb;

    if(!Set::common_range(common_lwb, common_upb, operand, object))
        return object;

    typename operand_type::const_iterator it_ = common_lwb;
    while(it_ != common_upb)
		object.erase(*it_++);

    return object; 
    //CL return object -= operand; 
}


//-----------------------------------------------------------------------------
// hull
//-----------------------------------------------------------------------------
template 
<
    class SubType, class DomainT, 
    ITL_COMPARE Compare, template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
inline typename interval_base_set<SubType,DomainT,Compare,Interval,Alloc>::interval_type 
hull(const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& object)
{
    typedef interval_base_set<SubType,DomainT,Compare,Interval,Alloc> IntervalSetT;
    typedef typename IntervalSetT::interval_type interval_type;
    return 
        object.empty() ? neutron<interval_type>::value()
        : (*object.begin()).span(*object.rbegin());
}


}} // namespace itl boost

#endif


