// Copyright Justinas Vygintas Daugmaudis, 2010.
// Use, modification and distribution is subject to the
// Boost Software License, Version 1.0. (See accompanying
// file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)

#ifndef IDENTITY_DISTRIBUTION_HPP_INCLUDED
#define IDENTITY_DISTRIBUTION_HPP_INCLUDED

// To be used when no transformation of a sequence is required
template<typename T = double>
struct identity_distribution
{
  typedef T result_type;
  template<typename Generator>
  result_type operator()(Generator& gen) const
  { return gen(); }
};

#endif // IDENTITY_DISTRIBUTION_HPP_INCLUDED
