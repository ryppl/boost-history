#ifndef BOOST_MANANGED_PTR_REFCYCLE_OVERHEAD_DESCRIBED_CON_HPP_LJE20040403
#define BOOST_MANANGED_PTR_REFCYCLE_OVERHEAD_DESCRIBED_CON_HPP_LJE20040403
#include "boost/managed_ptr/overhead_referent_vals.hpp"
#include "boost/managed_ptr/refcycle_overhead_described_abs.hpp"
namespace boost{namespace managed_ptr{

template
  < typename ProxVisitor
  , typename Referent
  >
  class
refcycle_overhead_described_con
: public refcycle_overhead_described_abs<ProxVisitor>
{
 public:
        typedef
      Referent
    referent_type
    ;
        typedef
      overhead_referent_vals<refcycle_overhead_described_con,referent_type>
    ovhd_ref_vals_type
    ;
        virtual
    ~refcycle_overhead_described_con(void)
    {
        //mout()<<"~refcycle_overhead_described_con:this="<<this<<std::endl;
    }

    refcycle_overhead_described_con(void)
    {
        //mout()<<"+refcycle_overhead_described_con:this="<<this<<std::endl;
    }

      typename prox_children<ProxVisitor>::prox_descriptor const*
    descriptor_con(void)const
    //Describes location of smart_pointers contained within
    //the gc'ed object.
    {
        return prox_children<ProxVisitor>::template prox_description_of<referent_type>::ptr();
    }
    
      void*
    referent_start(void)const
    {
        ovhd_ref_vals_type const* l_orv = static_cast<ovhd_ref_vals_type const*>(this);
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
          refcycle_overhead_described_con<ProxVisitor,Other>
        other
        ;
    };
    
 private:
 
    refcycle_overhead_described_con(refcycle_overhead_described_con const&)
    ;

};//end refcycle_overhead_described_con<ProxVisitor,Referent>
            
}}//exit boost::managed_ptr namespace
#endif
