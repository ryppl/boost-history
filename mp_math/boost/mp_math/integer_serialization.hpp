// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_SERIALIZATION_HPP
#define BOOST_MP_MATH_INTEGER_SERIALIZATION_HPP

#include <boost/mp_math/integer/gmp_integer_fwd.hpp>
#include <boost/mp_math/integer/integer_fwd.hpp>
#include <boost/mp_math/integer/libtom_integer_fwd.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/string.hpp>


namespace boost {
namespace serialization {


template<class Archive, class A, class T>
void save(Archive& ar,
          const boost::mp_math::unbounded_int<A,T>& x,
          const unsigned int /*version*/)
{
  const std::string s = x.template to_string<std::string>(std::ios_base::hex);
  ar & s;
}

template<class Archive, class A, class T>
void load(Archive& ar,
          boost::mp_math::unbounded_int<A,T>& x,
          const unsigned int /*version*/)
{
  std::string s;
  ar & s;
  x.template assign(s, std::ios_base::hex);
}

template<class Archive, class A, class T>
inline void serialize(Archive& ar,
                      boost::mp_math::unbounded_int<A,T>& x,
                      const unsigned int version)
{
  split_free(ar, x, version);
}

template<class Archive, class A, class T>
void save(Archive& ar,
          const boost::mp_math::unbounded_uint<A,T>& x,
          const unsigned int /*version*/)
{
  const std::string s = x.template to_string<std::string>(std::ios_base::hex);
  ar & s;
}

template<class Archive, class A, class T>
void load(Archive& ar,
          boost::mp_math::unbounded_uint<A,T>& x,
          const unsigned int /*version*/)
{
  std::string s;
  ar & s;
  x.template assign(s, std::ios_base::hex);
}

template<class Archive, class A, class T>
inline void serialize(Archive& ar,
                      boost::mp_math::unbounded_uint<A,T>& x,
                      const unsigned int version)
{
  split_free(ar, x, version);
}

template<class Archive, bool S, class A, class T>
inline void serialize(Archive & ar,
                      boost::mp_math::unbounded<S,A,T>& x,
                      const unsigned int /*version*/)
{
  ar & base_object<typename boost::mp_math::unbounded<S,A,T>::base_type>(x);
}

template<class Archive, class Type>
inline void serialize(Archive & ar,
                      boost::mp_math::integer<Type>& x,
                      const unsigned int /*version*/)
{
  ar & base_object<Type>(x);
}

template<class Archive, class B>
void save(Archive& ar,
          const boost::mp_math::gmp_integer<B>& x,
          const unsigned int /*version*/)
{
  const std::string s = x.template to_string<std::string>(std::ios_base::hex);
  ar & s;
}

template<class Archive, class B>
void load(Archive& ar,
          boost::mp_math::gmp_integer<B>& x,
          const unsigned int /*version*/)
{
  std::string s;
  ar & s;
  x.template assign(s, std::ios_base::hex);
}

template<class Archive, class B>
inline void serialize(Archive& ar,
                      boost::mp_math::gmp_integer<B>& x,
                      const unsigned int version)
{
  split_free(ar, x, version);
}

template<class Archive, class B>
void save(Archive& ar,
          const boost::mp_math::libtom_integer<B>& x,
          const unsigned int /*version*/)
{
  const std::string s = x.template to_string<std::string>(std::ios_base::hex);
  ar & s;
}

template<class Archive, class B>
void load(Archive& ar,
          boost::mp_math::libtom_integer<B>& x,
          const unsigned int /*version*/)
{
  std::string s;
  ar & s;
  x.template assign(s, std::ios_base::hex);
}

template<class Archive, class B>
inline void serialize(Archive& ar,
                      boost::mp_math::libtom_integer<B>& x,
                      const unsigned int version)
{
  split_free(ar, x, version);
}


} // namespace serialization
} // namespace boost

#endif

