/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_DETAIL_STD_SET_HPP_JOFA_101007
#define BOOST_ITL_DETAIL_STD_SET_HPP_JOFA_101007

#include <set>
#include <boost/config.hpp>
#include <boost/itl/type_traits/type_to_string.hpp>
#include <boost/itl/type_traits/is_set.hpp>


namespace boost{namespace itl
{

template <class Type>
struct is_set<std::set<Type> >
{ 
    typedef is_set<std::set<Type> > type;
    BOOST_STATIC_CONSTANT(bool, value = true); 
};


template <class Type>
struct type_to_string<std::set<Type> >
{
    static std::string apply()
    { return "set<"+ type_to_string<Type>::apply() +">"; }
};

}} // namespace itl boost

#endif // BOOST_ITL_DETAIL_STD_SET_HPP_JOFA_101007

