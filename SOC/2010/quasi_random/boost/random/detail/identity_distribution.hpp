// Copyright Justinas Vygintas Daugmaudis, 2010.
// Use, modification and distribution is subject to the
// Boost Software License, Version 1.0. (See accompanying
// file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)

#ifndef BOOST_IDENTITY_DISTRIBUTION_HPP_INCLUDED
#define BOOST_IDENTITY_DISTRIBUTION_HPP_INCLUDED

namespace boost {

namespace random {

namespace detail {

// To be used when no transformation of a sequence is required
struct identity_distribution
{
  // makes compatible with result_of<>
  template<typename FArgs>
  struct result;

  template<typename F, typename Engine>
  struct result<F(Engine)>
  {
    typedef typename Engine::result_type type;
  };

  template<typename Engine>
  typename Engine::result_type operator()(Engine& gen) const
  { return gen(); }
};

}}} // namespace boost::random::detail

#endif // BOOST_IDENTITY_DISTRIBUTION_HPP_INCLUDED
