/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include "my_blueprint_framework.hpp"
#include "../generic/my_static_vector.hpp"
#include <boost/dataflow/utility/containing_ptr.hpp>

#include <boost/dataflow/blueprint/static_vector_adapter.hpp>

typedef
    df::blueprint::static_vector_adapter
    <
        my_blueprint_framework,
        boost::dataflow::utility::containing_ptr<my_static_vector>
    >
        my_blueprint_static_vector;