/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#ifndef __itl_type_traits_is_map_JOFA_081107_H__
#define __itl_type_traits_is_map_JOFA_081107_H__

namespace boost{ namespace itl
{
    template <class Type> struct is_map;

    template <class Type> struct is_map
	{
		typedef is_map<Type> type;
		enum {value = false}; 
	};

}} // namespace boost itl

#endif


