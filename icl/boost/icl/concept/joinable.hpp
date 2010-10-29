/*-----------------------------------------------------------------------------+    
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ICL_CONCEPT_JOINABLE_HPP_JOFA_100920
#define BOOST_ICL_CONCEPT_JOINABLE_HPP_JOFA_100920

#include <boost/icl/type_traits/is_interval_container.hpp>
#include <boost/icl/concept/interval.hpp>

namespace boost{ namespace icl
{

template<class Type>
inline typename enable_if<is_interval_map<Type>, typename Type::segment_type>::type
make_segment(const typename Type::element_type& element)
{
    typedef typename Type::interval_type interval_type;
    typedef typename Type::segment_type  segment_type;
    return segment_type(icl::singleton<interval_type>(element.key), element.data);
}

namespace segmental
{
    template<class Type>
    typename enable_if<is_interval_set<Type>, bool>::type
    is_joinable(typename Type::iterator it_, typename Type::iterator next_, Type* = 0)
    {
        return touches(*it_, *next_);
    }

    template<class Type>
    typename enable_if<is_interval_map<Type>, bool>::type
    is_joinable(typename Type::iterator it_, typename Type::iterator next_, Type* = 0)
    {
        return touches(it_->first, next_->first)
            && it_->second == next_->second    ;
    }
}



}} // namespace boost icl

#endif


