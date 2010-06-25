//forward declare all template containers whose components specified with parameter packs.
#ifndef BOOST_COMPOSITE_STORAGE_PACK_CONTAINER_FWD_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_PWCK_CONTAINER_FWD_HPP_INCLUDED
#include <boost/composite_storage/tags.hpp>
namespace boost
{
namespace composite_storage
{
namespace pack
{
  template
  < class CompositeTag
  , class Index0
  , typename... Components
  >
  struct
container
;
}//exit pack namespace
}//exit composite_storage namespace
}//exit boost namespace
#endif
