//  (C) Copyright David Abrahams 2000. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  The author gratefully acknowleges the support of Dragon Systems, Inc., in
//  producing this work.
//
//  This file automatically generated by gen_signatures.python for 10 arguments.
#ifndef SIGNATURES_DWA050900_H_
# define SIGNATURES_DWA050900_H_

# include "pyconfig.h"

namespace python {

namespace detail {
// A stand-in for the built-in void. This one can be passed to functions and
// (under MSVC, which has a bug, be used as a default template type parameter).
struct void_t {};
}

// An envelope in which type information can be delivered for the purposes
// of selecting an overloaded from_python() function. This is needed to work
// around MSVC's lack of partial specialiation/ordering. Where normally we'd
// want to form a function call like void f<const T&>(), We instead pass
// type<const T&> as one of the function parameters to select a particular
// overload.
//
// The id typedef helps us deal with the lack of partial ordering by generating
// unique types for constructor signatures. In general, type<T>::id is type<T>,
// but type<void_t>::id is just void_t.
template <class T>
struct type
{
    typedef type id;
};

template <>
struct type<python::detail::void_t>
{
    typedef python::detail::void_t id;
};

namespace detail {
// These basically encapsulate a chain of types, , used to make the syntax of
// add(constructor<T1, ...>()) work. We need to produce a unique type for each number
// of non-default parameters to constructor<>.  Q: why not use a recursive
// formulation for infinite extensibility? A: MSVC6 seems to choke on constructs
// that involve recursive template nesting.
//
// signature chaining
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
struct Signature10 {};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
struct Signature9 {};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class X>
inline Signature10<X, T1, T2, T3, T4, T5, T6, T7, T8, T9> prepend(type<X>, Signature9<T1, T2, T3, T4, T5, T6, T7, T8, T9>)
    { return Signature10<X, T1, T2, T3, T4, T5, T6, T7, T8, T9>(); }

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
struct Signature8 {};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class X>
inline Signature9<X, T1, T2, T3, T4, T5, T6, T7, T8> prepend(type<X>, Signature8<T1, T2, T3, T4, T5, T6, T7, T8>)
    { return Signature9<X, T1, T2, T3, T4, T5, T6, T7, T8>(); }

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
struct Signature7 {};

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class X>
inline Signature8<X, T1, T2, T3, T4, T5, T6, T7> prepend(type<X>, Signature7<T1, T2, T3, T4, T5, T6, T7>)
    { return Signature8<X, T1, T2, T3, T4, T5, T6, T7>(); }

template <class T1, class T2, class T3, class T4, class T5, class T6>
struct Signature6 {};

template <class T1, class T2, class T3, class T4, class T5, class T6, class X>
inline Signature7<X, T1, T2, T3, T4, T5, T6> prepend(type<X>, Signature6<T1, T2, T3, T4, T5, T6>)
    { return Signature7<X, T1, T2, T3, T4, T5, T6>(); }

template <class T1, class T2, class T3, class T4, class T5>
struct signature5 {};

template <class T1, class T2, class T3, class T4, class T5, class X>
inline Signature6<X, T1, T2, T3, T4, T5> prepend(type<X>, signature5<T1, T2, T3, T4, T5>)
    { return Signature6<X, T1, T2, T3, T4, T5>(); }

template <class T1, class T2, class T3, class T4>
struct signature4 {};

template <class T1, class T2, class T3, class T4, class X>
inline signature5<X, T1, T2, T3, T4> prepend(type<X>, signature4<T1, T2, T3, T4>)
    { return signature5<X, T1, T2, T3, T4>(); }

template <class T1, class T2, class T3>
struct signature3 {};

template <class T1, class T2, class T3, class X>
inline signature4<X, T1, T2, T3> prepend(type<X>, signature3<T1, T2, T3>)
    { return signature4<X, T1, T2, T3>(); }

template <class T1, class T2>
struct signature2 {};

template <class T1, class T2, class X>
inline signature3<X, T1, T2> prepend(type<X>, signature2<T1, T2>)
    { return signature3<X, T1, T2>(); }

template <class T1>
struct signature1 {};

template <class T1, class X>
inline signature2<X, T1> prepend(type<X>, signature1<T1>)
    { return signature2<X, T1>(); }

struct signature0 {};

template <class X>
inline signature1<X> prepend(type<X>, signature0)
    { return signature1<X>(); }


// This one terminates the chain. Prepending void_t to the head of a void_t
// signature results in a void_t signature again.
inline signature0 prepend(void_t, signature0) { return signature0(); }

} // namespace detail

template <class A1 = detail::void_t, class A2 = detail::void_t, class A3 = detail::void_t, class A4 = detail::void_t, class A5 = detail::void_t, class A6 = detail::void_t, class A7 = detail::void_t, class A8 = detail::void_t, class A9 = detail::void_t, class A10 = detail::void_t>
struct constructor
{
};

namespace detail {
// Return value extraction:

// This is just another little envelope for carrying a typedef (see type,
// above). I could have re-used type, but that has a very specific purpose. I
// thought this would be clearer.
template <class T>
struct return_value_select { typedef T type; };

// free functions
template <class R>
return_value_select<R> return_value(R (*)()) { return return_value_select<R>(); }

template <class R, class A1>
return_value_select<R> return_value(R (*)(A1)) { return return_value_select<R>(); }

template <class R, class A1, class A2>
return_value_select<R> return_value(R (*)(A1, A2)) { return return_value_select<R>(); }

template <class R, class A1, class A2, class A3>
return_value_select<R> return_value(R (*)(A1, A2, A3)) { return return_value_select<R>(); }

template <class R, class A1, class A2, class A3, class A4>
return_value_select<R> return_value(R (*)(A1, A2, A3, A4)) { return return_value_select<R>(); }

template <class R, class A1, class A2, class A3, class A4, class A5>
return_value_select<R> return_value(R (*)(A1, A2, A3, A4, A5)) { return return_value_select<R>(); }

template <class R, class A1, class A2, class A3, class A4, class A5, class A6>
return_value_select<R> return_value(R (*)(A1, A2, A3, A4, A5, A6)) { return return_value_select<R>(); }

template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
return_value_select<R> return_value(R (*)(A1, A2, A3, A4, A5, A6, A7)) { return return_value_select<R>(); }

template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
return_value_select<R> return_value(R (*)(A1, A2, A3, A4, A5, A6, A7, A8)) { return return_value_select<R>(); }

template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
return_value_select<R> return_value(R (*)(A1, A2, A3, A4, A5, A6, A7, A8, A9)) { return return_value_select<R>(); }

template <class R, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
return_value_select<R> return_value(R (*)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10)) { return return_value_select<R>(); }

// TODO(?): handle 'const void'

// member functions
template <class R, class T>
return_value_select<R> return_value(R (T::*)()) { return return_value_select<R>(); }

template <class R, class T, class A1>
return_value_select<R> return_value(R (T::*)(A1)) { return return_value_select<R>(); }

template <class R, class T, class A1, class A2>
return_value_select<R> return_value(R (T::*)(A1, A2)) { return return_value_select<R>(); }

template <class R, class T, class A1, class A2, class A3>
return_value_select<R> return_value(R (T::*)(A1, A2, A3)) { return return_value_select<R>(); }

template <class R, class T, class A1, class A2, class A3, class A4>
return_value_select<R> return_value(R (T::*)(A1, A2, A3, A4)) { return return_value_select<R>(); }

template <class R, class T, class A1, class A2, class A3, class A4, class A5>
return_value_select<R> return_value(R (T::*)(A1, A2, A3, A4, A5)) { return return_value_select<R>(); }

template <class R, class T, class A1, class A2, class A3, class A4, class A5, class A6>
return_value_select<R> return_value(R (T::*)(A1, A2, A3, A4, A5, A6)) { return return_value_select<R>(); }

template <class R, class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
return_value_select<R> return_value(R (T::*)(A1, A2, A3, A4, A5, A6, A7)) { return return_value_select<R>(); }

template <class R, class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
return_value_select<R> return_value(R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8)) { return return_value_select<R>(); }

