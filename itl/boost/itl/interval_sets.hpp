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
/** \par \b Returns true, if \c left and \c right contain the same elements. 
	\par \b Complexity: linear. */
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

/** \b Returns true, if \c left is lexicographically less than \c right. 
	\par Intervals are interpreted as sequence of elements.
	\par \b Complexity: linear. */
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

/** \b Returns true, if \c left is lexicographically greater than \c right. 
    \par Intervals are interpreted as sequence of elements.
	\par \b Complexity: linear. */
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
/** \par \c operator \c += adds an interval_base_set \c operand to an interval set \c object. 
	\par \b Returns: A reference to \c object. 
	\par \b Complexity: loglinear. */
template 
<
    class ObjectT, class SubType, 
	class DomainT, ITL_COMPARE Compare, 
	template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
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

//-----------------------------------------------------------------------------
// insert
//-----------------------------------------------------------------------------
/** Inserts an interval_base_set \c operand to an interval set \c object. 
    In the itl \c insert and \c += have the same effect for all \c Sets. 
	\par \b Returns: A reference to \c object. 
	\par \b Complexity: loglinear. */
template 
<
    class ObjectT, class SubType, 
	class DomainT, ITL_COMPARE Compare, 
	template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
inline typename boost::enable_if<is_interval_set<ObjectT>, ObjectT>::type&
insert
(
          ObjectT& object,
    const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& operand
)
{
    return object += operand; 
}
    

//==============================================================================
//= Symmetric difference ^=
//==============================================================================
/** \c operator \c ^= symmetrically subtracts an interval_base_set \c operand from an interval set \c object. 
	\par \b Returns: A reference to \c object. 
	\par \b Complexity: loglinear.
*/
template 
<
    class ObjectT, class SubType, 
	class DomainT, ITL_COMPARE Compare, 
	template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
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
/** \b Returns true, if \c object and \c operand have no common elements.
    Intervals are interpreted as sequence of elements.
	\par \b Complexity: loglinear. */
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
// erase
//-----------------------------------------------------------------------------
/** Erases an interval_base_set \c operand from an interval set \c object. 
    In the itl \c erase and \c -= have the same effect for all \c Sets. 
	\par \b Returns: A reference to \c object. 
	\par \b Complexity: loglinear. */
template 
<
    class ObjectT, class SubType, 
	class DomainT, ITL_COMPARE Compare, 
	template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
>
inline typename boost::enable_if<is_interval_set<ObjectT>, ObjectT>::type&
erase
(
          ObjectT& object,
    const interval_base_set<SubType,DomainT,Compare,Interval,Alloc>& operand
)
{
    return object -= operand; 
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


