/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__CASTABLE_POLYMORPHIC_OBJECT_HPP
#define BOOST__DATAFLOW__BLUEPRINT__CASTABLE_POLYMORPHIC_OBJECT_HPP


#include <typeinfo>


namespace boost { namespace dataflow { namespace blueprint {

class castable_polymorphic_object
{
public:
    template<typename T>
    castable_polymorphic_object(const T &t)
        : m_type_info(typeid(t))
    {}
    virtual ~castable_polymorphic_object()
    {}
    template<typename T>
    const T &get_as() const
    {   return *reinterpret_cast<const T *>(get_ptr()); }
    const std::type_info &type_info() const
    {   return m_type_info; }
private:
    virtual const void * get_ptr() const = 0;
    const std::type_info &m_type_info;
};


} } } // namespace boost::dataflow::blueprint

#endif // BOOST__DATAFLOW__BLUEPRINT__CASTABLE_POLYMORPHIC_OBJECT_HPP
