//refcycle_counted_curry_prox_visitor fwd decl
#ifndef BOOST_MANAGED_PTR_REFCYCLE_COUNTED_CURRY_PROX_VISITOR_FPP_LJE20031009
#define BOOST_MANAGED_PTR_REFCYCLE_COUNTED_CURRY_PROX_VISITOR_FPP_LJE20031009

namespace boost{namespace managed_ptr{

template<typename ProxVisitor>
class refcycle_counted_curry_prox_visitor
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
//   2004-06-20: Larry Evans
//     WHAT:
//       renamed from curry_prox_visitor_refcycle_counted
//     WHY:
//       so name prefix is same as other related files
//       (e.g. refcycle_overhead_described_* ), thus suggesting
//       to someone scanning the directory listing.
//////////////////////////////////////////////////////////////////////////////
#endif
