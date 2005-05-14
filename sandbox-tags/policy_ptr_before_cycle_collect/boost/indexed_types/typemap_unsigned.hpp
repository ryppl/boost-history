//define wrapper template used to tag "unsigned" typemaps
#ifndef BOOST_INDEXED_TYPES_TYPEMAP_UNSIGNED_HPP_LJE20041220
#define BOOST_INDEXED_TYPES_TYPEMAP_UNSIGNED_HPP_LJE20041220
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
namespace boost{namespace indexed_types{

template
  < typename TypeSeq
  >
  struct
typemap_unsigned
{
        typedef
      unsigned
    indices_type
    ;
    template<indices a_index>
    struct at
    {
        typedef typename boost::mpl::at_c<TypeSeq,a_index>::type type;
    };
        static
      unsigned const 
    indices_size
      =size<TypeSeq>::value
    ;
};
#endif
