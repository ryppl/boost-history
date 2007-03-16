#ifndef BOOST_MANANGED_PTR_REFCYCLE_OVERHEAD_DESCRIBED_CURRY_PROX_VISITOR_HPP_LJE20040403
#define BOOST_MANANGED_PTR_REFCYCLE_OVERHEAD_DESCRIBED_CURRY_PROX_VISITOR_HPP_LJE20040403
#include "boost/managed_ptr/overhead_referent_vals.hpp"
#include "boost/managed_ptr/refcycle_overhead_described_abs.hpp"
namespace boost{namespace managed_ptr{

template
  < typename ProxVisitor
  >
  class
refcycle_overhead_described_curry_prox_visitor
//Purpose:
//  Simply to curry the ProxVisitor template parameter.
{
 public:
 
    template
      < typename Referent
      >
      class
    overhead_concrete
    ;
    
    template
      < typename Referent
      >
      struct
    ovhd_ref_vals_typedef
    {
            typedef
          overhead_referent_vals
            < overhead_concrete
            , Referent
            >
        type
        ;
    };
    
    template
      < typename Referent
      >
      class
    overhead_concrete
    //Purpose:
    //  Define a possible template argument to auto_overhead.
      : public refcycle_overhead_described_abs<ProxVisitor>
    {
     public:
         
            typedef
          refcycle_overhead_described_abs<ProxVisitor>
        top_overhead_type
        ; 
            typedef
          Referent
        referent_type
        ;
            typedef
          typename ovhd_ref_vals_typedef<referent_type>::type
        ovhd_ref_vals_type
        ;
            virtual
        ~overhead_concrete(void)
        {
          #ifdef TRACE_SCOPE_HPP
            mout()<<"~overhead_concrete:this="
            <<this<<std::endl;
          #endif
        }
    
        overhead_concrete(void)
        {
          #ifdef TRACE_SCOPE_HPP
            mout()<<"+overhead_concrete:this="
            <<this<<std::endl;
          #endif
        }
    
          typename prox_children<ProxVisitor>::prox_descriptor const*
        descriptor_con(void)const
        //Describes location of smart_pointers contained within
        //the gc'ed object.
        {
            return prox_children<ProxVisitor>::template 
              prox_description_of<referent_type>::ptr();
        }
        
          void*
        referent_start(void)const
        {
            ovhd_ref_vals_type const* l_orv 
              = static_cast<ovhd_ref_vals_type const*>(this);
            referent_type* l_r = const_cast<ovhd_ref_vals_type*>(l_orv);
            return l_r;
        }
        
        template
          < typename Other
          >
          struct
        rebind
        {
                typedef
              overhead_concrete<Other>
            other
            ;
        };
        
        template
          < typename SuperRef
          >
          typename rebind<SuperRef>::other&
        as_super(void)
        {
            top_overhead_type& l_me=*this;
            return static_cast<typename rebind<SuperRef>::other&>(l_me);
        }
        
        
     private:
     
        overhead_concrete
          (overhead_concrete const&
          )
        ;
        
    };//end overhead_concrete    

};//end refcycle_overhead_described_curry_prox_visitor<ProxVisitor>
            
}}//exit boost::managed_ptr namespace
//////////////////////////////////////////////////////////////////////////////
// ChangeLog:
//   2004-06-20: Larry Evans
//     WHAT:
//       1) Curried the template arguments so that ProxVisitor
//          was outer argument.
//       2) Renamed from refcycle_overhead_described_con.hpp
//     WHAT:
//       1) As part of effort to overcome smart_ptr templated
//          CTOR problem cited in libs /managed_ptr /test /sub_reset.cpp.
//       2) Reflect change 1) and remain consistent with other
//          file names (e.g. refcycle_counted_curry_prox_visitor.hpp).
//////////////////////////////////////////////////////////////////////////////
#endif
