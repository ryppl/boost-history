//-----------------------------------------------------------------------------
// boost move.hpp header file
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

#ifndef BOOST_MOVE_HPP
#define BOOST_MOVE_HPP

#include <iterator> // for iterator_traits

#include "boost/config.hpp"
#include "boost/type_traits/is_base_and_derived.hpp"
#include "boost/mpl/if.hpp"

namespace boost {

namespace detail {

template <class T>
class const_lvalue_ref
{
private: // representation
    const T* data_;

public: // structors
    explicit const_lvalue_ref(const T& obj)
        : data_(&obj)
    {
    }

public: // queries
    const T& get() const
    {
        return *data_;
    }
};

} // namespace detail

//////////////////////////////////////////////////////////////////////////
// class template move_source
//
// Passed to object constructors and assignment operators indicating to
// perform a move operation (i.e. instead of a copy operation).
//
template <class T>
class move_source
    : private detail::const_lvalue_ref<T>
{
public: // structors
    explicit move_source(T& obj)
        : detail::const_lvalue_ref<T>(obj)
    {
    }

public: // queries
    T& get() const
    {
        return const_cast<T&>(detail::const_lvalue_ref<T>::get());
    }
};

//////////////////////////////////////////////////////////////////////////
// class template move_return
//
// Facilitates return-by-move for function return values.
//
// Example usage for function f and moveable type X:
//   move_return< X > f( ... );
//
template <class T> 
class move_return
{
private: // representation
    T returned_;

public: // structors
    explicit move_return(T& returned)
        : returned_(move_source<T>(returned))
    {
    }

    // "The cast below is valid given that nobody ever really creates a 
    //  const move_return object..."

    move_return(const move_return& operand)
        : returned_(const_cast<move_return&>(operand))
    {
    }

public: // operators
    operator move_source<T>()
    {
        return move_source<T>(returned_);
    }
};

template <class Deriving>
struct moveable
{
public: // operators
    operator detail::const_lvalue_ref<Deriving>() const
    {
        return detail::const_lvalue_ref<Deriving>(
              static_cast<const Deriving&>(*this)
            );
    }

    operator move_source<Deriving>()
    {
        return move_source<Deriving>(
              static_cast<Deriving&>(*this)
            );
    }

    operator move_return<Deriving>()
    {
        return move_return<Deriving>(
              static_cast<Deriving&>(*this)
            );
    }

protected: // noninstantiable
    moveable() { }
    ~moveable() { }
};

//////////////////////////////////////////////////////////////////////////
// function template move
//
// Takes a T& and returns, if T derives moveable<T>, a move_source<T> for
// the object; else, returns the T&.
//

namespace detail {

template <typename T>
struct move_type
{
    typedef typename mpl::if_<
          is_base_and_derived<moveable<T>, T>
        , move_source<T>
        , T&
        >::type type;
};

} // namespace detail

template <typename T>
inline
    typename detail::move_type<T>::type
move(T& source)
{
    typedef typename detail::move_type<T>::type
        move_t;

    return move_t(source);
}

//////////////////////////////////////////////////////////////////////////
// function template move
//
// Moves-assigns each element in the range [first, last) to the range
// that begins with iterator result.
//
template <typename InputIterator, typename OutputIterator>
    OutputIterator
move(InputIterator first, InputIterator last, OutputIterator result)
{
    for (; first != last; ++first, ++result)
        *result = move(*first);

    return result;
}

//////////////////////////////////////////////////////////////////////////
// function template move_backward
//
// Move-assigns each element in the reverse range (last, first] to the
// range that begins with iterator result.
//
template <typename BidirectionalIterator1, typename BidirectionalIterator2>
    BidirectionalIterator2
move_backward(
      BidirectionalIterator1 first, BidirectionalIterator1 last
    , BidirectionalIterator2 result
    )
{
    while (last != first)
        *--result = move(*--last);

    return result;
}

//////////////////////////////////////////////////////////////////////////
// function template uninitialized_move
//
// Move-constructs each element in the reverse range (last, first] to the
// range that begins with iterator result.
//
template <typename InputIterator, typename OutputIterator>
    OutputIterator
uninitialized_move(
      InputIterator first, InputIterator last
    , OutputIterator result
    )
{
    typedef typename std::iterator_traits<
          OutputIterator
        >::value_type T;

    for (; first != last; ++first, ++result)
        new(*result) T(move(*first));

    return result;
}

} // namespace boost

#endif // BOOST_MOVE_HPP


/* Original copyright -- on mojo.h -- follows:

////////////////////////////////////////////////////////////////////////////////
// MOJO: MOving Joint Objects
// Copyright (c) 2002 by Andrei Alexandrescu
//
// Created by Andrei Alexandrescu
//
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// The author makes no representations about the suitability of this software 
//     for any purpose. It is provided "as is" 
//     without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

*/
