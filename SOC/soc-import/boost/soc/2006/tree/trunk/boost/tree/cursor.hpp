//  Copyright (c) 2006, Bernhard Reiter
//
//  This code may be used under either of the following two licences:
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy 
//  of this software and associated documentation files (the "Software"), to deal 
//  in the Software without restriction, including without limitation the rights 
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
//  copies of the Software, and to permit persons to whom the Software is 
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in 
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
//  THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

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

template <class Cursor>
struct cursor_vertical_traversal {
	typedef typename Cursor::vertical_traversal_type type;
};

//define freestanding begin, end, size, empty using node's member fns?

} // namespace tree
} // namespace boost


#endif // BOOST_TREE_CURSOR_HPP
