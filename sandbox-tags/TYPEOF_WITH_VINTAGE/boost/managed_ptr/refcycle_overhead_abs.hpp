//overhead for reference counting with local mark-scan for cycle collection
#ifndef BOOST_MANAGED_PTR_REFCYCLE_OVERHEAD_ABS_LJE20031109_HPP
#define BOOST_MANAGED_PTR_REFCYCLE_OVERHEAD_ABS_LJE20031109_HPP
#include "boost/managed_ptr/refcnt_overhead.hpp"
namespace boost{namespace managed_ptr{

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

  class
refcycle_overhead_abs
: public refcnt_overhead
//Purpose:
//  Abstract "Overhead" for cyclic reference counting
{
 public:
 
        typedef 
      refcnt_overhead 
    super_type
    ;
    
        virtual //to allow deletion of this to call referent_type DTOR
                //where referent_type is defined in
                //refcycle_counted_curry_prox_visitor<ProxVisitor>::owner<ReferentPtr>.
                //This is because each refcyle_overhead_abs is part of the
                //overhead_type which is typedef'ed in
                //refcycle_counted_curry_prox_visitor<ProxVisitor>::owner<ReferentPtr>.
    ~refcycle_overhead_abs()
    {
    }
    
    refcycle_overhead_abs()
    : super_type(0)
    , my_stat(refcycle_status::is_done)
    {
    }
    
      refcycle_status::flags
    status_get()const
    {
        return my_stat;
    }
    
      void
    status_put(refcycle_status::flags a_flag)
    {
        my_stat = a_flag;
    }
    
        virtual
      void*
    referent_start()const
    //Returns start of gc'ed object.
    //Useful in case of multiple inheritance where
    //smart_ptr does not point to beginning of object.
    =0
    ;

 private:
         
    refcycle_status::flags my_stat
    ;
    
};//end refcycle_overhead_abs struct

}}//exit boost::managed_ptr namespace
#endif
