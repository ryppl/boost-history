//template to create cartesian product composite
#ifndef BOOST_INDEXED_TYPES_COMPOSITE_PRODUCT_HPP_LJE20041220
#define BOOST_INDEXED_TYPES_COMPOSITE_PRODUCT_HPP_LJE20041220
#include "boost/indexed_types/composite_type.hpp"
#include <memory>
namespace boost{namespace indexed_types{


template
  < typename TypeMap
  , unsigned Index
  >
  struct
factor
  : public factor
  < TypeMap
  , Index-1
  >
{
        typedef
      factor<TypeMap, Index-1>
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
    = sizeof(type) + super_type::our_storage_size 
    ;
 protected:
      void
    inject(char* storage)
    {
        super_type::inject(storage+sizeof(type));
        new (storage) type;
    }
};

template
  < typename TypeMap
  >
  struct
factor
  < TypeMap
  , 0
  >
{
        static
      std::size_t const
    our_storage_size
    = 0
    ;
 protected:
      void
    inject(char*)
    {
    }
};

template
  < typename TypeMap
  >
  struct
composite_type
  < operator_product
  , TypeMap
  >
  : public factor
  < TypeMap
  , TypeMap::indices_size
  >
{
        typedef
      factor<TypeMap, TypeMap::indices_size>
    super_type
    ;
    composite_type(void)
    {
        super_type::inject(storage());
    }
    template
      < typename TypeMap::indices_type Index
      >
      typename TypeMap::template at<Index>::type&
    project(void)
    {
        typedef typename TypeMap::template at<Index>::type indexed_type;
        return *static_cast<indexed_type*>(storage_at<Index>());
    }
    template
      < typename TypeMap::indices_type Index
      >
      typename TypeMap::template at<Index>::type const&
    project(void)const
    {
        typedef typename TypeMap::template at<Index>::type indexed_type;
        return *static_cast<indexed_type const*>(storage_at<Index>());
    }
 private:
      char
    my_storage[super_type::our_storage_size]
    ;
      char*
    storage(void)
    {
        return my_storage;
    }
      char const*
    storage(void)const
    {
        return my_storage;
    }
    template
      < typename TypeMap::indices_type Index
      >
      void*
    storage_at(void)
    {
        return storage()+factor<TypeMap, Index>::our_storage_size;
    }
    template
      < typename TypeMap::indices_type Index
      >
      void const*
    storage_at(void)const
    {
        return storage()+factor<TypeMap, Index>::our_storage_size;
    }
};

}}//exit boost::indexed_types namespace
#endif
