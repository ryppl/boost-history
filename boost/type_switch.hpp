//-----------------------------------------------------------------------------
// boost type_switch.hpp header file
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

#ifndef BOOST_TYPE_SWITCH_HPP
#define BOOST_TYPE_SWITCH_HPP

#include "boost/config.hpp"
#include "boost/call_traits.hpp"

// The following are new/in-progress headers or fixes to existing headers:
#include "boost/config/no_using_declaration_overloads.hpp"
#include "boost/visitor/apply_visitor.hpp"

namespace boost {
namespace type_switch {

//////////////////////////////////////////////////////////////////////////
// class ignore
// Implements a no-op. That is, ignore(), ignore()(), and
// ignore()(t1, ..., tN) have no effect.
//
// NOTE: N is an implementation defined value.
//
struct ignore
{
    typedef void result_type;

    void operator()() const
    {
    }

    template <typename T>
    void operator()(const T&) const
    {
    }
};

//////////////////////////////////////////////////////////////////////////
// function template case_<T>// 
// Creates a function object that may be chained to other case_-generated
// function objects using operator| and when invoked, forwards its input
// (of the specified type T) to the given functionable object.
//

namespace detail {

// error_Case_Is_Missing : Unusual name is alerts user in compiler error messages.
struct error_Case_Is_Missing
{
    typedef error_Case_Is_Missing
        end_case;

    template <typename TemplateCase>
    struct rebind_with_template_case
    {
        typedef TemplateCase type;
    };

#if !defined(BOOST_NO_USING_DECLARATION_OVERLOADS)

    // | Must provide operator(), but it should never be invoked. Accepts |
    // | an argument of an implementation-defined type, which should at   |
    // | least keep the user from invoking the method.                    |
    void operator()(const error_Case_Is_Missing&) const
    {
    }

#endif // !defined(BOOST_NO_USING_DECLARATION_OVERLOADS)

protected:
    ~error_Case_Is_Missing()
    {
    }
};

template <typename TypeCase, typename TemplateCase>
struct push_template_case
{
    typedef typename TypeCase::template rebind_with_template_case<TemplateCase>::type
        type;
};

template <typename T, typename Func, typename NextCase = error_Case_Is_Missing>
class type_case
    : public NextCase
{
#if !defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)

    // [Make the class friendly to its own variants:]
    template <typename OtherT, typename OtherF, typename OtherNextCase>
    friend class type_case;

    template <typename TypeCase, typename TemplateCase>
    friend struct push_template_case;

private:

#else// defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)

public:

#endif// !defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)

    Func f_;

    const NextCase& as_NextCase() const
    {
        return *this;
    }

    template <typename TemplateCase>
    struct rebind_with_template_case
    {
        typedef type_case<
              T, Func
            , typename push_template_case<NextCase, TemplateCase>::type
            > type;
    };

protected:
    typedef typename NextCase::end_case
        end_case;

public:
    typedef void result_type;

public:
    explicit type_case(typename call_traits<Func>::param_type f)
        : NextCase()
        , f_(f)
    {
    }

    type_case(const type_case<T, Func>& this_case, const NextCase& next)
        : NextCase(next)
        , f_(this_case.f_)
    {
    }

    template <typename OldNextCase>
    type_case(const type_case<T, Func, OldNextCase>& this_case, const end_case& end)
        : NextCase(this_case.as_NextCase(), end)
        , f_(this_case.f_)
    {
    }

    void operator()(T& operand)
    {
        f_(operand);
    }

    void operator()(T& operand) const
    {
        f_(operand);
    }

#if !defined(BOOST_NO_USING_DECLARATION_OVERLOADS)

    using NextCase::operator();

#else// defined(BOOST_NO_USING_DECLARATION_OVERLOADS)

    template <typename U>
    void operator()(U& operand)
    {
        NextCase::operator()(operand);
    }

    template <typename U>
    void operator()(U& operand) const
    {
        NextCase::operator()(operand);
    }

#endif // BOOST_NO_USING_DECLARATION_OVERLOADS workaround

    };

template <typename NextT, typename NextF, typename NextNextCase, typename PriorT, typename PriorF>
type_case<
      PriorT, PriorF
    , type_case<NextT, NextF, NextNextCase>
    >
operator|(
      const type_case<NextT, NextF, NextNextCase>& next_case
    , const type_case<PriorT, PriorF>& prior_case
    )
{
    return type_case<
            PriorT, PriorF
        , type_case<NextT, NextF, NextNextCase>
        >(prior_case, next_case);
}

} // namespace detail

template <typename T, typename Func>
detail::type_case<T, Func> case_(Func f)
{
    return detail::type_case<T, Func>(f);
}

template <typename T>
detail::type_case<T, ignore> case_()
{
    return case_<T>(ignore());
}

//////////////////////////////////////////////////////////////////////////
// template_//
// Creates a function object that may be chained to case_-generated
// function objects using operator| and when invoked, forwards its input
// to the given functionable object.
//

namespace detail {

template <typename Func>
class template_case
{
    Func f_;

protected:
    typedef template_case
        end_case;

public:
    typedef void result_type;

public:
    explicit template_case(typename call_traits<Func>::param_type f)
        : f_(f)
    {
    }

