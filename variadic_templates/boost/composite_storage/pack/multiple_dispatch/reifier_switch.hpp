//
#ifndef BOOST_COMPOSITE_STORAGE_PACK_MULTIPLE_DISPATCH_REIFIER_SWITCH_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_PACK_MULTIPLE_DISPATCH_REIFIER_SWITCH_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
#include <boost/composite_storage/functor_indexed.hpp>
#include <boost/composite_storage/pack/multiple_dispatch/replace_source_with_target_ptr.hpp>

namespace boost
{
namespace composite_storage
{
namespace pack
{
namespace multiple_dispatch
{

  template
  < typename ReifyApply
  , typename ArgsConcreteAbstract
  >
struct reifier_switch
;
  template
  < typename ReifyApply
  , typename... HeadConcrete
  , typename HeadAbstract
  , typename... TailAbstract
  >
struct reifier_switch
  < ReifyApply
  , ptrs_target_source
    < mpl::package
      < HeadConcrete...
      >
    , HeadAbstract
    , TailAbstract...
    >
  >
  /**@brief
   *  Uses switch statment (in superclass) 
   *  to convert HeadAbstract,
   *  a composite_storage<one_of_maybe, _, TailConcrete...>
   *  to one of the TailConcretes, and then call
   *  a ReifyApply with the converted ptrs_target_source.
   */
: functor_indexed::layout_visitor<HeadAbstract>
{
        typedef
      typename functor_indexed::layout_visitor<HeadAbstract>::case_type
    case_type
    ;
        typedef
      typename HeadAbstract::index_type
    index_type
    ;
      ReifyApply const&
    my_reify
    ;
        typedef
      ptrs_target_source
      < mpl::package
        < HeadConcrete...
        >
      , HeadAbstract
      , TailAbstract...
      >
    now_tar_src_type
    ;
      now_tar_src_type*
    my_tar_src
    ;
      HeadAbstract&
    my_head_abstract
    ;
    reifier_switch
      ( ReifyApply const& a_reify
      , now_tar_src_type* a_ptrs_tar_src
      )
    : my_reify(a_reify)
    , my_tar_src(a_ptrs_tar_src)
    , my_head_abstract
      ( my_tar_src->template project
        < sizeof...(HeadConcrete)
        >()
      )
    {
    }
        typedef 
      typename ReifyApply::result_type 
    result_type
    ;
 private:   
      template
      < typename TailConcrete
      >
      result_type
    push_back_concrete
      ( TailConcrete& a_tail_concrete
      )const
    {
            typedef 
          ptrs_target_source
          < mpl::package
            < HeadConcrete...
            , TailConcrete
            >
          , TailAbstract...
          >
        next_tar_src_t;
          next_tar_src_t*
        next_tar_src_p=replace_source_with_target_ptr(my_tar_src,a_tail_concrete);
        return my_reify(next_tar_src_p);
    }
    
 public:
      template
      < case_type CaseValue
      >
      result_type 
    operator()
      ( mpl::integral_c<case_type,CaseValue> index
      )const
    {
        index_type const index_concrete=index_type(CaseValue);
        auto& a_tail_concrete=my_head_abstract.template project<index_concrete>();
        return this->push_back_concrete(a_tail_concrete);
    }
        
      result_type
    operator()
      ( void
      )const
    {
        return functor_indexed::apply//calls this->operator()(_)
          ( *this
          , my_head_abstract.which()//the CaseValue template arg to this->operator()(_).
          );
    }
};

}//exit namespace multiple_dispatch
}//exit namespace pack

}//exit composite_storage namespace
}//exit boost namespace
#endif
