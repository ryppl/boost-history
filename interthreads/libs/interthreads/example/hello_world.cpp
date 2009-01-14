//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Roland Schwarz 2006. 
// (C) Copyright Vicente J. Botet Escriba 2008-20009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
// Based on the shared.cpp example from the threadalert library of Roland Schwarz 
//////////////////////////////////////////////////////////////////////////////
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/interthreads/typeof/threader_decorator.hpp>

boost::mutex out_global_mutex;

#include <iostream>
#include <boost/interthreads/thread_decorator.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>
        
namespace bith = boost::interthreads;

void my_setup() {
    std::cout << "Hello World!" << std::endl;
}

void my_cleanup() {
    std::cout << "Bye, Bye!" << std::endl;
}

bith::thread_decoration my_decoration(my_setup, my_cleanup);

void my_thread() {
    std::cout << "..." << std::endl;
}

    
int main() {
    boost::thread th(bith::make_decorator(my_thread));
    th.join();
    return 0;
}
