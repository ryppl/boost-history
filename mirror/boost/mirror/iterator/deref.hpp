/**
 * \file boost/mirror/iterator/deref.hpp
 *
 *  Dereferencing of an igterator
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ITERATOR_DEREF_HPP
#define BOOST_MIRROR_ITERATOR_DEREF_HPP


namespace boost {
namespace mirror {

/** Dereferences an iterator
 */
template <class Iterator>
struct deref
{
	typedef typename Iterator::template get_pointed_to<void>::type
		type;
};

} // namespace mirror
} // namespace boost

#endif //include guard

