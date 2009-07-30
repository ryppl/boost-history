/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __itl_operators_JOFA_090115_H__
#define __itl_operators_JOFA_090115_H__

#include <boost/utility/enable_if.hpp>
#include <boost/itl/type_traits/is_combinable.hpp>

namespace boost{namespace itl
{

//==============================================================================
//= Addition
//==============================================================================
//------------------------------------------------------------------------------
//- Addition +=, + 
//------------------------------------------------------------------------------
/** \par \b Requires: Types \c ObjectT and \c OperandT are addable.
	\par \b Effects: \c operand is added to \c object.
	\par \b Returns: A reference to \c object.
	\b Complexity:
\code
                  \ OperandT:                      interval
                   \         element     segment   container
ObjectT:
       interval container    O(log n)     O(n)     O(m log(n+m))

             interval_set               amortized
    spearate_interval_set                O(log n) 

n = object.interval_count()
m = operand.interval_count()
\endcode

For the addition of \b elements, \b segments and \b interval \b containers
complexity is \b logarithmic, \b linear and \b loglinear respectively.
For \c interval_sets and \c separate_interval_sets addition of segments
is \b amortized \b logarithmic.
*/
template<class ObjectT, class OperandT>
typename boost::enable_if<is_intra_derivative<ObjectT, OperandT>, 
                          ObjectT>::type&
operator += (ObjectT& object, const OperandT& operand)
{ 
	return object.add(operand); 
}


template<class ObjectT, class OperandT>
typename boost::enable_if<is_binary_intra_combinable<ObjectT, OperandT>, ObjectT>::type
operator + (ObjectT object, const OperandT& operand)
{
    return object += operand; 
}

template<class ObjectT, class OperandT>
typename boost::enable_if<is_binary_intra_combinable<ObjectT, OperandT>, ObjectT>::type
operator + (const OperandT& operand, ObjectT object)
{
    return object += operand; 
}


template<class ObjectT>
ObjectT operator + (typename ObjectT::overloadable_type object, const ObjectT& operand)
{
    return object += operand; 
}


//------------------------------------------------------------------------------
//- Addition |=, | 
//------------------------------------------------------------------------------
/** \par \b Requires: Types \c ObjectT and \c OperandT are addable.
	\par \b Effects: \c operand is added to \c object.
	\par \b Returns: A reference to \c object.
	\b Complexity:
\code
                  \ OperandT:                      interval
                   \         element     segment   container
ObjectT:
       interval container    O(log n)     O(n)     O(m log(n+m))

             interval_set               amortized
    spearate_interval_set                O(log n) 

n = object.interval_count()
m = operand.interval_count()
\endcode

For the addition of \b elements, \b segments and \b interval \b containers
complexity is \b logarithmic, \b linear and \b loglinear respectively.
For \c interval_sets and \c separate_interval_sets addition of segments
is \b amortized \b logarithmic.
*/
template<class ObjectT, class OperandT>
typename boost::enable_if<is_right_intra_combinable<ObjectT, OperandT>, 
                          ObjectT>::type&
operator |= (ObjectT& object, const OperandT& operand)
{ return object += operand; }


template<class ObjectT, class OperandT>
typename boost::enable_if<is_binary_intra_combinable<ObjectT, OperandT>, ObjectT>::type
operator | (ObjectT object, const OperandT& operand)
{
    return object += operand; 
}

template<class ObjectT, class OperandT>
typename boost::enable_if<is_binary_intra_combinable<ObjectT, OperandT>, ObjectT>::type
operator | (const OperandT& operand, ObjectT object)
{
    return object += operand; 
}


template<class ObjectT>
ObjectT operator | (typename ObjectT::overloadable_type object, const ObjectT& operand)
{
    return object += operand; 
}


//==============================================================================
//= Subtraction
//==============================================================================
//------------------------------------------------------------------------------
//- Subtraction -=, -
//------------------------------------------------------------------------------
/** \par \b Requires: Types \c ObjectT and \c OperandT are subtractable.
	\par \b Effects: \c operand is subtracted from \c object.
	\par \b Returns: A reference to \c object.
	\b Complexity:
\code
                  \ OperandT:                      interval
                   \         element    segment    container
ObjectT:
       interval container    O(log n)     O(n)     O(m log(n+m))

                                       amortized
            interval_sets               O(log n) 

n = object.interval_count()
m = operand.interval_count()
\endcode

For the subtraction of \em elements, \b segments and \b interval \b containers
complexity is \b logarithmic, \b linear and \b loglinear respectively.
For interval sets subtraction of segments
is \b amortized \b logarithmic.
*/
template<class ObjectT, class OperandT>
typename boost::enable_if<is_intra_derivative<ObjectT, OperandT>, 
                          ObjectT>::type&
operator -= (ObjectT& object, const OperandT& operand)
{ 
	return object.subtract(operand); 
}

template<class ObjectT, class OperandT>
typename boost::enable_if<is_cross_derivative<ObjectT, OperandT>, 
                          ObjectT>::type&
operator -= (ObjectT& object, const OperandT& operand)
{ 
	return object.erase(operand); 
}

template 
<
    class ObjectT,
    class SubType, class DomainT, ITL_COMPARE Compare, 
	template<class,ITL_COMPARE>class Interval, ITL_ALLOC Alloc
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



template<class ObjectT, class OperandT>
typename boost::enable_if<is_right_inter_combinable<ObjectT, OperandT>, ObjectT>::type
operator - (ObjectT object, const OperandT& operand)
{
    return object -= operand; 
}

//==============================================================================
//= Intersection
//==============================================================================
//------------------------------------------------------------------------------
//- Intersection &=, &
//------------------------------------------------------------------------------
template<class ObjectT, class OperandT>
typename boost::enable_if<is_right_inter_combinable<ObjectT, OperandT>, ObjectT>::type&
operator &= (ObjectT& object, const OperandT& operand)
{
    ObjectT intersection;
    object.add_intersection(intersection, operand);
    object.swap(intersection);
    return object;
}

template<class ObjectT, class OperandT>
typename boost::enable_if<is_binary_inter_combinable<ObjectT, OperandT>, ObjectT>::type
operator & (ObjectT object, const OperandT& operand)
{
    return object &= operand; 
}

template<class ObjectT, class OperandT>
typename boost::enable_if<is_binary_inter_combinable<ObjectT, OperandT>, ObjectT>::type
operator & (const OperandT& operand, ObjectT object)
{
    return object &= operand; 
}

template<class ObjectT>
ObjectT operator & (typename ObjectT::overloadable_type object, const ObjectT& operand)
{
    return object &= operand; 
}

//==============================================================================
//= Symmetric difference
//==============================================================================
//------------------------------------------------------------------------------
//- Symmetric difference ^=, ^
//------------------------------------------------------------------------------
template<class ObjectT, class OperandT>
typename boost::enable_if<is_intra_derivative<ObjectT, OperandT>, 
                          ObjectT>::type&
operator ^= (ObjectT& object, const OperandT& operand)
{ 
	return object.flip(operand); 
}

template<class ObjectT, class OperandT>
typename boost::enable_if<is_binary_intra_combinable<ObjectT, OperandT>, ObjectT>::type
operator ^ (ObjectT object, const OperandT& operand)
{
    return object ^= operand; 
}

template<class ObjectT, class OperandT>
typename boost::enable_if<is_binary_intra_combinable<ObjectT, OperandT>, ObjectT>::type
operator ^ (const OperandT& operand, ObjectT object)
{
    return object ^= operand; 
}


template<class ObjectT>
ObjectT operator ^ (typename ObjectT::overloadable_type object, const ObjectT& operand)
{
    return object ^= operand; 
}


}} // namespace itl boost

#endif


