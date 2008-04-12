/*
 * Boost.Extension / factory:
 *         factory to register the implementations and create them
 *
 * (C) Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_PP_IS_ITERATING


#ifndef BOOST_EXTENSION_FACTORY_HPP
#define BOOST_EXTENSION_FACTORY_HPP

#include <boost/extension/common.hpp>

namespace boost{namespace extensions{


#define N BOOST_EXTENSION_MAX_FUNCTOR_PARAMS

template <class T, class D
          BOOST_PP_COMMA_IF(N)
          BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT( \
            BOOST_PP_INC(N), class Param, void) >
struct create_function;

#undef N


#define N BOOST_EXTENSION_MAX_FUNCTOR_PARAMS

template <class T
          BOOST_PP_COMMA_IF(N)
          BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(
              BOOST_PP_INC(N), class Param, void) >
class factory;

#undef N


// generate specializations of create_function and factory
# define BOOST_PP_ITERATION_LIMITS (0, BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS) - 1)
# define BOOST_PP_FILENAME_1 <boost/extension/factory.hpp> // this file
# include BOOST_PP_ITERATE()


}} // namespace boost::extension

#endif // BOOST_EXTENSION_FACTORY_HPP




#else // BOOST_PP_IS_ITERATING


// for convenience
#define n BOOST_PP_ITERATION()



template <
  class T,
  class D
  BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, class Param)
>
struct create_function<
  T,
  D
  BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, Param)
>
{
  static T * create(BOOST_PP_ENUM_BINARY_PARAMS(n, Param, p) )
  {
    return new D(BOOST_PP_ENUM_PARAMS(n, p));
  }
};



template <class T BOOST_PP_COMMA_IF(n)  BOOST_PP_ENUM_PARAMS(n, class Param) >
class factory<T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, Param) >
{
public:

  template <class D>
  void set()
  {
    this->func = &create_function<
        T, D BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n,Param)
      >::create;
  }

  factory() : func(0) {}

  factory(factory<T> const& first) : func(first.func) {}

  factory& operator=(factory<T> const& first)
  {
    this->func = first->func;
    return *this;
  }

  bool is_valid() { return this->func != 0; }

  T* create(BOOST_PP_ENUM_BINARY_PARAMS(n, Param, p))
  {
    if (this->func)
    {
      return this->func(BOOST_PP_ENUM_PARAMS(n, p));
    }
    else
    {
      return 0;
    }
  }

private:

  typedef T* (*func_ptr_type)(BOOST_PP_ENUM_PARAMS(n, Param));
  func_ptr_type func;

};


#undef n


#endif // BOOST_PP_IS_ITERATING
