#ifndef BOOST_MANANGED_PTR_REFCYCLE_OVERHEAD_DESCRIBED_ABS_HPP_LJE20040403
#define BOOST_MANANGED_PTR_REFCYCLE_OVERHEAD_DESCRIBED_ABS_HPP_LJE20040403
#include "boost/managed_ptr/prox_children.hpp"
#include "boost/managed_ptr/overhead_referent_vals.hpp"
#include "boost/managed_ptr/refcycle_overhead_abs.hpp"
namespace boost{namespace managed_ptr{

template<typename ProxVisitor>
  class
refcycle_overhead_described_abs
//Purpose:
//  Abstract "Overhead" for cyclic reference counting
//  where children offsets are described.
//
//  This was factored from refcycle_overhead_described_con to
//  allow curry_prox_visitor_refcycle_counted<ProxVisitor>::
//  owner_void to be used without need for Referent template
//  parameter (which is needed for descriptor_con virtual
//  method).
: public refcycle_overhead_abs
{
 public:
        virtual
    ~refcycle_overhead_described_abs(void)
    {
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("~refcycle_overhead_described_abs");
        mout()<<":this="<<this<<std::endl;
      #endif
    }

    refcycle_overhead_described_abs(void)
    {
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("+refcycle_overhead_described_abs");
        mout()<<":this="<<this<<std::endl;
      #endif
    }

        virtual
      typename prox_children<ProxVisitor>::prox_descriptor const*
    descriptor_con(void)const
    //Describes children offsets. 
    =0
    ;
    
 private:
 
    refcycle_overhead_described_abs(refcycle_overhead_described_abs const&)
    ;

};//end refcycle_overhead_described_abs<Referent>
            
}}//exit boost::managed_ptr namespace
#endif
