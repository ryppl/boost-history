// Copyright (c) 2006 Bernhard Reiter.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TREE_CURSOR_HPP
#define BOOST_TREE_CURSOR_HPP

/** 
 * @file cursor.hpp
 * Cursor traits structs, ...
 */

namespace boost {
namespace tree {

// Do we really need the following? We can use iterator and range traits, right?
// (esp. the second as it contains size_type)

template <class Cursor>
struct cursor_value {
	typedef typename Cursor::value_type type;
};

template <class Cursor>
struct cursor_reference {
	typedef typename Cursor::reference type;
};

template <class Cursor>
struct cursor_const_reference {
	typedef typename Cursor::const_reference type;
};

template <class Cursor>
struct cursor_pointer {
	typedef typename Cursor::pointer type;
};

template <class Cursor>
struct cursor_difference {
	typedef typename Cursor::difference_type type;
};

template <class Cursor>
struct cursor_size {
	typedef typename Cursor::size_type type;
};

template <class Cursor>
struct cursor_arity {
	typedef typename Cursor::arity type;
};

//define freestanding begin, end, size, empty using node's member fns?

} // namespace tree
} // namespace boost


#endif // BOOST_TREE_CURSOR_HPP
