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

#ifndef BOOST_TREE_SORTABLE_TRAITS_HPP
#define BOOST_TREE_SORTABLE_TRAITS_HPP


namespace boost {
namespace tree {

template <class Sortable>
struct sortable_traits
{
	typedef Sortable sortable_type;
	typedef typename sortable_type::iterator cursor;
	typedef typename sortable_type::value_type value_type;
 	typedef typename sortable_type::size_type size_type;
 	typedef sortable_type container_type; //remove const
 	//typedef sortable_type const_container_type; //remove const
};

//template <class Range>
//struct head_wrapper {
//	typedef Range sortable_type;
//	typename sortable_traits<Range>::container_type operator()(Range const& c) // TODO: const_ //use sortable_traits for return type?
//	{
//		return c;
//	}
//};

template <class Range>
bool has_child(Range)
{
	return true;
}


template <class Range>
bool has_child(typename Range::iterator)
{
	return false;
}


template <class Range>
bool has_child(typename Range::const_iterator)
{
	return false;
}

template <class Range>
typename sortable_traits<Range>::container_type head(Range const& c) // TODO: const_ //use sortable_traits for return type?
{
	return c;
}


} // namespace tree
} // namespace boost


#endif // BOOST_TREE_SORTABLE_TRAITS_HPP
