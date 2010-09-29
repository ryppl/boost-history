/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_CONCEPT_INTERVAL_HPP_JOFA_100323
#define BOOST_ITL_CONCEPT_INTERVAL_HPP_JOFA_100323

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/detail/is_incrementable.hpp>
#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/type_traits/given.hpp>
#include <boost/itl/type_traits/unon.hpp>
#include <boost/itl/type_traits/neutron.hpp>
#include <boost/itl/type_traits/infinity.hpp>
#include <boost/itl/type_traits/succ_pred.hpp>
#include <boost/itl/type_traits/is_numeric.hpp>
#include <boost/itl/type_traits/is_discrete.hpp>
#include <boost/itl/type_traits/is_asymmetric_interval.hpp>
#include <boost/itl/type_traits/is_universal_interval.hpp>
#include <boost/itl/type_traits/is_discrete_interval.hpp>
#include <boost/itl/type_traits/is_continuous_interval.hpp>
#include <boost/itl/type_traits/has_dynamic_bounds.hpp>
#include <boost/itl/concept/interval_bounds.hpp>
#include <boost/itl/interval_traits.hpp>
#include <boost/itl/dynamic_interval_traits.hpp>


namespace boost{namespace itl
{

//==============================================================================
//= Ordering
//==============================================================================
template<class Type>
inline typename enable_if<is_interval<Type>, bool>::type
domain_less(const typename interval_traits<Type>::domain_type& left, 
            const typename interval_traits<Type>::domain_type& right)
{
    return typename interval_traits<Type>::domain_compare()(left, right);
}

template<class Type>
inline typename enable_if<is_interval<Type>, bool>::type
domain_less_equal(const typename interval_traits<Type>::domain_type& left, 
                  const typename interval_traits<Type>::domain_type& right)
{
    return !(typename interval_traits<Type>::domain_compare()(right, left));
}

template<class Type>
inline typename enable_if<is_interval<Type>, bool>::type
domain_equal(const typename interval_traits<Type>::domain_type& left, 
             const typename interval_traits<Type>::domain_type& right)
{
    return !(typename interval_traits<Type>::domain_compare()(left, right))
        && !(typename interval_traits<Type>::domain_compare()(right, left));
}

//==============================================================================
//= Construct<Interval> singleton
//==============================================================================
template<class Type>
typename enable_if
<
    mpl::and_< is_static_rightopen<Type>
             , is_discrete<typename interval_traits<Type>::domain_type> >
  , Type
>::type
construct(const typename interval_traits<Type>::domain_type& value)
{
    //ASSERT: This always creates an interval with exactly one element
    return interval_traits<Type>::construct(value, itl::succ(value));
}

template<class Type>
typename enable_if
<
    mpl::and_< is_static_leftopen<Type>
             , is_discrete<typename interval_traits<Type>::domain_type> >
  , Type
>::type
construct(const typename interval_traits<Type>::domain_type& value)
{
    //ASSERT: This always creates an interval with exactly one element
    BOOST_ASSERT(given<is_numeric<Type>::value>
                 ::then((std::numeric_limits<Type>::min)() < value)); 
    return interval_traits<Type>::construct(itl::pred(value), value);
}

template<class Type>
typename enable_if
<
    mpl::and_< is_static_open<Type>
             , is_discrete<typename interval_traits<Type>::domain_type> >
  , Type
>::type
construct(const typename interval_traits<Type>::domain_type& value)
{
    //ASSERT: This always creates an interval with exactly one element
    BOOST_ASSERT(given<is_numeric<Type>::value>
                 ::then((std::numeric_limits<Type>::min)() < value)); 
    return interval_traits<Type>::construct(itl::pred(value), itl::succ(value));
}

template<class Type>
typename enable_if
<
    mpl::and_< is_static_closed<Type>
             , is_discrete<typename interval_traits<Type>::domain_type> >
  , Type
>::type
construct(const typename interval_traits<Type>::domain_type& value)
{
    //ASSERT: This always creates an interval with exactly one element
    return interval_traits<Type>::construct(value, value);
}

template<class Type>
typename enable_if<has_dynamic_bounds<Type>, Type>::type
construct(const typename interval_traits<Type>::domain_type& value)
{
    return dynamic_interval_traits<Type>::construct(value, value, interval_bounds::closed());
}

//==============================================================================
//= Construct<Interval> multon
//==============================================================================
template<class Type>
typename enable_if<is_asymmetric_interval<Type>, Type>::type
construct(const typename interval_traits<Type>::domain_type& low,
          const typename interval_traits<Type>::domain_type& up,
          interval_bounds bounds = interval_bounds::right_open())
{
    return interval_traits<Type>::construct(low, up);
}


template<class Type>
typename enable_if<is_dynamic_bounded<Type>, Type>::type
construct(const typename interval_traits<Type>::domain_type& low,
          const typename interval_traits<Type>::domain_type& up,
          interval_bounds bounds = interval_bounds::right_open())
{
    return dynamic_interval_traits<Type>::construct(low, up, bounds);
}


//- construct form bounded values ----------------------------------------------
template<class Type>
typename enable_if<is_dynamic_bounded<Type>, Type>::type
construct(const typename Type::bounded_domain_type& low,
          const typename Type::bounded_domain_type& up)
{
    return dynamic_interval_traits<Type>::construct_bounded(low, up);
}

template<class Type>
typename enable_if<is_interval<Type>, Type>::type
span(const typename interval_traits<Type>::domain_type& lhs,
     const typename interval_traits<Type>::domain_type& rhs)
{
    if(interval_traits<Type>::domain_compare(lhs,rhs))
        return construct(lhs, rhs);
    else
        return construct(rhs, lhs);
}


//==============================================================================
//= Selection
//==============================================================================

template<class Type>
inline typename enable_if<is_interval<Type>, 
                          typename interval_traits<Type>::domain_type>::type
lower(const Type& object)
{ 
    return interval_traits<Type>::lower(object);
}

template<class Type>
inline typename enable_if<is_interval<Type>, 
                          typename interval_traits<Type>::domain_type>::type
upper(const Type& object)
{ 
    return interval_traits<Type>::upper(object);
}


//- first ----------------------------------------------------------------------
template<class Type>
inline typename 
enable_if< mpl::or_<is_static_rightopen<Type>, is_static_closed<Type> >
         , typename interval_traits<Type>::domain_type>::type
first(const Type& object)
{ 
    return lower(object);
}

template<class Type>
inline typename 
enable_if< mpl::and_< mpl::or_<is_static_leftopen<Type>, is_static_open<Type> >
                    , is_discrete<typename interval_traits<Type>::domain_type> >
         , typename interval_traits<Type>::domain_type>::type
first(const Type& object)
{ 
    return succ(lower(object));
}

template<class Type>
inline typename enable_if<is_discrete_interval<Type>, 
                          typename interval_traits<Type>::domain_type>::type
first(const Type& object)
{ 
    return is_left_closed(object.bounds()) ? 
                    lower(object) : 
               succ(lower(object));
}

//- last -----------------------------------------------------------------------
template<class Type>
inline typename 
enable_if< mpl::or_<is_static_leftopen<Type>, is_static_closed<Type> >
         , typename interval_traits<Type>::domain_type>::type
last(const Type& object)
{ 
    return upper(object);
}

template<class Type>
inline typename 
enable_if< mpl::and_< mpl::or_<is_static_rightopen<Type>, is_static_open<Type> >
                    , is_discrete<typename interval_traits<Type>::domain_type>  >
         , typename interval_traits<Type>::domain_type>::type
last(const Type& object)
{ 
    BOOST_ASSERT(given<is_numeric<Type>::value>
                 ::then((std::numeric_limits<Type>::min)() < upper(object))); 
    return pred(upper(object));
}

template<class Type>
inline typename enable_if<is_discrete_interval<Type>, 
                          typename interval_traits<Type>::domain_type>::type
last(const Type& object)
{ 
    return is_right_closed(object.bounds()) ? 
                    upper(object) : 
               pred(upper(object));
}


//------------------------------------------------------------------------------
template<class Type>
typename enable_if<is_dynamic_bounded<Type>, 
                   typename Type::bounded_domain_type>::type
bounded_lower(const Type& object)
{ 
    return typename 
        Type::bounded_domain_type(lower(object), 
                                       object.bounds().left()); 
}

template<class Type>
typename enable_if<is_dynamic_bounded<Type>, 
                   typename Type::bounded_domain_type>::type
reverse_bounded_lower(const Type& object)
{ 
    return typename 
        Type::bounded_domain_type(lower(object), 
                                       object.bounds().reverse_left()); 
}

template<class Type>
typename enable_if<is_dynamic_bounded<Type>, 
                   typename Type::bounded_domain_type>::type
bounded_upper(const Type& object)
{ 
    return typename 
        Type::bounded_domain_type(upper(object), 
                                       object.bounds().right()); 
}

template<class Type>
typename enable_if<is_dynamic_bounded<Type>, 
                   typename Type::bounded_domain_type>::type
reverse_bounded_upper(const Type& object)
{ 
    return typename 
        Type::bounded_domain_type(upper(object), 
                                       object.bounds().reverse_right()); 
}


//==============================================================================
//= Containedness
//==============================================================================
//- is_empty -------------------------------------------------------------------
/** Is the interval empty? */
template<class Type>
typename boost::enable_if<is_asymmetric_interval<Type>, bool>::type
is_empty(const Type& object)
{ 
    return domain_less_equal<Type>(upper(object), lower(object)); 
}

template<class Type>
typename boost::enable_if<is_universal_interval<Type>, bool>::type
is_empty(const Type& object)
{ 
    return object.empty(); 
}

template<class Type>
typename boost::enable_if<is_discrete_interval<Type>, bool>::type
is_empty(const Type& object)
{ 
    if(object.bounds() == interval_bounds::closed())
        return domain_less<Type>(upper(object), lower(object)); 
    else if(object.bounds() == interval_bounds::open())
        return domain_less_equal<Type>(upper(object), succ(lower(object))); 
    else
        return domain_less_equal<Type>(upper(object), lower(object)); 
}

template<class Type>
typename boost::enable_if<is_continuous_interval<Type>, bool>::type
is_empty(const Type& object)
{
    return     domain_less<Type>(upper(object), lower(object))
        || (   domain_equal<Type>(upper(object), lower(object))
            && object.bounds() != interval_bounds::closed()    );
}

//- contains -------------------------------------------------------------------
template<class Type>
typename boost::enable_if<is_interval<Type>, bool>::type
contains(const Type& super, const Type& sub)
{ 
    return lower_less_equal(super,sub) && upper_less_equal(sub,super);
}

template<class Type>
typename boost::enable_if<is_interval<Type>, bool>::type
contains(const Type& super, const typename interval_traits<Type>::domain_type& element)
{ 
    return contains(super,Type(element));
}

//- within ---------------------------------------------------------------------
template<class Type>
typename boost::enable_if<is_interval<Type>, bool>::type
within(const Type& sub, const Type& super)
{ 
    return contains(super,sub);
}


//==============================================================================
//= Equivalences and Orderings
//==============================================================================
//- exclusive_less -------------------------------------------------------------
/** Maximal element of <tt>left</tt> is less than the minimal element of 
    <tt>right</tt> */
template<class Type>
typename boost::enable_if<is_asymmetric_interval<Type>, bool>::type
exclusive_less(const Type& left, const Type& right)
{ 
    return domain_less_equal<Type>(upper(left), lower(right)); 
}

template<class Type>
typename boost::enable_if<is_universal_interval<Type>, bool>::type
exclusive_less(const Type& left, const Type& right)
{ 
    return left.exclusive_less(right); 
}


template<class Type>
typename boost::enable_if<is_discrete_interval<Type>, bool>::type
exclusive_less(const Type& left, const Type& right)
{ 
    return domain_less<Type>(last(left), first(right)); 
}

template<class Type>
typename boost::enable_if<is_continuous_interval<Type>, bool>::type
exclusive_less(const Type& left, const Type& right)
{ 
    return     domain_less<Type>(left.upper(), right.lower())
        || (   domain_equal<Type>(left.upper(), right.lower())
            && inner_bounds(left,right) != interval_bounds::open() );
}

//------------------------------------------------------------------------------
template<class Type>
typename boost::enable_if<is_asymmetric_interval<Type>, bool>::type
lower_less(const Type& left, const Type& right)
{
    return domain_less<Type>(left.lower(), right.lower());
}
    
template<class Type>
typename boost::enable_if<is_universal_interval<Type>, bool>::type
lower_less(const Type& left, const Type& right)
{
    return left.lower_less(right);
}

template<class Type>
typename boost::enable_if<is_discrete_interval<Type>, bool>::type
lower_less(const Type& left, const Type& right)
{
    return domain_less<Type>(first(left), first(right));
}
    
template<class Type>
typename boost::enable_if<is_continuous_interval<Type>, bool>::type
lower_less(const Type& left, const Type& right)
{
    if(left_bounds(left,right) == interval_bounds::right_open())  //'[(' == 10
        return domain_less_equal<Type>(left.lower(), right.lower());
    else 
        return domain_less<Type>(left.lower(), right.lower());
}
    

//------------------------------------------------------------------------------
template<class Type>
typename boost::enable_if<is_asymmetric_interval<Type>, bool>::type
upper_less(const Type& left, const Type& right)
{
    return domain_less<Type>(left.upper(), right.upper());
}

template<class Type>
typename boost::enable_if<is_universal_interval<Type>, bool>::type
upper_less(const Type& left, const Type& right)
{
    return left.upper_less(right);
}

template<class Type>
typename boost::enable_if<is_discrete_interval<Type>, bool>::type
upper_less(const Type& left, const Type& right)
{
    return domain_less<Type>(last(left), last(right));
}
    
template<class Type>
typename boost::enable_if<is_continuous_interval<Type>, bool>::type
upper_less(const Type& left, const Type& right)
{
    if(right_bounds(left,right) == interval_bounds::left_open())
        return domain_less_equal<Type>(left.upper(), right.upper());
    else
        return domain_less<Type>(left.upper(), right.upper());
}
    
//------------------------------------------------------------------------------
template<class Type>
typename boost::enable_if<has_dynamic_bounds<Type>, 
                          typename Type::bounded_domain_type   >::type
lower_min(const Type& left, const Type& right)
{
    return lower_less(left, right) ? bounded_lower(left) : bounded_lower(right);
}

//------------------------------------------------------------------------------
template<class Type>
typename boost::enable_if<has_dynamic_bounds<Type>, 
                          typename Type::bounded_domain_type   >::type
lower_max(const Type& left, const Type& right)
{
    return lower_less(left, right) ? bounded_lower(right) : bounded_lower(left);
}

//------------------------------------------------------------------------------
template<class Type>
typename boost::enable_if<has_dynamic_bounds<Type>, 
                          typename Type::bounded_domain_type   >::type
upper_max(const Type& left, const Type& right)
{
    return upper_less(left, right) ? bounded_upper(right) : bounded_upper(left);
}

//------------------------------------------------------------------------------
template<class Type>
typename boost::enable_if<has_dynamic_bounds<Type>, 
                          typename Type::bounded_domain_type   >::type
upper_min(const Type& left, const Type& right)
{
    return upper_less(left, right) ? bounded_upper(left) : bounded_upper(right);
}


//------------------------------------------------------------------------------
template<class Type>
typename boost::enable_if<is_asymmetric_interval<Type>, bool>::type
lower_equal(const Type& left, const Type& right)
{
    return domain_equal<Type>(left.lower(), right.lower());
}

template<class Type>
typename boost::enable_if<is_universal_interval<Type>, bool>::type
lower_equal(const Type& left, const Type& right)
{
    return left.lower_equal(right);
}

template<class Type>
typename boost::enable_if<is_discrete_interval<Type>, bool>::type
lower_equal(const Type& left, const Type& right)
{
    return domain_equal<Type>(first(left), first(right));
}

template<class Type>
typename boost::enable_if<is_continuous_interval<Type>, bool>::type
lower_equal(const Type& left, const Type& right)
{
    return (left.bounds().left()==right.bounds().left())
        && domain_equal<Type>(left.lower(), right.lower());
}


//------------------------------------------------------------------------------
template<class Type>
typename boost::enable_if<is_asymmetric_interval<Type>, bool>::type
upper_equal(const Type& left, const Type& right)
{
    return domain_equal<Type>(left.upper(), right.upper());
}

template<class Type>
typename boost::enable_if<is_universal_interval<Type>, bool>::type
upper_equal(const Type& left, const Type& right)
{
    return left.upper_equal(right);
}
    
template<class Type>
typename boost::enable_if<is_discrete_interval<Type>, bool>::type
upper_equal(const Type& left, const Type& right)
{
    return domain_equal<Type>(last(left), last(right));
}

template<class Type>
typename boost::enable_if<is_continuous_interval<Type>, bool>::type
upper_equal(const Type& left, const Type& right)
{
    return (left.bounds().right()==right.bounds().right())
        && domain_equal<Type>(left.upper(), right.upper());
}

//------------------------------------------------------------------------------
template<class Type>
typename boost::enable_if<is_interval<Type>, bool>::type
lower_less_equal(const Type& left, const Type& right)
{
    return lower_less(left,right) || lower_equal(left,right);
}

template<class Type>
typename boost::enable_if<is_interval<Type>, bool>::type
upper_less_equal(const Type& left, const Type& right)
{
    return upper_less(left,right) || upper_equal(left,right);
}


//- operator == ----------------------------------------------------------------
template<class Type>
typename boost::enable_if<is_interval<Type>, bool>::type
operator == (const Type& lhs, const Type& rhs)
{
    return (itl::is_empty(lhs) && itl::is_empty(rhs)) 
        || (lower_equal(lhs,rhs) && upper_equal(lhs,rhs));
}

//- operator < -----------------------------------------------------------------
template<class Type>
typename boost::enable_if<is_interval<Type>, bool>::type
operator < (const Type& lhs, const Type& rhs)
{
    if(itl::is_empty(lhs)) 
        return !itl::is_empty(rhs);
    else 
        return lower_less(lhs,rhs) 
            || (lower_equal(lhs,rhs) && upper_less(lhs,rhs)); 
}



//------------------------------------------------------------------------------
template<class Type>
typename boost::enable_if<is_asymmetric_interval<Type>, bool>::type
touches(const Type& left, const Type& right)
{ 
    return domain_equal<Type>(upper(left), lower(right)); 
}

template<class Type>
typename boost::enable_if<is_universal_interval<Type>, bool>::type
touches(const Type& left, const Type& right)
{ 
    return left.touches(right); 
}

template<class Type>
typename boost::enable_if<is_discrete_interval<Type>, bool>::type
touches(const Type& left, const Type& right)
{
    return domain_equal<Type>(succ(last(left)), first(right));
}

template<class Type>
typename boost::enable_if<is_continuous_interval<Type>, bool>::type
touches(const Type& left, const Type& right)
{
    return is_complementary(inner_bounds(left,right))
        && domain_equal<Type>(left.upper(), right.lower());
}


//==============================================================================
//= Size
//==============================================================================
//- cardinality ----------------------------------------------------------------

template<class Type>
typename boost::enable_if<is_continuous_interval<Type>, 
    typename size_type_of<interval_traits<Type> >::type>::type
cardinality(const Type& object)
{
    typedef typename size_type_of<interval_traits<Type> >::type SizeT;
    if(itl::is_empty(object))
        return itl::neutron<SizeT>::value();
    else if(   object.bounds() == interval_bounds::closed() 
            && domain_equal<Type>(lower(object), upper(object)))
        return itl::unon<SizeT>::value();
    else 
        return infinity<SizeT>::value();
}

template<class Type>
typename boost::enable_if<is_discrete_interval<Type>, 
    typename size_type_of<interval_traits<Type> >::type>::type
cardinality(const Type& object)
{
    typedef typename size_type_of<interval_traits<Type> >::type SizeT;
    return (last(object) + itl::unon<SizeT>::value()) - first(object);
}


template<class Type>
typename boost::enable_if<is_continuous_asymmetric<Type>, 
    typename size_type_of<interval_traits<Type> >::type>::type
cardinality(const Type& object)
{
    typedef typename size_type_of<interval_traits<Type> >::type SizeT;
    if(itl::is_empty(object))
        return itl::neutron<SizeT>::value();
    else 
        return infinity<SizeT>::value();
}

template<class Type>
typename boost::enable_if<is_discrete_asymmetric<Type>, 
    typename size_type_of<interval_traits<Type> >::type>::type
cardinality(const Type& object)
{
    typedef typename size_type_of<interval_traits<Type> >::type SizeT;
    return (last(object) + itl::unon<SizeT>::value()) - first(object);
}





//- size -----------------------------------------------------------------------
template<class Type>
inline typename enable_if<is_interval<Type>, 
    typename size_type_of<interval_traits<Type> >::type>::type
size(const Type& object)
{
    return cardinality(object);
}

//- length ---------------------------------------------------------------------
template<class Type>
inline typename boost::enable_if<is_continuous_interval<Type>, 
    typename difference_type_of<interval_traits<Type> >::type>::type
length(const Type& object)
{
    return upper(object) - lower(object);
}

template<class Type>
inline typename boost::enable_if<is_discrete_interval<Type>, 
    typename difference_type_of<interval_traits<Type> >::type>::type
length(const Type& object)
{
    typedef typename difference_type_of<interval_traits<Type> >::type DiffT;
    return    (last(object) + itl::unon<DiffT>::value()) 
            -  first(object);
}

template<class Type>
typename boost::enable_if<is_continuous_asymmetric<Type>, 
    typename difference_type_of<interval_traits<Type> >::type>::type
length(const Type& object)
{
    return upper(object) - lower(object);
}

template<class Type>
inline typename boost::enable_if<is_discrete_asymmetric<Type>, 
    typename difference_type_of<interval_traits<Type> >::type>::type
length(const Type& object)
{
    typedef typename difference_type_of<interval_traits<Type> >::type DiffT;
    return    (last(object) + itl::unon<DiffT>::value()) 
            -  first(object);
}

//==============================================================================
//= Addition
//==============================================================================
//- hull -----------------------------------------------------------------------
/** \c hull returns the smallest interval containing \c left and \c right. */
template<class Type>
typename boost::enable_if<is_asymmetric_interval<Type>, Type>::type
hull(Type left, const Type& right)
{
    if(itl::is_empty(right))
        return left;
    else if(itl::is_empty(left))
        return right;

    return 
        construct<Type>
        (
            (std::min)(lower(left), lower(right), interval_traits<Type>::domain_compare()), 
            (std::max)(upper(left), upper(right), interval_traits<Type>::domain_compare())
        );
}

template<class Type>
typename boost::enable_if<is_universal_interval<Type>, Type>::type
hull(Type left, const Type& right)
{
    return left.extend(right);
}

template<class Type>
typename boost::enable_if<has_dynamic_bounds<Type>, Type>::type
hull(Type left, const Type& right)
{
    if(itl::is_empty(right))
        return left;
    else if(itl::is_empty(left))
        return right;

    return  dynamic_interval_traits<Type>::construct_bounded
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
template<class Type>
typename boost::enable_if<is_asymmetric_interval<Type>, Type>::type
left_subtract(Type right, const Type& left_minuend)
{
    if(exclusive_less(left_minuend, right))
        return right; 

    return construct<Type>(upper(left_minuend), upper(right));
}

template<class Type>
typename boost::enable_if<is_universal_interval<Type>, Type>::type
left_subtract(Type right, const Type& left_minuend)
{
    return right.left_subtract(left_minuend);
}

template<class Type>
typename boost::enable_if<has_dynamic_bounds<Type>, Type>::type
left_subtract(Type right, const Type& left_minuend)
{
    if(exclusive_less(left_minuend, right))
        return right; 
    return  dynamic_interval_traits<Type>::construct_bounded
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
template<class Type>
typename boost::enable_if<is_asymmetric_interval<Type>, Type>::type
right_subtract(Type left, const Type& right_minuend)
{
    if(exclusive_less(left, right_minuend))
        return left; 
    return construct<Type>(lower(left), lower(right_minuend));
}

template<class Type>
typename boost::enable_if<is_universal_interval<Type>, Type>::type
right_subtract(Type left, const Type& right_minuend)
{
    return left.right_subtract(right_minuend);
}

template<class Type>
typename boost::enable_if<has_dynamic_bounds<Type>, Type>::type
right_subtract(Type left, const Type& right_minuend)
{
    if(exclusive_less(left, right_minuend))
        return left; 

    return  dynamic_interval_traits<Type>::construct_bounded
            ( bounded_lower(left), reverse_bounded_lower(right_minuend) );
}

//==============================================================================
//= Intersection
//==============================================================================
//- operator & -----------------------------------------------------------------
/** Returns the intersection of \c left and \c right interval. */
template<class Type>
typename boost::enable_if<is_asymmetric_interval<Type>, Type>::type
operator & (Type left, const Type& right)
{
    if(itl::is_empty(left) || itl::is_empty(right))
        return neutron<Type>::value();
    else
        return
        construct<Type>
        (
            (std::max)(itl::lower(left), itl::lower(right), interval_traits<Type>::domain_compare()),
            (std::min)(itl::upper(left), itl::upper(right), interval_traits<Type>::domain_compare())
        );
}

template<class Type>
typename boost::enable_if<is_universal_interval<Type>, Type>::type
operator & (Type left, const Type& right)
{
    return left &= right;
}

template<class Type>
typename boost::enable_if<has_dynamic_bounds<Type>, Type>::type
operator & (Type left, const Type& right)
{
    if(itl::is_empty(left) || itl::is_empty(right))
        return neutron<Type>::value();
    else 
        return  dynamic_interval_traits<Type>::construct_bounded
                (
                    lower_max(left, right), 
                    upper_min(left, right) 
                );
}


//- intersects -----------------------------------------------------------------
template<class Type>
typename boost::enable_if<is_interval<Type>, bool>::type
intersects(const Type& left, const Type& right)
{ 
    return !(exclusive_less(left,right) || exclusive_less(right,left)); 
}

//- disjoint -------------------------------------------------------------------
template<class Type>
typename boost::enable_if<is_interval<Type>, bool>::type
disjoint(const Type& left, const Type& right)
{ 
    return exclusive_less(left,right) || exclusive_less(right,left); 
}

//------------------------------------------------------------------------------
template<class CharType, class CharTraits, class Type>
typename boost::enable_if<is_interval<Type>, 
                          std::basic_ostream<CharType, CharTraits> >::type&
operator << (std::basic_ostream<CharType, CharTraits> &stream, Type const& object)
{
    if(boost::itl::is_empty(object))
        return stream << "[)";
    else
        return stream << "[" << interval_traits<Type>::lower(object) << "," 
                             << interval_traits<Type>::upper(object) << ")";
}



}} // namespace itl boost

#endif

