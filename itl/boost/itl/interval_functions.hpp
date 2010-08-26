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
#include <boost/mpl/and.hpp>
#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/type_traits/is_discrete.hpp>
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

class interval_bounds; //JODO separate dynamically and statically bounded interval concepts
template<class DomainT> class bounded_value;

//------------------------------------------------------------------------------
//- Adapter classes
//------------------------------------------------------------------------------
template<class IntervalT>
struct intervals
{
    typedef typename IntervalT::domain_type    domain_type;
    typedef typename IntervalT::domain_compare domain_compare;

    static IntervalT construct(const domain_type& lo, const domain_type& up);

    static domain_type upper(const IntervalT& inter_val);
    static domain_type lower(const IntervalT& inter_val);
};


template<class IntervalT>
struct dynamic_intervals
{
    typedef typename IntervalT::domain_type    domain_type;
    typedef typename IntervalT::domain_compare domain_compare;

    static IntervalT construct(const domain_type& lo, const domain_type& up, interval_bounds bounds);
    static IntervalT construct_bounded(const bounded_value<domain_type>& lo, 
                                       const bounded_value<domain_type>& up);
};




//==============================================================================
//= Construct
//==============================================================================

template<class IntervalT>
typename enable_if<is_asymmetric_interval<IntervalT>, IntervalT>::type
construct(const typename IntervalT::domain_type& low, //JODO Parameter passing DomainP
          const typename IntervalT::domain_type& up,
          interval_bounds bounds = interval_bounds::right_open()) //JODO separate static intervals to avoid dependency
{
    return intervals<IntervalT>::construct(low, up);
}


template<class IntervalT>
typename enable_if<is_dynamic_bounded<IntervalT>, IntervalT>::type
construct(const typename IntervalT::domain_type& low,
          const typename IntervalT::domain_type& up,
          interval_bounds bounds = interval_bounds::right_open())
{
    return dynamic_intervals<IntervalT>::construct(low, up, bounds);
}


//- construct form bounded values ----------------------------------------------
template<class IntervalT>
typename enable_if<is_dynamic_bounded<IntervalT>, IntervalT>::type
construct(const typename IntervalT::bounded_domain_type& low,
          const typename IntervalT::bounded_domain_type& up)
{
    return dynamic_intervals<IntervalT>::construct_bounded(low, up);
}

template<class IntervalT>
typename enable_if<is_interval<IntervalT>, IntervalT>::type
span(const typename IntervalT::domain_type& lhs,
     const typename IntervalT::domain_type& rhs)
{
    if(IntervalT::domain_compare(lhs,rhs))
        return construct(lhs, rhs);
    else
        return construct(rhs, lhs);
}


//==============================================================================
//= Selection
//==============================================================================

template<class IntervalT>
inline typename enable_if<is_interval<IntervalT>, 
                          typename IntervalT::domain_type>::type
lower(const IntervalT& object)
{ 
    return intervals<IntervalT>::lower(object);
}

template<class IntervalT>
inline typename enable_if<is_interval<IntervalT>, 
                          typename IntervalT::domain_type>::type
upper(const IntervalT& object)
{ 
    return intervals<IntervalT>::upper(object);
}


//- first ----------------------------------------------------------------------
template<class IntervalT>
inline typename enable_if<is_static_rightopen<IntervalT>, 
                          typename IntervalT::domain_type>::type
first(const IntervalT& object)
{ 
    return lower(object);
}

template<class IntervalT>
inline typename enable_if<is_discrete_interval<IntervalT>, 
                          typename IntervalT::domain_type>::type
first(const IntervalT& object)
{ 
    return is_left_closed(object.bounds()) ? 
                    lower(object) : 
               succ(lower(object));
}

//- last -----------------------------------------------------------------------
template<class IntervalT>
inline typename enable_if<mpl::and_<is_static_rightopen<IntervalT>,
                                    is_discrete<typename IntervalT::domain_type> >,
                          typename IntervalT::domain_type>::type
last(const IntervalT& object)
{ 
    return pred(upper(object));
}

template<class IntervalT>
inline typename enable_if<is_discrete_interval<IntervalT>, 
                          typename IntervalT::domain_type>::type
last(const IntervalT& object)
{ 
    return is_right_closed(object.bounds()) ? 
                    upper(object) : 
               pred(upper(object));
}


//------------------------------------------------------------------------------
template<class IntervalT>
typename enable_if<is_dynamic_bounded<IntervalT>, 
                   typename IntervalT::bounded_domain_type>::type
bounded_lower(const IntervalT& object)
{ 
    return typename 
        IntervalT::bounded_domain_type(lower(object), 
                                       object.bounds().left()); 
}

template<class IntervalT>
typename enable_if<is_dynamic_bounded<IntervalT>, 
                   typename IntervalT::bounded_domain_type>::type
reverse_bounded_lower(const IntervalT& object)
{ 
    return typename 
        IntervalT::bounded_domain_type(lower(object), 
                                       object.bounds().reverse_left()); 
}

