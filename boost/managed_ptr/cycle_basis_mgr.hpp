#ifndef BOOST_MANAGED_PTR_CYCLE_BASIS_MGR_HPP_LJE200311120
#define BOOST_MANAGED_PTR_CYCLE_BASIS_MGR_HPP_LJE200311120
#include "boost/managed_ptr/basis_specializer.hpp"
#include "boost/managed_ptr/rm_nondeduced.hpp"
namespace boost
{
  namespace detail
  {
      template <typename StoragePolicy, typename OwnershipPolicy>
      class resource_manager;
  }
  
  namespace managed_ptr
  {
  
    template
      < typename StoragePolicy
      , typename OwnershipPolicy
      >
      class
    cycle_basis_mgr
    ;

    template
      < typename Referent
      , typename StoragePolicy
      , typename OwnershipPolicy
      , typename Nester
      >
      class
    cycle_basis_mgr_nester
    ;
    
    template
      < typename Referent
      , typename StoragePolicy
      , typename OwnershipPolicy
      >
      class
    cycle_basis_mgr_nester
      < Referent
      , StoragePolicy
      , OwnershipPolicy
      , void
      >
    {
     public:
            typedef
          basis_specializer<Referent,void>
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
            typedef 
          detail::resource_manager
          < StoragePolicy
          , OwnershipPolicy
          >
        resource_mgr_type
        ;
            static 
          void 
        release_to_basis
          ( resource_mgr_type& source
          , basis_sink_type& target
          )
        //Purpose:
        //  Release the referent in source to target
        ;
        
        cycle_basis_mgr_nester(resource_mgr_type*)
        //By default, no cycles are collected; hence,
        //this is a do-nothing CTOR.
        {}
    };
    
    template
      < typename Referent 
      , typename StoragePolicy
      , template<typename>class OwnershipPolicy
      >
      class
    cycle_basis_mgr< StoragePolicy, OwnershipPolicy<Referent*> >
    //Purpose:
    //  Provide a "specializable" superclass that:
    //    1) defines the "basis source/sink types" to smart pointer class.
    //
    //       a) the basis_source_type is the argument to the smart_ptr CTOR
    //          which forms the "basis" of all other instances of
    //          the smart_ptr. IOW, all instances of smart_ptr are
    //          constructed either from the bais_source_type, or other
    //          smart_ptr's.  It's somewhat like the basis step in
    //          of proof by induction.
    //       b) the basis_sink_type is used to receive the referent released
    //          by the smart_ptr's release method.
    //    
    //    2) provides a constructor taking a pointer arg to smart_ptr subclass
    //       This constructor, or its specialization, is used to 
    //       record this smart_ptr for later use in garbage collection
    //       of cycles in the smart pointer graph.
    : public cycle_basis_mgr_nester
      < Referent
      , StoragePolicy
      , OwnershipPolicy<Referent*>
      , typename rm_nondeduced::nester_of<OwnershipPolicy<Referent*> >::type
      >
    {
     public:
            typedef 
          typename rm_nondeduced::nester_of<OwnershipPolicy<Referent*> >::type
        nesting_type
        ;
            typedef 
          cycle_basis_mgr_nester
            < Referent
            , StoragePolicy
            , OwnershipPolicy<Referent*>
            , nesting_type
            >
        super_type
        ;
            typedef 
          typename super_type::resource_mgr_type 
        resource_mgr_type
        ;
            typedef
          typename super_type::basis_source_type 
        basis_source_type
        ;
            typedef
          typename super_type::basis_sink_type 
        basis_sink_type
        ;
        cycle_basis_mgr
        ( resource_mgr_type* a 
          //^the subtype part of *this.
        )
        : super_type(a)
        {}
        
    };//end cycle_basis_mgr<StoragePolicy,OwnershipPolicy<Referent*> >
    
  } //exit managed_ptr namespace
} //exit boost namespace
//////////////////////////////////////////////////////////////////////////////
// ChangeLog:
//   2004-04-04.1028: Larry Evans
//     WHAT:
//       mv'ed 'namespace rm_nondeduced' to separate file
//     WHY:
//       try to isolate reason for compiler error messages during
//       compile of libs/managed_ptr/test/smart_ptr_test.cpp.~1.9~:170
//////////////////////////////////////////////////////////////////////////////
#endif
