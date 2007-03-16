#ifndef BOOST_POLICY_PTR_DETAIL_SP_COUNTED_IMPL_HPP_INCLUDED
#define BOOST_POLICY_PTR_DETAIL_SP_COUNTED_IMPL_HPP_INCLUDED
#include <boost/type_traits/remove_pointer.hpp>

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
//   2005-06-10 Larry Evans
//     WHAT:
//       1) created another template class, sp_counted_impl_hook
//       2) removed CTOR/DTOR debug hooks to specialization of
//          sp_counted_impl_hook.
//       3) changed superclass of the original classes to
//          sp_counted_impl_hook.
//       4) added calls to debug_hook to both of the original class'
//          dispose functions.
//     WHY:
//       1-2) The debug hooks were used for 2 separate purposes:
//            a) debugging using:
//                 libs/smart_ptr/src/sp_debug_hooks.cpp
//            b) "tagged" collection using:
//                 libs/smart_ptr/src/sp_collector.cpp
//          Since specialization of:
//            sp_counted_impl_hook
//          on: 
//            SpCountedBase=sp_counted_tagged
//          will be used to select whether tagged collection is done, 
//          these hooks have been moved to that specialization, in:
//            sp_counted_impl_hook_tagged.hpp
//          to fulfill purpose b).  Purpose a) has been eliminated, at least
//          for now.
//       3) To allow both of original classes to be used for "tagged"
//          collection.
//       4) To enable "tagged" collection when SpCountedBase=sp_counted_tagged.
//   2005-05-18 Larry Evans
//     WHAT:
//       1) Copied from:
//            http://cvs.sourceforge.net/viewcvs.py/boost/boost/
//            boost/detail/sp_counted_impl.hpp?rev=1.2
//       2) Added another template parameter to sp_counted_base_impl_*.
//       2) Moved to policy_ptr/detail namespace.
//     WHY:
//       1) To modify.
//       2) Provide a general template  which can be specialized for collecting 
//          cycles.
//       3) Avoid name conflict.
//

#include <boost/config.hpp>

#if defined(BOOST_SP_USE_STD_ALLOCATOR) && defined(BOOST_SP_USE_QUICK_ALLOCATOR)
# error BOOST_SP_USE_STD_ALLOCATOR and BOOST_SP_USE_QUICK_ALLOCATOR are incompatible.
#endif

#include <boost/checked_delete.hpp>
#include <boost/policy_ptr/detail/sp_counted_base.hpp>

#if defined(BOOST_SP_USE_QUICK_ALLOCATOR)
#include <boost/detail/quick_allocator.hpp>
#endif

#include <memory>           // std::allocator
#include <typeinfo>         // std::type_info in get_deleter
#include <cstddef>          // std::size_t

