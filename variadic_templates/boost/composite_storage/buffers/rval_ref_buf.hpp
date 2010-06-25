//wrapper class to flag buffer as rval reference.
#ifndef BOOST_COMPOSITE_STORAGE_BUFFERS_RVAL_REF_BUF_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_BUFFERS_RVAL_REF_BUF_HPP_INCLUDED
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
namespace buffers
{

struct rval_ref_buf
//Purpose:
//  Flag that the buffer for this composite
//  is an rvalue-reference:
//MaintenanceNote.2010-03-26:
//  Tried just passing char&& as buffer type; however,
//  this lead to all kinds of difficulty because
//  calls to std::move were required many places.
//  Also, for some reason, in assign_one template,
//  the FrBuffer was 'char' instead of 'char&&' and
//  I tried everything to get it to 'char&&' but
//  nothing worked.
//
{
    rval_ref_buf(char*a_buf)
    : my_buf(a_buf)
    {}
    char*my_buf;
};

}//exit buffers namespace
}//exit composite_storage namespace
}//exit boost namespace
#endif

