#ifndef BOOST_MANAGED_PTR_RM_NONDEDUCED_HPP_LJE20040404
#define BOOST_MANAGED_PTR_RM_NONDEDUCED_HPP_LJE20040404
namespace boost
{
  namespace managed_ptr
  {
    namespace rm_nondeduced
    //Purpose:
    //  These classes overcome a restriction on specialization
    //  of nested template classes in nondeduced contexts.
    //Reference:
    //  1) c++ standard 14.8.2.4 "Deducing template arguments from a type"
    //     paragraph 4.
    //  2) http://groups.google.com/groups?hl=en&lr=&ie=UTF-8&selm=10422s0n4gtdaac%40corp.supernews.com
    {
    
      template<typename T>
      struct nester_of
      //Purpose:
      //  nester_of<T>::type = void //non-specialized template, i.e. this one
      //  nester_of<N::T>::type = N //must be defined by specializations
      {
          typedef void type;
      };
      
    } //exit rm_nondeduced namespace
  } //exit managed_ptr namespace
} //exit boost namespace
//////////////////////////////////////////////////////////////////////////////
// ChangeLog:
//   2004-04-04.1028: Larry Evans
//     WHAT:
//       mv'ed from cycle_basis_mgr.hpp
//     WHY:
//       See ChangeLog:this_date: in cycle_basis_mgr.hpp.
//////////////////////////////////////////////////////////////////////////////
#endif
