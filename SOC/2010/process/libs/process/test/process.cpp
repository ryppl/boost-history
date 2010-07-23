// 
// Boost.Process 
// ~~~~~~~~~~~~~ 
// 
// Copyright (c) 2006, 2007 Julio M. Merino Vidal 
// Copyright (c) 2008, 2009 Boris Schaeling
// Copyright (c) 2010 Felipe Tanus, Boris Schaeling
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying 
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) 
// 

#include "process.hpp" 
#include <boost/mpl/list.hpp> 
#include <boost/mpl/pair.hpp> 

namespace { 

class launcher 
{ 
public: 
    bp::process operator()(bp::pid_type id) const 
    { 
        return bp::process(id); 
    } 
}; 

} 

namespace mpl = boost::mpl; 

typedef mpl::list< 
    mpl::pair<bp::process, launcher> 
> test_types; 

BOOST_AUTO_TEST_CASE_TEMPLATE(process_test_case, T, test_types) 
{ 
    using namespace process_test; 
    test_getters<T::first, T::second>(); 
    test_terminate<T::first, T::second>(); 
} 
