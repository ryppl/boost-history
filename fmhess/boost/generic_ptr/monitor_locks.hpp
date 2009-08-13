// Extensions of Boost.Thread lock classes, which add support for
// operator->() and operator*().  Used in conjunction with
// boost::generic_ptr::monitor.

// Copyright Frank Mori Hess <fmhess@users.sourceforge.net>  2007, 2009

//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GENERIC_PTR_MONITOR_LOCKS_HPP_INCLUDED
#define BOOST_GENERIC_PTR_MONITOR_LOCKS_HPP_INCLUDED

#include <boost/bind.hpp>
#include <boost/config.hpp>
#include <boost/function.hpp>
#include <boost/generic_ptr/pointer_traits.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/exceptions.hpp>
#include <boost/thread/locks.hpp>
#include <boost/utility/addressof.hpp>
#include <boost/utility/swap.hpp>

namespace boost
{
  namespace generic_ptr
  {
    template<typename MutexPtr>
    class monitor_upgrade_lock;

    namespace detail
    {
      template<typename MonitorPtr, typename Lock>
      class lock_wrapper
      {
      public:
        typedef typename pointer_traits<MonitorPtr>::value_type value_type;
        typedef MonitorPtr pointer;
        typedef typename pointer_traits<MonitorPtr>::reference reference;

        lock_wrapper()
        {}
        explicit lock_wrapper(const MonitorPtr &mon):
          _mon(mon),
          _lock(_mon.get_mutex_ref())
        {}
        template<typename U>
        lock_wrapper(const MonitorPtr &mon, const U &arg):
          _mon(mon),
          _lock(_mon.get_mutex_ref(), arg)
        {}
#ifdef BOOST_NO_RVALUE_REFERENCES
        template<typename OtherLock>
        lock_wrapper(boost::detail::thread_move_t<OtherLock> other):
          _mon(other->_mon), _lock(other->_lock.move())
        {}
#else // BOOST_NO_RVALUE_REFERENCES
        template<typename OtherLock>
        lock_wrapper(OtherLock && other):
          _mon(std::move(other._mon)), _lock(std::move(other._lock))
        {}
#endif // BOOST_NO_RVALUE_REFERENCES
        // unique/shared_lock interface
        void swap(lock_wrapper &other)
        {
          boost::swap(_mon, other._mon);
          boost::swap(_lock, other._lock);
        }
        void lock()
        {
          _lock.lock();
        }
        bool try_lock()
        {
          return _lock.try_lock();
        }
        template<typename Timeout>
        bool timed_lock(const Timeout &timeout)
        {
          return _lock.timed_lock(timeout);
        }
        void unlock()
        {
          _lock.unlock();
        }
        bool owns_lock() const
        {
          return _lock.owns_lock();
        }
        bool locked() const // backwards compatibility
        {
          return owns_lock();
        }
        // safe bool idiom, somewhat safer than providing conversion to bool operator
        typedef Lock * Lock::* unspecified_bool_type;
        operator unspecified_bool_type() const
        {
          return !_lock ? 0 : &_lock;
        }
        bool operator!() const
        {
          return !_lock;
        }
        MonitorPtr monitor() const
        {
          return _mon;
        }
        MonitorPtr release()
        {
          _mon.reset();
          _lock.release();
          return _mon;
        }

        // monitor extensions to lock interface
        typename pointer_traits<MonitorPtr>::pointer operator->() const
        {
          if(owns_lock() == false)
          {
            throw boost::lock_error();
          }
          typename pointer_traits<MonitorPtr>::pointer p = _mon.get();
          detail::assert_plain_old_pointer_not_null(p);
          return p;
        }
        reference operator*() const
        {
          if(owns_lock() == false)
          {
            throw boost::lock_error();
          }
          typename pointer_traits<MonitorPtr>::pointer p = _mon.get();
          detail::assert_plain_old_pointer_not_null(p);
          return *p;
        }
  #ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
      protected:
  #endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        pointer _mon;
        typedef typename rebind
        <
          typename MonitorPtr::pointer,
          const typename MonitorPtr::value_type
        >::other const_wrapped_pointer_type;
  #ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
      private:
        template<typename Lockable>
        friend class monitor_shared_lock;
        template<typename Lockable>
        friend class monitor_upgrade_lock;
        template<typename UpgradeLock>
        friend class monitor_upgrade_to_unique_lock;
        template<typename M, typename L>
        friend class lock_wrapper;
  #endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

