//base class for detached reference counting
#ifndef BOOST_MANAGED_PTR_REF_COUNTED_DETACHED_BASE_LJE20031109_HPP
#define BOOST_MANAGED_PTR_REF_COUNTED_DETACHED_BASE_LJE20031109_HPP
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
#include <boost/managed_ptr/exceptions.hpp>
#include "boost/managed_ptr/refcnt_overhead.hpp"
namespace boost{namespace managed_ptr{
////////////////////////////////////////////////////////////////////////////////
// class template ref_counted_detached_base
// Implementation of the OwnershipPolicy used by smart_ptr
// Provides a classic detached reference counting implementation
////////////////////////////////////////////////////////////////////////////////

    template <typename ReferentPtr>
    class ref_counted_detached_base
    {
    public:
        typedef ref_counted_detached_base type;
        typedef ownership_policy_tag policy_category;
        typedef copy_semantics_tag ownership_category;
        typedef refcnt_overhead overhead_type;
        
    protected:
        ref_counted_detached_base()
        : count_(0)
        {
        }

        template <typename U>
        ref_counted_detached_base(ref_counted_detached_base<U> const& rhs)
# ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : count_(rhs.count_)
# else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : count_(reinterpret_cast<ref_counted_detached_base const&>(rhs).count_)
# endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        { 
            #ifdef TRACE_SCOPE_HPP
            utility::trace_scope ts("ref_counted_detached_base(ref_counted_detached_base<U>const&)");
            #endif
        }

        ref_counted_detached_base(overhead_type* subtype_overhead)
        : count_(subtype_overhead)
        //***DIFF_POLICY_PTR to allow reuse of this class
        //  as superclass 
        {
            #ifdef TRACE_SCOPE_HPP
            utility::trace_scope ts("ref_counted_detached_base(overhead_type*)");
            #endif
            //***DIFF_POLICY_PTR
            //since subjtype_overhead initialized with 0 count (because
            //subjtype_overhead MAY have come from auto_new and hence, may be
            //deleted without being inserted into a smart_ptr), increment now:
            increment();
        }
        
        ~ref_counted_detached_base()
        { 
            #ifdef TRACE_SCOPE_HPP
            utility::trace_scope ts("~ref_counted_detached_base()");
            mout()<<"delete count_="<<count_<<"\n";
            #endif
            delete count_; 
        }
        //SmallObject<>::operator delete(count_, sizeof(overhead_type));

        ReferentPtr clone(ReferentPtr const& val)
        {
            #ifdef TRACE_SCOPE_HPP
            utility::trace_scope ts("ref_counted_detached_base::clone");
            mout()<<":count_=";
            marg_ostream_insert_refcnt_overhead_ptr(mout(),count_);
            mout()<<std::endl;
            #endif
            increment();
            return val;
        }

        bool release(ReferentPtr const&)
        //Purpose:
        //  Indicates whether this is the last release; hence,
        //  ~storage_policy should delete it's pointee.
        //
        //  See ~resource_manager.
        //Paraphrase:
        //  if count is already released:
        //    return true.
        //  otherwise:
        //    decrement count and 
        //    if result is zero:
        //      DO NOT release count
        //      (so that ~ref_counted_detached_base will delete it)
        //      and return true.
        //    otherwise:
        //      DO release count
        //      (so that ~ref_counted_detached_base will NOT delete it)
        //      and return false.
        {
            #ifdef TRACE_SCOPE_HPP
            utility::trace_scope ts("ref_counted_detached_base::release");
            mout()<<"b4 count_=";
            marg_ostream_insert_refcnt_overhead_ptr(mout(),count_);
            mout()<<"\n";
            #endif
            if (!decrement()) return true;
            prevent_dtor_delete(); //i.e. release count
            #ifdef TRACE_SCOPE_HPP
            mout()<<"af count_=";
            marg_ostream_insert_refcnt_overhead_ptr(mout(),count_);
            mout()<<"\n";
            #endif
            return false;
        }

        void swap(ref_counted_detached_base& rhs)
        {
            utility::trace_scope ts("ref_counted_detached_base::swap");
            std::swap(count_, rhs.count_);
        }
        
    public:
        static void on_release(ref_counted_detached_base& sp)
        {
            if (sp.count_ && sp.count_->as_count() > 1) throw bad_release();
        }

        static void reset(ref_counted_detached_base& sp)
        {
            if (sp.count_)
            {
                sp.decrement();
                sp.prevent_dtor_delete();
            }
        }

        friend inline overhead_type::count_type risky_use_count(ref_counted_detached_base const& p)
        { 
            return p.count_->as_count(); 
        }

        bool does_count_exist()const
        {
            return count_ != 0;
        }

        friend inline overhead_type::count_type use_count(ref_counted_detached_base const& p)
        { 
            return p.does_count_exist() ? risky_use_count(p) : 0; 
        }

    public:
        bool is_count_valid()const
        {
            return does_count_exist() && count_->is_valid();
        }

        bool operator==(ref_counted_detached_base const& rhs)
        {
            return count_ == rhs.count_;
        }

        bool operator!=(ref_counted_detached_base const& rhs)
        {
            return !(this->operator==(rhs));
        }

    private:
        ref_counted_detached_base& operator=(ref_counted_detached_base const&);

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        template <typename U> friend class ref_counted_detached_base;
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

        // Data
        overhead_type* count_;
        
        void increment()
        {
            if(does_count_exist()) count_->increment();
        }
        
        overhead_type::count_type decrement()
        {
            overhead_type::count_type l_val=0;
            if(does_count_exist())
            {
                count_->decrement();
                l_val = count_->as_count();
            }
            return l_val;
        }
        
    protected:
        //allow use of next methods by
        //  curry_prox_visitor_refcycle_counted<ProxVisitor>::owner<ReferentPtr>:
        
        void prevent_dtor_delete()
        //prevent delete of count_ by the DTOR.
        {
            count_ = 0;
        }
        
        void reset(overhead_type* a_ovhd)
        //used by refcycle_counted
        {
            reset(*this);
            count_ = a_ovhd;
            count_->increment();
        }
        
        overhead_type* get_overhead()
        {
            return count_;
        }

    };

}}//exit boost::managed_ptr namespace
#endif
