/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__OPERATION_HPP
#define BOOST__DATAFLOW__BLUEPRINT__OPERATION_HPP


#include <boost/dataflow/blueprint/castable_polymorphic_object.hpp>
#include <boost/dataflow/generic/operation.hpp>
#include <typeinfo>


namespace boost { namespace dataflow { namespace blueprint {

class operation : public castable_polymorphic_object, public dataflow::operation<semantics::unknown>
{
public:
    template<typename Operation>
    operation(const Operation &operation)
        : castable_polymorphic_object(typeid(operation))
    {}
};


} } } // namespace boost::dataflow::blueprint

#endif // BOOST__DATAFLOW__BLUEPRINT__OPERATION_HPP
