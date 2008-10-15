/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/blueprint/framework.hpp>

#include "../generic/my_framework.hpp"

namespace df = boost::dataflow;

typedef df::blueprint::framework<my_framework> my_blueprint_framework;
