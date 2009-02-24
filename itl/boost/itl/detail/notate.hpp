/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
    Macro definitions for useful and abstract notations e.g. iteration headers
-----------------------------------------------------------------------------*/
#ifndef  __itl_notate_hpp_JOFA_990119__
#define  __itl_notate_hpp_JOFA_990119__


// Iterations over stl or stl-compatible containers:
#define FORALL(type,iter,obj) for(type::iterator iter=(obj).begin(); (iter)!=(obj).end(); (iter)++)
#define const_FORALL(type,iter,obj) for(type::const_iterator iter=(obj).begin(); !((iter)==(obj).end()); (iter)++)

#define FORALL_THIS(iter) for(iterator iter=begin(); (iter)!=end(); (iter)++)
#define const_FORALL_THIS(iter) for(const_iterator iter=this->begin(); (iter)!=this->end(); (iter)++)

// Plain old array iteration (assuming memberfunction VecT::size()!)
#define FORALL_VEC(idx, vec) for(int idx=0; idx<vec.size(); idx++)

// Parallel iteration
#define const_FORALL_THIS_2(it, it2, cnt2)\
    for(const_iterator it=begin(), it2=(cnt2).begin(); (it)!=end() && (it2)!=cnt2.end(); (it)++, (it2)++)
#define const_FORALL_THIS_PARALLEL(it, it2, cnt2)\
    for(const_iterator it=begin(), it2=(cnt2).begin(); (it)!=end() && (it2)!=cnt2.end(); (it)++, (it2)++)

// Variants that requires already declared iterators
#define FOR_ALL(iter,obj) for((iter)=(obj).begin(); (iter)!=(obj).end(); (iter)++)
#define FOR_ALL_THIS(iter) for((iter)=this->begin(); (iter)!=this->end(); (iter)++)

#define FOR_ALL2(iter1,obj1, iter2,obj2)\
    for((iter1)=(obj1).begin(), (iter2)=(obj2).begin(); (iter1)!=(obj1).end(); ++(iter1),++(iter2))

// for readability and ease of memory: pair selectors for different contexts
#define ITERATOR        first        // x.insert(y).second
#define WAS_SUCCESSFUL  second

#define KEY_VALUE       first        // for map valuePairs
#define CONT_VALUE      second

// universal and maths
namespace boost{namespace itl
{
    const int UNDEFINED_INDEX = -1;
}} // namespace itl boost


#endif // __itl_notate_hpp_JOFA_990119__


