//layout data for individual components of a composite.
#ifndef BOOST_COMPOSITE_STORAGE_LAYOUT_OF_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_LAYOUT_OF_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//#include <boost/composite_storage/buffers/char_buf.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_empty.hpp>
#include <boost/type_traits/alignment_of.hpp>

namespace boost
{
namespace composite_storage
{
namespace layout
{
  template
  < typename T
  >
  struct
alignment_of
  /**@brief
   *  Allow specialization of alignment calculation.
   *  This is useful when the alignment for some
   *  composite is partially calculated 
   *  (see ../detail/layout_operators*.hpp)
   *  but the complete object type has not been
   *  defined yet.  For example, the one_of_maybe
   *  composite needs to calculate the alignment
   *  of all it's components, then calculate the
   *  alignment of those components following by
   *  the alignment of the discriminant.
   */
: public ::boost::alignment_of<T>
{
};

  template
  < typename T
  >
  struct
size_of
: mpl::if_
  < is_empty<T>
  , mpl::integral_c<std::size_t,0>
  , mpl::integral_c<std::size_t,sizeof(T)>
  >::type
{
};

struct components_aligned_yes
{
      template
      < std::size_t Size=0
      , std::size_t Align=1
      >
    struct layout_data
    {
            static
          std::size_t const
        size
        =Size
        ;
            static
          std::size_t const
        align
        =Align
        ;
    };
      template
      < typename Component
      >
    struct layout_of
    : layout_data
      < size_of<Component>::value
      , alignment_of<Component>::value
      >
    {
    };
};
struct components_aligned_not
{
      template
      < std::size_t Size=0
      >
    struct layout_data
    {
            static
          std::size_t const
        size
        =Size
        ;
    };
      template
      < typename Component
      >
    struct layout_of
    : layout_data
      < size_of<Component>::value
      >
    {
    };
};
}//exit layout namespace
}//exit composite_storage namespace
}//exit boost namespace
#endif

