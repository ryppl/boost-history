/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_IS_KEY_CONTAINER_OF_HPP_JOFA_100829
#define BOOST_ITL_TYPE_TRAITS_IS_KEY_CONTAINER_OF_HPP_JOFA_100829

#include <boost/itl/type_traits/is_combinable.hpp>
#include <boost/itl/type_traits/is_container.hpp>

namespace boost{ namespace itl
{

    template<class KeyT, class ObjectT>
    struct is_strict_key_container_of
    {
        typedef is_strict_key_container_of<KeyT, ObjectT> type;
        BOOST_STATIC_CONSTANT(bool, value =
			(mpl::and_< is_map<ObjectT>
			          , is_same<typename ObjectT::key_object_type, KeyT> >::value)
            );
    };

    template<class KeyT, class ObjectT>
    struct is_key_container_of
    {
        typedef is_key_container_of<KeyT, ObjectT> type;
        BOOST_STATIC_CONSTANT(bool, value =
			(mpl::or_< is_strict_key_container_of<KeyT, ObjectT> 
			         , mpl::and_< is_container<ObjectT> //JODO is_itl_associative_container
			                    , is_same<ObjectT, KeyT> > >::value)
            );
    };


}} // namespace boost itl

#endif


