/*
 * Boost.Extension / functor:
 *        functor used as the exported function of the libraries (the one that
 *        registers the implementations at library loading time)
 *
 * (C) Copyright Jeremy Pack 2008
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_PP_IS_ITERATING



#ifndef BOOST_EXTENSION_FUNCTOR_HPP
#define BOOST_EXTENSION_FUNCTOR_HPP

#include <boost/extension/common.hpp>
#include <boost/extension/impl/library_impl.hpp>



namespace boost { namespace extensions {

//using boost::extensions::detail::generic_function_ptr;


/// Declaration of functor class template.
template <class ReturnValue,
          BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT( \
            BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS), class Param, void) >
    class functor;

/// Functor template specializations.
# define BOOST_PP_ITERATION_LIMITS (0, BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS) - 1)
# define BOOST_PP_FILENAME_1 <boost/extension/functor.hpp> // this file
# include BOOST_PP_ITERATE()



}} // namespace boost::extensions

#endif  // BOOST_EXTENSION_FUNCTOR_HPP



#else // BOOST_PP_IS_ITERATING




# define n BOOST_PP_ITERATION()

template <class ReturnValue
         BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, class Param) >
class functor<ReturnValue
              BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, Param) >
{
private:

  typedef ReturnValue (*FunctionType)(BOOST_PP_ENUM_PARAMS(n, Param));
  FunctionType func_;

public:

  bool is_valid() const { return func_ != 0; }

  explicit functor(FunctionType func) : func_(func) {}

  explicit functor(generic_function_ptr func) : func_(FunctionType(func)) {}

  ReturnValue operator()(BOOST_PP_ENUM_BINARY_PARAMS(n, Param, p)) {
    return func_(BOOST_PP_ENUM_PARAMS(n, p));
  }
};

# undef n

#endif  // BOOST_PP_IS_ITERATING
