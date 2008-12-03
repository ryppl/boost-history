/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/blueprint/framework.hpp>
#include <boost/dataflow/blueprint/framework_context.hpp>

#include "../generic/my_framework.hpp"
#include "../generic/my_framework_with_context.hpp"

namespace df = boost::dataflow;

typedef df::blueprint::framework<my_framework> my_blueprint_framework;
typedef df::blueprint::framework_context<my_blueprint_framework> my_blueprint_framework_context;

typedef df::blueprint::framework<my_framework_with_context> my_blueprint_framework_with_context;
typedef df::blueprint::framework_context<my_blueprint_framework_with_context> my_blueprint_framework_with_context_context;
