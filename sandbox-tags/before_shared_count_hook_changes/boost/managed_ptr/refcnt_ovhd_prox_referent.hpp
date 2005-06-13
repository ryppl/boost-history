//base class for proxy for reference count w/clone release.
#ifndef BOOST_MANAGED_PTR_REFCNT_OVHD_PROX_REFERENT_LJE20040627_HPP
#define BOOST_MANAGED_PTR_REFCNT_OVHD_PROX_REFERENT_LJE20040627_HPP
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
#include <boost/managed_ptr/policy_move_copy_tags.hpp>
#include "boost/managed_ptr/refcnt_overhead.hpp"
#include "boost/managed_ptr/refcnt_ovhd_prox_coord.hpp"
namespace boost{namespace managed_ptr{
    template
      < typename ReferentPtr
      >
    class refcnt_ovhd_prox_referent
    : public refcnt_ovhd_prox_coord
    //Purpose:
    //  Provides the additional 
    //    1) typedefs
    //    2) methods 
    //  needed by an OwnershipPolicy for smart_ptr.
    {
    public:
    
        typedef refcnt_ovhd_prox_referent<ReferentPtr> type;
        typedef ownership_policy_tag policy_category;
        typedef copy_semantics_tag ownership_category;
        
    protected:
    
        refcnt_ovhd_prox_referent()
        {}
        
        refcnt_ovhd_prox_referent(overhead_type* subtype_overhead)
        : refcnt_ovhd_prox_coord(subtype_overhead)
        {}
        
        template <typename U>
        refcnt_ovhd_prox_referent(refcnt_ovhd_prox_referent<U> const& rhs)
# ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : super_type(rhs)
# else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : super_type(reinterpret_cast<ref_counted const&>(rhs))
# endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        {
        }
        
        ReferentPtr clone(ReferentPtr const& val)
        {
            #ifdef TRACE_SCOPE_HPP
            utility::trace_scope ts("refcnt_ovhd_prox_referent::clone");
            mout()<<":count_=";
            marg_ostream_insert_refcnt_overhead_ptr(mout(),get_overhead());
            mout()<<std::endl;
            #endif
            increment();
            return val;
        }

        bool release(ReferentPtr const&)
        {
           return refcnt_ovhd_prox_coord::release();
        }
        
    private:
    
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        template <typename U> friend class refcnt_ovhd_prox_referent;
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

        typedef refcnt_ovhd_prox_coord super_type;

    };//end refcnt_ovhd_prox_referent class

}}//exit boost::managed_ptr namespace
#endif
