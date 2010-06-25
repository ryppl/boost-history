//meta function returning the base of an enumeration.
#ifndef BOOST_COMPOSITE_STORAGE_ENUM_BASE_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_ENUM_BASE_HPP_INCLUDED
#include <boost/mpl/integral_c.hpp>

namespace boost
{
namespace composite_storage
{
  template
  < typename Index
  >
struct enum_base
;
  template
  < typename Type
  , Type Valu
  >
struct enum_base
  < mpl::integral_c<Type,Valu>
  >
{
        typedef
      int
    type
    ;
};
}//exit composite_storage namespace
}//exit boost namespace
#endif
