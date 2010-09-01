/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_FUNCTIONS_CONTAINER_HPP_JOFA_100828
#define BOOST_ITL_FUNCTIONS_CONTAINER_HPP_JOFA_100828

#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/type_traits/is_container.hpp>

namespace boost{namespace itl
{

//==============================================================================
//= Emptieness
//==============================================================================

/** Tests if the container is empty. 
    Complexity: constant. */
template<class Type>
typename enable_if<is_container<Type>, bool>::type
is_empty(const Type& object)
{
    return object.begin()==object.end();
}


/** All content of the container is dropped. 
    Complexity: linear. */
template<class Type>
typename enable_if<is_container<Type>, void>::type
clear(Type& object)
{
    object.erase(object.begin(), object.end());
}

//==============================================================================
//= Size
//==============================================================================

template<class Type> 
typename enable_if<is_container<Type>, typename Type::size_type>::type
iterative_size(const Type& object)
{ 
    return object.size(); 
}

//==============================================================================
//= Swap
//==============================================================================

template<class Type>
typename enable_if<is_container<Type>, void>::type
swap(Type& left, Type& right)
{
    left.swap(right); //JODO test
}

}} // namespace itl boost


#endif


