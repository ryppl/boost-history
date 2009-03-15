/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__PORT_HPP
#define BOOST__DATAFLOW__BLUEPRINT__PORT_HPP


#include <boost/dataflow/blueprint/framework_entity.hpp>
#include <boost/dataflow/generic/port/category.hpp>

namespace boost { namespace dataflow { namespace blueprint {

template<typename BlueprintFramework>
class port : public framework_entity<BlueprintFramework>
{
    typedef framework_entity<BlueprintFramework> base_type;
public:
    port(blueprint::framework_context<BlueprintFramework> &fo, const std::type_info &ti)
        : base_type(fo, ti)
    {}
    struct dataflow_traits : public base_type::dataflow_traits
    {
        typedef ports::producer_consumer category;
    };
    const std::type_info &category_type_info() const
    {   return *m_category_type_info; }
protected:
    void set_category_type_info(const std::type_info &ti)
    {   m_category_type_info = &ti; }
private:
    const std::type_info *m_category_type_info;
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST__DATAFLOW__BLUEPRINT__PORT_HPP
