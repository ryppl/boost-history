#ifndef BOOST_INDEXED_TYPES_PRODUCT_HPP_LJE20041205
#define BOOST_INDEXED_TYPES_PRODUCT_HPP_LJE20041205
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
namespace boost
{
namespace indexed_types
{

template
  < typename SeqOfTypes
  , long Index
  >
  struct
factor
  : public factor<SeqOfTypes,Index-1>
{
        typedef
      typename mpl::at_c<SeqOfTypes,Index-1>::type
    type
    ;
      type  
    my_field
    ;
};

template
  < typename SeqOfTypes
  >
  struct
factor
  < SeqOfTypes
  , 0
  >
{
};

template
  < typename SeqOfTypes
  >
  struct
product
/**@begin
 *  A product of types in SeqTypes
 *
 * @pre
 *  SeqTypes is an mpl sequence.
 *
 * @post
 *  product's sequence of supertypes are:
 *    factor<SeqOfTypes, {SeqSize, SeqSize-1, ..., 0}>
 *  and: 
 *    for Index in SeqSize-1...0
 *    {
 *        at_c<SeqOfTypes,Index  >::type
 *      factor<SeqOfTypes,Index+1>::my_field
 *      ;
 *    }
 */
  : public factor<SeqOfTypes, mpl::size<SeqOfTypes>::value >
{
    template
      < long Index
      >
      typename mpl::at_c<SeqOfTypes,Index>::type&
    project(void)
    {
        return this->factor<SeqOfTypes, Index+1>::my_field;
    }
};

}//exit indexed_types namespace
}//exit boost namespace
#endif
