//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Alejandro Cabrera 2011.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bloom_filter for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include "detail/pow.hpp"
#include "detail/generator.hpp"
#include "detail/benchmark.hpp"

#include <boost/bloom_filter/bloom.hpp>
#include <boost/progress.hpp>
#include <iostream>
using namespace std;
using boost::detail::Pow;
using boost::detail::benchmark;
using boost::detail::generator;
using boost::bloom_filter::bloom_filter;

int main()
{
  static const size_t REPEAT = Pow<10, 3>::val; // 1000 
  static const size_t OPS = Pow<10, 6>::val; // 1,000,000 inserts
  static const size_t BITS = Pow<2, 21>::val; // 2MB

  boost::progress_display progress(REPEAT);
  double total_time = 0.0;

  for (size_t i = 0; i < REPEAT; ++i, ++progress) {
    benchmark<int, OPS, 
	      generator<int>,
	      bloom_filter<int, BITS> > bench;    

    bench.run();

    total_time += bench.time();
  }

  cout << REPEAT << " trials of " << OPS << " insertions took " 
       << total_time << " seconds" << endl;

  return 0;
}
