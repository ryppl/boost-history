#ifndef LIBS_COMPOSITE_STORAGE_UTILITY_LAYOUT_AT_INCLUDED
#define LIBS_COMPOSITE_STORAGE_UTILITY_LAYOUT_AT_INCLUDED
#include <boost/mpl/while.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/integral_c.hpp>
namespace libs
{
namespace composite_storage
{
namespace utility
{

   
    template
    < typename IndexUndefined
    , typename IndexType
    >
  struct layout_if
    /**@brief
     *  Arg to while_ template.
     */
  {
          typedef
        typename IndexUndefined::value_type
      index_base
      ;
          typedef
        boost::mpl::integral_c
        < index_base
        , index_base(IndexType::value)
        >
      index2base
       /**@brief
        *  Converts IndexType to
        *  integral_c having index_base as type argument.
        */
      ;
        template
        < typename Layout
        >
      struct if_
      : boost::mpl::not_
        < boost::mpl::or_
          < boost::is_same<typename Layout::index_part, index2base>
          , boost::is_same<typename Layout::index_part, IndexUndefined>
          >
        >
        /**@brief
         *  Terminate while_ loop if
         *    either: IndexUndefined is found
         *    or: IndexType is found
         */
      {};
      
        template
        < typename Layout
        >
      struct then_
        /**@brief
         *  Return next layout.
         */
      {
          typedef typename Layout::head_layout type;
      };
  };
  
    template
    < typename Composite
    , typename Index
    >
  struct layout_at
    /**@brief
     *  Returns Layout at given Index in Composite, if it exists,
     *  Otherwise, returns layout at undefined Index.
     */
  : boost::mpl::while_
    < typename Composite::scanned
    , layout_if
      < typename Composite::index_undefined
      , Index
      >
    >
  {
  };
  
}}}
#endif
