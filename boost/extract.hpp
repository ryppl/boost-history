//-----------------------------------------------------------------------------
// boost extract.hpp header file
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

#ifndef BOOST_EXTRACT_HPP
#define BOOST_EXTRACT_HPP

#include <exception>

#include "boost/utility/addressof.hpp"
#include "boost/type_traits/is_pointer.hpp"
#include "boost/type_traits/is_const.hpp"
#include "boost/type_traits/add_pointer.hpp"
#include "boost/type_traits/add_reference.hpp"

#include "boost/mpl/apply_if.hpp"
#include "boost/mpl/assert_is_same.hpp"
#include "boost/mpl/bool_c.hpp"
#include "boost/mpl/if.hpp"

// The following are new/in-progress headers or fixes to existing headers:
#include "boost/extract_fwd.hpp"
#include "boost/detail/variant_workaround.hpp"

namespace boost {

// class bad_extract
//
// The exception thrown in the event of a failed extract
// of a reference value.
//
class bad_extract
    : public std::exception
{
public:
    virtual const char * what() const throw()
    {
        return "boost::bad_extract: "
               "failed extraction using boost::extract";
    }
};

// class template extract_traits
//
// Class template meant to be specialized for types wishing to offer
//   extract<T>(...) functionality.
// NOTE: T* extract_traits::execute( [const ]Holder& ) must be defined
//   in any specialization of extract_traits.
//
template <typename Holder>
struct extract_traits
{
private:
    template <typename T>
    static T* execute_impl(
          T& operand
        , mpl::true_c// extract_self
        )
    {
        return boost::addressof(operand);
    }

    template <typename T>
    static const T* execute_impl(
          const T& operand
        , mpl::true_c// extract_self
        )
    {
        return boost::addressof(operand);
    }

    template <typename T>
    static T* execute_impl(
          T& operand
        , mpl::false_c// extract_self
        )
    {
        return static_cast<T*>(0);
    }

    template <typename T>
    static const T* execute_impl(
          const T& operand
        , mpl::false_c// extract_self
        )
    {
        return static_cast<const T*>(0);
    }

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

public:
    template <typename T>
    static T* execute(Holder& operand)
    {
        return execute_impl<T>(
              operand
            , mpl::bool_c< is_same<Holder, T>::value >() // extract_self
            );
    }

    template <typename T>
    static const T* execute(const Holder& operand)
    {
        return execute_impl<T>(
              operand
            , mpl::bool_c< is_same<Holder, const T>::value >() // extract_self
            );
    }

#else// defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

private:
    template <typename T>
    static T* execute_no_partial_spec_impl(
          Holder& operand
        , mpl::true_c// is_variant
        )
    {
        return detail::variant_extract_pointer<T>(operand);
    }

    template <typename T>
    static const T* execute_no_partial_spec_impl(
          const Holder& operand
        , mpl::true_c// is_variant
        )
    {
        return detail::variant_extract_pointer<const T>(operand);
    }

    template <typename T>
    static T* execute_no_partial_spec_impl(
          Holder& operand
        , mpl::false_c// is_variant
        )
    {
        return execute_impl(
              operand
            , mpl::bool_c< is_same<Holder, T>::value >() // extract_self
            );
    }

    template <typename T>
    static const T* execute_no_partial_spec_impl(
          const Holder& operand
        , mpl::false_c// is_variant
        )
    {
        return execute_impl(
              operand
            , mpl::bool_c< is_same<Holder, const T>::value >() // extract_self
            );
    }

public:
    template <typename T>
    static T* execute(Holder& operand)
    {
        return execute_no_partial_spec_impl<T>(
              operand
            , mpl::bool_c< is_variant<Holder>::value >()
            );
    }

    template <typename T>
    static const T* execute(const Holder& operand)
    {
        return execute_no_partial_spec_impl<T>(
              operand
            , mpl::bool_c< is_variant<Holder>::value >()
            );
    }

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION workaround

};

//////////////////////////////////////////////////////////////////////////
// function template extract<T>
//
// Attempts a conversion of the given value to type T.

namespace detail {

template <typename T>
struct extract_pointer_impl
{
private:
    template <typename Holder> 
    static T* execute_impl(Holder* operand, mpl::false_c)
    {
        if (!operand)
            return static_cast<T*>(0);
        return extract_traits<Holder>::template execute<T>(*operand);
    }

    template <typename Holder> 
    static T* execute_impl(const Holder* operand, mpl::true_c)
    {
        if (!operand)
            return static_cast<T*>(0);
        return extract_traits<Holder>::template execute<T>(*operand);
    }

public:
    template <typename Holder>
    static T* execute(Holder* operand)
    {
        return execute_impl(operand, mpl::bool_c< is_const<Holder>::value >());
    }
};

template <typename T>
struct extract_reference_impl
{
private:
    template <typename Holder>
    static T& execute_impl(Holder& operand, mpl::false_c)
    {
        T* result = extract_traits<Holder>::template execute<T>(operand);
        if (!result)
            throw bad_extract();
        return *result;
    }

    template <typename Holder>
    static T& execute_impl(const Holder& operand, mpl::true_c)
    {
        T* result = extract_traits<Holder>::template execute<T>(operand);
        if (!result)
            throw bad_extract();
        return *result;
    }

public:
    template <typename Holder>
    static T& execute(Holder& operand)
    {
        return execute_impl(operand, mpl::bool_c<is_const<Holder>::value>());
    }
};

} // namespace detail

template <typename T, typename Holder>
    typename mpl::apply_if<
          is_pointer<Holder>
        , add_pointer<T>
        , add_reference<T>
        >::type
extract(Holder& operand)
{
    typedef typename mpl::if_<
          is_pointer<Holder>
        , detail::extract_pointer_impl<T>
        , detail::extract_reference_impl<T>
        >::type impl;

    return impl::execute(operand);
}

} // namespace boost

#endif // BOOST_EXTRACT_HPP
