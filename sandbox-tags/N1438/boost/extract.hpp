//-----------------------------------------------------------------------------
// boost extract.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002-2003
// Eric Friedman, Itay Maman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_EXTRACT_HPP
#define BOOST_EXTRACT_HPP

#include <exception>

#include "boost/config.hpp"
#include "boost/extractable.hpp"

#include "boost/type_traits/is_base_and_derived.hpp"
#include "boost/type_traits/is_const.hpp"
#include "boost/mpl/logical/not.hpp"
#include "boost/mpl/logical/or.hpp"
#include "boost/mpl/bool_c.hpp"

#include "boost/visitor/unary_apply_visitor.hpp"
#include "boost/static_visitor.hpp"
#include "boost/visitor/dynamic_visitor_base.hpp"
#include "boost/visitor/dynamic_visitor_interface.hpp"

namespace boost {

//////////////////////////////////////////////////////////////////////////
// class bad_extract
//
// The exception thrown in the event of a failed extract
// of a reference value.
//
class bad_extract
    : public std::exception
{
public: // std::exception implementation

    virtual const char * what() const throw()
    {
        return "boost::bad_extract: "
               "failed extraction using boost::extract";
    }

};

//////////////////////////////////////////////////////////////////////////
// class template extract<T>
//
// Facilitates a conversion of the given value to specified type.
//

namespace detail {

// (detail) metafunction is_directly_extractable
//
// Value metafunction indicates whether the specified type explicitly
// supports extraction of a specified type T.
//
template <typename T>
struct is_directly_extractable
{
public: // metafunction result

    typedef typename mpl::logical_or< // directly-extractable || NOT default-traits
          is_base_and_derived<
              detail::extractable::directly_extractable_tag
            , T
            >
        , mpl::logical_not<
              is_base_and_derived<
                  detail::extractable::default_traits_tag
                , extractable_traits<T>
                >
            >
        >::type type;

    BOOST_STATIC_CONSTANT(bool, value = type::value);
};

} // namespace detail

template <typename T>
class extract
{
private: // representation

    T* p_;

private: // helpers (extract by visitation)

    // (helper) class template extracting_visitor
    //
    // Visitor that: if the value is of the specified type, returns a pointer
    // to the value it visits; else returns a null pointer.
    //
    struct visitor
        : static_visitor<>
        , dynamic_visitor_base
        , dynamic_visitor_interface<T>
    {
    public: // typedefs

        typedef void result_type;

    public: // fields

        T* p;

    public: // structors

        visitor()
            : p(0)
        {
        }

    public: // static visitor interfaces

        template <typename U>
        void operator()(U&)
        {
            p = 0;
        }

        void operator()(T& operand)
        {
            p = boost::addressof(operand);
        }

    public: // dynamic visitor interfaces

        void visit(T& operand)
        {
            p = boost::addressof(operand);
        }

    };

    template <typename Extractable, typename B>
    static T* extract_impl(
          Extractable& operand
        , B// is_const
        , mpl::false_c// directly_extractable
        )
    {
        visitor visiting;
        boost::apply_visitor(visiting, operand);
        return visiting.p;
    }

private: // helpers (extract by extractable_traits)

    template <typename Extractable>
    static T* extract_impl(
          Extractable& operand
        , mpl::false_c// is_const
        , mpl::true_c// directly_extractable
        )
    {
        // Instantiate traits as MSVC workaround:
        extractable_traits<Extractable> traits;
        return traits.template extract<T>(operand);
    }

    template <typename Extractable>
    static T* extract_impl(
          const Extractable& operand
        , mpl::true_c// is_const
        , mpl::true_c// directly_extractable
        )
    {
        // Instantiate traits as MSVC workaround:
        extractable_traits<Extractable> traits;
        return traits.template extract<T>(operand);
    }

private: // workaround for structors (below)

    template <typename Extractable>
    static T* extract_impl(
          Extractable& operand
        , mpl::false_c is_const
        )
    {
        typedef detail::is_directly_extractable<Extractable>
            directly_extractable;

        return extract_impl(
              operand
            , is_const
            , mpl::bool_c<directly_extractable::value>()
            );
    }

    template <typename Extractable>
    static T* extract_impl(
          const Extractable& operand
        , mpl::true_c is_const
        )
    {
        typedef detail::is_directly_extractable<Extractable>
            directly_extractable;

        return extract_impl(
              operand
            , is_const
            , mpl::bool_c<directly_extractable::value>()
            );
    }

public: // structors

    template <typename Extractable>
    explicit extract(Extractable& operand)
    {
        p_ = extract_impl(
              operand
            , mpl::bool_c<
                  is_const<Extractable>::value
                >()
            );
    }

public: // queries

    bool check() const
    {
        return p_ != 0;
    }

public: // operators

    typedef T& result_type;

    T& operator()() const
    {
        if (!check())
            throw bad_extract();

        return *p_;
    }

#ifdef BOOST_MSVC
    operator T&() const
    {
        return (*this)();
    }
#endif

};

} // namespace boost

#endif // BOOST_EXTRACT_HPP
