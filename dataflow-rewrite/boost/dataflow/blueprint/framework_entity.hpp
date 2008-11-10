/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_ENTITY_HPP
#define BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_ENTITY_HPP


#include <boost/dataflow/blueprint/castable_polymorphic_object.hpp>
#include <boost/dataflow/blueprint/framework_object_fwd.hpp>

namespace boost { namespace dataflow { namespace blueprint {

template<typename BlueprintFramework>
class framework_entity : public castable_polymorphic_object
{
    typedef framework_object<BlueprintFramework> framework_object_type;
public:
    struct dataflow_traits
    {
        typedef BlueprintFramework framework;
    };
    
    framework_entity(framework_object<BlueprintFramework> &fo, const std::type_info &ti)
        : castable_polymorphic_object(ti)
        , m_framework_object(fo)
    {}
    framework_object_type &framework_object()
    {   return m_framework_object; }
private:
    framework_object_type &m_framework_object;
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_ENTITY_HPP
