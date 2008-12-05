/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_ENTITY_HPP
#define BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_ENTITY_HPP


#include <boost/dataflow/blueprint/castable_polymorphic_object.hpp>
#include <boost/dataflow/blueprint/framework_context_fwd.hpp>

namespace boost { namespace dataflow { namespace blueprint {

template<typename BlueprintFramework>
class framework_entity : public castable_polymorphic_object
{
    typedef blueprint::framework_context<BlueprintFramework> framework_context_type;
public:
    struct dataflow_traits
    {
        typedef BlueprintFramework framework;
    };
    
    framework_entity(framework_context<BlueprintFramework> &fo, const std::type_info &ti)
        : castable_polymorphic_object(ti)
        , m_framework_context(fo)
    {}
    framework_context_type &framework_context()
    {   return m_framework_context; }
private:
    framework_context_type &m_framework_context;
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_ENTITY_HPP
