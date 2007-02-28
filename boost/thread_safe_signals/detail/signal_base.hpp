// Boost.Signals library

// Copyright Douglas Gregor 2001-2004. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#ifndef BOOST_TSS_SIGNALS_SIGNAL_BASE_HEADER
#define BOOST_TSS_SIGNALS_SIGNAL_BASE_HEADER

#include <boost/detail/lightweight_mutex.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
	namespace signalslib {
		namespace detail {
			class slot_base;

			class signal_impl_base
			{
			public:
				typedef boost::detail::lightweight_mutex mutex_type;

				signal_impl_base(): _mutex(new boost::detail::lightweight_mutex)
				{}
			protected:
				mutable shared_ptr<mutex_type> _mutex;
			};

			class signal_base : public noncopyable
			{
			public:
				friend class slot_base;

				virtual ~signal_base() {}
			protected:
				virtual shared_ptr<void> lock_pimpl() const = 0;
			};
		} // end namespace detail
	} // end namespace BOOST_SIGNALS_NAMESPACE
} // end namespace boost

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_TSS_SIGNALS_SIGNAL_BASE_HEADER