namespace boost
{

namespace policy_ptr
{

namespace detail
{

template<class Referent, class SpCountedBase> 
class sp_counted_impl_hook
  : public SpCountedBase
/**@brief
 *  Define general template to allow specializations on
 *  SpCountedBase to be defined.
 */
{
protected:

    virtual Referent* typed_ref_start(void)const
    =0
    ;
    
    sp_counted_impl_hook( Referent* )
    {}
    
    void dispose_hook()
    /**@brief
     *   See SHARED_PTR_DIFF_NOTE: in comments under
     *   sp_counted_impl_hook<Referent,sp_counted_tagged>::dispose_hook
     *   located in sp_counted_impl_hook_sp_counted_tagged.hpp.
     */
    {}
    
};

template<class X, class SpCountedBase> 
class sp_counted_impl_p
: public sp_counted_impl_hook<X, SpCountedBase>
/**@brief
 *  Implements virtual functions of SpCountBase which depend
 *  on the type of the "referent" (i.e. pointee) in the smart
 *  pointer.
 */
{
private:

    typedef X ref_type;
    typedef sp_counted_impl_hook<ref_type,SpCountedBase> super_type;

    ref_type * px_;

    sp_counted_impl_p( sp_counted_impl_p const & );
    sp_counted_impl_p & operator= ( sp_counted_impl_p const & );

    typedef sp_counted_impl_p this_type;
    
protected:

    ref_type* typed_ref_start(void)const
    {
        return px_;
    }

public:

    explicit sp_counted_impl_p( ref_type * px ): super_type(px), px_( px )
    {
    }

    virtual void dispose() // nothrow
    {
        this->dispose_hook();
        boost::checked_delete( px_ );
    }

    virtual void * get_deleter( std::type_info const & )
    {
        return 0;
    }

#if defined(BOOST_SP_USE_STD_ALLOCATOR)

    void * operator new( std::size_t )
    {
        return std::allocator<this_type>().allocate( 1, static_cast<this_type *>(0) );
    }

    void operator delete( void * p )
    {
        std::allocator<this_type>().deallocate( static_cast<this_type *>(p), 1 );
    }

#endif

#if defined(BOOST_SP_USE_QUICK_ALLOCATOR)

    void * operator new( std::size_t )
    {
        return quick_allocator<this_type>::alloc();
    }

    void operator delete( void * p )
    {
        quick_allocator<this_type>::dealloc( p );
    }

#endif
};

//
// Borland's Codeguard trips up over the -Vx- option here:
//
#ifdef __CODEGUARD__
# pragma option push -Vx-
#endif

template<class P, class D, class SpCountedBase> 
class sp_counted_impl_pd
: public sp_counted_impl_hook<typename remove_pointer<P>::type,SpCountedBase>
//2005-06-09 Larry Evans
//   SHARED_PTR_DIFF_NOTE:
//     In the original code, which is at boost/detail/sp_counted_impl.hpp,
//     the superclass was sp_counted_base.  This was changed to the above
//     for reasons given in the SHARED_PTR_DIFF_NOTE in the comments to
//     sp_counted_impl_hook<X,sp_counted_tagged>::dispose_hook.
//
//   QUESTIONS:
//     Why is P not the Referent?  If it were, this would eliminate the
//     need for remove_pointer.
{
private:

    typedef typename remove_pointer<P>::type ref_type;
    typedef sp_counted_impl_hook<ref_type,SpCountedBase> super_type;

    P ptr; // copy constructor must not throw
    D del; // copy constructor must not throw

    sp_counted_impl_pd( sp_counted_impl_pd const & );
    sp_counted_impl_pd & operator= ( sp_counted_impl_pd const & );

    typedef sp_counted_impl_pd this_type;

protected:

    ref_type* typed_ref_start(void)const
    {
        return ptr;
    }

public:

    // pre: del(p) must not throw

    sp_counted_impl_pd( P p, D d ): super_type(p), ptr(p), del(d)
    {
    }

    virtual void dispose() // nothrow
    {
        this->dispose_hook();
        del( ptr );
    }

    virtual void * get_deleter( std::type_info const & ti )
    {
        return ti == typeid(D)? &del: 0;
    }

#if defined(BOOST_SP_USE_STD_ALLOCATOR)

    void * operator new( std::size_t )
    {
        return std::allocator<this_type>().allocate( 1, static_cast<this_type *>(0) );
    }

    void operator delete( void * p )
    {
        std::allocator<this_type>().deallocate( static_cast<this_type *>(p), 1 );
    }

#endif

#if defined(BOOST_SP_USE_QUICK_ALLOCATOR)

    void * operator new( std::size_t )
    {
        return quick_allocator<this_type>::alloc();
    }

    void operator delete( void * p )
    {
        quick_allocator<this_type>::dealloc( p );
    }

#endif
};

#ifdef __CODEGUARD__
# pragma option pop
#endif

} // namespace detail

} // namespace policy_ptr

} // namespace boost

#endif  // #ifndef BOOST_POLICY_PTR_DETAIL_SP_COUNTED_IMPL_HPP_INCLUDED
