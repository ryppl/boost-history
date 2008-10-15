/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_ENTITY_HPP
#define BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_ENTITY_HPP


#include <boost/dataflow/blueprint/castable_polymorphic_object.hpp>


namespace boost { namespace dataflow { namespace blueprint {

template<typename BlueprintFramework>
class framework_entity : public castable_polymorphic_object
{
public:
    struct dataflow_traits
    {
        typedef BlueprintFramework framework;
    };
    
    template<typename Entity>
    framework_entity(const Entity &entity)
        : castable_polymorphic_object(entity)
    {}
    virtual ~framework_entity()
    {}
private:
    virtual const void *get_ptr() const=0;
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_ENTITY_HPP
