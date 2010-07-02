// Boost.Signals library

// Copyright Timmo Stange 2007.
// Copyright Douglas Gregor 2001-2004. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#ifndef BOOST_SIGNALS_SIGNAL_BASE_HEADER
#define BOOST_SIGNALS_SIGNAL_BASE_HEADER

#include <boost/signals/detail/config.hpp>
#include <boost/signals/detail/signals_common.hpp>
#include <boost/signals/detail/signal_impl_base.hpp>
#ifndef BOOST_SIGNALS_NO_LEGACY_SUPPORT
#include <boost/signals/detail/legacy/signal_impl_base.hpp>
#endif
#include <boost/smart_ptr.hpp>
#include <boost/utility.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
  namespace BOOST_SIGNALS_NAMESPACE {
    namespace detail {
      // signal_impl class template.
      // Holds the combiner and derives from a signal_impl_base instantiation
      // which is providing the slot maintenance functionality.
      template<typename Combiner, 
        typename Group, 
        typename GroupCompare, 
        typename ThreadingModel, 
        typename Allocator>
      class signal_impl 
        : public signal_impl_base<Group, GroupCompare, ThreadingModel, Allocator>
      {
      public:
        typedef signal_impl_base<Group, GroupCompare, ThreadingModel, 
                                 Allocator> base_type;

        signal_impl(const Combiner& c, 
          const GroupCompare& comp)
          : base_type(comp), combiner_(c)
        { }

        // Combiner access.
        Combiner& combiner() 
        {
          return combiner_;
        }
        const Combiner& combiner() const 
        {
          return combiner_;
        }

      private:
        Combiner combiner_;
      };

      // signal_base_tag struct.
      // Used for simplified type identification internally.
      struct signal_base_tag { };

      // signal_base class template.
      // Implement common functionality for all signal types which is not
      // dependent on the signal call signature. Holds the shared_ptr to
      // the actual implementation.
      template<typename Combiner, 
        typename Group, 
        typename GroupCompare, 
        typename ThreadingModel, 
        typename Allocator>
      class BOOST_SIGNALS_DECL signal_base 
        : public signal_base_tag, 
        public noncopyable
      {
      protected:
        typedef signal_impl<Combiner, Group, GroupCompare, ThreadingModel, 
                            Allocator> impl_type;
        typedef typename impl_type::base_type impl_base_type;

      public:
        signal_base(const Combiner& combiner, const GroupCompare& comp)
          : impl_(new impl_type(combiner, comp))
        { }

        ~signal_base()
        {
          impl_->disconnect_all_slots();
        }

      public:
        // Disconnect all slots connected to this signal
        void disconnect_all_slots() {
          impl_->disconnect_all_slots(); 
        }

        // Are there any connected slots?
        bool empty() const { 
          return impl_->empty(); 
        }

        // How many slots are connected?
        std::size_t num_slots() const { 
          return impl_->num_slots(); 
        }

      protected:
        connection connect_slot(
          const shared_ptr<slot_signal_interface<impl_type> >& slot,
          connect_position at)
        {
          return impl_->connect_slot(slot, at);
        }

        typedef typename impl_type::slot_iterator iterator;

        shared_ptr<impl_type> impl_;
      };
    } // end namespace detail
  } // end namespace BOOST_SIGNALS_NAMESPACE
} // end namespace boost

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_SIGNALS_SIGNAL_BASE_HEADER