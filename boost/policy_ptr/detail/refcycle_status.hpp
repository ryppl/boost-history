//status flags used during refcounted smart ptr cycle collection
#ifndef BOOST_POLICY_PTR_DETAIL_REFCYCLE_STATUS_HPP_20050514_HPP
#define BOOST_POLICY_PTR_DETAIL_REFCYCLE_STATUS_HPP_20050514_HPP
namespace boost
{
namespace policy_ptr
{
namespace detail
{
  class
refcycle_status
  //Purpose
  //  provide enumeration of the status flags of 
  //  cyclic reference counting as described in 
  //  following references.
  //References:
  //  [JL96]
  //    Jones,Richard; Lins,Rafael
  //    _Garbage Collection_
  //    Wiley, 1996
  //    with errata available (on June 2, 2001) at: 
  //      http://www.cs.ukc.ac.uk/people/staff/rej/gcbook/errata.html
  //  [LV91]
  //    Lins,Rafael; Vasques,Marcio
  //    "A Comparative Study of Algorithms for Cyclic Reference Counting"
  //    in  _Technical Report 92_
  //      Computing Laboratory, The University of Kent at Canterbury, August 1991.
  //  [MWL90]
  //    Martinez; Wachenchauzer; Lins
  //    "Cyclic Reference Counting with Local Mark-Scan"
  //    in  _Information Processing Letters_, 34:31-35, 1990
{
 public:
                       //Cell Color in [MWL90],[LV91] and [JL96,p.63]
                       //[MWL90][LV91] [JL96,p.63]
    enum flags         //-------       -----------
    { is_done          //Green         Black    -> before gc, and after, if live
    , rm_internal      //Red           Grey     -> visited by rm_internal
    , is_dead          //Blue          White    -> after rm_internal, had 0 count
    , brk_cycles       //--------none------------> after is_dead, visited by break_cycles
    };
};

}}}//exit boost::policy_ptr::detail namespace
#endif
