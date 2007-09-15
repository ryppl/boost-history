/*
 * Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_REFLECTION_FACTORY_HPP
#define BOOST_REFLECTION_FACTORY_HPP
#include <boost/reflection/generic_constructor.hpp>
namespace boost {namespace reflections {
template <class T>
class factory : public generic_constructor<T> {
public:
  factory(T(*factory_func)())
  : factory_func_(factory_func) {}
  virtual T * create(void ** params) {return (*factory_func_)();}
private:
  T (*factory_func_)();
};
}}
#endif
