/*-----------------------------------------------------------------------------+    
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_IS_FRAGMENT_TYPE_OF_HPP_JOFA_100909
#define BOOST_ITL_TYPE_TRAITS_IS_FRAGMENT_TYPE_OF_HPP_JOFA_100909

#include <boost/itl/type_traits/is_combinable.hpp>

namespace boost{ namespace itl
{


template<class CompanionT, class GuideT>
struct is_interval_map_fragment_type_of //JODO equivalent to is_interval_map_right_intra_combinable
{                                       //JODO generalize
    typedef is_interval_map_fragment_type_of<CompanionT, GuideT> type;
    BOOST_STATIC_CONSTANT(bool, value = 
        (mpl::and_
        <
            is_interval_map<GuideT>
          , mpl::or_
            <
                is_interval_map_derivative<GuideT, CompanionT> 
              , is_concept_compatible<is_interval_map, GuideT, CompanionT>
            >
        >::value)
        );
};


}} // namespace boost itl

#endif


