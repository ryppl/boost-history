//-----------------------------------------------------------------------------
// boost apply_visitor.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_APPLY_VISITOR_HPP
#define BOOST_APPLY_VISITOR_HPP

// The following are new/in-progress headers or fixes to existing headers:
#include "boost/apply_visitor_fwd.hpp"
#include "boost/detail/variant_workaround.hpp"

namespace boost {

//////////////////////////////////////////////////////////////////////////
// function template visitor_ptr
//
// Adapts a function pointer for use as visitor capable of handling
// values of a single type.
//

template <typename T, typename R = void>
struct visitor_ptr_t
{
public:
    typedef void result_type;

private:
    typedef R (*visitor_t)(T);

    visitor_t visitor_;

public:
    visitor_ptr_t(visitor_t& visitor)
      : visitor_(visitor)
    {
    }

    void operator()(T operand) const
    {
        visitor_(operand);
    }

    template <typename U>
    void operator()(const U& operand) const
    {
    }
};

template <typename R, typename T>
visitor_ptr_t<T,R> visitor_ptr(R (*visitor)(T))
{
    return visitor_ptr_t<T,R>(visitor);
}

//////////////////////////////////////////////////////////////////////////
// function template apply_visitor(visitor, visitable)
//
// Visits visitable with visitor.
//

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

namespace detail {

template <typename Visitor>
struct visitor_traits
{
    typedef typename Visitor::result_type result_type;

    template <typename Visitable>
    static result_type execute(Visitor visitor, Visitable& visitable)
    {
        return apply_visitor_traits<Visitable>::execute(visitor, visitable);
    }

    template <typename Visitable>
    static result_type execute(Visitor visitor, const Visitable& visitable)
    {
        return apply_visitor_traits<Visitable>::execute(visitor, visitable);
    }
};

template <typename R, typename T>
struct visitor_traits< R (*)(T) >
{
private:
    typedef visitor_ptr_t<T,R> Visitor;

public:
    typedef typename Visitor::result_type result_type;

    template <typename Visitable>
    static result_type execute(Visitor visitor, Visitable& visitable)
    {
        return apply_visitor_traits<Visitable>::execute(visitor, visitable);
    }

    template <typename Visitable>
    static result_type execute(Visitor visitor, const Visitable& visitable)
    {
        return apply_visitor_traits<Visitable>::execute(visitor, visitable);
    }
};

} // namespace detail

template <typename Visitor, typename Visitable>
    typename detail::visitor_traits<Visitor>::result_type
apply_visitor(Visitor visitor, Visitable& visitable)
{
    return detail::visitor_traits<Visitor>::execute(visitor, visitable);
}

template <typename Visitor, typename Visitable>
    typename detail::visitor_traits<Visitor>::result_type
apply_visitor(Visitor visitor, const Visitable& visitable)
{
    return detail::visitor_traits<Visitor>::execute(visitor, visitable);
}

#else// defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

namespace detail {

template <typename Visitor>
struct visitor_traits
{
    typedef typename Visitor::result_type result_type;

    template <typename Visitable>
    static result_type execute(
		  Visitor visitor
		, Visitable& visitable
		, mpl::false_c// is_variant
		)
    {
        return apply_visitor_traits<Visitable>::execute(visitor, visitable);
    }

    template <typename Visitable>
	static result_type execute(
		  Visitor visitor
		, const Visitable& visitable
		, mpl::false_c// is_variant
		)
    {
        return apply_visitor_traits<Visitable>::execute(visitor, visitable);
    }

	template <typename Visitable>
	static result_type execute(
		  Visitor visitor
		, Visitable& visitable
		, mpl::true_c// is_variant
		)
    {
        return variant_apply_visitor(visitor, visitable);
    }
};

template <typename Visitor>
struct visitor_ptr_traits
{
	typedef void result_type;

	template <typename Visitable>
    static result_type execute(
		  Visitor visitor
		, Visitable& visitable
		, mpl::false_c// is_variant
		)
    {
        return apply_visitor_traits<Visitable>::execute(visitor_ptr(visitor), visitable);
    }

    template <typename Visitable>
	static result_type execute(
		  Visitor visitor
		, const Visitable& visitable
		, mpl::false_c// is_variant
		)
    {
        return apply_visitor_traits<Visitable>::execute(visitor_ptr(visitor), visitable);
    }

	template <typename Visitable>
	static result_type execute(
		  Visitor visitor
		, Visitable& visitable
		, mpl::true_c// is_variant
		)
    {
        return variant_apply_visitor(visitor_ptr(visitor), visitable);
    }
};

template <typename Visitor>
struct select_visitor_traits
{
	typedef mpl::if_<
		  is_pointer<Visitor>
		, visitor_ptr_traits<Visitor>
		, visitor_traits<Visitor>
		>::type type;

	// convienence typedef
	typedef typename type::result_type result_type;
};

} // namespace detail

template <typename Visitor, typename Visitable>
	typename detail::select_visitor_traits<Visitor>::result_type
apply_visitor(Visitor visitor, Visitable& visitable)
{
	typedef detail::select_visitor_traits<Visitor>::type traits;

	return traits::execute(
          visitor
        , visitable
        , mpl::bool_c< is_variant<Visitable>::value >()
        );
}

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION workaround

//////////////////////////////////////////////////////////////////////////
// function template apply_visitor(visitor)
//
// Returns an unary function object that visits its arguments using
// visitor (or a copy) via apply_visitor( visitor, [argument] ).
//

template <typename Visitor>
class apply_visitor_t
{
public:
    typedef typename Visitor::result_type
        result_type;

private:
    Visitor visitor_;

public:
    explicit apply_visitor_t(Visitor& visitor)
      : visitor_(visitor)
    {
    }

    template <typename Visitable>
    result_type operator()(Visitable& operand)
    {
        apply_visitor(visitor_, operand);
    }

    template <typename Visitable>
    result_type operator()(Visitable& operand) const
    {
        apply_visitor(visitor_, operand);
    }
};

template <typename Visitor>
apply_visitor_t<Visitor> apply_visitor(Visitor visitor)
{
    return apply_visitor_t<Visitor>(visitor);
}

} // namespace boost

#endif // BOOST_APPLY_VISITOR_HPP
