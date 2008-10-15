/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_ENTITY_ADAPTER_HPP
#define BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_ENTITY_ADAPTER_HPP


#include <boost/dataflow/blueprint/framework_entity.hpp>
#include <boost/type_traits/remove_reference.hpp>


namespace boost { namespace dataflow { namespace blueprint {

template<typename BlueprintFramework, typename EntityOrRef, typename Base=framework_entity<BlueprintFramework> >
class framework_entity_adapter : public Base
{
public:
    typedef typename remove_reference<EntityOrRef>::type entity_type;
    framework_entity_adapter()
        : Base(EntityOrRef())
    {}
    template<typename T>
    framework_entity_adapter(const T &t)
        : Base(t)
        , m_entity(t)
    {}
    template<typename T>
    framework_entity_adapter(T &t)
        : Base(t)
        , m_entity(t)
    {}
private:
    virtual const void *get_ptr() const
    {
        return &m_entity;
    };
    EntityOrRef m_entity;
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_ENTITY_ADAPTER_HPP
