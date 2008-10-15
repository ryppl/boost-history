/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_HPP
#define BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_HPP


#include <boost/mpl/bool.hpp>


namespace boost { namespace dataflow { namespace blueprint {

template<typename BlueprintFramework>
class framework_object;

template<typename Framework> struct framework : public Framework
{
    typedef Framework framework_type;
    typedef framework_object<Framework> framework_object_type;
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
