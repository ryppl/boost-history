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

#include "boost/type_traits/is_pointer.hpp"
#include "boost/type_traits/is_const.hpp"
#include "boost/type_traits/add_pointer.hpp"
#include "boost/type_traits/add_reference.hpp"

#include "boost/mpl/apply_if.hpp"
#include "boost/mpl/bool_c.hpp"
#include "boost/mpl/if.hpp"

namespace boost {

//////////////////////////////////////////////////////////////////////////
// Forward declares
//

template <typename Deriving> class extractable;

//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
// function template extract<T>
//
// Attempts a conversion of the given value to specified type.
//
// Workaround below simulates following:
//
//   template <typename T, typename Holder> T * extract(Holder *);
//   template <typename T, typename Holder> T * extract(const Holder *);
//
//   template <typename T, typename Holder> T & extract(Holder &);
//   template <typename T, typename Holder> T & extract(const Holder &);
//
// Workaround rationale: see ISO standard defect 214 ("Partial ordering of
// function templates is underspecified").
//

namespace detail {

template <typename T>
struct extract_pointer_impl
{
private:
    template <typename Extractable> 
    static T* execute_impl(
          Extractable* operand
        , mpl::false_c// is_const
        )
    {
        if (!operand)
            return static_cast<T*>(0);
        return extractable_traits<Extractable>::template extract<T>(*operand);
    }

    template <typename Extractable> 
    static T* execute_impl(
          const Extractable* operand
        , mpl::true_c// is_const
        )
    {
        if (!operand)
            return static_cast<T*>(0);
        return extractable_traits<Extractable>::template extract<T>(*operand);
    }

public:
    template <typename Extractable>
    static T* execute(Extractable* operand)
    {
        return execute_impl(
              operand
            , mpl::bool_c< is_const<Extractable>::value >()
            );
    }
};

template <typename T>
struct extract_reference_impl
{
private:
    template <typename Extractable>
    static T& execute_impl(
          Extractable& operand
        , mpl::false_c// is_const
        )
    {
        T* result = extractable_traits<Extractable>::template extract<T>(operand);
        if (!result)
            throw bad_extract();
        return *result;
    }

    template <typename Extractable>
    static T& execute_impl(
          const Extractable& operand
        , mpl::true_c// is_const
        )
    {
        T* result = extractable_traits<Extractable>::template extract<T>(operand);
        if (!result)
            throw bad_extract();
        return *result;
    }

public:
    template <typename Extractable>
    static T& execute(Extractable& operand)
    {
        return execute_impl(
              operand
            , mpl::bool_c< is_const<Extractable>::value >()
            );
    }
};

} // namespace detail

template <typename T, typename Extractable>
    typename mpl::apply_if<
          is_pointer<Extractable>
        , add_pointer<T>
        , add_reference<T>
        >::type
extract(Extractable& operand)
{
    typedef typename mpl::if_<
          is_pointer<Extractable>
        , detail::extract_pointer_impl<T>
        , detail::extract_reference_impl<T>
        >::type impl;

    return impl::execute(operand);
}

} // namespace boost

#endif // BOOST_EXTRACT_HPP
