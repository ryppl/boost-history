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
 * @file cursor_helpers.hpp
 * Cursor facade and adaptor implementations
 */

// TODO: Use TR1 type_traits (integral_constant)
// can we abstract the cursor stuff any further, 
// eventually producing cursor_adaptor?

#ifndef BOOST_TREE_CURSOR_HELPERS_HPP
#define BOOST_TREE_CURSOR_HELPERS_HPP

#include <boost/tree/detail/node/nary.hpp>

#include <boost/type_traits/integral_constant.hpp>

#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/iterator/iterator_facade.hpp>


#include <iterator>
#include <utility>

namespace boost {
namespace tree {

using boost::iterator_core_access;

class access_rotate {
 public:
      template <class Cursor>
      static void rotate(Cursor& c)
      {
          c.rotate();
      }
};

class cursor_core_access {
 public:
  	template <class Facade>
	static bool empty_(Facade const& f)
	{
		return f.empty_();
	}
	
 	template <class Facade>
	static typename Facade::size_type size_(Facade const& f)
	{
		return f.size_();
	}
	
	template <class Facade>
	static typename Facade::size_type max_size_(Facade const& f)
	{
		return f.max_size_();
	}
	
	template <class Facade>
	static Facade left(Facade const& f)
	{
		return f.left();
	}
	
	template <class Facade>
	static Facade right(Facade const& f)
	{
		return f.right();
	}
	
	template <class Facade>
	static Facade up(Facade const& f)
	{
		return f.up();
	}
};

template <class Hor, class Vert, class T, class Dist = std::ptrdiff_t,
		  class Size = std::size_t, class Ptr = T*, class Ref = T&>
class cursor {
	typedef Hor		horizontal_iterator_traversal;
	typedef Vert		vertical_iterator_traversal;
	
	typedef Size		size_type;
	
	typedef Hor		iterator_category;
	typedef T		value_type;
	typedef Dist		difference_type;
	typedef Ptr		pointer;
	typedef Ref		reference;
};

template <
    class Derived             // The derived cursor type being constructed
  , class Value
  , class HorizontalCategoryOrTraversal
  , class VerticalCategoryOrTraversal
  , class Reference   = Value&
  , class Difference  = std::ptrdiff_t
  , class Size		  = std::size_t
>
class cursor_facade 
 : public iterator_facade<Derived, Value, HorizontalCategoryOrTraversal, 
						  Reference, Difference> {
 private:
 	//typedef Derive<Value> Derived;
	//
	// Curiously Recurring Template interface.
	//
	Derived& derived()
	{
	    return *static_cast<Derived*>(this);
	}
	
	Derived const& derived() const
	{
	    return *static_cast<Derived const*>(this);
	}

//	typedef iterator_facade<Derived, Value, HorizontalCategoryOrTraversal, 
//							Reference, Difference> iterator_facade_;
	typedef typename cursor_facade::iterator_facade_ iterator_facade_;
 protected:
 	// For use by derived classes
	typedef cursor_facade<Derived, Value, HorizontalCategoryOrTraversal,
						  VerticalCategoryOrTraversal, Reference, Difference> 
			cursor_facade_;
 public:

	typedef typename iterator_facade_::value_type value_type;
	typedef Reference reference;
	typedef Difference difference_type;
	typedef typename iterator_facade_::pointer pointer;
	typedef typename iterator_facade_::iterator_category iterator_category;

	typedef Size size_type;

	typedef bidirectional_traversal_tag vertical_traversal_type;

//	typedef typename Derived::template rebind<value_type>::other cursor;
//	typedef typename Derived::template rebind<add_const<value_type> >::other
//			const_cursor;

	bool const empty() const
	{
		return cursor_core_access::empty_(this->derived());
	}
	
	size_type const size() const
	{
		return cursor_core_access::size_(this->derived());
	}
	
	size_type const max_size() const
	{
		return cursor_core_access::max_size_(this->derived());
	}
		 
 	// if Value is const: Derived == const_cursor: only Derived
 	// otherwise: also Derived const. Really? implicit conversion to const_cursor?
 	Derived begin()
 	{
		return cursor_core_access::left(this->derived()); 		
 	}

 	Derived end()
 	{
		return cursor_core_access::right(this->derived()); 		
 	}
 	
 	Derived parent()
 	{
		return cursor_core_access::up(this->derived()); 		
 	}
 	//if Value isn't const: also cursor.
};

template <
    class Derived
  , class Base
  , class Value               = use_default
  , class HorizontalTraversal = use_default
  , class VerticalTraversal	  = use_default
  , class Reference           = use_default
  , class Difference          = use_default
  , class Size                = use_default
>
class cursor_adaptor 
  : public iterator_adaptor<Derived, Base, Value, HorizontalTraversal, Reference,
  							Difference>
{
	friend class iterator_core_access;
	friend class cursor_core_access;
	typedef iterator_adaptor<Derived, Base, Value, HorizontalTraversal, Reference,
  							Difference> iterator_adaptor_;
 public:
    cursor_adaptor() : iterator_adaptor_()
    { }
    
    explicit cursor_adaptor(Base const& iter) : iterator_adaptor_(iter)
    { }
    
    typedef HorizontalTraversal horizontal_traversal;
    typedef HorizontalTraversal vertical_traversal;
    
    typedef Size size_type;
    typedef Base base_type;
    
    
 protected:
    typedef cursor_adaptor cursor_adaptor_;

 public:
 	bool const empty_() const
	{
		return iterator_adaptor_::base().empty();
	}
	
	size_type const size_() const
	{
		return iterator_adaptor_::base().size();
	}
	
	size_type const max_size_() const
	{
		return iterator_adaptor_::base().max_size();
	}
	
	Derived begin()
	{
		return Derived(this->base_reference().begin());
	}
		
	Derived up()
	{
		return Derived(this->base_reference().parent());
	}
};


} // namespace tree
} // namespace boost

#endif // BOOST_TREE_CURSOR_HELPERS_HPP