        // non-copyable
        lock_wrapper(lock_wrapper &);
        lock_wrapper& operator=(lock_wrapper&);

        Lock _lock;
      };
    }

    template<typename MonitorPtr>
    class monitor_unique_lock: public detail::lock_wrapper<MonitorPtr,
      boost::unique_lock<typename MonitorPtr::mutex_type> >
    {
      typedef detail::lock_wrapper
      <
        MonitorPtr,
        boost::unique_lock<typename MonitorPtr::mutex_type>
      > base_class;
    public:
      monitor_unique_lock()
      {}
      explicit monitor_unique_lock(const MonitorPtr &mon): base_class(mon)
      {}
      template<typename U>
      monitor_unique_lock(const MonitorPtr &mon, const U &arg):
        base_class(mon, arg)
      {}
      // move constructors
#ifdef BOOST_NO_RVALUE_REFERENCES
      monitor_unique_lock(boost::detail::thread_move_t<monitor_unique_lock> other):
        base_class(other)
      {}
      monitor_unique_lock(boost::detail::thread_move_t<monitor_upgrade_lock<MonitorPtr> > other):
        base_class(other)
      {}

      // move emulation
      boost::detail::thread_move_t<monitor_unique_lock> move()
      {
        return boost::detail::thread_move_t<monitor_unique_lock>(*this);
      }
      template<typename Lock>
      monitor_unique_lock& operator=(boost::detail::thread_move_t<Lock> other)
      {
        monitor_unique_lock(other).swap(*this);
        return *this;
      }
      operator boost::detail::thread_move_t<monitor_unique_lock>()
      {
        return move();
      }
#else // BOOST_NO_RVALUE_REFERENCES
      monitor_unique_lock(monitor_unique_lock && other):
        base_class(std::move(other))
      {}
      monitor_unique_lock(monitor_upgrade_lock<MonitorPtr> && other):
        base_class(std::move(other))
      {}

      monitor_unique_lock&& move()
      {
        return std::move(*this);
      }
      template<typename Lock>
      monitor_unique_lock& operator=(Lock && other)
      {
        monitor_unique_lock(std::move(other)).swap(*this);
        return *this;
      }
#endif // BOOST_NO_RVALUE_REFERENCES
    };

    template<typename MonitorPtr>
    void swap(monitor_unique_lock<MonitorPtr> &lockA, monitor_unique_lock<MonitorPtr> &lockB)
    {
      lockA.swap(lockB);
    }

