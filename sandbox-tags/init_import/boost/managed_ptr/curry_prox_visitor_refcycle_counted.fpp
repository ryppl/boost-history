//curry_prox_visitor_refcycle_counted fwd decl w/cycle_mgr specialization fwd decl
#ifndef BOOST_MANAGED_PTR_CURRY_PROX_VISITOR_REFCYCLE_COUNTED_FPP_LJE20031009
#define BOOST_MANAGED_PTR_CURRY_PROX_VISITOR_REFCYCLE_COUNTED_FPP_LJE20031009

namespace boost{namespace managed_ptr{

template<typename ProxVisitor>
class curry_prox_visitor_refcycle_counted
//PURPOSE:
//  Forward declare nested template owner to enable it
//  to take a single template argument; yet, depend on
//  another template argument in outer class.
//WHY:
//  So that it has same number template arguments as
//  smart_ptr OwnershipProperty; hence, can be used as
//  argument to smart_ptr.
{
 public:

        class 
    owner_void
    ;
    
    template <typename ReferentPtr>
        class 
    owner
    ;
};

}}//exit boost::managed_ptr namespace
//////////////////////////////////////////////////////////////////////////////
// ChangeLog:
//   2003-12-05: Larry Evans
//     WHAT:
//       See ChangeLog: in the .hpp file.
//   2003-12-08: Larry Evans
//     WHAT:
//       Added owner_void.
//     WHY:
//       Reduce code bloat due to template<...> class owner.
//////////////////////////////////////////////////////////////////////////////
#endif
