//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/async for documentation.
//
// Based on the shared.cpp example from the threadalert library of Roland Schwarz 
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <boost/async/typeof/threader.hpp>
#include <boost/function.hpp>
        
namespace basync = boost::async;

void my_thread() {
    std::cout << "Hello World!" << std::endl;
}
    
int main() {
    basync::shared_threader ae;
    BOOST_AUTO(act, basync::fork(ae, my_thread));
    basync::join(act);
    return 0;
}
