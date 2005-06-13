#ifndef BOOST_POLICY_PTR_DETAIL_SP_COUNTED_BASE_NT_HPP_INCLUDED
#define BOOST_POLICY_PTR_DETAIL_SP_COUNTED_BASE_NT_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  Copyright (c) 2001, 2002, 2003 Peter Dimov and Multi Media Ltd.
//  Copyright 2004-2005 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// ChangeLog:
//   2005-05-24 Larry Evans
//     WHAT:
//       1) Copied from boost/detail
//       2) Moved to policy_ptr/detail namespace.
//       3) Added void_ptr.
//       4) Added friend gc_strong_count_decrementer,
//     WHY:
//       1) See ChangeLog in sp_counted_impl.hpp.
//       2) Avoid name conflict with original class.
//       3) Convenience during debugging (used by shared_count_visitor_print.hpp
//          in libs/policy_ptr/test ).
//       4) Allow garbage collector to decrement strong count without
//          calling dispose.

#include <typeinfo>

namespace boost
{

namespace policy_ptr
{

namespace detail
{

class gc_strong_count_decrementer;

class shared_count_visitor_abs;  

class sp_counted_base
{
    friend class gc_strong_count_decrementer
    /**@brief
     *  Allow garbage collectors to access strong count
     */
    ;
private:

    sp_counted_base( sp_counted_base const & );
    sp_counted_base & operator= ( sp_counted_base const & );

    long use_count_;        // #shared
    long weak_count_;       // #weak + (#shared != 0)

    friend class shared_count_visitor_abs;
    
public:

    sp_counted_base(): use_count_( 1 ), weak_count_( 1 )
    {
    }

    virtual ~sp_counted_base() // nothrow
    {
    }

    // dispose() is called when use_count_ drops to zero, to release
    // the resources managed by *this.

    virtual void dispose() = 0; // nothrow

    // destroy() is called when weak_count_ drops to zero.

    virtual void destroy() // nothrow
    {
        delete this;
    }

    virtual void * get_deleter( std::type_info const & ti ) = 0;

    void add_ref_copy()
    {
        ++use_count_;
    }

    bool add_ref_lock() // true on success
    {
        if( use_count_ == 0 ) return false;
        ++use_count_;
        return true;
    }

    void release() // nothrow
    {
        if( --use_count_ == 0 )
        {
            dispose();
            weak_release();
        }
    }

    void weak_add_ref() // nothrow
    {
        ++weak_count_;
    }

    void weak_release() // nothrow
    {
        if( --weak_count_ == 0 )
        {
            destroy();
        }
    }

    long use_count() const // nothrow
    {
        return use_count_;
    }
};

class gc_strong_count_decrementer
{
 protected:
    static void decrement_strong(sp_counted_base& a_count)
    {
        --a_count.use_count_;
    }
};

} // namespace detail

} // namespace policy_ptr

} // namespace boost

#endif  // #ifndef BOOST_POLICY_PTR_DETAIL_SP_COUNTED_BASE_NT_HPP_INCLUDED
