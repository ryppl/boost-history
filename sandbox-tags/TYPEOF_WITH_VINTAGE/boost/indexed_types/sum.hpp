//disjoint sum of types in TypeMap<Indices> accessed by Index in Indices.
#ifndef BOOST_INDEXED_TYPES_SUM_HPP_LJE20041205
#define BOOST_INDEXED_TYPES_SUM_HPP_LJE20041205
//  (C) Copyright Larry Evans 2004.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
#include "boost/indexed_types/enum_size.hpp"
#include <cstddef> // for std::size_t
#include <new> // for placement new
namespace boost
{
namespace indexed_types
{

template
  < typename Indices
  , template <Indices> class TypeMap
  , unsigned Index
  >
  struct
term
  : public term
  < Indices
  , TypeMap
  , Index-1
  >
{
        typedef
      term<Indices, TypeMap, Index-1>
    super_type
    ;
        typedef
      typename TypeMap<Indices(Index-1)>::type
    type
    ;
        static
      std::size_t const
    max_size
    = sizeof(type) > super_type::max_size 
    ? sizeof(type) 
    : super_type::max_size
    ;
};

template
  < typename Indices
  , template <Indices> class TypeMap
  >
  struct
term
  < Indices
  , TypeMap
  , 0
  >
{
        static
      std::size_t const
    max_size
    = 0
    ;
};

template
  < typename Indices
  , template <Indices> class TypeMap
  >
  struct
sum
/**@begin
 *  A sum of types, (TypeMap<0>::type, TypeMap<1>::type, ..., TypeMap<n-1>)
 *  where n is number of enumerators in enumeration, Indices.
 *
 * @pre
 *  - Indices is an enumeration with initial enumerator value=0 and
 *    subsequent enumerator's increasing by 1.
 *  - n == enum_size<Indices>::value
 *  - TypeMap<I>::type is a default constructable type.
 *
 * @post
 *  sum's sequence of supertypes are:
 *    term<Indices,TypeMap, n-0>
 *    term<Indices,TypeMap, n-1>
 *    ...
 *    term<Indices,TypeMap, n-n>
 *  and: 
 *    this->index() in 0..n;
 *    if(i == this->index())
 *    {
 *       this->project<i> == this->inject<i>(value_i)
 *       where: value_i is the value of the argument to
 *         the last call of inject.
 *    }
 *    else
 *    {
 *        this->project<i> == 0;
 *    }
 */
  : public term
  < Indices
  , TypeMap
  , enum_size<Indices>::value
  >
{
        static
      unsigned const 
    map_size
    = enum_size<Indices>::value
    ;
        typedef
      term<Indices, TypeMap, map_size>
    super_type
    ;
        static
      unsigned const 
    max_size
    = super_type::max_size
    ;
    sum(void)
      : my_index(map_size)
    {}
    template
      < Indices Index
      >
      typename TypeMap<Index>::type*
    project(void)
    {
        typedef typename TypeMap<Index>::type required_type;
        return Index==my_index
          ? static_cast<required_type*>(storage())
          : 0
          ;
    }
    template
      < Indices Index
      >
      typename TypeMap<Index>::type const*
    project(void)const
    {
        typedef typename TypeMap<Index>::type required_type;
        return Index==my_index
          ? static_cast<required_type const*>(storage())
          : 0
          ;
    }
    template
      < Indices Index
      >
      typename TypeMap<Index>::type*
    inject(typename TypeMap<Index>::type const& a_value)
    {
        typedef typename TypeMap<Index>::type required_type;
        my_index= Index;
        return new (storage()) required_type(a_value);
    }
      unsigned
    index(void)const
    {
        return my_index;
    }
 private:
      unsigned
    my_index
    ;
      char
    my_storage[max_size]
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

}//exit indexed_types namespace
}//exit boost namespace
#endif
