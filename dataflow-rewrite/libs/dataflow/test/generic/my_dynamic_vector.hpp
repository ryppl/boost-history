/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__TEST__GENERIC__MY_DYNAMIC_VECTOR_HPP
#define BOOST__DATAFLOW__TEST__GENERIC__MY_DYNAMIC_VECTOR_HPP


#include <boost/dataflow/generic/dynamic_vector.hpp>
#include "my_ports.hpp"
#include <vector>

typedef df::dynamic_vector_traits<my_port_producer> my_dynamic_vector_traits;

namespace df=boost::dataflow;

struct my_dynamic_vector
{
    my_dynamic_vector()
        : ports(2) {}
    
    typedef my_dynamic_vector_traits dataflow_traits;
    std::vector<my_port_producer> ports;
};

namespace boost { namespace dataflow { namespace extension {

template<>
struct entities_impl<my_dynamic_vector_traits>
{
    typedef std::vector<my_port_producer> & result_type;

    template<typename Entity>
    result_type operator()(Entity &e)
    {
        return e.ports;
    }
};

}}}

#endif // BOOST__DATAFLOW__TEST__GENERIC__MY_DYNAMIC_VECTOR_HPP