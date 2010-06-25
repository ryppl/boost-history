//tags for various types of composites.
#ifndef BOOST_COMPOSITE_STORAGE_TAGS_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_TAGS_HPP_INCLUDED

namespace boost
{
namespace composite_storage
{
namespace tags
{
    struct one_of_maybe
      //composite contains an instance of 'one of' template arguments, *maybe*.
      //This composite is like haskell's Maybe except there's more than one
      //optional value.
      ; 
    struct all_of_aligned 
      //composite contains instances of 'all of' template arguments, properly 'aligned'.
      ; 
    struct all_of_packed  
      //composite contains instances of 'all of' template arguments, 'packed' together.
      ; 
}//exit tags namespace
}//exit composite_storage namespace
}//exit boost namespace
#endif
