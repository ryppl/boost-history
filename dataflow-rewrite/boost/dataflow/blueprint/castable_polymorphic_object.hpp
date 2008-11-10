/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__CASTABLE_POLYMORPHIC_OBJECT_HPP
#define BOOST__DATAFLOW__BLUEPRINT__CASTABLE_POLYMORPHIC_OBJECT_HPP


#include <typeinfo>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_const.hpp>


namespace boost { namespace dataflow { namespace blueprint {

class castable_polymorphic_object
{
public:
    castable_polymorphic_object(const std::type_info &ti)
        : m_type_info(ti)
    {}
    virtual ~castable_polymorphic_object()
    {}
    
    template<typename T>
    typename disable_if<is_const<T>, T &>::type get_as()
    {   return *reinterpret_cast<T *>(get_ptr()); }
    
    template<typename T>
    typename enable_if<is_const<T>, const T &>::type get_as() const
    {
        return *reinterpret_cast<typename remove_const<T>::type *>(
            const_cast<castable_polymorphic_object *>(this)->get_ptr());
    }
    
    const std::type_info &type_info() const
    {   return m_type_info; }
private:
    virtual void * get_ptr() = 0;
    const std::type_info &m_type_info;
};


} } } // namespace boost::dataflow::blueprint

#endif // BOOST__DATAFLOW__BLUEPRINT__CASTABLE_POLYMORPHIC_OBJECT_HPP
