/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#ifndef __itl_operators_JOFA_090115_H__
#define __itl_operators_JOFA_090115_H__

#include <boost/utility/enable_if.hpp>
#include <boost/itl/type_traits/is_combinable.hpp>

namespace boost{namespace itl
{
/*
//------------------------------------------------------------------------------
// Addability
//------------------------------------------------------------------------------
template<class ObjectT, class OperandT>
typename boost::enable_if<is_combinable<ObjectT, OperandT>, ObjectT>::type
operator + (const ObjectT& object, const OperandT& operand)
{ return ObjectT(object) += operand; }

template<class ObjectT, class OperandT>
typename boost::enable_if<is_combinable<ObjectT, OperandT>, ObjectT>::type
operator + (const OperandT& operand, const ObjectT& object)
{ return ObjectT(object) += operand; }


template<class ObjectT>
ObjectT operator + (const ObjectT& object, const ObjectT& operand)
{ return ObjectT(object) += operand; }


//------------------------------------------------------------------------------
// Intersection
//------------------------------------------------------------------------------
template<class ObjectT, class OperandT>
typename boost::enable_if<is_combinable<ObjectT, OperandT>, ObjectT>::type
operator & (const ObjectT& object, const OperandT& operand)
{ return ObjectT(object) &= operand; }

template<class ObjectT, class OperandT>
typename boost::enable_if<is_combinable<ObjectT, OperandT>, ObjectT>::type
operator & (const OperandT& operand, const ObjectT& object)
{ return ObjectT(object) &= operand; }

template<class ObjectT>
ObjectT operator & (const typename ObjectT::overloadable_type& object, const ObjectT& operand)
{ return ObjectT(object) &= operand; }
*/


//------------------------------------------------------------------------------
// Addability
//------------------------------------------------------------------------------
template<class ObjectT, class OperandT>
typename boost::enable_if<is_interval_map_companion<ObjectT, OperandT>, ObjectT>::type
operator + (const ObjectT& object, const OperandT& operand)
{ return ObjectT(object) += operand; }

template<class ObjectT, class OperandT>
typename boost::enable_if<is_interval_map_companion<ObjectT, OperandT>, ObjectT>::type
operator + (const OperandT& operand, const ObjectT& object)
{ return ObjectT(object) += operand; }


template<class ObjectT>
ObjectT operator + (const typename ObjectT::overloadable_type& object, const ObjectT& operand)
{ return ObjectT(object) += operand; }


//------------------------------------------------------------------------------
// Intersection
//------------------------------------------------------------------------------
template<class ObjectT, class OperandT>
typename boost::enable_if<is_interval_set_companion<ObjectT, OperandT>, ObjectT>::type
operator & (const ObjectT& object, const OperandT& operand)
{ return ObjectT(object) &= operand; }

template<class ObjectT, class OperandT>
typename boost::enable_if<is_interval_set_companion<ObjectT, OperandT>, ObjectT>::type
operator & (const OperandT& operand, const ObjectT& object)
{ return ObjectT(object) &= operand; }

template<class ObjectT>
ObjectT operator & (const typename ObjectT::overloadable_type& object, const ObjectT& operand)
{ return ObjectT(object) &= operand; }



}} // namespace itl boost

#endif


