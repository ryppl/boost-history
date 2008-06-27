//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file cursor_helpers.hpp
 * Cursor facade and adaptor implementations
 */

// TODO: Use TR1 type_traits (integral_constant)

#ifndef BOOST_TREE_CURSOR_HELPERS_HPP
#define BOOST_TREE_CURSOR_HELPERS_HPP

#include <boost/tree/cursor.hpp>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/iterator_adaptor.hpp>

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

#include <iterator>
#include <utility>

namespace boost {
namespace tree {

using boost::iterator_core_access;

class cursor_core_access {
 public:
 
 	friend class iterator_core_access;
 	
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
	static typename Facade::size_type par(Facade const& f)
	{
		return f.par();
	}
		
	template <class Facade>
	static void left(Facade& f)
	{
		f.left();
	}
	
	template <class Facade>
	static void right(Facade& f)
	{
		f.right();
	}
	
	//only if ascending
	template <class Facade>
	static void up(Facade& f)
	{
		f.up();
	}

private:
	cursor_core_access();
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
	// Curiously Recurring Template interface.

	Derived& derived()
	{
	    return *static_cast<Derived*>(this);
	}
	
	Derived const& derived() const
	{
	    return *static_cast<Derived const*>(this);
	}

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

	typedef bidirectional_traversal_tag cursor_category; //TODO
	typedef typename 
		iterator_category_to_traversal<iterator_category>::type
		horizontal_traversal; 
	typedef typename 
		iterator_category_to_traversal<VerticalCategoryOrTraversal>::type
		vertical_traversal; 

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

	size_type const parity() const
	{
		return cursor_core_access::par(this->derived());
	}

 	Derived& to_begin()
 	{
		cursor_core_access::left(this->derived());
		return this->derived();
 	}
 				 
 	Derived begin()
 	{
 		Derived tmp(this->derived());
 		return tmp.to_begin();
 	}

 	Derived& to_end()
 	{
		cursor_core_access::right(this->derived());
		return this->derived();
 	}
 	
 	Derived end()
 	{
 		Derived tmp(this->derived());
 		return tmp.to_end();
  	}
 	
 	Derived& to_parent()
 	{
 		cursor_core_access::up(this->derived());
 		return this->derived();
 	}
 	
 	Derived parent()
 	{
 		Derived tmp(this->derived());
 		return tmp.to_parent();
 	}
};

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
	
	// size_type isn't in iterator_adaptor, 
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

//TODO: Put somewhere else?
template <class Value>
struct metadata {
	struct type {};
};

} // namespace tree
} // namespace boost

#endif // BOOST_TREE_CURSOR_HELPERS_HPP
