/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __itl_type_traits_is_element_container_JOFA_090830_H__
#define __itl_type_traits_is_element_container_JOFA_090830_H__

#include <boost/mpl/and.hpp> 
#include <boost/mpl/or.hpp> 
#include <boost/mpl/not.hpp> 
#include <boost/itl/type_traits/is_interval_container.hpp> 
#include <boost/itl/type_traits/is_set.hpp> 

namespace boost{ namespace itl
{
    template <class Type> 
    struct is_element_container
    { 
        typedef is_element_container<Type> type;
        BOOST_STATIC_CONSTANT(bool, value = 
            (mpl::and_<is_set<Type>, mpl::not_<is_interval_container<Type> > >::value)
			);
    };

    template<class Type> 
    struct is_element_map
    {
        typedef is_element_map<Type> type;
        BOOST_STATIC_CONSTANT(bool, value = 
            (mpl::and_<is_map<Type>, mpl::not_<is_interval_container<Type> > >::value)
			);
    };


}} // namespace boost itl

#endif