template<class IntervalT>
typename enable_if<is_dynamic_bounded<IntervalT>, 
                   typename IntervalT::bounded_domain_type>::type
bounded_upper(const IntervalT& object)
{ 
    return typename 
        IntervalT::bounded_domain_type(upper(object), 
                                       object.bounds().right()); 
}

template<class IntervalT>
typename enable_if<is_dynamic_bounded<IntervalT>, 
                   typename IntervalT::bounded_domain_type>::type
reverse_bounded_upper(const IntervalT& object)
{ 
    return typename 
        IntervalT::bounded_domain_type(upper(object), 
                                       object.bounds().reverse_right()); 
}


//==============================================================================
//= Containedness
//==============================================================================
//- is_empty -------------------------------------------------------------------
/** Is the interval empty? */
template<class IntervalT>
typename boost::enable_if<is_asymmetric_interval<IntervalT>, bool>::type
is_empty(const IntervalT& object)
{ 
    return IntervalT::domain_less_equal(upper(object), 
                                        lower(object)); 
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
    //CL return IntervalT::domain_less(last(object), first(object)); 

    if(object.bounds() == interval_bounds::closed())
        return IntervalT::domain_less(upper(object), lower(object)); 
    else if(object.bounds() == interval_bounds::open())
        return IntervalT::domain_less_equal(upper(object), succ(lower(object))); 
    else
        return IntervalT::domain_less_equal(upper(object), lower(object)); 
}

template<class IntervalT>
typename boost::enable_if<is_continuous_interval<IntervalT>, bool>::type
is_empty(const IntervalT& object)
{
    return     IntervalT::domain_less(upper(object), lower(object))
        || (   IntervalT::domain_equal(upper(object), lower(object))
            && object.bounds() != interval_bounds::closed()             );
}

//- contains -------------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<is_interval<IntervalT>, bool>::type
contains(const IntervalT& super, const IntervalT& sub)
{ 
    return lower_less_equal(super,sub) && upper_less_equal(sub,super);
}

template<class IntervalT>
typename boost::enable_if<is_interval<IntervalT>, bool>::type
contains(const IntervalT& super, const typename IntervalT::domain_type& element)
{ 
    return contains(super,IntervalT(element));
}

//- within ---------------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<is_interval<IntervalT>, bool>::type
within(const IntervalT& sub, const IntervalT& super)
{ 
    return contains(super,sub);
}


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
    return IntervalT::domain_less(last(left), first(right)); 
}

template<class IntervalT>
typename boost::enable_if<is_continuous_interval<IntervalT>, bool>::type
exclusive_less(const IntervalT& left, const IntervalT& right)
{ 
    return     IntervalT::domain_less(left.upper(), right.lower())
        || (   IntervalT::domain_equal(left.upper(), right.lower())
            && inner_bounds(left,right) != interval_bounds::open() );
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
    return IntervalT::domain_less(first(left), first(right));
}
    
template<class IntervalT>
typename boost::enable_if<is_continuous_interval<IntervalT>, bool>::type
lower_less(const IntervalT& left, const IntervalT& right)
{
    if(left_bounds(left,right) == interval_bounds::right_open())  //'[(' == 10
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
    return IntervalT::domain_less(last(left), last(right));
}
    
template<class IntervalT>
typename boost::enable_if<is_continuous_interval<IntervalT>, bool>::type
upper_less(const IntervalT& left, const IntervalT& right)
{
    if(right_bounds(left,right) == interval_bounds::left_open())
        return IntervalT::domain_less_equal(left.upper(), right.upper());
    else
        return IntervalT::domain_less(left.upper(), right.upper());
}
    
//------------------------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<has_dynamic_bounds<IntervalT>, 
                          typename IntervalT::bounded_domain_type   >::type
lower_min(const IntervalT& left, const IntervalT& right)
{
    return lower_less(left, right) ? bounded_lower(left) : bounded_lower(right);
}

//------------------------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<has_dynamic_bounds<IntervalT>, 
                          typename IntervalT::bounded_domain_type   >::type
lower_max(const IntervalT& left, const IntervalT& right)
{
    return lower_less(left, right) ? bounded_lower(right) : bounded_lower(left);
}

//------------------------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<has_dynamic_bounds<IntervalT>, 
                          typename IntervalT::bounded_domain_type   >::type
upper_max(const IntervalT& left, const IntervalT& right)
{
    return upper_less(left, right) ? bounded_upper(right) : bounded_upper(left);
}

//------------------------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<has_dynamic_bounds<IntervalT>, 
                          typename IntervalT::bounded_domain_type   >::type
upper_min(const IntervalT& left, const IntervalT& right)
{
    return upper_less(left, right) ? bounded_upper(left) : bounded_upper(right);
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
    return IntervalT::domain_equal(first(left), first(right));
}

