//forward declare all ctor_protected templates whose components specified with parameter packs.
#ifndef BOOST_COMPOSITE_STORAGE_PACK_CTOR_PROTECTED_FWD_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_PWCK_CTOR_PROTECTED_FWD_HPP_INCLUDED
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
ctor_protected
  /**@brief
   *  Specializations provide all member functions
   *  except constructors.  Constructors must be
   *  provided by derived classes.
   **@rationale
   *  Since Components... may be anything, it's unknown, here,
   *  what the requirements are for contruction of those components.  
   *  For instance, there may not be a default CTOR for
   *  one component, and no copy CTOR for another.
   *  Only at the actual instantiation will these
   *  be known; hence, only a derived class should
   *  specify the public CTOR's for this composite.
   */
;
}//exit pack namespace
}//exit composite_storage namespace
}//exit boost namespace
#endif
