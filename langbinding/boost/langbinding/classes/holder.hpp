// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HOLDER_DWA2004918_HPP
# define HOLDER_DWA2004918_HPP

# include <boost/langbinding/aux_/config.hpp>
# include <boost/langbinding/classes/polymorphic.hpp>
# include <boost/langbinding/classes/aux_/inheritance_query.hpp>
# include <boost/noncopyable.hpp>
# include <boost/get_pointer.hpp>

namespace boost { namespace langbinding { namespace classes { 

struct instance_link
{
 public:
    instance_link(instance_link* siblings)
      : siblings(siblings)
    {}
    
    virtual ~instance_link() {};
    
    instance_link* sibling() const
    { return siblings; }
    
    virtual void* find(util::type_info const&) = 0;
    
 private:
    instance_link(instance_link const&);    // noncopyable
    void operator=(instance_link const&);
    
    instance_link* const siblings;
};

template <class Pointer>
struct instance_holder
  : instance_link
{
    instance_holder(
        backend::class_instance const& instance, Pointer ptr, instance_link* siblings
    )
      : instance_link(siblings)
      , ptr(ptr)
    {
        construct_polymorphic(instance, get_pointer(ptr));
    }
        
    void* find(util::type_info const& sought)
    {
        if (util::type_id<Pointer>() == sought)
            return &this->ptr;

        return this->find_pointee(sought, get_pointer(this->ptr));
    }

 private:
    
    template <class T>
    inline void construct_polymorphic(backend::class_instance const& instance, polymorphic<T>* p)
    {
        if (p) p->instance = instance;
    }
    
    inline void construct_polymorphic(backend::class_instance const&, ...)
    {}
    
    template <class Pointee>
    inline void* find_pointee(util::type_info const& sought, Pointee* p)
    {
        if (p == 0)
            return 0;

        if (void* found = this->find_polymorphic(sought, (Pointee*)0, p))
            return found;
            
        util::type_info const held = util::type_id<Pointee>();
        return held == sought ? p : aux::find_dynamic_type(p, held, sought);
    }
        
    template <class T>
    inline void* find_polymorphic(util::type_info const& sought, polymorphic<T>*, T* p)
    {
        return sought == util::type_id<T>() ? p : 0;
    }

    inline void* find_polymorphic(util::type_info const&, ...)
    {
        return 0;
    }
    
 private:
    Pointer ptr;
};

}}} // namespace boost::langbinding::classes

#endif // HOLDER_DWA2004918_HPP
