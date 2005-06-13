#ifndef BOOST_POLICY_PTR_DETAIL_SHARED_COUNT_HPP_INCLUDED
#define BOOST_POLICY_PTR_DETAIL_SHARED_COUNT_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  policy_ptr/detail/shared_count.hpp
//
//  Copyright (c) 2001, 2002, 2003 Peter Dimov and Multi Media Ltd.
//  Copyright 2004-2005 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// ChangeLog(latest at top):
//   2005-06-08 Larry Evans
//     WHAT:
//       1) removed all use of BOOST_SP_ENABLE_DEBUG_HOOKS
//     WHY:
//       1) the code guarded by this macro has been moved to specialization
//          of shared_count<C> on C=sp_counted_tagged.  This was done
//          to simplify the specialization code and because then the hook
//          functions ( sp_scalar_{constructor,destructor}_hook ) only
//          are not overloaded.  This, IMO, made understanding the code
//          and linking the code easier.
//   2005-05-25 Larry Evans
//     WHAT:
//       1) added shared_count_base::is_valid
//     WHY:
//       1) convenience (for use in shared_count_sp_counted_accepting:
//          shared_count_visitor_abs::visit_field).
//   2005-05-18 Larry Evans
//     WHAT:
//       1) Copied from boost/detail and then modified by adding another
//          template parameter to shared_count
//       2) Moved to policy_ptr/detail namespace.
//     WHY:
//       1) See ChangeLog in boost/policy_ptr/detail/sp_counted_impl.hpp.
//       2) Avoid name conflict.
//

#include <boost/config.hpp>
#include <boost/checked_delete.hpp>
#include <boost/throw_exception.hpp>
#include <boost/detail/bad_weak_ptr.hpp>
#include <boost/policy_ptr/detail/sp_counted_impl.hpp>

#include <memory>           // std::auto_ptr, std::allocator
#include <functional>       // std::less
#include <new>              // std::bad_alloc
#include <typeinfo>         // std::type_info in get_deleter

#ifdef __BORLANDC__
# pragma warn -8027     // Functions containing try are not expanded inline
#endif

namespace boost
{

namespace policy_ptr
{

namespace detail
{

template<class SpCountedBase=sp_counted_base>
class weak_count;

template<class SpCountedBase>
class shared_count_base
/**@brief
 *  Base class enabling derived classes to:
 *    1) Emulate current boost shared_count
 *    2) Do 1) but also allow collection of cycles.
 */
{
protected:

    SpCountedBase * pi_;

    friend class weak_count<SpCountedBase>;

public:

    bool is_valid() const
    { return pi_ != 0; }
    
    shared_count_base(): pi_(0) // nothrow
    {
    }

    template<class Y> explicit shared_count_base( Y * p ): pi_( 0 )
    {
#ifndef BOOST_NO_EXCEPTIONS

        try
        {
            pi_ = new sp_counted_impl_p<Y,SpCountedBase>( p );
        }
        catch(...)
        {
            boost::checked_delete( p );
            throw;
        }

#else

        pi_ = new sp_counted_impl_p<Y,SpCountedBase>( p );

        if( pi_ == 0 )
        {
            boost::checked_delete( p );
            boost::throw_exception( std::bad_alloc() );
        }

#endif
    }

    template<class P, class D> shared_count_base(P p, D d): pi_(0)
    {
#ifndef BOOST_NO_EXCEPTIONS

        try
        {
            pi_ = new sp_counted_impl_pd<P, D, SpCountedBase>(p, d);
        }
        catch(...)
        {
            d(p); // delete p
            throw;
        }

#else

        pi_ = new sp_counted_impl_pd<P, D, SpCountedBase>(p, d);

        if(pi_ == 0)
        {
            d(p); // delete p
            boost::throw_exception(std::bad_alloc());
        }

#endif
    }

#ifndef BOOST_NO_AUTO_PTR

    // auto_ptr<Y> is special cased to provide the strong guarantee

    template<class Y>
    explicit shared_count_base( std::auto_ptr<Y> & r )
      : pi_( new sp_counted_impl_p<Y,SpCountedBase>( r.get() ) )
    {
#ifdef BOOST_NO_EXCEPTIONS

        if( pi_ == 0 )
        {
            boost::throw_exception(std::bad_alloc());
        }

#endif

        r.release();
    }

#endif 

    ~shared_count_base() // nothrow
    {
#if 1
        if( pi_ != 0 ) pi_->release();
#endif
    }

    shared_count_base(shared_count_base const & r): pi_(r.pi_) // nothrow
    {
        if( pi_ != 0 ) pi_->add_ref_copy();
    }

    explicit shared_count_base(weak_count<SpCountedBase> const & r); // throws bad_weak_ptr when r.use_count() == 0

    shared_count_base & operator= (shared_count_base const & r) // nothrow
    {
        SpCountedBase * tmp = r.pi_;

        if( tmp != pi_ )
        {
            if( tmp != 0 ) tmp->add_ref_copy();
            if( pi_ != 0 ) pi_->release();
            pi_ = tmp;
        }

        return *this;
    }

    void swap(shared_count_base & r) // nothrow
    {
        SpCountedBase * tmp = r.pi_;
        r.pi_ = pi_;
        pi_ = tmp;
    }

    long use_count() const // nothrow
    {
        return pi_ != 0? pi_->use_count(): 0;
    }

    bool unique() const // nothrow
    {
        return use_count() == 1;
    }

    friend inline bool operator==(shared_count_base const & a, shared_count_base const & b)
    {
        return a.pi_ == b.pi_;
    }

    friend inline bool operator<(shared_count_base const & a, shared_count_base const & b)
    {
        return std::less<SpCountedBase *>()(a.pi_, b.pi_);
    }

    void * get_deleter(std::type_info const & ti) const
    {
        return pi_? pi_->get_deleter( ti ): 0;
    }
};

template<class SpCountedBase>
class shared_count
  : public shared_count_base<SpCountedBase>
/**@brief
 *  General template which
 *    1) Emulates current boost shared_count
 *  but allow specialzation (in shared_cyclic_count.hpp) to:
 *    2) Do 1) but also allow collection of cycles.
 */
{
        typedef
      shared_count_base<SpCountedBase>
    count_super_type
    ;
 public:

    shared_count(void)
    { }
    
    shared_count(shared_count const & r)
      : count_super_type(r)
    { }
    
    template<class Y>
    shared_count(Y* p)
      : count_super_type(p)
    { }
    
    explicit shared_count(weak_count<SpCountedBase> const & r)
      : count_super_type(r)
    { }
};

template<class SpCountedBase>
class weak_count
{
private:

