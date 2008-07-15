/*
 * Copyright Jeremy Pack 2008
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */


// No header guard - this file is intended to be included multiple times.

# define N BOOST_PP_ITERATION()
// Free functions. These should only be used internally by the
// reflector class to generate function pointers.
//
// This is a generic factory function to construct an instance of
// a given class using a constructor with the given signature.
template <class T BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, class Param)>
static instance construct_instance(BOOST_PP_ENUM_BINARY_PARAMS(N, Param, p)) {
  // An instance is similar to boost::any. Initialize it with
  // a void ptr.
  return instance(static_cast<void*>(
    construct<T BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, Param)>
     (BOOST_PP_ENUM_PARAMS(N, p))),
                  &destruct<T>);
}

// This auxiliary function is used to call a member function on
// a given instance, assuming the instance is of type T.
template <class T, class ReturnValue BOOST_PP_COMMA_IF(N)
          BOOST_PP_ENUM_PARAMS(N, class Param)>
static ReturnValue call_member(void * val,
                               impl::MemberFunctionPtr member_function
                               BOOST_PP_COMMA_IF(N)
                               BOOST_PP_ENUM_BINARY_PARAMS(N, Param, p)) {
  // Convert to a T*.
  T * actual = static_cast<T*>(val);

  // Convert the MemberFunctionPtr to the requested type.
  ReturnValue (T::*func)(BOOST_PP_ENUM_PARAMS(N, Param)) =
    reinterpret_cast<ReturnValue (T::*)(BOOST_PP_ENUM_PARAMS(N, Param))>
      (member_function);

  // Call the function and return the result.
  return (actual->*func)(BOOST_PP_ENUM_PARAMS(N, p));
}
#undef N