template <class R, class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
return_value_select<R> return_value(R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9)) { return return_value_select<R>(); }

template <class R, class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
return_value_select<R> return_value(R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10)) { return return_value_select<R>(); }

template <class R, class T>
return_value_select<R> return_value(R (T::*)() const) { return return_value_select<R>(); }

template <class R, class T, class A1>
return_value_select<R> return_value(R (T::*)(A1) const) { return return_value_select<R>(); }

template <class R, class T, class A1, class A2>
return_value_select<R> return_value(R (T::*)(A1, A2) const) { return return_value_select<R>(); }

template <class R, class T, class A1, class A2, class A3>
return_value_select<R> return_value(R (T::*)(A1, A2, A3) const) { return return_value_select<R>(); }

template <class R, class T, class A1, class A2, class A3, class A4>
return_value_select<R> return_value(R (T::*)(A1, A2, A3, A4) const) { return return_value_select<R>(); }

template <class R, class T, class A1, class A2, class A3, class A4, class A5>
return_value_select<R> return_value(R (T::*)(A1, A2, A3, A4, A5) const) { return return_value_select<R>(); }

template <class R, class T, class A1, class A2, class A3, class A4, class A5, class A6>
return_value_select<R> return_value(R (T::*)(A1, A2, A3, A4, A5, A6) const) { return return_value_select<R>(); }

template <class R, class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
return_value_select<R> return_value(R (T::*)(A1, A2, A3, A4, A5, A6, A7) const) { return return_value_select<R>(); }

template <class R, class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
return_value_select<R> return_value(R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8) const) { return return_value_select<R>(); }

template <class R, class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
return_value_select<R> return_value(R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9) const) { return return_value_select<R>(); }

template <class R, class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10>
return_value_select<R> return_value(R (T::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10) const) { return return_value_select<R>(); }

}} // namespace python::detail

#endif
