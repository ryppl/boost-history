#ifndef BOOST_MANAGED_PTR_BASIS_SPECIALIZER4REFCYCLE_COUNTED_CURRY_PROX_VISITOR_HPP_LJE20040402
#define BOOST_MANAGED_PTR_BASIS_SPECIALIZER4REFCYCLE_COUNTED_CURRY_PROX_VISITOR_HPP_LJE20040402
#include "boost/managed_ptr/refcycle_counted_curry_prox_visitor.fpp"
#include "boost/managed_ptr/auto_overhead.hpp"
#include "boost/managed_ptr/basis_specializer.hpp"
#include "boost/managed_ptr/refcycle_overhead_described_curry_prox_visitor.hpp"
namespace boost
{
namespace managed_ptr
{

template
  < typename Referent
  , typename ProxVisitor
  >
  struct 
basis_specializer
  < Referent
  , refcycle_counted_curry_prox_visitor<ProxVisitor> 
  >
{
        typedef
      Referent
    referent_type
    ;
        typedef
      refcycle_counted_curry_prox_visitor<ProxVisitor>
    nesting_type
    ;
        typedef 
      auto_overhead
        < referent_type
        , refcycle_overhead_described_curry_prox_visitor<ProxVisitor>
          ::template overhead_concrete
        > 
    basis_sink_type
    ;
        typedef 
      typename basis_sink_type::overhead_type
    overhead_type
    ;
        typedef 
      typename basis_sink_type::refwrap_type
    basis_source_type
    ;
    template
      < typename Other
      >
      struct
    rebind
    {
            typedef
          basis_specializer<Other,nesting_type>
        other
        ;
    };
};

}//exit managed_ptr namespace
}//exit boost namespace
#endif
