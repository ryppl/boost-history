//cartesian product of values in TypeMap<Indices> accessed by Index in Indices.
#ifndef BOOST_INDEXED_TYPES_PRODUCT_HPP_LJE20041205
#define BOOST_INDEXED_TYPES_PRODUCT_HPP_LJE20041205
//  (C) Copyright Larry Evans 2004.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
#include "boost/indexed_types/enum_size.hpp"
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
factor
  : public factor
  < Indices
  , TypeMap
  , Index-1
  >
{
        typedef
      typename TypeMap<Indices(Index-1)>::type
    type
    ;
      type  
    my_field
    ;
};

template
  < typename Indices
  , template <Indices> class TypeMap
  >
  struct
factor
  < Indices
  , TypeMap
  , 0
  >
{
};

template
  < typename Indices
  , template <Indices> class TypeMap
  >
  struct
product
/**@begin
 *  A product of types, (TypeMap<0>::type, TypeMap<1>::type, ..., TypeMap<n-1>)
 *  where n is number of enumerators in enumeration, Indices.
 *
 * @pre
 *  - Indices is an enumeration with initial enumerator value=0 and
 *    subsequent enumerator's increasing by 1.
 *  - n == enum_size<Indices>::value
 *  - TypeMap<I>::type is a default constructable type.
 *
 * @post
 *  product's sequence of supertypes are:
 *    factor<Indices,TypeMap, n-0>
 *    factor<Indices,TypeMap, n-1>
 *    ...
 *    factor<Indices,TypeMap, n-n>
 *  and: 
 *    for Index in n-1...0
 *    {
 *        TypeMap<Indices(Index) >::type
 *      factor<Indices,TypeMap,Index+1>::my_field
 *      ;
 *    }
 */
  : public factor
  < Indices
  , TypeMap
  , enum_size<Indices>::value
  >
{
    template
      < Indices Index
      >
      typename TypeMap<Index>::type&
    project(void)
    {
        return this->factor<Indices, TypeMap, Index+1>::my_field;
    }
};

}//exit indexed_types namespace
}//exit boost namespace
#endif
