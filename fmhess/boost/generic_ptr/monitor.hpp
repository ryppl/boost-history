//
//  generic_ptr/monitor.hpp
//
//  Copyright (c) 2009 Frank Mori Hess
//  Copyright (c) 2001, 2002 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/generic_ptr for documentation.
//

#ifndef BOOST_GENERIC_PTR_MONITOR_HPP_INCLUDED
#define BOOST_GENERIC_PTR_MONITOR_HPP_INCLUDED

#include <boost/generic_ptr/detail/unique_lock.hpp>
#include <boost/generic_ptr/monitor_locks.hpp>  // for monitor_unique_lock
#include <boost/generic_ptr/pointer_cast.hpp>
#include <boost/generic_ptr/pointer_traits.hpp>
#include <boost/generic_ptr/shared.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/utility/swap.hpp>

namespace boost
{
  class mutex;
  template<typename T>
  class shared_ptr;

  namespace generic_ptr
  {
    template<typename T>
    class shared;

    template<typename MutexSharedPointer>
    MutexSharedPointer create_default_mutex(mpl::identity<MutexSharedPointer> /*adl_helper*/,
      typename shared_pointer_traits<MutexSharedPointer>::weak_type * /*SFINAE_helper*/ = 0)
    {
      // FIXME: replace with make_shared
      typedef typename shared_pointer_traits<MutexSharedPointer>::value_type mutex_type;
      mutex_type *mutex = new mutex_type();
      MutexSharedPointer result;
      try
      {
        result.reset(mutex);
      }
      catch(...)
      {
        delete mutex;
        throw;
      }
      return result;
    }

    template<typename T, typename MutexPointer = shared<boost::mutex*> >
    class monitor
    {
    public:
      typedef typename pointer_traits<MutexPointer>::value_type mutex_type;
    private:
      typedef monitor this_type; // for detail/operator_bool.hpp

      // we could use monitor_unique_lock, but this should be slightly faster
      class moving_monitor_lock
      {
      public:
        moving_monitor_lock(const monitor& mon):
          _object_p(mon.px),
          _lock(mon.get_mutex_ref())
        {}
#ifdef BOOST_NO_RVALUE_REFERENCES
        moving_monitor_lock(const moving_monitor_lock & other):
          _object_p(other._object_p),
          _lock(const_cast<moving_monitor_lock &>(other)._lock.move())
        {}
#else // BOOST_NO_RVALUE_REFERENCES
        moving_monitor_lock(moving_monitor_lock && other):
          _object_p(std::move(other._object_p)),
          _lock(std::move(other._lock))
        {}
#endif // BOOST_NO_RVALUE_REFERENCES
        T operator->() const
        {
          detail::assert_plain_old_pointer_not_null(_object_p);
          return _object_p;
        }
      private:
        moving_monitor_lock(moving_monitor_lock & other);

        T _object_p;
        detail::unique_lock<mutex_type> _lock;
      };

    public:
      typedef typename pointer_traits<T>::value_type value_type;
      typedef T pointer;
      typedef typename pointer_traits<T>::reference reference;
      typedef MutexPointer mutex_pointer_type;

      template<typename ValueType>
      struct rebind
      {
        typedef monitor<typename generic_ptr::rebind<pointer, ValueType>::other, MutexPointer > other;
      };

      monitor(): px(), _mutex_p()
      {}
      template<typename U>
      explicit monitor
      (
        U p,
        const MutexPointer & mutex_p = create_default_mutex(typename mpl::identity<MutexPointer>())
      ): px( p ), _mutex_p(mutex_p)
      {}
      template<typename U>
      monitor
      (
        const monitor<U, MutexPointer> & other
#ifndef BOOST_NO_SFINAE
        , typename enable_if<is_convertible<U, T> >::type * = 0
#endif // BOOST_NO_SFINAE
      ): px(other.px), _mutex_p(other._mutex_p)
      {}
#ifndef BOOST_NO_RVALUE_REFERENCES
      monitor(monitor && other): px(std::move(other.px)), _mutex_p(std::move(other._mutex_p))
      {}
      template<typename U>
      monitor
      (
        monitor<U> && other
#ifndef BOOST_NO_SFINAE
        , typename enable_if<is_convertible<U, T> >::type * = 0
#endif // BOOST_NO_SFINAE
      ): px(std::move(other.px)), _mutex_p(std::move(other._mutex_p))
      {}
#endif

      // default copy constructor and assignment operator are fine

      void swap(monitor & other)
      {
        boost::swap(px, other.px);
        boost::swap(_mutex_p, other._mutex_p);
      }

      template<typename U>
      monitor & operator=(const monitor<U, mutex_type> & other)
      {
        monitor(other).swap(*this);
        return *this;
      }
#ifndef BOOST_NO_RVALUE_REFERENCES
      monitor & operator=(monitor && other)
      {
        monitor(std::move(other)).swap(*this);
        return *this;
      }
      template<typename U>
      monitor & operator=(monitor<U, mutex_type> && other)
      {
        monitor(std::move(other)).swap(*this);
        return *this;
      }
#endif
      void reset()
      {
        monitor().swap(*this);
      }
      template<typename U> void reset(U object_p, const mutex_pointer_type &mutex_p)
      {
        monitor(object_p, mutex_p).swap(*this);
      }

