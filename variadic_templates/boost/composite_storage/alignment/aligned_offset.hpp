//compile-time aligned offset.
#ifndef BOOST_COMPOSITE_STORAGE_ALIGNMENT_ALIGNED_OFFSET_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_ALIGNMENT_ALIGNED_OFFSET_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//====================================================================

namespace boost
{
namespace composite_storage
{
namespace alignment
{
  
  template
  < std::size_t Offset //minimum offset from start of some composite, C,
  , std::size_t Alignment//for a component, Ci, whose alignment is Alignment.
  >
  struct
aligned_offset
{
        static
      std::size_t const
    remainder
    = Offset%Alignment
    ;
        static
      std::size_t const
    value
    = remainder == 0
    ? Offset
    : Offset+(Alignment-remainder)
    /**@brief
     * value is minimum value > Offset such that
     * value%Alignment == 0.
     *
     *Why is this useful?
     * If:
     *   1) the composite, C, is located in memory at Offset0
     * and:
     *   2) Offset0%Alignment == 0,
     * then:
     *   (Offset0+value)%Alignment == 0, 
     * IOW, Ci will have the required alignment, Alignment.
     *
     */
    ;
    
};

}//exit alignment namespace
}//exit composite_storage namespace
}//exit boost namespace
#endif
