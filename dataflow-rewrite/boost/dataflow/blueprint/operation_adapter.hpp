/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__OPERATION_ADAPTER_HPP
#define BOOST__DATAFLOW__BLUEPRINT__OPERATION_ADAPTER_HPP


#include <boost/dataflow/blueprint/operation.hpp>


namespace boost { namespace dataflow { namespace blueprint {

template<typename Operation>
class operation_adapter : public operation
{
public:
    operation_adapter(const Operation &op = Operation())
        : operation(op)
        , m_operation(op)
    {}
private:
    virtual void * get_ptr()
    {
        return &m_operation;
    };
    Operation m_operation;
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST__DATAFLOW__BLUEPRINT__OPERATION_ADAPTER_HPP
