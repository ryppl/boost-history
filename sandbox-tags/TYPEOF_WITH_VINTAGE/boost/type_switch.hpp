//-----------------------------------------------------------------------------
// boost type_switch.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_TYPE_SWITCH_HPP
#define BOOST_TYPE_SWITCH_HPP

#include "boost/mpl/lambda_match.hpp"
#include "boost/ref.hpp"
#include "boost/variant/apply_visitor.hpp"

namespace boost {
namespace type_switch {

//////////////////////////////////////////////////////////////////////////
// class ignore
//
// Implements a no-op. That is, ignore(), ignore()(), and
// ignore()(t1, ..., tN) have no effect.
//
// NOTE: N is an implementation defined value.
//
class ignore
{
public: // typedefs

    typedef void result_type;

public: // function object interface

    void operator()() const
    {
    }

    template <typename T>
    void operator()(const T&) const
    {
    }

};

//////////////////////////////////////////////////////////////////////////
// function template default_
//

namespace detail {

template <typename F>
class default_t
{
private: // representation

    mutable F f_;

public: // typedefs

    typedef void result_type;

public: // structors

    explicit default_t(const F& f)
        : f_(f)
    {
    }

private: // nonassignable

    default_t& operator=(const default_t&);

public: // function call interface

    template <typename U>
    void operator()(U&) const
    {
        f_(); // default_ shouldn't pass operand
    }

};

} // namespace detail

template <typename F>
inline
    detail::default_t<F>
default_(F f)
{
    return detail::default_t<F>( f );
}

inline
    detail::default_t<ignore>
default_()
{
    return detail::default_t<ignore>( ignore() );
}

///////////////////////////////////////////////////////////////////////////////
// function template case_<T>
//
// Creates a function object that may be chained to other case_-generated
// function objects using operator|= and when invoked, forwards its input (of
// the specified type T) to the given functionable object.
//

namespace detail {

// (detail) class ERROR_case_is_missing
// 
// Unusual name alerts user in compiler error messages, which appear when
// cases are not provided to cover all possible types.
//
struct ERROR_case_is_missing
{
};

struct base_case_t
{
    template <typename U>
    void operator()(const U&) const
    {
        ERROR_case_is_missing::undefined();
    }
};

// (detail) class template case_t
//
// Implementation class for chained switch-cases.
//
template <typename T, typename F, typename NextCase = base_case_t>
class case_t
    : private NextCase
{
private: // representation

    mutable F f_;

public: // typedefs

    typedef void result_type;

public: // structors

    explicit case_t(const F& f)
        : NextCase()
        , f_(f)
    {
    }

    explicit case_t(const F& f, const NextCase& next_case)
        : NextCase(next_case)
        , f_(f)
    {
    }

private: // nonassignable

    case_t& operator=(const case_t&);

public: // switch-case chaining support

    template <typename NextT, typename NextF, typename NextNextCase>
        case_t<
              T, F
            , case_t<NextT, NextF, NextNextCase>
            >
    operator|=(const case_t<NextT, NextF, NextNextCase>& next_case)
    {
        return case_t<
              T, F
            , case_t<NextT, NextF, NextNextCase>
            >(f_, next_case);
    }

    template <typename DefaultF>
        case_t<
              T, F
            , default_t<DefaultF>
            >
    operator|=(const default_t<DefaultF>& default_case)
    {
        return case_t<
              T, F
            , default_t<DefaultF>
            >(f_, default_case);
    }

private: // helpers, for function call interface (below)

    template <typename U>
    void execute(U& operand, mpl::true_) const
    {
        f_(operand);
    }

    template <typename U>
    void execute(U& operand, mpl::false_) const
    {
        NextCase::operator()(operand);
    }

public: // function call interface

    // pattern match:

    template <typename U>
    void operator()(U& operand) const
    {
        typedef typename mpl::lambda_match<T,U>::type pattern_match;
        execute(operand, pattern_match());
    }

    // exact match:

    void operator()(T& operand) const
    {
        f_(operand);
    }

};

} // namespace detail

template <typename T, typename F>
inline
    detail::case_t<T,F>
case_(F f)
{
    return detail::case_t<T,F>( f );
}

template <typename T>
inline
    detail::case_t<T,ignore>
case_()
{
    return detail::case_t<T,ignore>( ignore() );
}

//////////////////////////////////////////////////////////////////////////
// function template switch_
//

namespace detail {

template <typename Visitable>
struct switch_t
{
private: // representation

    boost::reference_wrapper< Visitable > visitable_;

public: // structors

    explicit switch_t(Visitable& visitable)
        : visitable_(visitable)
    {
    }

private: // nonassignable

    switch_t& operator=(const switch_t&);

public: // switch-case chaining support

    template <typename T, typename F, typename NC>
    void operator|=(const detail::case_t<T,F,NC>& cases)
    {
        boost::apply_visitor(cases, visitable_.get());
    }

};

} // namespace detail

template <typename Visitable>
inline
    detail::switch_t<Visitable>
switch_(Visitable& visitable)
{
    return detail::switch_t<Visitable>( visitable );
}

} // namespace type_switch
} // namespace boost

#endif // BOOST_TYPE_SWITCH_HPP