    template<typename MonitorPtr>
    class monitor_shared_lock:
      public detail::lock_wrapper
      <
        MonitorPtr,
        boost::shared_lock<typename MonitorPtr::mutex_type>
      >
    {
      typedef detail::lock_wrapper
      <
        MonitorPtr,
        boost::shared_lock<typename MonitorPtr::mutex_type>
      > base_class;
    public:
      monitor_shared_lock()
      {}
      explicit monitor_shared_lock(const MonitorPtr &mon): base_class(mon)
      {}
      template<typename U>
      monitor_shared_lock(const MonitorPtr &mon, const U &arg):
        base_class(mon, arg)
      {}
      // move constructors
#ifdef BOOST_NO_RVALUE_REFERENCES
      monitor_shared_lock(boost::detail::thread_move_t<monitor_shared_lock> other):
        base_class(other)
      {}
      monitor_shared_lock(boost::detail::thread_move_t<monitor_upgrade_lock<MonitorPtr> > other):
        base_class(other)
      {}
      monitor_shared_lock(boost::detail::thread_move_t<monitor_unique_lock<MonitorPtr> > other):
        base_class(other)
      {}
#else // BOOST_NO_RVALUE_REFERENCES
      monitor_shared_lock(monitor_shared_lock && other):
        base_class(std::move(other))
      {}
      monitor_shared_lock(monitor_upgrade_lock<MonitorPtr> && other):
        base_class(std::move(other))
      {}
      monitor_shared_lock(monitor_unique_lock<MonitorPtr> && other):
        base_class(std::move(other))
      {}
#endif // BOOST_NO_RVALUE_REFERENCES

      // monitor extensions to lock interface, only allow pointer to const access for shared_lock
      typename base_class::const_wrapped_pointer_type operator->() const
      {
        if(this->owns_lock() == false)
        {
          throw boost::lock_error();
        }
        typename pointer_traits<MonitorPtr>::pointer p = this->_mon.get();
        detail::assert_plain_old_pointer_not_null(p);
        return p;
      }
      typename pointer_traits<typename base_class::const_wrapped_pointer_type>::reference operator*() const
      {
        if(this->owns_lock() == false)
        {
          throw boost::lock_error();
        }
        typename pointer_traits<MonitorPtr>::pointer p = this->_mon.get();
        detail::assert_plain_old_pointer_not_null(p);
        return *p;
      }

      // move emulation
#ifdef BOOST_NO_RVALUE_REFERENCES
      boost::detail::thread_move_t<monitor_shared_lock> move()
      {
        return boost::detail::thread_move_t<monitor_shared_lock>(*this);
      }
      template<typename Lock>
      monitor_shared_lock& operator=(boost::detail::thread_move_t<Lock> other)
      {
        monitor_shared_lock(other).swap(*this);
        return *this;
      }
      operator boost::detail::thread_move_t<monitor_shared_lock>()
      {
        return move();
      }
#else // BOOST_NO_RVALUE_REFERENCES
      monitor_shared_lock && move()
      {
        return std::move(*this);
      }
      template<typename Lock>
      monitor_shared_lock& operator=(Lock && other)
      {
        monitor_shared_lock(std::move(other)).swap(*this);
        return *this;
      }
#endif // BOOST_NO_RVALUE_REFERENCES
    };

    template<typename MonitorPtr>
    void swap(monitor_shared_lock<MonitorPtr> &lockA, monitor_shared_lock<MonitorPtr> &lockB)
    {
      lockA.swap(lockB);
    }

