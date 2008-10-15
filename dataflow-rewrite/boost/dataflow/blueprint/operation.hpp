/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__OPERATION_HPP
#define BOOST__DATAFLOW__BLUEPRINT__OPERATION_HPP


#include <boost/dataflow/generic/operation.hpp>
#include <typeinfo>


namespace boost { namespace dataflow { namespace blueprint {

class operation : public dataflow::operation<semantics::unknown>
{
public:
    template<typename Operation>
    operation(const Operation &operation)
        : m_type_info(typeid(operation))
    {}
    virtual ~operation()
    {}
    template<typename Operation>
    const Operation &get_as() const
    {
        return *reinterpret_cast<const Operation *>(get_ptr());
    }
    const std::type_info &type_info() const
    {   return m_type_info; }
private:
    virtual const void * get_ptr() const = 0;
    const std::type_info &m_type_info;
};


} } } // namespace boost::dataflow::blueprint

#endif // BOOST__DATAFLOW__BLUEPRINT__OPERATION_HPP
