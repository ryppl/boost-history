//-----------------------------------------------------------------------------
// boost visitor/unary_apply_visitor.hpp header file
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

#ifndef BOOST_VISITOR_UNARY_APPLY_VISITOR_HPP
#define BOOST_VISITOR_UNARY_APPLY_VISITOR_HPP

#include "boost/static_visitable.hpp"
#include "boost/visitor/dynamic_visitable_base.hpp"
#include "boost/visitor/dynamic_visitor_base.hpp"
#include "boost/visitor/dynamic_visitor_return_base.hpp"
#include "boost/visitor/dynamic_visitor_interface.hpp"
#include "boost/visitor/detail/wrap_dynamic_visitor.hpp"
#include "boost/visitor/detail/dynamic_return_error.hpp"

#include "boost/type_traits/is_base_and_derived.hpp"
#include "boost/mpl/bool_c.hpp"

namespace boost {

//////////////////////////////////////////////////////////////////////////
// function template apply_visitor(visitor, visitable)
//
// Visits visitable with visitor.
//

namespace detail { namespace visitor {

void apply_visitor_impl(
      dynamic_visitor_base& visitor
    , dynamic_visitable_base& visitable
    , mpl::true_c// dynamic_visitor
    , mpl::true_c// dynamic_visitable
    )
{
    visitable.apply_visitor(visitor);
}

template <typename R>
R apply_visitor_impl(
      dynamic_visitor_return_base<R>& visitor
    , dynamic_visitable_base& visitable
    , mpl::true_c// dynamic_visitor
    , mpl::true_c// dynamic_visitable
    )
{
    visitor.return_value.clear();

    visitable.apply_visitor(visitor);

    if (visitor.return_value.empty())
        throw detail::dynamic_return_error();

    return visitor.return_value.get();
}

template <typename DynamicVisitor, typename StaticVisitable>
    typename DynamicVisitor::result_type
apply_visitor_impl(
      DynamicVisitor& visitor
    , StaticVisitable& visitable
    , mpl::true_c// dynamic_visitor
    , mpl::false_c// dynamic_visitable
    )
{
    wrap_dynamic_visitor_t<DynamicVisitor> wrapped(visitor);

    return static_visitable_traits<StaticVisitable>
        ::apply_visitor(wrapped, visitable);
}

template <typename DynamicVisitor, typename StaticVisitable>
    typename DynamicVisitor::result_type
apply_visitor_impl(
      DynamicVisitor& visitor
    , const StaticVisitable& visitable
    , mpl::true_c// dynamic_visitor
    , mpl::false_c// dynamic_visitable
    )
{
    wrap_dynamic_visitor_t<DynamicVisitor> wrapped(visitor);

    return static_visitable_traits<StaticVisitable>
        ::apply_visitor(wrapped, visitable);
}

template <typename Visitor, typename Visitable>
    typename Visitor::result_type
apply_visitor_impl(
      Visitor& visitor
    , Visitable& visitable
    , mpl::false_c// dynamic_visitor
    , mpl::false_c// dynamic_visitable
    )
{
    return static_visitable_traits<Visitable>
        ::apply_visitor(visitor, visitable);
}

template <typename Visitor, typename Visitable>
    typename Visitor::result_type
apply_visitor_impl(
      Visitor& visitor
    , const Visitable& visitable
    , mpl::false_c// dynamic_visitor
    , mpl::false_c// dynamic_visitable
    )
{
    return static_visitable_traits<Visitable>
        ::apply_visitor(visitor, visitable);
}

template <typename Visitor, typename Visitable>
    typename Visitor::result_type
apply_visitor_impl(Visitor& visitor, Visitable& visitable)
{
    typedef mpl::bool_c<
          is_base_and_derived<
              dynamic_visitor_base
            , Visitor
            >::type::value
        > is_dynamic_visitor;

    typedef mpl::bool_c<
          is_base_and_derived<
              dynamic_visitable_base
            , Visitable
            >::type::value
        > is_dynamic_visitable;

    return detail::visitor::apply_visitor_impl(
          visitor
        , visitable
        , is_dynamic_visitor()
        , is_dynamic_visitable()
        );
}

}} // namespace detail::visitor

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

// [These two instances may take dynamic visitors:]

template <typename Visitor, typename Visitable>
    typename Visitor::result_type
apply_visitor(Visitor& visitor, Visitable& visitable)
{
    return detail::visitor::apply_visitor_impl(visitor, visitable);
}

template <typename Visitor, typename Visitable>
    typename Visitor::result_type
apply_visitor(Visitor& visitor, const Visitable& visitable)
{
    return detail::visitor::apply_visitor_impl(visitor, visitable);
}

// [These two take only static visitors, b/c dynamic is never const:]

template <typename Visitor, typename Visitable>
    typename Visitor::result_type
apply_visitor(const Visitor& visitor, Visitable& visitable)
{
    return static_visitable_traits<Visitable>
        ::apply_visitor(visitor, visitable);
}

template <typename Visitor, typename Visitable>
    typename Visitor::result_type
apply_visitor(const Visitor& visitor, const Visitable& visitable)
{
    return static_visitable_traits<Visitable>
        ::apply_visitor(visitor, visitable);
}

#else// defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

// TODO : Test following under MSVC 7

namespace detail { namespace visitor {

template <typename Visitor, typename Visitable>
    typename Visitor::result_type
apply_visitor_aux(
      Visitor& visitor
    , Visitable& visitable
    , mpl::false_c// is_variant
    )
{
    return apply_visitor_traits<Visitable>
        ::execute(visitor, visitable);
}

template <typename Visitor, typename Visitable>
    typename Visitor::result_type
apply_visitor_aux(
      Visitor& visitor
    , Visitable& visitable
    , mpl::true_c// is_variant
    )
{
    return variant_apply_visitor(visitor, visitable);
}

}} // namespace detail::visitor

template <typename Visitor, typename Visitable>
    typename Visitor::result_type
apply_visitor(Visitor& visitor, Visitable& visitable)
{
    return detail::visitor::apply_visitor_aux(
          visitor
        , visitable
        , mpl::bool_c< is_variant<Visitable>::value >()
        );
}

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION workaround

} // namespace boost

#endif // BOOST_VISITOR_UNARY_APPLY_VISITOR_HPP