      pointer get() const {return px;}
      MutexPointer get_mutex_pointer() const {return _mutex_p;}
      mutex_type& get_mutex_ref() const {return *_mutex_p;}

// implicit conversion to "bool"
#include <boost/generic_ptr/detail/operator_bool.hpp>

      moving_monitor_lock operator->() const
      {
        return moving_monitor_lock(*this);
      }

// Tasteless as this may seem, making all members public allows member templates
// to work in the absence of member template friends. (Matthew Langston)
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
    private:
      template<typename Y, typename OtherMutexPointer> friend class monitor;
#endif

      pointer px;
      mutex_pointer_type _mutex_p;
    };

    template<typename T, typename MutexPointer>
    T get_pointer(const monitor<T, MutexPointer> &mp)
    {
      //FIXME: should get_pointer return a monitor lock?
      // if it does not, it will not be safe to use monitor as "this" pointer with bind.
      // if it does, the monitor lock types will need to support generic pointer concepts better
      return mp.get();
    }

    // casts
    template<typename ToValueType, typename U, typename MutexPointer>
    typename rebind<monitor<U, MutexPointer>, ToValueType>::other static_pointer_cast
    (
      monitor<U, MutexPointer> const & p,
      mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
    )
    {
       typedef typename rebind<monitor<U, MutexPointer>, ToValueType>::other result_type;
       return result_type(static_pointer_cast(p.get(), to_type_iden), p.get_mutex_pointer());
    }
    template<typename ToValueType, typename U, typename MutexPointer>
    typename rebind<monitor<U, MutexPointer>, ToValueType>::other const_pointer_cast
    (
       monitor<U, MutexPointer> const & p,
       mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
    )
    {
       typedef typename rebind<monitor<U, MutexPointer>, ToValueType>::other result_type;
       return result_type(const_pointer_cast(p.get(), to_type_iden), p.get_mutex_pointer());
    }
    template<typename ToValueType, typename U, typename MutexPointer>
    typename rebind<monitor<U, MutexPointer>, ToValueType>::other dynamic_pointer_cast
    (
       monitor<U, MutexPointer> const & p,
        mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
    )
    {
       typedef typename rebind<monitor<U, MutexPointer>, ToValueType>::other result_type;
        return result_type(dynamic_pointer_cast(p.get(), to_type_iden), p.get_mutex_pointer());
    }

    // comparisons
    template<class T, class U, typename MutexPointer> inline bool operator==(monitor<T, MutexPointer> const & a, monitor<U, MutexPointer> const & b)
    {
      return a.get() == b.get();
    }
    template<class T, class U, typename MutexPointer> inline bool operator!=(monitor<T, MutexPointer> const & a, monitor<U, MutexPointer> const & b)
    {
      return a.get() != b.get();
    }
    template<class T, class U, typename MutexPointer> inline bool operator==(monitor<T, MutexPointer> const & a, U const & b)
    {
      return a.get() == b;
    }
    template<class T, class U, typename MutexPointer> inline bool operator!=(monitor<T, MutexPointer> const & a, U const & b)
    {
      return a.get() != b;
    }
    template<class T, class U, typename MutexPointer> inline bool operator==(T const & a, monitor<U, MutexPointer> const & b)
    {
      return a == b.get();
    }
    template<class T, class U, typename MutexPointer> inline bool operator!=(T const & a, monitor<U, MutexPointer> const & b)
    {
      return a != b.get();
    }
    #if __GNUC__ == 2 && __GNUC_MINOR__ <= 96
    // Resolve the ambiguity between our op!= and the one in rel_ops
    template<class T, typename MutexPointer> inline bool operator!=(monitor<T, MutexPointer> const & a, monitor<T, MutexPointer> const & b)
    {
      return a.get() != b.get();
    }
    #endif
    template<class T, typename MutexPointer> inline bool operator<(monitor<T, MutexPointer> const & a, monitor<T, MutexPointer> const & b)
    {
      return std::less<typename monitor<T, MutexPointer>::pointer>()(a.get(), b.get());
    }

    // construct_clone overload for safe (mutex locked) use with generic_ptr::cloning
    template<typename GenericPointer, typename MutexPointer>
    monitor<GenericPointer, MutexPointer> construct_clone(void *location, const monitor<GenericPointer, MutexPointer> &p)
    {
      if(get_plain_old_pointer(p) == 0) return p;
      monitor_unique_lock<monitor<GenericPointer, MutexPointer> > lock(p);
      using boost::generic_ptr::construct_clone;
      return monitor<GenericPointer, MutexPointer>(construct_clone(location, p.get()));
    }
  } // namespace generic_ptr
} // namespace boost

#endif  // #ifndef BOOST_GENERIC_PTR_MONITOR_HPP_INCLUDED