    template<typename MonitorPtr>
    class monitor_upgrade_lock: public detail::lock_wrapper<MonitorPtr,
      boost::upgrade_lock<typename MonitorPtr::mutex_type> >
    {
      typedef detail::lock_wrapper
      <
        MonitorPtr,
        boost::upgrade_lock<typename MonitorPtr::mutex_type>
      > base_class;
    public:
      monitor_upgrade_lock()
      {}
      explicit monitor_upgrade_lock(const MonitorPtr &mon): base_class(mon)
      {}
      template<typename U>
      monitor_upgrade_lock(const MonitorPtr &mon, const U &arg):
        base_class(mon, arg)
      {}
      // move constructors
#ifdef BOOST_NO_RVALUE_REFERENCES
      monitor_upgrade_lock(boost::detail::thread_move_t<monitor_upgrade_lock> other):
        base_class(other)
      {}
      monitor_upgrade_lock(boost::detail::thread_move_t<monitor_unique_lock<MonitorPtr> > other):
        base_class(other)
      {}
#else // BOOST_NO_RVALUE_REFERENCES
      monitor_upgrade_lock(monitor_upgrade_lock && other):
        base_class(std::move(other))
      {}
      monitor_upgrade_lock(monitor_unique_lock<MonitorPtr> && other):
        base_class(std::move(other))
      {}
#endif // BOOST_NO_RVALUE_REFERENCES

      // monitor extensions to lock interface, only allow pointer to const access for upgrade_lock
      typename base_class::const_wrapped_pointer_type operator->() const
      {
        if(this->owns_lock() == false)
        {
          throw boost::lock_error();
        }
        typename pointer_traits<MonitorPtr>::pointer p = this->_mon.get();
        detail::assert_plain_old_pointer_not_null(p);
        return p;
      }
      const typename pointer_traits<typename base_class::const_wrapped_pointer_type>::reference operator*() const
      {
        if(this->owns_lock() == false)
        {
          throw boost::lock_error();
        }
        typename pointer_traits<MonitorPtr>::pointer p = this->_mon.get();
        detail::assert_plain_old_pointer_not_null(p);
        return *p;
      }

#ifdef BOOST_NO_RVALUE_REFERENCES
      // move emulation
      boost::detail::thread_move_t<monitor_upgrade_lock> move()
      {
        return boost::detail::thread_move_t<monitor_upgrade_lock>(*this);
      }
      template<typename Lock>
      monitor_upgrade_lock& operator=(boost::detail::thread_move_t<Lock> other)
      {
        monitor_upgrade_lock(other).swap(*this);
        return *this;
      }
      operator boost::detail::thread_move_t<monitor_upgrade_lock>()
      {
        return move();
      }
#else // BOOST_NO_RVALUE_REFERENCES
      monitor_upgrade_lock && move()
      {
        return std::move(*this);
      }
      template<typename Lock>
      monitor_upgrade_lock& operator=(Lock && other)
      {
        monitor_upgrade_lock(std::move(other)).swap(*this);
        return *this;
      }
#endif // BOOST_NO_RVALUE_REFERENCES
    };

    template<typename MonitorPtr>
    void swap(monitor_upgrade_lock<MonitorPtr> &lockA, monitor_upgrade_lock<MonitorPtr> &lockB)
    {
      lockA.swap(lockB);
    }

    template<typename MonitorPtr>
    class monitor_upgrade_to_unique_lock
    {
      typedef boost::upgrade_to_unique_lock<typename MonitorPtr::mutex_type> wrapped_lock_type;
    public:
      typedef MonitorPtr monitor_pointer_type;

      explicit monitor_upgrade_to_unique_lock(monitor_upgrade_lock<MonitorPtr> &upgrade_lock):
        _mon(upgrade_lock._mon),
        _lock(upgrade_lock._lock)
      {}
      // move constructor
#ifdef BOOST_NO_RVALUE_REFERENCES
      monitor_upgrade_to_unique_lock(boost::detail::thread_move_t<monitor_upgrade_to_unique_lock> other):
        _mon(other->_mon), _lock(boost::detail::thread_move_t<wrapped_lock_type>(other->_lock))
      {}
#else // BOOST_NO_RVALUE_REFERENCES
      monitor_upgrade_to_unique_lock(monitor_upgrade_to_unique_lock && other):
        _mon(std::move(other._mon)), _lock(std::move(other._lock))
      {}
#endif // BOOST_NO_RVALUE_REFERENCES

      void swap(monitor_upgrade_to_unique_lock &other)
      {
        boost::swap(_mon, other._mon);
        boost::swap(_lock, other._lock);
      }
      bool operator!()
      {
        return !_lock;
      }
      // safe bool idiom, somewhat safer than providing conversion to bool operator
      typedef wrapped_lock_type * wrapped_lock_type::* unspecified_bool_type;
      operator unspecified_bool_type() const
      {
        return !_lock ? 0 : &_lock;
      }
      bool owns_lock() const
      {
        return _lock.owns_lock();
      }

