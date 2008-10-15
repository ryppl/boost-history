/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__TEST__GENERIC__MY_FRAMEWORK_HPP
#define BOOST__DATAFLOW__TEST__GENERIC__MY_FRAMEWORK_HPP


#include <boost/dataflow/generic/framework.hpp>
#include <boost/mpl/vector.hpp>


namespace df=boost::dataflow;

struct my_framework : public df::framework<boost::mpl::vector<> >
{};


#endif // BOOST__DATAFLOW__TEST__GENERIC__MY_FRAMEWORK_HPP