#ifndef BOOST_MANAGED_PTR_BASIS_SPECIALIZER_HPP_LJE20040402
#define BOOST_MANAGED_PTR_BASIS_SPECIALIZER_HPP_LJE20040402
#include "boost/managed_ptr/curry_prox_visitor_refcycle_counted.fpp"
#include "boost/managed_ptr/auto_overhead.hpp"
#include "boost/managed_ptr/basis_specializer.hpp"
#include "boost/managed_ptr/refcycle_overhead_described_con.hpp"
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
  , curry_prox_visitor_refcycle_counted<ProxVisitor> 
  >
{
        typedef
      Referent
    referent_type
    ;
        typedef
      curry_prox_visitor_refcycle_counted<ProxVisitor>
    nesting_type
    ;
        typedef 
      auto_overhead
        < referent_type
        , refcycle_overhead_described_con<ProxVisitor,referent_type>
        > 
    basis_sink_type
    ;
        typedef 
      typename basis_sink_type::overhead_type
    overhead_type
    ;
        typedef 
      typename basis_sink_type::refwrap 
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
