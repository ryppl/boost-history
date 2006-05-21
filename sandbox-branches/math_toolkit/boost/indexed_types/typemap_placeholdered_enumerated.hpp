//define wrapper template used to tag "enumerated" typemaps
#ifndef BOOST_INDEXED_TYPES_TYPEMAP_PLACEHOLDERED_ENUMERATED_HPP_LJE20041220
#define BOOST_INDEXED_TYPES_TYPEMAP_PLACEHOLDERED_ENUMERATED_HPP_LJE20041220
#include "boost/indexed_types/enum_size.hpp"
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/apply.hpp>
namespace boost{namespace indexed_types{

template
  < typename Enumeration 
    //^an enumeration
  , typename TypeMapPlaceholdered
    //^placeholder expression with _1 as placeholder
  >
  struct
typemap_placeholdered_enumerated
{
        typedef
      Enumeration
    indices_type
    ;
    template<indices_type a_index>
    struct at
    {
        typedef mpl::lambda<TypeMapPlaceholdered> f_;
        typedef typename mpl::integral_c<indices_type,a_index> x_;
        typedef typename mpl::apply1<f_, x_>::type type;
    };
        static
      unsigned const 
    indices_size
      =enum_size<Enumeration>::value
    ;
};

}}//exit boost::indexed_types namespace
#endif
