//container template  containing all components in template arg list, all of which are aligned.
#ifndef BOOST_COMPOSITE_STORAGE_CONTAINER_ALL_OF_ALIGNED_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_CONTAINER_ALL_OF_ALIGNED_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//====================================================================
#include <boost/composite_storage/pack/ctor_protected_all_of_aligned.hpp>
#include <boost/composite_storage/pack/container_fwd.hpp>

namespace boost
{
namespace composite_storage
{
namespace pack
{
  template
  < class Index0
  , typename... Components
  >
  struct
container
  < tags::all_of_aligned
  , Index0
  , Components...
  >
: ctor_protected
  < tags::all_of_aligned
  , Index0
  , Components...
  >
{

        typedef
      ctor_protected
      < tags::all_of_aligned
      , Index0
      , Components...
      >
    ctor_prot
    ;
        typedef
      typename ctor_prot::methods_all
    methods_all
    ;
 public:
    container(void)
    {
        char*to_buf=this->address();
        methods_all::ctor_default_all(to_buf);
    }
    container( container& from)
    {
        char      *to_buf=this->address();
        char const*fr_buf=from.address();
          methods_all::
        ctor_copy_all
        ( to_buf
        , fr_buf
        );
    }
    container( container&& from)
    {
        char      *to_buf=this->address();
        char      *fr_buf=from.address();
          methods_all::
        ctor_copy_all
        ( to_buf
        , fr_buf
        );
    }
};

}//exit pack namespace
}//exit composite_storage namespace
}//exit boost namespace

#endif
