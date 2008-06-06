//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file cursor.hpp
 * Cursor traits structs, traversal tags, ...
 */
 
#ifndef BOOST_TREE_CURSOR_HPP
#define BOOST_TREE_CURSOR_HPP

#include <stddef.h>
#include <iterator>

using std::input_iterator_tag;
using std::output_iterator_tag;
using std::forward_iterator_tag;
using std::bidirectional_iterator_tag;
using std::random_access_iterator_tag;

namespace boost {
namespace tree {


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
struct cursor_category {
	typedef typename Cursor::cursor_category type;
};

template <class Cursor>
struct cursor_horizontal_traversal {
	typedef typename Cursor::horizontal_traversal type;
};

template <class Cursor>
struct cursor_vertical_traversal {
	typedef typename Cursor::vertical_traversal type;
};

template <class Cat, class T, class Dist = ptrdiff_t, class Size = size_t,
		  class Ptr = T*, class Ref = T&>
struct cursor {
	typedef Cat		cursor_category;
	typedef T		value_type;
	typedef Dist	difference_type;
	typedef Size	size_type;
	typedef Ptr		pointer;
	typedef Ref		reference;	
};

// Deprecate?
struct cursor_tag {};

struct descending_tag {}; 
struct descending_cursor_tag
	: public cursor_tag, public descending_tag, 
	  public input_iterator_tag, public output_iterator_tag {};
struct descending_forward_cursor_tag
	: public cursor_tag, public descending_tag, public forward_iterator_tag {};
struct descending_bidirectional_cursor_tag
	: public cursor_tag, public descending_tag, public bidirectional_iterator_tag {};
struct descending_random_access_cursor_tag
	: public cursor_tag, public descending_tag, public random_access_iterator_tag {};

struct ascending_tag {};
struct ascending_cursor_tag
	: public descending_cursor_tag, public ascending_tag {};
struct ascending_forward_cursor_tag
	: public descending_forward_cursor_tag, public ascending_tag {};
struct ascending_bidirectional_cursor_tag
	: public descending_bidirectional_cursor_tag, public ascending_tag {};
struct ascending_random_access_cursor_tag
	: public descending_random_access_cursor_tag, public ascending_tag {};

/*
template <class Hor, class Vert>
struct produce_cursor_category;

template <>
struct produce_cursor_category<> {
	typedef descending_cursor_tag type;
};
*/

/**
 * @brief Output cursor wrapper around an output iterator.
 * 
 * This can be very useful e.g. to have cursor algorithms actually work on
 * iterators, thus permitting some kind of linearization of a given subtree.
 * (Modelled after std::insert_iterator and the like.)
 * 
 * For construction, the outputter_cursor_iterator_wrapper might come in useful
 * in saving keystrokes.
 */
// TODO: Complete this.
template<class OutputIterator>
class output_cursor_iterator_wrapper {
protected:
	OutputIterator* iter;
public:
	/// Make the iterator type publicly accessible.
	typedef OutputIterator iterator;
	
	/**
	 * For construction, we obviously need an Output Iterator to work on (i.e., write to).
	 */
	explicit output_cursor_iterator_wrapper(OutputIterator& i) : iter(&i) {}

	/** 
	 * @param value A const& value of the value_type of container that iter is
	 *              associated with.
	 * @return      This cursor, for chained operations.
	 * Assigning a value to this cursor will insert it before iter, the iterator it is
	 * wrapped around.
	 * 
	 * Unfortunately, Output Iterators do not necessarily expose their
	 * value_type (they might just give back void), so the following assignment operator
	 * has to be a template.
	 */
	// TODO: Consult C++0x if this has been changed
	template <class ValueType>
	output_cursor_iterator_wrapper& operator=(ValueType const& value)
	{ 
		*((*iter)++) = value;
		return *this; 
	}

	/// Returns *this.
	output_cursor_iterator_wrapper& operator*() { return *this; }

	/// Returns *this, as this %cursor doesn't "move".
	output_cursor_iterator_wrapper& operator++() { return *this; }

	/// Returns *this, as this %cursor doesn't "move".
	output_cursor_iterator_wrapper operator++(int) { return *this; }

	/// Returns *this, as this %cursor doesn't "move".
	output_cursor_iterator_wrapper& begin() { return *this; }
};

/** 
 * @param o	An output iterator.
 * @result	An instance of output_cursor_iterator_wrapper working on o.
 * 
 * Use as shortcut for cumbersome typenames, just as with std::inserter and the like.
 */
template<class OutputIterator>
inline output_cursor_iterator_wrapper<OutputIterator>
outputter_cursor_iterator_wrapper(OutputIterator o)
{
	return output_cursor_iterator_wrapper<OutputIterator>(o);
}

//define freestanding begin, end, size, empty using node's member fns?

} // namespace tree
} // namespace boost


#endif // BOOST_TREE_CURSOR_HPP
