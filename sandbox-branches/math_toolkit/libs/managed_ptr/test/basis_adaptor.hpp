//template specializations for smart_ptr basis adaptors
#ifndef BOOST_MANAGED_PTR_BASIS_ADAPTOR_HPP_LJE20031230
#define BOOST_MANAGED_PTR_BASIS_ADAPTOR_HPP_LJE20031230
#include "boost/managed_ptr/basis_specializer4refcycle_counted_curry_prox_visitor.hpp"
#include "boost/managed_ptr/refcycle_prox_visitor_abs.hpp"
namespace boost
{
namespace managed_ptr
{

template
  < typename Referent
  , typename Nester
  >
  class
basis_adaptor_nester
;

template
  < typename Referent
  >
  class
basis_adaptor_nester
  < Referent
  , void
  >
//Purpose:
//  Provide an adaptation to smart pointer initializers so that
//  the initializers "appear" the same despite differing
//  ownership policies.
//
//  The "basis" argument (see Purpose: of basis_specializer in cycle_basis_mgr.hpp) 
//  to the smart pointer is returned by the as_basis member function.
//
//  Specializations on OwnershipPolicy may define different
//  types returned by as_basis.
{
 public:
        typedef
      Referent
    referent_type
    ;
        typedef
      void
    nesting_type
    ;
        typedef
      basis_specializer
        < referent_type
        , nesting_type
        >
    basis_spec_type
    ;
        typedef
      typename basis_spec_type::basis_source_type
    basis_source_type //should be referent_type*
    ;
 
        typedef
      typename basis_spec_type::basis_sink_type
    basis_sink_type //should be referent_type*
    ;
 
    basis_adaptor_nester(void)
    : my_basis(0)
    {}
    
    MANAGED_PTR_CTOR_FORWARDER_OVERLOAD(MANAGED_PTR_CTOR_FORWARDER_MAX_ARGS,basis_adaptor_nester,(my_basis)(new referent_type),0)
    
    basis_adaptor_nester(basis_source_type a_basis)
    : my_basis(a_basis)
    {}
    
      basis_source_type
    as_basis_source(void)
    {
        return my_basis;
    }
      basis_sink_type&
    as_basis_sink_ref(void)
    {
        return my_basis;
    }
      referent_type*
    referent(void)const
    {
        return my_basis;
    }
      void
    destroy(void)
    {
        delete my_basis;
        my_basis=0;
    }
 private:    
      basis_sink_type
    my_basis
    ;        
};

template
  < typename Referent
  , typename ProxVisitor
  >
  class
basis_adaptor_nester
  < Referent
  , refcycle_counted_curry_prox_visitor<ProxVisitor> 
  >
{
 public:
        typedef
      Referent
    referent_type
    ;
        typedef
      refcycle_counted_curry_prox_visitor<ProxVisitor>
    nesting_type
    ;
        typedef
      basis_specializer
        < referent_type
        , nesting_type
        >
    basis_spec_type
    ;
        typedef
      typename basis_spec_type::basis_source_type
    basis_source_type
    ;
    
        typedef
      typename basis_spec_type::basis_sink_type
    basis_sink_type
    ;
    
    basis_adaptor_nester(void)
    {
        utility::trace_scope 
          ts("basis_adaptor_nestor<Referent,curry_prox...>::CTOR(void)");
    }
    
    basis_adaptor_nester(basis_adaptor_nester& a_me)
    : my_new(a_me.my_new)
    {}
    
    MANAGED_PTR_CTOR_FORWARDER_OVERLOAD(MANAGED_PTR_CTOR_FORWARDER_MAX_ARGS,basis_adaptor_nester,(my_new),1)

      basis_source_type
    as_basis_source(void)
    {
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope 
          ts("basis_adaptor_nestor<Referent,curry_prox...>::as_basis_source");
      #endif
        return my_new.as_refwrap();
    }
    
      basis_sink_type&
    as_basis_sink_ref(void)
    {
        return my_new;
    }
    
      referent_type*
    referent(void)const
    {
        return my_new.referent();
    }
      void
    destroy(void)
    {
        delete my_new.release_overhead();
    }
 private:    
      basis_sink_type
    my_new
    ;        
};//basis_adaptor_nester<curry...> specialization

template
  < typename OwnershipPolicy
  >
  class
basis_adaptor
  ;

template
  < typename Referent 
  , template<typename>class OwnershipPolicy
  >
  class
basis_adaptor
  < OwnershipPolicy<Referent*> 
  >
: public basis_adaptor_nester
  < Referent
  , typename rm_nondeduced::nester_of<OwnershipPolicy<Referent*> >::type
  >
{
 public:
        typedef
      OwnershipPolicy<Referent*>
    ownership_type
    ;
        typedef
      basis_adaptor_nester
        < Referent
        , typename rm_nondeduced::nester_of<ownership_type>::type
        >
    super_type
    ;
    
    basis_adaptor(void)
    {
        utility::trace_scope ts("basis_adaptor");
    }
    
    basis_adaptor(basis_adaptor& a_me)
    : super_type(a_me)
    {}
    
    MANAGED_PTR_CTOR_FORWARDER_OVERLOAD(MANAGED_PTR_CTOR_FORWARDER_MAX_ARGS,basis_adaptor,(super_type),1)
    
    template
      < typename Other>
      struct
    rebind
    {
            typedef
          basis_adaptor<OwnershipPolicy<Other*> >
        other
        ;
    };
};

}//exit managed_ptr namespace
}//exit boost namespace
#endif
