/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/blueprint/framework.hpp>
#include <boost/dataflow/blueprint/framework_context.hpp>
#include <boost/dataflow/blueprint/static_vector_adapter.hpp>
#include <boost/dataflow/managed/io_component.hpp>

#include <boost/scoped_ptr.hpp>


namespace df = boost::dataflow;

typedef df::blueprint::framework<df::managed::framework> managed_blueprint_framework;
typedef df::blueprint::framework_context<managed_blueprint_framework> managed_blueprint_framework_context;

struct component : public df::managed::io_component<int, int>
{
};

typedef
    df::blueprint::static_vector_adapter
    <
        managed_blueprint_framework,
        boost::scoped_ptr<component>
    >
        managed_blueprint_static_vector;
        

int main()
{
    managed_blueprint_framework_context fo;
    managed_blueprint_static_vector vector(fo, new component());

    return 0;
}