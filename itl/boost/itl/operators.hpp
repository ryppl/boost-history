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

//------------------------------------------------------------------------------
// Addability +=, +
//------------------------------------------------------------------------------
template<class ObjectT, class OperandT>
typename boost::enable_if<is_intra_derivative<ObjectT, OperandT>, 
                          ObjectT>::type&
operator += (ObjectT& object, const OperandT& operand)
{ return object.add(operand); }


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
// Addability |=, | 
//------------------------------------------------------------------------------
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


//------------------------------------------------------------------------------
// Subtraction -=, -
//------------------------------------------------------------------------------
template<class ObjectT, class OperandT>
typename boost::enable_if<is_intra_derivative<ObjectT, OperandT>, 
                          ObjectT>::type&
operator -= (ObjectT& object, const OperandT& operand)
{ return object.subtract(operand); }

template<class ObjectT, class OperandT>
typename boost::enable_if<is_cross_derivative<ObjectT, OperandT>, 
                          ObjectT>::type&
operator -= (ObjectT& object, const OperandT& operand)
{ return object.erase(operand); }


template<class ObjectT, class OperandT>
typename boost::enable_if<is_right_inter_combinable<ObjectT, OperandT>, ObjectT>::type
operator - (ObjectT object, const OperandT& operand)
{
    return object -= operand; 
}

//------------------------------------------------------------------------------
// Intersection &=, &
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

//------------------------------------------------------------------------------
// Symmetric difference ^=, ^
//------------------------------------------------------------------------------

template<class ObjectT, class OperandT>
typename boost::enable_if<is_intra_derivative<ObjectT, OperandT>, 
                          ObjectT>::type&
operator ^= (ObjectT& object, const OperandT& operand)
{ return object.flip(operand); }

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


