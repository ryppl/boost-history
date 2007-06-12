// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/signal_network/storage.hpp>

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
    signet::storage<void ()>::unfused banger;
	signet::storage<void (std::vector<float> &)>::unfused floater;
	signet::storage<void (std::vector<float> &)>::unfused collector;
    
	// create the network
	banger >>= floater >>= collector;
    
    start = clock();
    for (int i=0; i<iter; i++)
    {
        v[0] = i;
        floater(v);
        banger();
        assert(collector.at<0>()[0] == i);
    }
    finish = clock();
    std::cout << "unfused:" << double(finish - start) / CLOCKS_PER_SEC << std::endl;
    
    boost::fusion::vector<const std::vector<float> &> fv(v);
    
    signet::storage<void ()> fbanger;
    signet::storage<void (std::vector<float> &)> ffloater;
    signet::storage<void (std::vector<float> &)> fcollector;
    
    fbanger >>= ffloater >>= fcollector;
    
    start = clock();
    for (int i=0; i<iter; i++)
    {
        v[0] = i;
        ffloater(fv);
        fbanger();
        assert(fcollector.at<0>()[0] == i);
    }
    finish = clock();
    
    std::cout << "fused:" << double(finish - start) / CLOCKS_PER_SEC << std::endl;

    return 0;
} // int main()