/*
  Provides a basic subset of boost::unique_lock functionality.  Provided only because
  including boost/thread/locks.hpp requires linking to thread library
*/
// Copyright Frank Mori Hess 2008.
// Distributed under the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/signals2 for library home page.

#ifndef BOOST_GENERIC_PTR_UNIQUE_LOCK_HPP
#define BOOST_GENERIC_PTR_UNIQUE_LOCK_HPP

#include <boost/noncopyable.hpp>

namespace boost
{
  namespace generic_ptr
  {
    namespace detail
    {
      template<typename Mutex>
      class unique_lock: public noncopyable
      {
      public:
        unique_lock(Mutex &m): _mutex(m), _owns_lock(false)
        {
          _mutex.lock();
          _owns_lock = true;
        }
 #ifdef BOOST_NO_RVALUE_REFERENCES
        unique_lock(boost::detail::thread_move_t<unique_lock> other):
          _mutex(other->_mutex), _owns_lock(true)
        {
          other->_owns_lock = false;
        }
        boost::detail::thread_move_t<unique_lock> move()
        {
          return boost::detail::thread_move_t<unique_lock>(*this);
        }
#else // BOOST_NO_RVALUE_REFERENCES
        unique_lock(unique_lock && other):
          _mutex(other._mutex), _owns_lock(true)
        {
          other._owns_lock = false;
        }
        unique_lock && move()
        {
          return std::move(*this);
        }
#endif // BOOST_NO_RVALUE_REFERENCES
       ~unique_lock()
        {
          if(_owns_lock)
            _mutex.unlock();
        }
      private:
        Mutex &_mutex;
        bool _owns_lock;
      };
    } // namespace detail
  } // namespace generic_ptr
} // namespace boost

#endif  // BOOST_GENERIC_PTR_UNIQUE_LOCK_HPP

