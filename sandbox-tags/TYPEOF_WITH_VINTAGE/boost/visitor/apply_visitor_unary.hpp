//-----------------------------------------------------------------------------
// boost visitor/apply_visitor_unary.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002-2003
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_VISITOR_APPLY_VISITOR_UNARY_HPP
#define BOOST_VISITOR_APPLY_VISITOR_UNARY_HPP

#include "boost/visitor/static_visitable.hpp" // for is_static_visitable
#include "boost/visitor/dynamic_visitable_base.hpp"
#include "boost/visitor/dynamic_visitor_base.hpp"
#include "boost/visitor/dynamic_visitor_return_base.hpp"
#include "boost/visitor/detail/wrap_dynamic_visitor.hpp"
#include "boost/visitor/detail/dynamic_return_error.hpp"

#include "boost/preprocessor/define_forwarding_func.hpp"
#include "boost/type_traits/is_const.hpp"
#include "boost/mpl/bool.hpp"

namespace boost {

//////////////////////////////////////////////////////////////////////////
// function template apply_visitor(visitor, visitable)
//
// Visits visitable with visitor.
//

namespace detail { namespace visitor {

//////////////////////////////////////////////////////////////////////////
// implementations for dynamic visitables...

// (detail) apply_visitor_impl(DynamicVisitor, DynamicVisitable)
//
void apply_visitor_impl(
      dynamic_visitor_base& visitor
    , dynamic_visitable_base& visitable
    , mpl::true_// dynamic_visitor
    , mpl::true_// dynamic_visitable
    )
{
    // Visit:
    visitable.apply_visitor(visitor);
}

// (detail) apply_visitor_impl(ReturningDynamicVisitor, DynamicVisitable)
//
template <typename R>
R apply_visitor_impl(
      dynamic_visitor_return_base<R>& visitor
    , dynamic_visitable_base& visitable
    , mpl::true_// dynamic_visitor
    , mpl::true_// dynamic_visitable
    )
{
    // Before visiting, clear the result value...
    visitor.result_value.reset();

    // ...visit...
    visitable.apply_visitor(visitor);

    // ...but if the result value is still empty...
    if (!visitor.result_value)
        // ...then the visitor is not properly implemented, so throw:
        throw detail::dynamic_return_error();

    // Otherwise (the normal case), return the visitor's result:
    return *visitor.result_value;
}

//////////////////////////////////////////////////////////////////////////
// implementations for static visitables...
//
// NOTE: BOOST_NO_FUNCTION_TEMPLATE_ORDERING workaround does not prohibit
// proper functioning on conforming compilers.
//

template <typename Visitor, typename Visitable>
    typename Visitor::result_type
apply_static_visitor_impl(
      Visitor& visitor
    , Visitable& visitable
    , mpl::false_// is_const_visitable
    )
{
    return static_visitable_traits<Visitable>
        ::apply_visitor(visitor, visitable);
}

template <typename Visitor, typename Visitable>
    typename Visitor::result_type
apply_static_visitor_impl(
      Visitor& visitor
    , const Visitable& visitable
    , mpl::true_// is_const_visitable
    )
{
    return static_visitable_traits<Visitable>
        ::apply_visitor(visitor, visitable);
}

template <typename Visitor, typename Visitable>
    typename Visitor::result_type
apply_visitor_impl(
      Visitor& visitor
    , Visitable& visitable
    , mpl::false_// dynamic_visitor
    , mpl::false_// dynamic_visitable
    )
{
    typedef mpl::bool_<
          is_const<Visitable>::value
        > is_const_visitable;

    return apply_static_visitor_impl(
          visitor
        , visitable
        , is_const_visitable()
        );
}

// (detail) apply_visitor_impl(DynamicVisitor, StaticVisitable)
//
template <
      typename DynamicVisitor
    , typename StaticVisitable
    >
    typename DynamicVisitor::result_type
apply_visitor_impl(
      DynamicVisitor& visitor
    , StaticVisitable& visitable
    , mpl::true_// dynamic_visitor
    , mpl::false_// dynamic_visitable
    )
{
    typename wrap_dynamic_visitor<DynamicVisitor>::type
        wrapped(visitor);

    return apply_visitor_impl(
          wrapped
        , visitable
        , mpl::false_()// dynamic_visitor
        , mpl::false_()// dynamic_visitable
        );
}       

}} // namespace detail::visitor

#define BOOST_AUX_APPLY_VISITOR_FUNC(CV1__, CV2__)  \
    template <                                      \
          typename Visitor                          \
        , typename Visitable                        \
        >                                           \
        typename Visitor::result_type               \
    apply_visitor(                                  \
          CV1__ Visitor& visitor                    \
        , CV2__ Visitable& visitable                \
        )                                           \
    {                                               \
        /* [Determine if visitor is dynamic:] */    \
        typedef mpl::bool_<                        \
              is_dynamic_visitor<                   \
                  CV1__ Visitor                     \
                >::type::value                      \
            > dynamic_visitor_flag;                 \
                                                    \
        /* [Determine if visitable is dynamic:] */  \
        typedef mpl::bool_<                        \
              is_dynamic_visitable<                 \
                  CV2__ Visitable                   \
                >::type::value                      \
            > dynamic_visitable_flag;               \
                                                    \
        /* Dispatch appropriate implementation: */  \
        return detail::visitor::apply_visitor_impl( \
              visitor                               \
            , visitable                             \
            , dynamic_visitor_flag()                \
            , dynamic_visitable_flag()              \
            );                                      \
    }                                               \
    /**/
#
BOOST_PP_DEFINE_FORWARDING_FUNC( 
      BOOST_AUX_APPLY_VISITOR_FUNC
    , 2
    )
#
#undef BOOST_AUX_APPLY_VISITOR_FUNC

} // namespace boost

#endif // BOOST_VISITOR_APPLY_VISITOR_UNARY_HPP
