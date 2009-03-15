/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/generic/set_context.hpp>
#include "my_ports_with_context.hpp"


#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace df=boost::dataflow;

namespace boost { namespace dataflow { namespace extension {

    template<>
    struct set_context_impl<my_port_with_context_traits>
    {
        typedef void result_type;
        
        void operator()(my_port_with_context &context, my_object &object)
        {
        }
    };

}}}

BOOST_AUTO_TEST_CASE( test )
{
    my_object object;
    my_port_with_context port(object);
    
    df::set_context(port, object);
}