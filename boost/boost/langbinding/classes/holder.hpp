// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HOLDER_DWA2004918_HPP
# define HOLDER_DWA2004918_HPP

# include <boost/langbinding/aux_/config.hpp>
# include <boost/langbinding/aux_/holder.hpp>
# include <boost/noncopyable.hpp>

namespace boost { namespace langbinding { namespace classes { 

struct instance_link
{
 public:
    instance_link(instance_link* siblings)
      : siblings(siblings)
    {}
    
    virtual ~instance_link() {};
    
    holder* sibling() const
    { return siblings; }
    
    virtual void* find(util::type_info const&) const = 0;
    
 private:
    instance_link(holder_base const&);    // noncopyable
    void operator=(instance_link const&);
    
    instance_link* const siblings;
};

template <class Pointer>
struct instance_holder
  : instance_link
{
    instance_holder(Pointer ptr, instance_link* siblings)
      : instance_link(siblings)
      , ptr(ptr)
    {}
        
    void* find(util::type_info const& sought) const
    {
        if (typeid(Pointer) == sought)
            return &this->ptr;

        typedef typename boost::pointee<Pointer>::type pointee;

        pointee* p = get_pointer(this->ptr);
        if (p == 0)
            return 0;

        if (void* found = find_polymorphic(sought, (pointee*)0, p))
            return found;
            
        util::type_info const held = python::type_id<pointee>();
        return held == sought ? p : aux::find_dynamic_type(p, held, sought);
    }

 private:
    template <class T>
    void* find_polymorphic(util::type_info const& sought, polymorphic<T>*, T* p)
    {
        return sought == util::type_id<T>() ? p : 0;
    }
    
    template <class T>
    void* find_polymorphic(util::type_info const&, ...)
    {
        return 0;
    }
    
 private:
    Pointer ptr;
};

}}} // namespace boost::langbinding::classes

#endif // HOLDER_DWA2004918_HPP
