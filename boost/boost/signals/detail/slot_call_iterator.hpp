// Boost.Signals library
//
// Copyright (C) 2001-2002 Doug Gregor (gregod@cs.rpi.edu)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.

// For more information, see http://www.boost.org

#ifndef BOOST_SIGNALS_SLOT_CALL_ITERATOR
#define BOOST_SIGNALS_SLOT_CALL_ITERATOR

#include <functional>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/signals/connection.hpp>

namespace boost {
  namespace BOOST_SIGNALS_NAMESPACE {
    namespace detail {
      // A cached return value from a slot
      template<typename T>
      struct cached_return_value {
        cached_return_value(const T& t) : value(t) {}

        T value;
      };

      // Generates a slot call iterator. Essentially, this is an iterator that:
      //   - skips over disconnected slots in the underlying list
      //   - calls the connected slots when dereferenced
      //   - caches the result of calling the slots
      template<typename Function, typename Iterator>
      class slot_call_iterator
        : public iterator_facade<slot_call_iterator<Function, Iterator>,
                                 typename Function::result_type,
                                 readable_lvalue_iterator_tag,
                                 single_pass_traversal_tag>
      {
        typedef iterator_facade<slot_call_iterator<Function, Iterator>,
                                typename Function::result_type,
                                readable_lvalue_iterator_tag,
                                single_pass_traversal_tag> inherited;

        typedef typename Function::result_type result_type;

        friend class iterator_core_access;

      public:
        slot_call_iterator() {}

        slot_call_iterator(Iterator iter_in, Iterator end_in, Function f)
          : iter(iter_in), end(end_in), f(f), cache()
        {
          iter = std::find_if(iter, end, std::not1(is_disconnected()));
        }

        typename slot_call_iterator::reference
        dereference() const
        {
          if (!cache.get()) {
            cache.reset(new cached_return_value<result_type>(f(*iter)));
          }

          return cache->value;
        }

        void increment()
        {
          iter = std::find_if(++iter, end, std::not1(is_disconnected()));
          cache.reset();
        }

        bool equal(const slot_call_iterator& other) const
        {
          iter = std::find_if(iter, end, std::not1(is_disconnected()));
          other.iter = std::find_if(other.iter, other.end,
                                    std::not1(is_disconnected()));
          return iter == other.iter;
        }

      private:
        mutable Iterator iter;
        Iterator end;
        Function f;
        mutable shared_ptr< cached_return_value<result_type> > cache;
      };
    } // end namespace detail
  } // end namespace BOOST_SIGNALS_NAMESPACE
} // end namespace boost
#endif // BOOST_SIGNALS_SLOT_CALL_ITERATOR
