#ifndef BOOST_PARTITION_POINT_LAZY_TYPEDEF_HPP
#define BOOST_PARTITION_POINT_LAZY_TYPEDEF_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <boost/config.hpp>

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#error lazy_typedef.hpp requires partial template specialization!
#endif

namespace boost {
	namespace detail {
		template< typename T > struct exists { typedef void type; };
	}
}

// Defines extract_some_typedef<T> which exposes T::some_typedef as extract_some_typedef<T>::type
// if T::some_typedef exists. Otherwise extract_some_typedef<T> is empty.
#define BOOST_DECLARE_LAZY_TYPEDEF( some_typedef )                                                     \
	template< typename C, typename Enable=void >                                                       \
	struct lazy_typedef_ ## some_typedef                                                               \
	{};                                                                                                \
	template< typename C >                                                                             \
	struct lazy_typedef_ ## some_typedef< C                                                            \
		, BOOST_DEDUCED_TYPENAME boost::detail::exists< BOOST_DEDUCED_TYPENAME C::some_typedef >::type \
	> {                                                                                                \
		typedef BOOST_DEDUCED_TYPENAME C::some_typedef type;                                           \
	};

#endif // BOOST_PARTITION_POINT_LAZY_TYPEDEF_HPP