    template <typename OldEndCase>
    template_case(const OldEndCase&, const template_case& operand)
        : f_(operand.f_)
    {
    }

    template <typename T>
    void operator()(T& operand)
    {
        f_(operand);
    }

    template <typename T>
    void operator()(T& operand) const
    {
        f_(operand);
    }
};

template <typename T, typename F, typename NextCase, typename TemplateF>
    typename push_template_case<
          type_case<T, F, NextCase>
        , template_case<TemplateF>
        >::type
operator|(
      const type_case<T, F, NextCase>& reg_case
    , const template_case<TemplateF>& templ_case
    )
{
    typedef typename push_template_case<
          type_case<T, F, NextCase>
        , template_case<TemplateF>
        >::type return_type;

    return return_type(reg_case, templ_case);
}

} // namespace detail

template <typename Func>
detail::template_case<Func> template_(Func f)
{
    return detail::template_case<Func>(f);
}

//////////////////////////////////////////////////////////////////////////
// function template default_// Creates a function object that may be chained to case_-generated
// function objects using operator| and, when invoked, invokes the given
// functionable object without arguments.
//

namespace detail {

template <typename Func>
class default_func
{
    Func f_;

public:
    typedef void result_type;

public:
    explicit default_func(Func f)
        : f_(f)
    {
    }

    template <typename T>
    void operator()(const T&)
    {
        f_();
    }

    template <typename T>
    void operator()(const T&) const
    {
        f_();
    }
};

} // namespace detail

template <typename Func>
detail::template_case< detail::default_func<Func> > default_(Func f)
{
    return template_(detail::default_func<Func>(f));
}

detail::template_case< detail::default_func<ignore> > default_()
{
    return default_(ignore());
}

//////////////////////////////////////////////////////////////////////////
// function template switch_//
// Returns an implementation-defined type whose indexer (i.e.,
// operator[]), given an operand and a functionable object F, invokes the
// traits template member switch_traits::execute(operand, F).
//

template <typename T>
struct switch_traits
{
    template <typename Cases>
    static void execute(T& operand, Cases cases)
    {
        boost::apply_visitor(cases, operand);
    }

    template <typename Cases>
    static void execute(const T& operand, Cases cases)
    {
        boost::apply_visitor(cases, operand);
    }
};

namespace detail {

template <typename Operand, typename Traits>
class switch_impl
{
    Operand* operand_;

public:
    explicit switch_impl(Operand& operand)
        : operand_(&operand)
    {
    }

    template <typename T, typename F, typename NC>
    void operator[](const type_case<T, F, NC>& cases)
    {
        Traits::execute(*operand_, cases);
    }
};

} // namespace detail

template <typename Operand>
    detail::switch_impl<
          Operand
        , switch_traits<Operand>
        >
switch_(Operand& operand)
{
    return detail::switch_impl<
          Operand
        , switch_traits<Operand>
        >(operand);
}

template <typename Operand>
    detail::switch_impl<
          const Operand
        , switch_traits<Operand>
        >
switch_(const Operand& operand)
{
    return detail::switch_impl<
          const Operand
        , switch_traits<Operand>
        >(operand);
}

} // namespace type_switch

using type_switch::switch_;
using type_switch::case_;
using type_switch::template_;
using type_switch::default_;

} // namespace boost

#endif // BOOST_TYPE_SWITCH_HPP
