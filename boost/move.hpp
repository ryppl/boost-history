// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MOVE_DWA2004410_HPP
# define BOOST_MOVE_DWA2004410_HPP

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/if.hpp>
#include <boost/preprocessor/comparison/equal.hpp>

namespace boost { 

template <class T, class X, class R = void*>
struct enable_if_same
{
};

template <class X, class R>
struct enable_if_same<X, X, R>
{
    typedef R type;
};

// This is the class that enables the distinction of rvalues in the
// move overload set.  Any type T with a conversion to move_from<T> is
// movable.
template <class X>
struct move_from
{
    explicit move_from(X& p)
      : p(&p) {}

    // Convenience for move constructing/assigning base classes.
    template <class Y>
    operator move_from<Y>()
    { return move_from<Y>(*p); }

    // It acts like a smart pointer, for syntactic convenience
    X& operator*() const { return *p; }
    X* operator->() const { return p; }
      
 private:
    X* p; // this pointer will refer to the object from which to move
};

// CRTP base class for conveniently making movable types.  You don't
// have to use this to make a type movable (if, for example, you don't
// want the MI non-EBO penalty); you just need to provide the
// conversion to move_from<Derived> yourself in that case.
template <class Derived>
struct movable
{
    // Provide the conversion operator
    operator move_from<Derived>()
    {
        return move_from<Derived>(*static_cast<Derived*>(this));
    }
    
 protected:
    movable() {}
    
 private:
    // any class that wants to be movable should to define its copy
    // ctor and assignment using the macros below, or else those
    // should remain private.
    movable(movable&);
    movable& operator=(movable&);
};

// Algorithms lifted directly from
// http://std.dkuug.dk/jtc1/sc22/wg21/docs/papers/2002/n1377.htm
template <class T>
void
swap(T& a, T& b)
{
    T tmp(boost::move(a));
    a = boost::move(b);
    b = boost::move(tmp);
}

// I wonder whether it's a good idea to have the 1-argument and
// 3-argument overloads that do different things?
template <class InputIterator, class OutputIterator>
OutputIterator
move(InputIterator first, InputIterator last, OutputIterator result)
{
    for (; first != last; ++first, ++result)
        *result = move(*first);
    return result;
}
 
template <class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator2
move_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
              BidirectionalIterator2 result)
{
    while (last != first)
        *--result = move(*--last);
    return result;
}

#if BOOST_WORKAROUND(__GNUC__, BOOST_TESTED_AT(3))                  \
    && BOOST_WORKAROUND(__GNUC_MINOR__, BOOST_TESTED_AT(3))         \
    && BOOST_WORKAROUND(__GNUC_PATCHLEVEL__, BOOST_TESTED_AT(1))  

// A strange bug lurks here
namespace detail
{
  template <class T>
  inline T move_impl(T& x, boost::mpl::true_)
  {
      return T(boost::move_from<T>(x));
  }

  template <class T>
  inline T& move_impl(T& x, boost::mpl::false_)
  {
      return x;
  }
}

template <class T>
typename boost::mpl::if_<
    boost::is_convertible<T,move_from<T> >
  , T
  , T&
>::type
inline
move(T& x)
{
    return detail::move_impl(x, boost::is_convertible<T,move_from<T> >());
}

#else

template <class T>
typename boost::mpl::if_<
    boost::is_convertible<T,move_from<T> >
  , move_from<T>
  , T&
>::type
inline
move(T& x)
{
    return x;
}
#endif 

//
// Move library convenience macros for generating lvalue copy ctors
// assignment operators
//

// Generates copy constructors for lvalue right-hand-side.  Usage:
//
//     BOOST_LVALUE_COPY_CTOR(
//            class-name, (argument-name)
//          , { ...body code... }
//     )
//
// or
//
//     BOOST_LVALUE_COPY_CTOR(
//            class-name, (argument-name) // note no comma
//              (: initializer1(a1), initializer2(a2), ... )
//          , { ...body code... }
//     )
//
// if you only want to declare a copy ctor, but not define it:
//
//     BOOST_LVALUE_COPY_CTOR(class-name, (argument-name), ;)
//
#define BOOST_LVALUE_COPY_CTOR(klass, arg_init, body)                           \
    klass(klass& BOOST_PP_SEQ_HEAD(arg_init))                                   \
        BOOST_MOVE_INITIALIZER_LIST(arg_init)                                   \
        body                                                                    \
                                                                                \
    template <class BoostMove_##klass>                                          \
    klass(                                                                      \
        BoostMove_##klass& BOOST_PP_SEQ_HEAD(arg_init)                          \
      , typename boost::enable_if_same<klass const,BoostMove_##klass>::type = 0 \
    )                                                                           \
        BOOST_MOVE_INITIALIZER_LIST(arg_init)                                   \
        body

// Generates copy assignment operators for lvalue right-hand-side.
// Usage:
//
//      BOOST_LVALUE_ASSIGN(
//             class-name, (argument-name)
//          , { ...body code... }
//      )
//
// if you only want to declare a copy assignment operator, but not
// define it:
//
//     BOOST_LVALUE_ASSIGN(class-name, (argument-name), ;)
//
#define BOOST_LVALUE_ASSIGN(klass, rhs, body)                                   \
                                                                                \
    klass&                                                                      \
    operator=(klass& BOOST_PP_SEQ_HEAD(rhs))                                    \
        body                                                                    \
                                                                                \
    template <class BoostMove_##klass>                                          \
    typename boost::enable_if_same<klass const,BoostMove_##klass,klass&>::type  \
    operator=(BoostMove_##klass& BOOST_PP_SEQ_HEAD(rhs))                        \
        body

//
// Helper macros for BOOST_LVALUE_COPY_CTOR
//

// Given a SEQ appropriate for the 2nd argument to
// BOOST_LVALUE_COPY_CTOR, extract the initializer list.
#define BOOST_MOVE_INITIALIZER_LIST(arg_init)           \
    BOOST_PP_IIF(                                       \
        BOOST_PP_EQUAL(BOOST_PP_SEQ_SIZE(arg_init), 1)  \
      , BOOST_MOVE_NO_INIT                              \
      , BOOST_MOVE_SEQ_2ND)(arg_init)

// Return the 2nd element of a SEQ
#define BOOST_MOVE_SEQ_2ND(seq) BOOST_PP_SEQ_HEAD(BOOST_PP_SEQ_TAIL(seq))

// Generate an empty initializer list
#define BOOST_MOVE_NO_INIT(seq)

} // namespace boost

#endif // BOOST_MOVE_DWA2004410_HPP
