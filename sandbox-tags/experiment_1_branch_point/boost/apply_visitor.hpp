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
#include "boost/preprocessor/define_forwarding_func.hpp"

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

#define BOOST_AUX_UNARY_APPLY_VISITOR(CV1__, CV2__)                     \
    template <typename Visitor, typename Visitable>                     \
        typename Visitor::result_type                                   \
    apply_visitor(CV1__ Visitor& visitor, CV2__ Visitable& visitable)   \
    {                                                                   \
        return apply_visitor_traits<Visitable>                          \
            ::execute(visitor, visitable);                              \
    }                                                                   \
    /**/

BOOST_PP_DEFINE_FORWARDING_FUNC(
      BOOST_AUX_UNARY_APPLY_VISITOR
    , 2
    )

#undef BOOST_AUX_UNARY_APPLY_VISITOR

#else// defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

namespace detail {
namespace apply_visitor {

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

} // namespace apply_visitor
} // namespace detail

template <typename Visitor, typename Visitable>
    typename Visitor::result_type
apply_visitor(Visitor& visitor, Visitable& visitable)
{
    return detail::apply_visitor::apply_visitor_aux(
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
    typedef typename Visitor::result_type
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

    template <typename Value0>
    result_type operator()(const Value0& value0)
    {
        return visitor_(value0, value1_);
    }
};

template <typename Visitor, typename Visitable1>
class binary_delay1
{
public:
    typedef typename Visitor::result_type
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

#   define BOOST_AUX_BINARY_VISITOR_DELAY1_FUNC_OPERATOR(CV__)  \
    template <typename Visitable2>                              \
    result_type operator()(CV__ Visitable2& visitable2)         \
    {                                                           \
        binary_delay0<                                          \
              Visitor                                           \
            , CV__ Visitable2                                   \
            > delayer(visitor_, visitable2);                    \
        return boost::apply_visitor(delayer, visitable1_);      \
    }                                                           \
    /**/

    BOOST_PP_DEFINE_FORWARDING_FUNC(
          BOOST_AUX_BINARY_VISITOR_DELAY1_FUNC_OPERATOR
        , 1
        )

#   undef BOOST_AUX_BINARY_VISITOR_DELAY1_FUNC_OPERATOR
};

} // namespace apply_visitor
} // namespace detail

#define BOOST_AUX_BINARY_APPLY_VISITOR(CV1__, CV2__, CV3__)                 \
    template <typename Visitor, typename Visitable1, typename Visitable2>   \
        typename Visitor::result_type                                       \
    apply_visitor(                                                          \
          CV1__ Visitor& visitor                                            \
        , CV2__ Visitable1& visitable1                                      \
        , CV3__ Visitable2& visitable2                                      \
        )                                                                   \
    {                                                                       \
        detail::apply_visitor::binary_delay1<                               \
              CV1__ Visitor                                                 \
            , CV2__ Visitable1                                              \
            > delayer(visitor, visitable1);                                 \
        return boost::apply_visitor(delayer, visitable2);                   \
    }                                                                       \
    /**/

BOOST_PP_DEFINE_FORWARDING_FUNC(
      BOOST_AUX_BINARY_APPLY_VISITOR
    , 3
    )

#undef BOOST_AUX_BINARY_APPLY_VISITOR

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
    typedef typename Visitor::result_type
        result_type;

private: // representation
    Visitor& visitor_;

public: // structors
    explicit apply_visitor_t(Visitor& visitor)
      : visitor_(visitor)
    {
    }

public: // unary function operators

#   define BOOST_AUX_APPLY_VISITOR_T_UNARY_FUNC_OPERATOR(CV__)    \
    template <typename Visitable>                       \
    result_type operator()(                             \
          CV__ Visitable& visitable                     \
        )                                               \
    {                                                   \
        apply_visitor(visitor_, visitable);             \
    }                                                   \
    /**/

    BOOST_PP_DEFINE_FORWARDING_FUNC(
          BOOST_AUX_APPLY_VISITOR_T_UNARY_FUNC_OPERATOR
        , 1
        )

#   undef BOOST_AUX_APPLY_VISITOR_T_UNARY_FUNC_OPERATOR

public: // binary function operators

#   define BOOST_AUX_APPLY_VISITOR_T_BINARY_FUNC_OPERATOR(CV1__, CV2__)   \
    template <typename Visitable1, typename Visitable2>         \
    result_type operator()(                                     \
          CV1__ Visitable1& visitable1                          \
        , CV2__ Visitable2& visitable2                          \
        )                                                       \
    {                                                           \
        apply_visitor(visitor_, visitable1, visitable2);        \
    }                                                           \
    /**/

    BOOST_PP_DEFINE_FORWARDING_FUNC(
          BOOST_AUX_APPLY_VISITOR_T_BINARY_FUNC_OPERATOR
        , 2
        )

#   undef BOOST_AUX_APPLY_VISITOR_T_BINARY_FUNC_OPERATOR

};

#define MAKE_APPLY_VISITOR_FUNC_OBJ(CV__)               \
    template <typename Visitor>                         \
    apply_visitor_t<CV__ Visitor> apply_visitor(        \
          CV__ Visitor& visitor                         \
        )                                               \
    {                                                   \
        return apply_visitor_t<CV__ Visitor>(visitor);  \
    }                                                   \
    /**/

BOOST_PP_DEFINE_FORWARDING_FUNC(
      MAKE_APPLY_VISITOR_FUNC_OBJ
    , 1
    )

#undef MAKE_APPLY_VISITOR_FUNC_OBJ

} // namespace boost

#endif // BOOST_APPLY_VISITOR_HPP
