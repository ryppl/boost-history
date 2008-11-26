/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__TEST__GENERIC__MY_FRAMEWORK_WITH_OBJECT_HPP
#define BOOST__DATAFLOW__TEST__GENERIC__MY_FRAMEWORK_WITH_OBJECT_HPP


#include <boost/dataflow/generic/framework.hpp>


struct my_object
{};

struct my_framework_with_object
    : public df::framework<void, my_object>
{};


#endif // BOOST__DATAFLOW__TEST__GENERIC__MY_FRAMEWORK_WITH_OBJECT_HPP