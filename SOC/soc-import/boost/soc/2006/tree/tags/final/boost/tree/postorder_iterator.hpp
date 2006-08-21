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

/**
 * @file postorder_iterator.hpp
 * Postorder iterator wrapper around cursor.
 */

// TODO: concept checks.

#ifndef BOOST_TREE_POSTORDER_ITERATOR_HPP
#define BOOST_TREE_POSTORDER_ITERATOR_HPP


#include <boost/tree/postorder.hpp>

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>


namespace boost {
namespace tree {
	
namespace postorder {

template <class Cursor>
class iterator
 : public boost::iterator_adaptor<iterator<Cursor>
      , Cursor
      , boost::use_default
      , boost::bidirectional_traversal_tag
    > {
 private:
    struct enabler {};

 public:
    iterator()
      : iterator::iterator_adaptor_() {}

    explicit iterator(Cursor p)
      : iterator::iterator_adaptor_(p) {}

    template <class OtherCursor>
    iterator(
        iterator<OtherCursor> const& other
      , typename boost::enable_if<
            boost::is_convertible<OtherCursor,Cursor >
          , enabler
        >::type = enabler()
    )
      : iterator::iterator_adaptor_(other.base()) {}

	operator Cursor()
	{
		return this->base();
	}
 private:
    friend class boost::iterator_core_access;
    
    void increment()
    {
		postorder::forward(this->base_reference());
    }
    
    void decrement()
    {
    		postorder::back(this->base_reference());
    }
};

} // namespace postorder

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_POSTORDER_ITERATOR_HPP
