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
namespace apply_visitor {

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

} // namespace apply_visitor
} // namespace detail

template <typename Visitor, typename Visitable>
	typename detail::apply_visitor::visitor_traits<Visitor>::result_type
apply_visitor(Visitor visitor, Visitable& visitable)
{
    return detail::apply_visitor::visitor_traits<Visitor>::execute(visitor, visitable);
}

template <typename Visitor, typename Visitable>
    typename detail::apply_visitor::visitor_traits<Visitor>::result_type
apply_visitor(Visitor visitor, const Visitable& visitable)
{
    return detail::apply_visitor::visitor_traits<Visitor>::execute(visitor, visitable);
}

#else// defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

namespace detail {
namespace apply_visitor {

template <typename Visitor>
struct visitor_obj_traits
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
struct visitor_traits
{
private:
	typedef mpl::if_<
		  is_pointer<Visitor>
		, visitor_ptr_traits<Visitor>
		, visitor_obj_traits<Visitor>
		>::type traits;

public:
	typedef typename traits::result_type result_type;

	template <typename Visitable, typename IsVariant>
    static result_type execute(
		  Visitor visitor
		, Visitable& visitable
		, IsVariant is_variant
		)
    {
        return traits::execute(visitor, visitable, is_variant);
    }
};

} // namespace apply_visitor
} // namespace detail

template <typename Visitor, typename Visitable>
	typename detail::apply_visitor::visitor_traits<Visitor>::result_type
apply_visitor(Visitor visitor, Visitable& visitable)
{
	return detail::apply_visitor::visitor_traits<Visitor>::execute(
          visitor
        , visitable
        , mpl::bool_c< is_variant<Visitable>::value >()
        );
}

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION workaround

//////////////////////////////////////////////////////////////////////////
// function template apply_visitor(visitor, visitable1, visitable2)
//
// Visits visitable1 and visitable2 such that their values (which we
// shall call x and y, respectively) are used as arguments in the
// expression visitor(x, y).
//

namespace detail {
namespace apply_visitor {

template <typename Visitor, typename Value1>
class binary_delay0
{
public:
    typedef typename visitor_traits<Visitor>::result_type
        result_type;

private:
    Visitor& visitor_;
    Value1& value1_;

public:
    binary_delay0(Visitor& visitor, Value1& value1)
        : visitor_(visitor)
        , value1_(value1)
    {
    }

    template <typename Value0>
    result_type operator()(Value0& value0)
    {
        return visitor_(value0, value1_);
    }
};

template <typename Visitor, typename Visitable1>
class binary_delay1
{
public:
    typedef typename visitor_traits<Visitor>::result_type
        result_type;

private:
    Visitor& visitor_;
    Visitable1& visitable1_;

public:
    binary_delay1(Visitor& visitor, Visitable1& visitable1)
        : visitor_(visitor)
        , visitable1_(visitable1)
    {
    }

    template <typename Visitable2>
    result_type operator()(Visitable2& visitable2)
    {
        binary_delay0<
              Visitor
            , Visitable2
            > delayer(visitor_, visitable2);

        return boost::apply_visitor(delayer, visitable1_);
    }
};

} // namespace apply_visitor
} // namespace detail

template <typename Visitor, typename Visitable1, typename Visitable2>
    typename detail::apply_visitor::visitor_traits<Visitor>::result_type
apply_visitor(Visitor visitor, Visitable1& visitable1, Visitable2& visitable2)
{
	detail::apply_visitor::binary_delay1<
          Visitor
        , Visitable1
        > delayer(visitor, visitable1);

    return boost::apply_visitor(delayer, visitable2);
}

//////////////////////////////////////////////////////////////////////////
// function template apply_visitor(visitor)
//
// Returns a function object, overloaded for unary and binary usage, that
// visits its arguments using visitor (or a copy of visitor) via
//  * apply_visitor( visitor, [argument] )
// under unary invocation, or
//  * apply_visitor( visitor, [argument1], [argument2] )
// under binary invocation.
//

template <typename Visitor>
class apply_visitor_t
{
public:
	typedef typename detail::apply_visitor::visitor_traits<Visitor>::result_type
        result_type;

private:
    Visitor visitor_;

public:
    explicit apply_visitor_t(Visitor& visitor)
      : visitor_(visitor)
    {
    }

    template <typename Visitable>
    result_type operator()(Visitable& visitable)
    {
        apply_visitor(visitor_, visitable);
    }

	template <typename Visitable1, typename Visitable2>
    result_type operator()(Visitable1& visitable1, Visitable2& visitable2)
    {
        apply_visitor(visitor_, visitable1, visitable2);
    }
};

template <typename Visitor>
apply_visitor_t<Visitor> apply_visitor(Visitor visitor)
{
    return apply_visitor_t<Visitor>(visitor);
}

} // namespace boost

#endif // BOOST_APPLY_VISITOR_HPP
