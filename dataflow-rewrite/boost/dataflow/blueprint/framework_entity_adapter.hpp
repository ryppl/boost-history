/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_ENTITY_ADAPTER_HPP
#define BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_ENTITY_ADAPTER_HPP


#include <boost/dataflow/blueprint/framework_entity.hpp>
#include <boost/dataflow/blueprint/framework_context.hpp>
#include <boost/dataflow/utility/enable_if_type.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/pointee.hpp>

namespace boost { namespace dataflow { namespace blueprint {

namespace detail {

template<typename T, typename Enable=void>
struct dataflow_base_of
{
    typedef T type;
};

template<typename T>
struct dataflow_base_of<T,
    typename utility::enable_if_type<typename T::dataflow_base_type>::type>
{
    typedef typename T::dataflow_base_type type;
};

}

template<typename BlueprintFramework, typename Dereferencable, typename Base=framework_entity<BlueprintFramework>, typename Enable=void >
class framework_entity_adapter : public Base
{
public:
    typedef typename pointee<Dereferencable>::type entity_type;
    typedef
        typename detail::dataflow_base_of<
            typename pointee<Dereferencable>::type
        >::type castable_entity_type;
    
    framework_entity_adapter(blueprint::framework_context<BlueprintFramework> &fo)
        : Base(fo, typeid(castable_entity_type))
    {}
    template<typename T>
    framework_entity_adapter(blueprint::framework_context<BlueprintFramework> &fo, const T &t)
        : Base(fo, typeid(castable_entity_type))
        , m_entity(t)
    {}
    template<typename T>
    framework_entity_adapter(blueprint::framework_context<BlueprintFramework> &fo, T &t)
        : Base(fo, typeid(castable_entity_type))
        , m_entity(t)
    {}

    entity_type &entity()
    {
        return *m_entity;
    }
    const entity_type &entity() const
    {
        return *m_entity;
    }
private:
    virtual void *get_ptr()
    {
        return static_cast<castable_entity_type *>(&*m_entity);
    };
    Dereferencable m_entity;
};

/*template<typename BlueprintFramework, typename Dereferencable, typename Base>
class framework_entity_adapter<BlueprintFramework, Dereferencable, Base,
    typename enable_if<typename BlueprintFramework::framework_has_object>::type>
    : public Base
{
    typedef typename is_reference<EntityOrRef>::type is_reference;
public:
    typedef typename remove_reference<EntityOrRef>::type entity_type;
    
    framework_entity_adapter(blueprint::framework_context<BlueprintFramework> &fo)
        : Base(fo, typeid(m_entity))
        , m_entity(fo.object())
    {}
    framework_entity_adapter(blueprint::framework_context<BlueprintFramework> &fo, entity_type &t)
        : Base(fo, typeid(m_entity))
        , m_entity(t)
    {}
    framework_entity_adapter(blueprint::framework_context<BlueprintFramework> &fo, const entity_type &t)
        : Base(fo, typeid(m_entity))
        , m_entity(t)
    {}
    template<typename T>
    framework_entity_adapter(blueprint::framework_context<BlueprintFramework> &fo, const T &t)
        : Base(fo, typeid(m_entity))
        , m_entity(fo.object(), t)
    {}
    template<typename T>
    framework_entity_adapter(blueprint::framework_context<BlueprintFramework> &fo, T &t,
        typename disable_if<mpl::and_<utility::is_type<T>, is_reference> >::type* dummy = 0)
        : Base(fo, typeid(m_entity))
        , m_entity(fo.object(), t)
    {}
    template<typename T>
    framework_entity_adapter(blueprint::framework_context<BlueprintFramework> &fo, T &t,
        typename enable_if<mpl::and_<utility::is_type<T>, is_reference> >::type* dummy = 0)
        : Base(fo, typeid(m_entity))
        , m_entity(t)
    {}
    entity_type &entity()
    {
        return m_entity;
    }
    const entity_type &entity() const
    {
        return m_entity;
    }
private:
    virtual void *get_ptr()
    {
        return &m_entity;
    };
    EntityOrRef m_entity;
};*/

} } } // namespace boost::dataflow::blueprint

#endif // BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_ENTITY_ADAPTER_HPP