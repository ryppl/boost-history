// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/signal_network/storage.hpp>

#include <boost/fusion/sequence/generation/make_vector.hpp>

// for access to connection operators >>= and |
using namespace boost::signal_network;
using namespace boost;

#include <time.h>
#include <iostream>
#include <vector>

int main()
{
    clock_t start, finish;
    int iter = 1000000;
    
    std::vector<float> v(1);
    
	// instantiate all of the components we need
	signals::storage<void (std::vector<float> &, std::vector<float>), signals::unfused> floater;
	signals::storage<void (std::vector<float> &, std::vector<float>), signals::unfused> collector;
    
	// create the network
	floater >>= collector;
    
    start = clock();
    for (int i=0; i<iter; i++)
    {
        v[0] = i;
        floater(boost::fusion::make_vector(boost::ref(v), boost::ref(v)));
        floater.send();
        assert(collector.at<0>()[0] == i);
    }
    finish = clock();
    std::cout << "unfused:" << double(finish - start) / CLOCKS_PER_SEC << std::endl;
    
    boost::fusion::vector<std::vector<float> &> fv(boost::fusion::make_vector(boost::ref(v), boost::ref(v)));
    
    signals::storage<void (std::vector<float> &)> ffloater;
    signals::storage<void (std::vector<float> &)> fcollector;
    
    ffloater >>= fcollector;
    
    start = clock();
    for (int i=0; i<iter; i++)
    {
        v[0] = i;
        ffloater(fv);
        ffloater.send();
        assert(fcollector.at<0>()[0] == i);
    }
    finish = clock();
    
    std::cout << "fused:" << double(finish - start) / CLOCKS_PER_SEC << std::endl;

    return 0;
} // int main()