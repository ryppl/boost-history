//define wrapper template used to tag "enumerated" typemaps
#ifndef BOOST_INDEXED_TYPES_TYPEMAP_ENUMERATED_HPP_LJE20041220
#define BOOST_INDEXED_TYPES_TYPEMAP_ENUMERATED_HPP_LJE20041220
#include "boost/indexed_types/enum_size.hpp"
namespace boost{namespace indexed_types{

template
  < typename Enumeration 
    //^an enumeration
  , template <Enumeration> class TypeMap 
    //^template with specialization for elements of Enumeration
  >
  struct
typemap_enumerated
{
        typedef
      Enumeration
    indices_type
    ;
    template<indices_type a_index>
    struct at
    {
        typedef typename TypeMap<a_index>::type type;
    };
        static
      unsigned const 
    indices_size
      =enum_size<Enumeration>::value
    ;
};

}}//exit boost::indexed_types namespace
#endif
