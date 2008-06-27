//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file cursor_adaptor.hpp
 * Cursor adaptor implementation
 */

// TODO: Use TR1 type_traits (integral_constant)

#ifndef BOOST_TREE_CURSOR_ADAPTOR_HPP
#define BOOST_TREE_CURSOR_ADAPTOR_HPP

#include <boost/tree/cursor.hpp>

#include <boost/tree/cursor_facade.hpp>

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost {
namespace tree {

using boost::iterator_core_access;

// We'll abuse iterator_adaptor to determine our types.
template <
    class Derived
  , class Base
  , class Value               			= use_default
  , class HorizontalTraversalOrCategory = use_default
  , class VerticalTraversalOrCategory	= use_default
  , class Reference           			= use_default
  , class Difference          			= use_default
  , class Size                			= use_default
>
class eval_use_default
: private iterator_adaptor<Derived 
  						, Base
  						, Value
						, HorizontalTraversalOrCategory
						, Reference
						, Difference>
, private iterator_adaptor<Derived 
  						, Base
  						, Value
						, VerticalTraversalOrCategory
						, Reference
						, Difference>
{
private:
	eval_use_default() {} // This class is used for typedefs only.
	
	typedef typename iterator_adaptor<Derived, Base, Value
								, HorizontalTraversalOrCategory, Reference
								, Difference>::super_t h_super_t;
	typedef typename iterator_adaptor<Derived, Base, Value
								, VerticalTraversalOrCategory, Reference
								, Difference>::super_t v_super_t;
public:
	typedef typename h_super_t::value_type value_type;	
	typedef typename h_super_t::reference reference;
	typedef typename h_super_t::difference_type difference_type;
	typedef typename h_super_t::pointer pointer;
	typedef typename h_super_t::iterator_category iterator_category;
	typedef typename v_super_t::iterator_category vertical_traversal;
	
	// size_type isn't in iterator_adaptor or _facade, 
	// so we have to calculate it on our own: 
	typedef typename mpl::eval_if<
		is_same<Size, use_default>
	  , boost::tree::cursor_size<Base>
	  , mpl::identity<Size>
	>::type size_type;
};

template <
    class Derived
  , class Base
  , class Value               			= use_default
  , class HorizontalTraversalOrCategory = use_default
  , class VerticalTraversalOrCategory	= use_default
  , class Reference           			= use_default
  , class Difference          			= use_default
  , class Size                			= use_default
>
class cursor_adaptor 
: public cursor_facade<
	Derived 
  , typename eval_use_default<Derived
  							, Base
  							, Value
							, HorizontalTraversalOrCategory
							, VerticalTraversalOrCategory
							, Reference
							, Difference
							, Size>::value_type
  , typename eval_use_default<Derived
  							, Base
  							, Value
							, HorizontalTraversalOrCategory
							, VerticalTraversalOrCategory
							, Reference
							, Difference
							, Size>::iterator_category
  , typename eval_use_default<Derived
  							, Base
  							, Value
							, HorizontalTraversalOrCategory
							, VerticalTraversalOrCategory
							, Reference
							, Difference
							, Size>::vertical_traversal
  , typename eval_use_default<Derived
  							, Base
  							, Value
							, HorizontalTraversalOrCategory
							, VerticalTraversalOrCategory
							, Reference
							, Difference
							, Size>::reference
  , typename eval_use_default<Derived
  							, Base
  							, Value
							, HorizontalTraversalOrCategory
							, VerticalTraversalOrCategory
							, Reference
							, Difference
							, Size>::difference_type
  , typename eval_use_default<Derived
  							, Base
  							, Value
							, HorizontalTraversalOrCategory
							, VerticalTraversalOrCategory
							, Reference
							, Difference
							, Size>::size_type>
{
	friend class iterator_core_access;
	friend class cursor_core_access;
	typedef cursor_facade<Derived
						, Value
						, HorizontalTraversalOrCategory
						, VerticalTraversalOrCategory
						, Reference
						, Difference
						, Size> cursor_facade_;

 public:
    cursor_adaptor() {}
    
    explicit cursor_adaptor(Base const& cur) : m_cursor(cur)
    { }
    
    Base const& base() const
    { return m_cursor; }
    
	typedef typename cursor_facade_::iterator_category iterator_category;

    typedef typename cursor_facade_::horizontal_traversal horizontal_traversal;
    typedef typename cursor_facade_::vertical_traversal cursor_category;
    
    typedef Size size_type;
    typedef Base base_type;
     
 protected:
    typedef cursor_adaptor<Derived, Base, Value, HorizontalTraversalOrCategory,
    					   VerticalTraversalOrCategory, Reference, Difference,
    					   Size> cursor_adaptor_;
	
	typedef eval_use_default<Derived
							, Base
							, Value
							, HorizontalTraversalOrCategory
							, VerticalTraversalOrCategory
							, Reference
							, Difference
							, Size> super_t;
	
	Base const& base_reference() const
	{ return m_cursor; }
	
	Base& base_reference()
	{ return m_cursor; }
	
 public:
 
 	typename super_t::reference dereference() const
 	{
 		return *m_cursor;
 	}
 	
 	void increment()
 	{
 		++m_cursor;
 	}
 	
 	void decrement()
 	{
 		--m_cursor;
 	}
 	
 	template <class OtherDerived, class OtherCursor, class V, class C, 
 			  class R, class D, class S>
 	bool equal(cursor_adaptor<OtherDerived, OtherCursor, 
 							  V, C, R, D, S> const& x) const
 	{
 		return m_cursor == x.base();
 	}
 	
 	template <class OtherDerived, class OtherCursor, class V, class C, 
 			  class R, class D, class S>
 	Difference distance_to(cursor_adaptor<OtherDerived, OtherCursor, 
 							  			  V, C, R, D, S> const& x) const
	{
		return x.base() - m_cursor;
	}
 	
 	bool const empty_() const
	{
		return m_cursor.empty();
	}
	
	size_type const size_() const
	{
		return m_cursor.size();
	}
	
	size_type const max_size_() const
	{
		return m_cursor.max_size();
	}

	size_type const par() const
	{
		return m_cursor.parity();
	}

	void left()
	{
		m_cursor.to_begin();
	}

	void right()
	{
		m_cursor.to_end();
	}
	
	void up()
	{
		m_cursor.to_parent();
	}
	
private:
	Base m_cursor;
};


} // namespace tree
} // namespace boost

#endif // BOOST_TREE_CURSOR_ADAPTOR_HPP
