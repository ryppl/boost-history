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

#include <boost/generic_ptr/pointer_traits.hpp>
#include <boost/generic_ptr/detail/unique_lock.hpp>
#include <boost/generic_ptr/shared.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/utility/swap.hpp>

namespace boost
{
  class mutex;

  namespace generic_ptr
  {
    template<typename T, typename Mutex = boost::mutex>
    class monitor
    {
      typedef monitor this_type; // for detail/operator_bool.hpp
#ifdef BOOST_NO_RVALUE_REFERENCES
      class moveable_monitor_lock
      {
      public:
        moveable_monitor_lock(T p, Mutex &m): _object_p(p),
          _lock(new detail::unique_lock<Mutex>(m))
        {}
        T operator->() const
        {
          return _object_p;
        }
      private:
        T _object_p;
        shared<detail::unique_lock<Mutex> *> _lock;
      };
#else
      typedef monitor_unique_lock<monitor> moveable_monitor_lock;
#endif // BOOST_NO_RVALUE_REFERENCES

    public:
      typedef typename pointer_traits<T>::value_type value_type;
      typedef T pointer;
      typedef typename pointer_traits<T>::reference reference;
      typedef Mutex mutex_type;

      template<typename ValueType>
      struct rebind
      {
        typedef monitor<typename generic_ptr::rebind<pointer, ValueType>::other, Mutex > other;
      };

      monitor(): px(), _mutex_p()
      {}
      template<typename U>
      monitor( U p, const shared<Mutex*> & mutex_p = shared<Mutex*>(new Mutex()) ): px( p ), _mutex_p(mutex_p)
      {}
      template<typename U>
      monitor(const monitor<U, Mutex> & other): px(other.px), _mutex_p(other._mutex_p)
      {}
#ifndef BOOST_NO_RVALUE_REFERENCES
      monitor(monitor && other): px(std::move(other.px)), _mutex_p(std::move(other._mutex_p)
      {}
      template<typename U>
      monitor(monitor<U> && other): px(std::move(other.px)), _mutex_p(std::move(other._mutex_p)
      {}
#endif

      void swap(monitor & other)
      {
        boost::swap(px, other.px);
        boost::swap(_mutex_p, other._mutex_p);
      }

      monitor & operator=(const monitor & other)
      {
        monitor(other).swap(*this);
        return *this;
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
      template<typename U> void reset(U object_p, const shared<mutex_type*> &mutex_p)
      {
        monitor(object_p, mutex_p).swap(*this);
      }

      pointer get() const {return px;}
      shared<mutex_type*> get_shared_mutex() const {return _mutex_p;}
      mutex_type& get_mutex_ref() const {return *_mutex_p;}

// implicit conversion to "bool"
#include <boost/generic_ptr/detail/operator_bool.hpp>

      moveable_monitor_lock operator->() const
      {
        return moveable_monitor_lock(px, *_mutex_p);
      }

    private:
      pointer px;
      shared<mutex_type*> _mutex_p;
    };

    template<typename T, typename Mutex>
    T get_pointer(const monitor<T, Mutex> &mp)
    {
      return mp.get();
    }

    // casts
    template<typename ToValueType, typename U, typename Mutex>
    monitor<ToValueType, Mutex> static_pointer_cast
    (
      monitor<U, Mutex> const & p,
      mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
    )
    {
        return static_pointer_cast(p.get(), to_type_iden);
    }
    template<typename ToValueType, typename U, typename Mutex>
    monitor<ToValueType, Mutex> const_pointer_cast
    (
      monitor<U, Mutex> const & p,
      mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
    )
    {
        return const_pointer_cast(p.get(), to_type_iden);
    }
    template<typename ToValueType, typename U, typename Mutex>
    monitor<ToValueType, Mutex> dynamic_pointer_cast
    (
      monitor<U, Mutex> const & p,
      mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
    )
    {
        return dynamic_pointer_cast(p.get(), to_type_iden);
    }

    // comparisons
    template<class T, class U, typename Mutex> inline bool operator==(monitor<T, Mutex> const & a, monitor<U, Mutex> const & b)
    {
      return a.get() == b.get();
    }
    template<class T, class U, typename Mutex> inline bool operator!=(monitor<T, Mutex> const & a, monitor<U, Mutex> const & b)
    {
      return a.get() != b.get();
    }
    template<class T, class U, typename Mutex> inline bool operator==(monitor<T, Mutex> const & a, U const & b)
    {
      return a.get() == b;
    }
    template<class T, class U, typename Mutex> inline bool operator!=(monitor<T, Mutex> const & a, U const & b)
    {
      return a.get() != b;
    }
    template<class T, class U, typename Mutex> inline bool operator==(T const & a, monitor<U, Mutex> const & b)
    {
      return a == b.get();
    }
    template<class T, class U, typename Mutex> inline bool operator!=(T const & a, monitor<U, Mutex> const & b)
    {
      return a != b.get();
    }
    #if __GNUC__ == 2 && __GNUC_MINOR__ <= 96
    // Resolve the ambiguity between our op!= and the one in rel_ops
    template<class T, typename Mutex> inline bool operator!=(monitor<T, Mutex> const & a, monitor<T, Mutex> const & b)
    {
      return a.get() != b.get();
    }
    #endif
    template<class T, typename Mutex> inline bool operator<(monitor<T, Mutex> const & a, monitor<T, Mutex> const & b)
    {
      return std::less<typename monitor<T, Mutex>::pointer>()(a.get(), b.get());
    }
  } // namespace generic_ptr
} // namespace boost

#endif  // #ifndef BOOST_GENERIC_PTR_MONITOR_HPP_INCLUDED
