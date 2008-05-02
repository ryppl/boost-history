/*
 * Boost.Extension / factory map:
 *         map of factories (for the implementations)
 *
 * (C) Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_EXTENSION_FACTORY_MAP_HPP
#define BOOST_EXTENSION_FACTORY_MAP_HPP

#include <map>
#include <boost/extension/factory.hpp>
#include <boost/extension/impl/typeinfo.hpp>

namespace boost {
namespace extensions {

template <class TypeInfo>
class basic_factory_map
{

public:

  // generate get and conversion template member functions from the
  // specification in impl/
# define BOOST_PP_ITERATION_LIMITS (0, \
    BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS) - 1)
# define BOOST_PP_FILENAME_1 <boost/extension/impl/factory_map.hpp>
# include BOOST_PP_ITERATE()

  ~basic_factory_map() {
    for (typename std::map<TypeInfo, generic_map_holder*>::iterator
         it =maps_.begin(); it != maps_.end(); ++it) {
      delete it->second;
    }
  }

private:

  struct generic_map_holder {
    virtual ~generic_map_holder() {}
  };

  template <class T>
  struct map_holder : generic_map_holder, T {};

  std::map<TypeInfo, generic_map_holder*> maps_;
};

typedef basic_factory_map<default_type_info> factory_map;

} // namespace extensions
} // namespace boost

#endif  // BOOST_EXTENSION_FACTORY_MAP_HPP

