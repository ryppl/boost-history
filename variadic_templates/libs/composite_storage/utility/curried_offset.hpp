#ifndef LIBS_COMPOSITE_STORAGE_UTILITY_CURRIED_OFFSET_INCLUDED
#define LIBS_COMPOSITE_STORAGE_UTILITY_CURRIED_OFFSET_INCLUDED
#include "layout_at.hpp"
namespace libs
{
namespace composite_storage
{
namespace utility
{
  template
  < typename Composite
  >
struct curried_offset
{
        typedef
      typename Composite::index_type
    index_type
    ;
      template
      < index_type Index
      >
    struct offset_at
    /**@brief
     *  Find offset at given Index within Composite.
     */
    {
            typedef 
            typename
          layout_at
          < Composite
          , boost::mpl::integral_c<index_type,Index>
          >::type
        layout_type
        ;
            static
          std::size_t const
        value
        =layout_type::comp_part::offset
        ;
    };
};

}}}
#endif