    SpCountedBase * pi_;

    friend class shared_count_base<SpCountedBase>;

public:

    weak_count(): pi_(0) // nothrow
    {
    }

    weak_count(shared_count<SpCountedBase> const & r): pi_(r.pi_) // nothrow
    {
        if(pi_ != 0) pi_->weak_add_ref();
    }

    weak_count(weak_count const & r): pi_(r.pi_) // nothrow
    {
        if(pi_ != 0) pi_->weak_add_ref();
    }

    ~weak_count() // nothrow
    {
        if(pi_ != 0) pi_->weak_release();
    }

    weak_count & operator= (shared_count<SpCountedBase> const & r) // nothrow
    {
        SpCountedBase * tmp = r.pi_;
        if(tmp != 0) tmp->weak_add_ref();
        if(pi_ != 0) pi_->weak_release();
        pi_ = tmp;

        return *this;
    }

    weak_count & operator= (weak_count const & r) // nothrow
    {
        SpCountedBase * tmp = r.pi_;
        if(tmp != 0) tmp->weak_add_ref();
        if(pi_ != 0) pi_->weak_release();
        pi_ = tmp;

        return *this;
    }

    void swap(weak_count & r) // nothrow
    {
        SpCountedBase * tmp = r.pi_;
        r.pi_ = pi_;
        pi_ = tmp;
    }

    long use_count() const // nothrow
    {
        return pi_ != 0? pi_->use_count(): 0;
    }

    friend inline bool operator==(weak_count const & a, weak_count const & b)
    {
        return a.pi_ == b.pi_;
    }

    friend inline bool operator<(weak_count const & a, weak_count const & b)
    {
        return std::less<SpCountedBase *>()(a.pi_, b.pi_);
    }
};

template<class SpCountedBase>
inline shared_count_base<SpCountedBase>::
shared_count_base(weak_count<SpCountedBase> const & r): pi_(r.pi_)
{
    if( pi_ == 0 || !pi_->add_ref_lock() )
    {
        boost::throw_exception( boost::bad_weak_ptr() );
    }
}

} // namespace detail

} // namespace policy_ptr

} // namespace boost

#ifdef __BORLANDC__
# pragma warn .8027     // Functions containing try are not expanded inline
#endif

#endif  // #ifndef BOOST_POLICY_PTR_DETAIL_SHARED_COUNT_HPP_INCLUDED
