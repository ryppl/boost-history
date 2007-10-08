//  Copyright (c) 2006, Bernhard Reiter
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

template <class Cat, class T, class Dist = ptrdiff_t, class Size = size_t,
		  class Ptr = T*, class Ref = T&>
struct cursor {
	typedef Cat		cursor_category;
	typedef T		value_type;
	typedef Dist		difference_type;
	typedef Size		size_type;
	typedef Ptr		pointer;
	typedef Ref		reference;	
};

struct cursor_tag 
	: public input_iterator_tag, public output_iterator_tag {};
struct descending_cursor_tag
	: public cursor_tag {};
struct descending_forward_cursor_tag
	: public descending_cursor_tag, public forward_iterator_tag {};
struct descending_bidirectional_cursor_tag
	: public descending_cursor_tag, public bidirectional_iterator_tag {};
struct descending_random_access_cursor_tag
	: public descending_cursor_tag, public random_access_iterator_tag {};
struct ascending_cursor_tag
	: public descending_cursor_tag {};
struct ascending_forward_cursor_tag
	: public descending_forward_cursor_tag {};
struct ascending_bidirectional_cursor_tag
	: public descending_bidirectional_cursor_tag {};
struct ascending_random_access_cursor_tag
	: public descending_random_access_cursor_tag {};

// TODO: Complete this.
template<class OutputIterator>
class output_cursor_iterator_wrapper {
protected:
	OutputIterator& iter;
public:
	explicit output_cursor_iterator_wrapper(OutputIterator& i) : iter(i) {}

	output_cursor_iterator_wrapper& operator=(output_cursor_iterator_wrapper const& x)
	{
		iter = x.iter;
		return *this;
	}

	// Unfortunately, Output Iterators do not necessarily expose their
	// value_type (they just give void), so the following member 
	// function has to be a template.
	// TODO: Consult C++0x if this has been changed
	template <class ValueType>
	output_cursor_iterator_wrapper& operator=(ValueType const& value)
	{ 
		*(iter++) = value;
		return *this; 
	}

	output_cursor_iterator_wrapper& operator*() { return *this; }
	output_cursor_iterator_wrapper& operator++() { return *this; }
	output_cursor_iterator_wrapper operator++(int) { return *this; }
	output_cursor_iterator_wrapper& begin() { return *this; }
};

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
