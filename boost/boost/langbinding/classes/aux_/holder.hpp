// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HOLDER_DWA2004918_HPP
# define HOLDER_DWA2004918_HPP

# include <boost/langbinding/aux_/config.hpp>
# include <boost/noncopyable.hpp>

# include <typeinfo>

namespace boost { namespace langbinding { namespace classes { namespace aux_ { 

struct holder_base
{
 protected:
    struct vtbl
    {
        void (*destroy)(holder_base const*);
        void* (*find)(holder_base const*, std::type_info const&);
    };
    
 public:
    holder_base(vtbl& methods, holder_base* siblings);
    
    holder* sibling() const;
    
    void* find(std::type_info const&) const;
    void destroy() const;
    
 private:
    holder_base(holder_base const&);    // noncopyable
    void operator=(holder_base const&);
    
    vtbl& methods;
    holder_base* siblings;
};

template <class Derived>
struct holder : holder_base
{
    holder(holder_base* siblings)
      : holder_base(virtual_table, siblings)
    {}

 private:
    static void destroy_(holder_base const* self)
    {
        delete static_cast<Derived const*>(self);
    }
    
    static void* find_(holder_base const* self, std::type_info const& x)
    {
        return static_cast<Derived const*>(self)->find(x);
    }
    
    static vtbl virtual_table;
};

template <class Derived>
holder_base::vtbl holder<Derived>::virtual_table
= { &holder<Derived>::destroy_, &holder<Derived>::find_ };
    
}}}} // namespace boost::langbinding::classes::aux_

#endif // HOLDER_DWA2004918_HPP
