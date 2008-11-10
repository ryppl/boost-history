/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_HPP
#define BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_HPP


#include <boost/dataflow/blueprint/framework_object_fwd.hpp>
#include <boost/dataflow/blueprint/operation.hpp>
#include <boost/dataflow/generic/framework.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/transform.hpp>


namespace boost { namespace dataflow { namespace blueprint {

template<typename Framework> struct framework : public Framework
{
    typedef Framework framework_type;
    typedef framework_object<Framework> framework_object_type;
    typedef typename has_framework_object<Framework>::type framework_has_object;
    typedef mpl::vector<operation> operations;
};


template<typename T>
struct is_blueprint_framework
    : public mpl::false_
{};

template<typename Framework>
struct is_blueprint_framework<framework<Framework> >
    : public mpl::true_
{};

} } } // namespace boost::dataflow::blueprint


#endif // BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_HPP
