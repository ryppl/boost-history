//template to create disjoint sum composite
#ifndef BOOST_INDEXED_TYPES_COMPOSITE_SUM_HPP_LJE20041220
#define BOOST_INDEXED_TYPES_COMPOSITE_SUM_HPP_LJE20041220
#include "boost/indexed_types/composite_type.hpp"
#include <boost/mpl/int.hpp>
#include <exception>
namespace boost{namespace indexed_types{


template
  < typename TypeMap
  , unsigned Index
  >
  struct
term
  : public term
  < TypeMap
  , Index-1
  >
{
        typedef
      term<TypeMap, Index-1>
    super_type
    ;
        typedef
      typename TypeMap::indices_type
    indices_type
    ;
        typedef
      typename TypeMap::template at<indices_type(Index-1)>::type
    type
    ;
        static
      std::size_t const
    our_storage_size
    = sizeof(type) > super_type::our_storage_size 
    ? sizeof(type) 
    : super_type::our_storage_size
    ;
    template
      < typename Visitor
      >
      void
    accept
      ( Visitor& a_visitor
      , void const* a_storage
      )const
    {
        if(this->index() == Index-1)
        {
            type const* l_type=static_cast<type const*>(a_storage);
            a_visitor.visit(mpl::int_<Index-1>(), *l_type);
        }
        else
        {
            super_type::accept(a_visitor, a_storage);
        }
    }
};

template
  < typename TypeMap
  >
  struct
term
  < TypeMap
  , 0
  >
{
        static
      std::size_t const
    our_storage_size
    = 0
    ;
    template
      < typename Visitor
      >
      void
    accept
      ( Visitor& a_visitor
      , void const*
      )const
    {
        a_visitor.invalid_sum_index();
    }
      unsigned
    index(void)const
    {
        return my_index;
    }
      struct
    bad_project
      : public std::exception
    {
        virtual const char * what() const throw()
        {
            return "composite_sum::project";
        }
    };    
 protected:
      unsigned
    my_index
    ;
};

template
  < typename TypeMap
  >
  struct
composite_type
  < operator_sum
  , TypeMap
  >
  : public term
  < TypeMap
  , TypeMap::indices_size
  >
{
        typedef
      term<TypeMap, TypeMap::indices_size>
    super_type
    ;
    composite_type(void)
    {
        this->my_index= TypeMap::indices_size;
    }
    
    template
      < typename TypeMap::indices_type Index
      >
      typename term<TypeMap,Index+1>::type&
    project(void)
    {
        typedef typename term<TypeMap,Index+1>::type required_type;
        required_type* l_ptr=
          Index==this->index()
          ? static_cast<required_type*>(storage())
          : 0
          ;
        if(l_ptr) return *l_ptr;
        throw bad_project();
    }
    template
      < typename TypeMap::indices_type Index
      >
      typename term<TypeMap,Index+1>::type const&
    project(void)const
    {
        typedef typename term<TypeMap,Index+1>::type const required_type;
        required_type* l_ptr=
          Index==this->index()
          ? static_cast<required_type*>(storage())
          : 0
          ;
        if(l_ptr) return *l_ptr;
        throw bad_project;
    }
    template
      < typename TypeMap::indices_type Index
      >
      typename term<TypeMap,Index+1>::type*
    inject(typename term<TypeMap,Index+1>::type const& a_value)
    {
        typedef typename term<TypeMap,Index+1>::type required_type;
        this->my_index= Index;
        return new (storage()) required_type(a_value);
    }
    template
      < typename Visitor
      >
      void
    accept
      ( Visitor& a_visitor
      )const
    {
        super_type::accept(a_visitor, storage());
    }
 private:
      char
    my_storage[super_type::our_storage_size]
    ;
      void*
    storage(void)
    {
        return my_storage;
    }
      void const*
    storage(void)const
    {
        return my_storage;
    }
};

}}//exit boost::indexed_types namespace
#endif