template<class IntervalT>
typename boost::enable_if<is_continuous_interval<IntervalT>, bool>::type
lower_equal(const IntervalT& left, const IntervalT& right)
{
    return (left.bounds().left()==right.bounds().left())
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
    return IntervalT::domain_equal(last(left), last(right));
}

template<class IntervalT>
typename boost::enable_if<is_continuous_interval<IntervalT>, bool>::type
upper_equal(const IntervalT& left, const IntervalT& right)
{
    return (left.bounds().right()==right.bounds().right())
        && IntervalT::domain_equal(left.upper(), right.upper());
}

//------------------------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<is_interval<IntervalT>, bool>::type
lower_less_equal(const IntervalT& left, const IntervalT& right)
{
    return lower_less(left,right) || lower_equal(left,right);
}

template<class IntervalT>
typename boost::enable_if<is_interval<IntervalT>, bool>::type
upper_less_equal(const IntervalT& left, const IntervalT& right)
{
    return upper_less(left,right) || upper_equal(left,right);
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
    return IntervalT::domain_equal(succ(last(left)), first(right));
}

template<class IntervalT>
typename boost::enable_if<is_continuous_interval<IntervalT>, bool>::type
touches(const IntervalT& left, const IntervalT& right)
{
    return is_complementary(inner_bounds(left,right))
        && IntervalT::domain_equal(left.upper(), right.lower());
}


//==============================================================================
//= Size
//==============================================================================
//- cardinality ----------------------------------------------------------------

template<class IntervalT>
typename boost::enable_if<is_continuous_interval<IntervalT>, 
    typename IntervalT::size_type>::type
cardinality(const IntervalT& object)
{
    typedef typename IntervalT::size_type SizeT;
    if(itl::is_empty(object))
        return itl::neutron<SizeT>::value();
    else if(   object.bounds() == interval_bounds::closed() 
            && IntervalT::domain_equal(lower(object), upper(object)))
        return itl::unon<SizeT>::value();
    else 
        return infinity<SizeT>::value();
}

template<class IntervalT>
typename boost::enable_if<is_discrete_interval<IntervalT>, 
    typename IntervalT::size_type>::type
cardinality(const IntervalT& object)
{
    return (last(object) + itl::unon<typename IntervalT::size_type>::value()) - first(object);
}


template<class IntervalT>
typename boost::enable_if<is_continuous_asymmetric<IntervalT>, 
    typename IntervalT::size_type>::type
cardinality(const IntervalT& object)
{
    typedef typename IntervalT::size_type SizeT;
    if(itl::is_empty(object))
        return itl::neutron<SizeT>::value();
    else 
        return infinity<SizeT>::value();
}

template<class IntervalT>
typename boost::enable_if<is_discrete_asymmetric<IntervalT>, 
    typename IntervalT::size_type>::type
cardinality(const IntervalT& object)
{
    return (last(object) + itl::unon<typename IntervalT::size_type>::value()) - first(object);
}





//- size -----------------------------------------------------------------------
template<class IntervalT>
inline typename enable_if<is_interval<IntervalT>, 
                          typename IntervalT::size_type>::type
size(const IntervalT& object)
{
    return cardinality(object);
}

//- length ---------------------------------------------------------------------
template<class IntervalT>
typename boost::enable_if<is_continuous_interval<IntervalT>, 
    typename IntervalT::difference_type>::type
length(const IntervalT& object)
{
    return upper(object) - lower(object);
}

template<class IntervalT>
inline typename boost::enable_if<is_discrete_interval<IntervalT>, 
    typename IntervalT::difference_type>::type
length(const IntervalT& object)
{
    return    (last(object) + itl::unon<typename IntervalT::difference_type>::value()) 
            -  first(object);
}

template<class IntervalT>
typename boost::enable_if<is_continuous_asymmetric<IntervalT>, 
    typename IntervalT::difference_type>::type
length(const IntervalT& object)
{
    return upper(object) - lower(object);
}

template<class IntervalT>
inline typename boost::enable_if<is_discrete_asymmetric<IntervalT>, 
    typename IntervalT::difference_type>::type
length(const IntervalT& object)
{
    return    (last(object) + itl::unon<typename IntervalT::difference_type>::value()) 
            -  first(object);
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

    //JODO return  construct<IntervalT,std::less>
    return  dynamic_intervals<IntervalT>::construct_bounded
            (
                lower_min(left, right), 
                upper_max(left, right)
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
    if(exclusive_less(left_minuend, right))
        return right; 
    return  dynamic_intervals<IntervalT>::construct_bounded
            ( reverse_bounded_upper(left_minuend), bounded_upper(right) );
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
    //JODO return construct<IntervalT,std::less>(left.lower(), right_minuend.lower(),
    return  dynamic_intervals<IntervalT>::construct_bounded
            ( bounded_lower(left), reverse_bounded_lower(right_minuend) );
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
        //JODO return  construct<IntervalT,std::less>
        return  dynamic_intervals<IntervalT>::construct_bounded
                (
                    lower_max(left, right), 
                    upper_min(left, right) 
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

