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


#ifndef BOOST_PP_IS_ITERATING




#ifndef BOOST_EXTENSION_FACTORY_MAP_HPP
#define BOOST_EXTENSION_FACTORY_MAP_HPP

#include <map>
#include <boost/extension/factory.hpp>
#include <boost/extension/impl/typeinfo.hpp>

namespace boost{namespace extensions{

template <class TypeInfo>
class basic_factory_map
{

public:

  // generate get and conversion template member functions from the
  // specification at the end of this file
# define BOOST_PP_ITERATION_LIMITS (0, \
    BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS) - 1)
# define BOOST_PP_FILENAME_1 <boost/extension/factory_map.hpp> // this file
# include BOOST_PP_ITERATE()

  ~basic_factory_map()
  {
    for (typename std::map<TypeInfo, generic_map_holder*>::iterator
         it =maps_.begin(); it != maps_.end(); ++it)
    {
      delete it->second;
    }
  }

private:

  struct generic_map_holder
  {
    virtual ~generic_map_holder() {}
  };

  template <class T>
  struct map_holder : generic_map_holder, T {};

  std::map<TypeInfo, generic_map_holder*> maps_;
};

typedef basic_factory_map<default_type_info> factory_map;

}} // namespace boost::extensions

// Note! This is the end of the include guard
#endif // BOOST_EXTENSION_FACTORY_MAP_HPP




#else // BOOST_PP_IS_ITERATING



// for convenience
# define n BOOST_PP_ITERATION()


template <class Interface, class Info
          BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, class Param) >
std::map<
  Info,
  factory<Interface  BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, Param) >
  > &
get()
{
  typedef Interface* (* func_ptr_type )(BOOST_PP_ENUM_PARAMS(n, Param));
  typedef type_info_handler<TypeInfo, func_ptr_type> handler_type;

  TypeInfo t = handler_type::get_class_type();

  typename std::map<TypeInfo, generic_map_holder*>::iterator
  it = maps_.find(t);

  typedef factory<
    Interface
    BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, Param)
  > factory_type;
  typedef std::map<Info, factory_type> map_type;

  map_holder<map_type>* holder;
  if (it == maps_.end())
  {
    holder = new map_holder<map_type>;
    it = maps_.insert(std::make_pair(t, holder)).first;
  }
  else
  {
    holder = static_cast<map_holder<map_type>* > (it->second);
  }

  return *(static_cast<map_type* >(holder));
}


template <class Interface, class Info
          BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, class Param) >
operator
  std::map<
    Info,
    factory<Interface BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, Param) >
  >
  ()
{
  return get< Interface, Info
             BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, Param)>();
}


# undef n



#endif // BOOST_PP_IS_ITERATING
