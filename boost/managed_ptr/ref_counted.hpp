//boost::ref_counted but with different smart_ptr
#ifndef BOOST_MANAGED_PTR_REF_COUNTED_LJE20031109_HPP
#define BOOST_MANAGED_PTR_REF_COUNTED_LJE20031109_HPP
//////////////////////////////////////////////////////////////////////////////
// The Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any
//     purpose is hereby granted without fee, provided that the above copy-
//     right notice appear in all copies and that both that copyright notice
//     and this permission notice appear in supporting documentation.
// The author or Addison-Welsey Longman make no representations about the
//     suitability of this software for any purpose. It is provided "as is"
//     without express or implied warranty.
//////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2002, David Held.
//////////////////////////////////////////////////////////////////////////////
// The above Copyrights apply to the ref_counted class as it appeared
// around 2003-12-03 22:22 in 
//   http://cvs.sourceforge.net/viewcvs.py/boost-sandbox/boost-sandbox/
//     boost/policy_ptr/latest/smart_ptr.hpp
//////////////////////////////////////////////////////////////////////////////
#include "boost/managed_ptr/refcnt_ovhd_prox_referent.hpp"
namespace boost{

////////////////////////////////////////////////////////////////////////////////
// Ownership Policies
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// class template ref_counted
// Implementation of the OwnershipPolicy used by smart_ptr
// Provides a classic external reference counting implementation
////////////////////////////////////////////////////////////////////////////////

template <typename ReferentPtr>
class ref_counted
: public managed_ptr::refcnt_ovhd_prox_referent<ReferentPtr>
{
public:
        typedef 
      ref_counted 
    type
    ;
        typedef 
      managed_ptr::refcnt_ovhd_prox_referent<ReferentPtr> 
    super_type
    ;
        typedef
      typename super_type::overhead_type 
    overhead_type
    ;
    ~ref_counted(void)
    {
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("~ref_counted(void)");
      #endif
    }
    
    ref_counted(void)
    {}

    template <typename U>
    ref_counted(ref_counted<U> const& rhs)
# ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
    : super_type(rhs)
# else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
    : super_type(reinterpret_cast<ref_counted const&>(rhs))
# endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
    {
    }

    ref_counted(ReferentPtr)
    : super_type(new overhead_type(0))
    //***DIFF_POLICY_PTR : instead of overhead_type(1) because increment occurs later.
    {
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("ref_counted(ReferentPtr)");
      #endif
    }
    
    private:
    
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        template <typename U> friend class ref_counted;
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
};

}//exit boost namespace
//////////////////////////////////////////////////////////////////////////////
// ChangeLog:
//   2003-12-10: Larry Evans
//     WHAT:
//       Moved most methods to new super type, ref_counted_detached.
//     WHY:
//       To disambiguate CTOR's for this class and refcycle_counted.
//       The refcycle_counted class has CTOR which can be confused
//       with the (U const&) CTOR above.
//////////////////////////////////////////////////////////////////////////////
#endif
