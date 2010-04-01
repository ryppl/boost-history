/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_INTERVAL_FUNCTIONS_HPP_JOFA_100323
#define BOOST_ITL_INTERVAL_FUNCTIONS_HPP_JOFA_100323

#include <boost/utility/enable_if.hpp>
#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/type_traits/is_asymmetric_interval.hpp>
#include <boost/itl/type_traits/is_universal_interval.hpp>
#include <boost/itl/type_traits/is_discrete_interval.hpp>
#include <boost/itl/type_traits/is_continuous_interval.hpp>
#include <boost/itl/type_traits/has_dynamic_bounds.hpp>

namespace boost{namespace itl
{
typedef unsigned char bound_type; //JODO encapsulation in a small class


//==============================================================================
//= Construct
//==============================================================================

//- construct(3) ---------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<is_asymmetric_interval<IntervalT>, IntervalT>::type
construct(const typename IntervalT::domain_type& low, //JODO Parameter passing DomainP
          const typename IntervalT::domain_type& up,
          itl::bound_type bounds)
{
    return IntervalT(low, up);
}

template<class IntervalT>
typename boost::enable_if<is_universal_interval<IntervalT>, IntervalT>::type
construct(const typename IntervalT::domain_type& low,
          const typename IntervalT::domain_type& up,
          itl::bound_type bounds)
{
    return IntervalT(low, up, bounds);
}

template<class IntervalT>
typename boost::enable_if<is_continuous_interval<IntervalT>, IntervalT>::type
construct(const typename IntervalT::domain_type& low,
          const typename IntervalT::domain_type& up,
          itl::bound_type bounds)
{
    return IntervalT(low, up, bounds);
}

//- is_empty -------------------------------------------------------------------
/** Is the interval empty? */
template<class IntervalT>
typename boost::enable_if<is_asymmetric_interval<IntervalT>, bool>::type
is_empty(const IntervalT& object)
{ 
    return IntervalT::domain_less_equal(object.upper(), object.lower()); 
}

template<class IntervalT>
typename boost::enable_if<is_universal_interval<IntervalT>, bool>::type
is_empty(const IntervalT& object)
{ 
    return object.empty(); 
}

template<class IntervalT>
typename boost::enable_if<is_discrete_interval<IntervalT>, bool>::type
is_empty(const IntervalT& object)
{ 
    return IntervalT::domain_less(object.last(), object.first()); 
}

template<class IntervalT>
typename boost::enable_if<is_continuous_interval<IntervalT>, bool>::type
is_empty(const IntervalT& object)
{ 
    if(is_closed(object))
        return IntervalT::domain_less(object.lower(), object.upper());
    else
        return IntervalT::domain_less_equal(object.lower(), object.upper());
}

//==============================================================================
//= Selection
//==============================================================================

/*JODO
template<class IntervalT>
typename boost::enable_if<!is_continuous<typename IntervalT::domain_type>, 
                          typename IntervalT::domain_type>::type
first(const IntervalT& object)
{ 
}
*/

//==============================================================================
//= Equivalences and Orderings
//==============================================================================
//- exclusive_less -------------------------------------------------------------
/** Maximal element of <tt>left</tt> is less than the minimal element of 
    <tt>right</tt> */
template<class IntervalT>
typename boost::enable_if<is_asymmetric_interval<IntervalT>, bool>::type
exclusive_less(const IntervalT& left, const IntervalT& right)
{ 
    return IntervalT::domain_less_equal(left.upper(), right.lower()); 
}

template<class IntervalT>
typename boost::enable_if<is_universal_interval<IntervalT>, bool>::type
exclusive_less(const IntervalT& left, const IntervalT& right)
{ 
    return left.exclusive_less(right); 
}


template<class IntervalT>
typename boost::enable_if<is_discrete_interval<IntervalT>, bool>::type
exclusive_less(const IntervalT& left, const IntervalT& right)
{ 
    return IntervalT::domain_less(left.last(), right.first()); 
}

template<class IntervalT>
typename boost::enable_if<is_continuous_interval<IntervalT>, bool>::type
exclusive_less(const IntervalT& left, const IntervalT& right)
{ 
    if(inner_bounds(left,right).is_closed())
        return IntervalT::domain_less(left.lower(), right.upper());
    else
        return IntervalT::domain_less_equal(left.lower(), right.upper());
}

//------------------------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<is_asymmetric_interval<IntervalT>, bool>::type
lower_less(const IntervalT& left, const IntervalT& right)
{
    return IntervalT::domain_less(left.lower(), right.lower());
}
    
template<class IntervalT>
typename boost::enable_if<is_universal_interval<IntervalT>, bool>::type
lower_less(const IntervalT& left, const IntervalT& right)
{
    return left.lower_less(right);
}

template<class IntervalT>
typename boost::enable_if<is_discrete_interval<IntervalT>, bool>::type
lower_less(const IntervalT& left, const IntervalT& right)
{
    return IntervalT::domain_less(left.first(), right.first());
}
    
template<class IntervalT>
typename boost::enable_if<is_continuous_interval<IntervalT>, bool>::type
lower_less(const IntervalT& left, const IntervalT& right)
{
    if(left_bounds(left,right).is_right_open())  //'[(' == 10
        return IntervalT::domain_less_equal(left.lower(), right.lower());
    else 
        return IntervalT::domain_less(left.lower(), right.lower());
}
    

//------------------------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<is_asymmetric_interval<IntervalT>, bool>::type
upper_less(const IntervalT& left, const IntervalT& right)
{
    return IntervalT::domain_less(left.upper(), right.upper());
}

template<class IntervalT>
typename boost::enable_if<is_universal_interval<IntervalT>, bool>::type
upper_less(const IntervalT& left, const IntervalT& right)
{
    return left.upper_less(right);
}

template<class IntervalT>
typename boost::enable_if<is_discrete_interval<IntervalT>, bool>::type
upper_less(const IntervalT& left, const IntervalT& right)
{
    return IntervalT::domain_less(left.last(), right.last());
}
    
template<class IntervalT>
typename boost::enable_if<is_continuous_interval<IntervalT>, bool>::type
upper_less(const IntervalT& left, const IntervalT& right)
{
    if(right_bounds(left,right).is_left_open())
        return IntervalT::domain_less_equal(left.upper(), right.upper());
    else
        return IntervalT::domain_less(left.upper(), right.upper());
}
    
//------------------------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<has_dynamic_bounds<IntervalT>, 
                          typename IntervalT::domain_type   >::type
lower_min(const IntervalT& left, const IntervalT& right)
{
    return lower_less(left, right) ? left.lower() : right.lower();
}

//------------------------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<has_dynamic_bounds<IntervalT>, 
                          typename IntervalT::domain_type   >::type
lower_max(const IntervalT& left, const IntervalT& right)
{
    return lower_less(left, right) ? right.lower() : left.lower();
}

//------------------------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<has_dynamic_bounds<IntervalT>, 
                          typename IntervalT::domain_type   >::type
upper_max(const IntervalT& left, const IntervalT& right)
{
    return upper_less(left, right) ? right.upper() : left.upper();
}

//------------------------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<has_dynamic_bounds<IntervalT>, 
                          typename IntervalT::domain_type   >::type
upper_min(const IntervalT& left, const IntervalT& right)
{
    return upper_less(left, right) ? left.upper() : right.upper();
}


//------------------------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<is_asymmetric_interval<IntervalT>, bool>::type
lower_equal(const IntervalT& left, const IntervalT& right)
{
    return IntervalT::domain_equal(left.lower(), right.lower());
}

template<class IntervalT>
typename boost::enable_if<is_universal_interval<IntervalT>, bool>::type
lower_equal(const IntervalT& left, const IntervalT& right)
{
    return left.lower_equal(right);
}

template<class IntervalT>
typename boost::enable_if<is_discrete_interval<IntervalT>, bool>::type
lower_equal(const IntervalT& left, const IntervalT& right)
{
    return IntervalT::domain_equal(left.first(), right.first());
}

template<class IntervalT>
typename boost::enable_if<is_continuous_interval<IntervalT>, bool>::type
lower_equal(const IntervalT& left, const IntervalT& right)
{
    return (left_bound(left)==left_bound(right))
        && IntervalT::domain_equal(left.lower(), right.lower());
}


//------------------------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<is_asymmetric_interval<IntervalT>, bool>::type
upper_equal(const IntervalT& left, const IntervalT& right)
{
    return IntervalT::domain_equal(left.upper(), right.upper());
}

template<class IntervalT>
typename boost::enable_if<is_universal_interval<IntervalT>, bool>::type
upper_equal(const IntervalT& left, const IntervalT& right)
{
    return left.upper_equal(right);
}
    
template<class IntervalT>
typename boost::enable_if<is_discrete_interval<IntervalT>, bool>::type
upper_equal(const IntervalT& left, const IntervalT& right)
{
    return IntervalT::domain_equal(left.last(), right.last());
}

template<class IntervalT>
typename boost::enable_if<is_continuous_interval<IntervalT>, bool>::type
upper_equal(const IntervalT& left, const IntervalT& right)
{
    return (right_bound(left)==right_bound(right))
        && IntervalT::domain_equal(left.upper(), right.upper());
}


//- operator == ----------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<is_interval<IntervalT>, bool>::type
operator == (const IntervalT& lhs, const IntervalT& rhs)
{
    return (itl::is_empty(lhs) && itl::is_empty(rhs)) 
        || (lower_equal(lhs,rhs) && upper_equal(lhs,rhs));
}

//- operator < -----------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<is_interval<IntervalT>, bool>::type
operator < (const IntervalT& lhs, const IntervalT& rhs)
{
    if(itl::is_empty(lhs)) 
        return !itl::is_empty(rhs);
    else 
        return lower_less(lhs,rhs) 
            || (lower_equal(lhs,rhs) && upper_less(lhs,rhs)); 
}



//------------------------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<is_asymmetric_interval<IntervalT>, bool>::type
touches(const IntervalT& left, const IntervalT& right)
{ 
    return IntervalT::domain_equal(left.upper(), right.lower()); 
}

template<class IntervalT>
typename boost::enable_if<is_universal_interval<IntervalT>, bool>::type
touches(const IntervalT& left, const IntervalT& right)
{ 
    return left.touches(right); 
}

template<class IntervalT>
typename boost::enable_if<is_discrete_interval<IntervalT>, bool>::type
touches(const IntervalT& left, const IntervalT& right)
{
    return IntervalT::domain_equal(succ(left.last()), right.first());
}

template<class IntervalT>
typename boost::enable_if<is_continuous_interval<IntervalT>, bool>::type
touches(const IntervalT& left, const IntervalT& right)
{
    return inner_bounds(left,right).are_complementary() 
        && IntervalT::domain_equal(left.upper(), right.lower());
}



//==============================================================================
//= Addition
//==============================================================================
//- hull -----------------------------------------------------------------------
/** \c hull returns the smallest interval containing \c left and \c right. */
template<class IntervalT>
typename boost::enable_if<is_asymmetric_interval<IntervalT>, IntervalT>::type
hull(IntervalT left, const IntervalT& right)
{
    if(itl::is_empty(right))
        return left;
    else if(itl::is_empty(left))
        return right;

    //JODO return construct
    return 
        IntervalT
        (
            (std::min)(left.lower(), right.lower(), IntervalT::domain_compare()), 
            (std::max)(left.upper(), right.upper(), IntervalT::domain_compare())
        );
}

template<class IntervalT>
typename boost::enable_if<is_universal_interval<IntervalT>, IntervalT>::type
hull(IntervalT left, const IntervalT& right)
{
    return left.extend(right);
}

template<class IntervalT>
typename boost::enable_if<has_dynamic_bounds<IntervalT>, IntervalT>::type
hull(IntervalT left, const IntervalT& right)
{
    if(itl::is_empty(right))
        return left;
    else if(itl::is_empty(left))
        return right;

    return  construct
            (
                lower_min(left, right), 
                upper_max(left, right), 
                outer_bounds(left, right)
            );
}

//==============================================================================
//= Subtraction
//==============================================================================
//- left_subtract --------------------------------------------------------------
/** subtract \c left_minuend from the \c right interval on it's left side. 
    Return the difference: The part of \c right right of \c left_minuend.
\code
right_over = right - left_minuend; //on the left.
...      d) : right
... c)      : left_minuend
     [c  d) : right_over
\endcode
*/
template<class IntervalT>
typename boost::enable_if<is_asymmetric_interval<IntervalT>, IntervalT>::type
left_subtract(IntervalT right, const IntervalT& left_minuend)
{
    if(exclusive_less(left_minuend, right))
        return right; 
    //JODO return construct(left_minuend.upper(), right.upper());
    return IntervalT(left_minuend.upper(), right.upper());
}

template<class IntervalT>
typename boost::enable_if<is_universal_interval<IntervalT>, IntervalT>::type
left_subtract(IntervalT right, const IntervalT& left_minuend)
{
    return right.left_subtract(left_minuend);
}

template<class IntervalT>
typename boost::enable_if<has_dynamic_bounds<IntervalT>, IntervalT>::type
left_subtract(IntervalT right, const IntervalT& left_minuend)
{
    //JODO
    if(exclusive_less(left_minuend, right))
        return right; 
    return construct(left_minuend.upper(), right.upper(), 
                     left_subtract_bounds(right, left_minuend));
}


//- right_subtract -------------------------------------------------------------
/** subtract \c right_minuend from the \c left interval on it's right side. 
    Return the difference: The part of \c left right of \c right_minuend.
\code
left_over = left - right_minuend; //on the right side.
[a      ...  : left
     [b ...  : right_minuend
[a  b)       : left_over
\endcode
*/
template<class IntervalT>
typename boost::enable_if<is_asymmetric_interval<IntervalT>, IntervalT>::type
right_subtract(IntervalT left, const IntervalT& right_minuend)
{
    if(exclusive_less(left, right_minuend))
        return left; 
    return IntervalT(left.lower(), right_minuend.lower());
    //JODO return construct(left.lower(), right_minuend.lower());
}

template<class IntervalT>
typename boost::enable_if<is_universal_interval<IntervalT>, IntervalT>::type
right_subtract(IntervalT left, const IntervalT& right_minuend)
{
    return left.right_subtract(right_minuend);
}

template<class IntervalT>
typename boost::enable_if<has_dynamic_bounds<IntervalT>, IntervalT>::type
right_subtract(IntervalT left, const IntervalT& right_minuend)
{
    //JODO s.o.
    if(exclusive_less(left, right_minuend))
        return left; 
    return construct(left.lower(), right_minuend.lower(),
                     right_subtract_bounds(left, right)  );
}

//==============================================================================
//= Intersection
//==============================================================================
//- operator & -----------------------------------------------------------------
/** Returns the intersection of \c left and \c right interval. */
template<class IntervalT>
typename boost::enable_if<is_asymmetric_interval<IntervalT>, IntervalT>::type
operator & (IntervalT left, const IntervalT& right)
{
    if(itl::is_empty(left) || itl::is_empty(right))
        return IntervalT(); //JODO return neutron<IntervalT>::value; neutron for new interval_types.
    else 
        return
        IntervalT((std::max)(left.lower(), right.lower(), IntervalT::domain_compare()),
                  (std::min)(left.upper(), right.upper(), IntervalT::domain_compare()));
}

template<class IntervalT>
typename boost::enable_if<is_universal_interval<IntervalT>, IntervalT>::type
operator & (IntervalT left, const IntervalT& right)
{
    return left &= right;
}

template<class IntervalT>
typename boost::enable_if<has_dynamic_bounds<IntervalT>, IntervalT>::type
operator & (IntervalT left, const IntervalT& right)
{
    if(itl::is_empty(left) || itl::is_empty(right))
        return IntervalT(); //JODO return neutron<IntervalT>::value; neutron for new interval_types.
    else 
        return  construct
                (
                    lower_max(left, right), 
                    upper_min(left, right), 
                    outer_bounds(left, right)
                );
}


//- intersects -----------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<is_interval<IntervalT>, bool>::type
intersects(const IntervalT& left, const IntervalT& right)
{ 
    return !(exclusive_less(left,right) || exclusive_less(right,left)); 
}

//- disjoint -------------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<is_interval<IntervalT>, bool>::type
disjoint(const IntervalT& left, const IntervalT& right)
{ 
    return exclusive_less(left,right) || exclusive_less(right,left); 
}

//------------------------------------------------------------------------------

}} // namespace itl boost

#endif