      // monitor extensions to lock interface
      typename MonitorPtr::pointer operator->() const
      {
        if(this->owns_lock() == false)
        {
          throw boost::lock_error();
        }
        typename pointer_traits<MonitorPtr>::pointer p = _mon.get();
        detail::assert_plain_old_pointer_not_null(p);
        return p;
      }
      typename MonitorPtr::reference operator*() const
      {
        if(this->owns_lock() == false)
        {
          throw boost::lock_error();
        }
        typename pointer_traits<MonitorPtr>::pointer p = _mon.get();
        detail::assert_plain_old_pointer_not_null(p);
        return *p;
      }

      // move emulation
#ifdef BOOST_NO_RVALUE_REFERENCES
      boost::detail::thread_move_t<monitor_upgrade_to_unique_lock> move()
      {
        return boost::detail::thread_move_t<monitor_upgrade_to_unique_lock>(*this);
      }
      monitor_upgrade_to_unique_lock& operator=(boost::detail::thread_move_t<monitor_upgrade_to_unique_lock> other)
      {
        monitor_upgrade_to_unique_lock(other).swap(*this);
        return *this;
      }
      operator boost::detail::thread_move_t<monitor_upgrade_to_unique_lock>()
      {
        return move();
      }
#else // BOOST_NO_RVALUE_REFERENCES
      monitor_upgrade_to_unique_lock && move()
      {
        return std::move(*this);
      }
      monitor_upgrade_to_unique_lock& operator=(monitor_upgrade_to_unique_lock && other)
      {
        monitor_upgrade_to_unique_lock(std::move(other)).swap(*this);
        return *this;
      }
#endif // BOOST_NO_RVALUE_REFERENCES
    private:
      monitor_pointer_type _mon;
      wrapped_lock_type _lock;
    };

    template<typename MonitorPtr>
    void swap(monitor_upgrade_to_unique_lock<MonitorPtr> &lockA, monitor_upgrade_to_unique_lock<MonitorPtr> &lockB)
    {
      lockA.swap(lockB);
    }

  };

  template<typename MonitorPtr>
#ifdef BOOST_NO_RVALUE_REFERENCES
  boost::detail::thread_move_t<generic_ptr::monitor_unique_lock<MonitorPtr> >
#else
  generic_ptr::monitor_unique_lock<MonitorPtr> &&
#endif
    move(generic_ptr::monitor_unique_lock<MonitorPtr> &lock)
  {
    return lock.move();
  }

  template<typename MonitorPtr>
#ifdef BOOST_NO_RVALUE_REFERENCES
  boost::detail::thread_move_t<generic_ptr::monitor_shared_lock<MonitorPtr> >
#else
  generic_ptr::monitor_shared_lock<MonitorPtr> &&
#endif
    move(generic_ptr::monitor_shared_lock<MonitorPtr> &lock)
  {
    return lock.move();
  }

  template<typename MonitorPtr>
#ifdef BOOST_NO_RVALUE_REFERENCES
  boost::detail::thread_move_t<generic_ptr::monitor_upgrade_lock<MonitorPtr> >
#else // BOOST_NO_RVALUE_REFERENCES
  generic_ptr::monitor_upgrade_lock<MonitorPtr> &&
#endif // BOOST_NO_RVALUE_REFERENCES
    move(generic_ptr::monitor_upgrade_lock<MonitorPtr> &lock)
  {
    return lock.move();
  }

  template<typename MonitorPtr>
#ifdef BOOST_NO_RVALUE_REFERENCES
  boost::detail::thread_move_t<generic_ptr::monitor_upgrade_to_unique_lock<MonitorPtr> >
#else // BOOST_NO_RVALUE_REFERENCES
  generic_ptr::monitor_upgrade_to_unique_lock<MonitorPtr> &&
#endif // BOOST_NO_RVALUE_REFERENCES
    move(generic_ptr::monitor_upgrade_to_unique_lock<MonitorPtr> &lock)
  {
    return lock.move();
  }
} // namespace boost

#endif // BOOST_GENERIC_PTR_MONITOR_LOCKS_HPP_INCLUDED
