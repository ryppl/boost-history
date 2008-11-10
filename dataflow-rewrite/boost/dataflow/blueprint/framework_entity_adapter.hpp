/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_ENTITY_ADAPTER_HPP
#define BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_ENTITY_ADAPTER_HPP


#include <boost/dataflow/blueprint/framework_entity.hpp>
#include <boost/dataflow/blueprint/framework_object.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/enable_if.hpp>


namespace boost { namespace dataflow { namespace blueprint {

template<typename BlueprintFramework, typename EntityOrRef, typename Base=framework_entity<BlueprintFramework>, typename Enable=void >
class framework_entity_adapter : public Base
{
public:
    typedef typename remove_reference<EntityOrRef>::type entity_type;
    
    framework_entity_adapter(framework_object<BlueprintFramework> &fo)
        : Base(fo, typeid(m_entity))
    {}
    template<typename T>
    framework_entity_adapter(framework_object<BlueprintFramework> &fo, const T &t)
        : Base(fo, typeid(m_entity))
        , m_entity(t)
    {}
    template<typename T>
    framework_entity_adapter(framework_object<BlueprintFramework> &fo, T &t)
        : Base(fo, typeid(m_entity))
        , m_entity(t)
    {}
private:
    virtual void *get_ptr()
    {
        return &m_entity;
    };
    EntityOrRef m_entity;
};

template<typename BlueprintFramework, typename EntityOrRef, typename Base>
class framework_entity_adapter<BlueprintFramework, EntityOrRef, Base,
    typename enable_if<typename BlueprintFramework::framework_has_object>::type>
    : public Base
{
public:
    typedef typename remove_reference<EntityOrRef>::type entity_type;
    
    framework_entity_adapter(framework_object<BlueprintFramework> &fo)
        : Base(fo, typeid(m_entity))
        , m_entity(fo.object())
    {}
    template<typename T>
    framework_entity_adapter(framework_object<BlueprintFramework> &fo, const T &t)
        : Base(fo, typeid(m_entity))
        , m_entity(t)
    {}
    template<typename T>
    framework_entity_adapter(framework_object<BlueprintFramework> &fo, T &t)
        : Base(fo, typeid(m_entity))
        , m_entity(t)
    {}
private:
    virtual void *get_ptr()
    {
        return &m_entity;
    };
    EntityOrRef m_entity;
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_ENTITY_ADAPTER_HPP
