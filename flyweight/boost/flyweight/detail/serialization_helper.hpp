/* Copyright 2006-2008 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#ifndef BOOST_FLYWEIGHT_DETAIL_SERIALIZATION_HELPER_HPP
#define BOOST_FLYWEIGHT_DETAIL_SERIALIZATION_HELPER_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/noncopyable.hpp>
#include <boost/serialization/extended_type_info.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace boost{

namespace flyweights{

namespace detail{

/* The serialization helpers for flyweight<T> map numerical IDs to
 * flyweight exemplars --an exemplar is the flyweight object
 * associated to a given value that appears first on the serialization
 * stream, so that subsequent equivalent flyweight objects will be made
 * to refer to it during the serialization process.
 */

template<typename Flyweight>
struct flyweight_value_address
{
  typedef const typename Flyweight::value_type* result_type;

  result_type operator()(const Flyweight& x)const{return &x.get();}
};

template<typename Flyweight>
class save_helper:private noncopyable
{
  typedef multi_index::multi_index_container<
    Flyweight,
    multi_index::indexed_by<
      multi_index::random_access<>,
      multi_index::hashed_unique<flyweight_value_address<Flyweight> >
    >
  > table;

public:

  typedef typename table::size_type size_type;

  size_type size()const{return t.size();}

  size_type find(const Flyweight& x)const
  {
    return multi_index::project<0>(t,multi_index::get<1>(t).find(&x.get()))
             -t.begin();
  }

  void push_back(const Flyweight& x){t.push_back(x);}
  
private:
  table t;
};

template<typename Flyweight>
class load_helper:private noncopyable
{
  typedef std::vector<Flyweight> table;

public:

  typedef typename table::size_type size_type;

  size_type size()const{return t.size();}

  Flyweight operator[](size_type n)const{return t[n];}

  void push_back(const Flyweight& x){t.push_back(x);}
  
private:
  table t;
};

template<typename Helper,class Archive>
Helper& get_serialization_helper(
  Archive& ar
  BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Helper))
{
  serialization::extended_type_info* eti=
    serialization::type_info_implementation<Helper>::type::get_instance();
  shared_ptr<void>                   sph;
  ar.lookup_helper(eti,sph);
  if(!sph.get()){
      sph=shared_ptr<Helper>(new Helper);
      ar.insert_helper(eti, sph);
  }
  return *static_cast<Helper*>(sph.get());
}

} /* namespace flyweights::detail */

} /* namespace flyweights */

} /* namespace boost */

#endif
