//  Copyright (c) 2006-2009, Bernhard Reiter
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

namespace boost {
namespace tree {

using std::input_iterator_tag;
using std::output_iterator_tag;
using std::forward_iterator_tag;
using std::bidirectional_iterator_tag;
using std::random_access_iterator_tag;

struct descending_vertical_traversal_tag {};
struct ascending_vertical_traversal_tag {};

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

template <class T>
struct cursor_size<T*> {
    typedef std::size_t type;
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
    typedef Cat     cursor_category;
    typedef T       value_type;
    typedef Dist    difference_type;
    typedef Size    size_type;
    typedef Ptr     pointer;
    typedef Ref     reference;    
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

//define freestanding begin, end, size, empty using cursor's member fns?

} // namespace tree
} // namespace boost


#endif // BOOST_TREE_CURSOR_HPP
