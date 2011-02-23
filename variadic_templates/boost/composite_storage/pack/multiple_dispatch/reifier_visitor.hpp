//
#ifndef BOOST_COMPOSITE_STORAGE_PACK_MULTIPLE_DISPATCH_REIFIER_VISITOR_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_PACK_MULTIPLE_DISPATCH_REIFIER_VISITOR_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
#include <boost/composite_storage/pack/multiple_dispatch/replace_source_with_target_ptr.hpp>
#include <boost/mpl/pair.hpp>

namespace boost
{
namespace composite_storage
{
namespace pack
{
namespace multiple_dispatch
{
  template
  < typename ResultType
  , typename... ConcreteHosts
  >
struct reifier_visit_abstract_pack
;
  template
  < typename ResultType
  >
struct reifier_visit_abstract_pack
  < ResultType
  >
{
    void visit(void)
    //only declared to enable use of 
    //  using super_type::visit;
    //in subclasses.
    //Should never be called.
    ;
};
  template
  < typename ResultType
  , typename ConcreteHead
  , typename... ConcreteTail
  >
struct reifier_visit_abstract_pack
  < ResultType
  , ConcreteHead
  , ConcreteTail...
  >
: reifier_visit_abstract_pack
  < ResultType
  , ConcreteTail...
  >
{
        typedef
      reifier_visit_abstract_pack
      < ResultType
      , ConcreteTail...
      >
    super_type;
    
    using super_type::visit;
    
        virtual 
      ResultType 
    visit(ConcreteHead const&)const=0;
};
  template
  < typename ResultType
  , typename ConcreteHosts
  >
struct reifier_visit_abstract_seq
;  
  template
  < typename ResultType
  , typename... ConcreteHosts
  >
struct reifier_visit_abstract_seq
  < ResultType
  , mpl::package
    < ConcreteHosts...
    >
  >
  /**@brief
   *  Abstract Visitor class
   *  for abstract Host class whose 
   *  concrete subclasses
   *  are ConcreteHosts...
   */
: reifier_visit_abstract_pack
  < ResultType
  , ConcreteHosts...
  >
{  
};  
  template
  < typename ReifierVisitor
  , typename ConcretePkg
  , typename... ConcreteHosts
  >
struct reifier_visit_concrete_pack
;
  template
  < typename ReifierVisitor
  , typename... ConcreteHosts
  >
struct reifier_visit_concrete_pack
  < ReifierVisitor
  , mpl::package
    < ConcreteHosts...
    >
  >
: reifier_visit_abstract_seq
  < typename ReifierVisitor::first
  , mpl::package
    < ConcreteHosts...
    >
  >
{
};
  template
  < typename ReifierVisitor
  , typename ConcretePkg
  , typename ConcreteHead
  , typename... ConcreteTail
  >
struct reifier_visit_concrete_pack
  < ReifierVisitor
  , ConcretePkg
  , ConcreteHead
  , ConcreteTail...
  >
  /**@brief
   *  Define concrete visit method for ConcreteHead
   *  and then repeat for each ConcreteTail...
   */
: reifier_visit_concrete_pack
  < ReifierVisitor
  , ConcretePkg
  , ConcreteTail...
  >
{
      typename ReifierVisitor::first
    visit(ConcreteHead const&a_host)const
    {
            typedef
          typename ReifierVisitor::second
        self_derived;
          self_derived  const&
        self_value=static_cast<self_derived const&>(*this);
        return self_value.push_back_concrete(a_host);
    }
};
  template
  < typename ReifierVisitor
  , typename ConcreteHosts
  >
struct reifier_visit_concrete_seq
;  
  template
  < typename ReifierVisitor
  , typename... ConcreteHosts
  >
struct reifier_visit_concrete_seq
  < ReifierVisitor
  , mpl::package
    < ConcreteHosts...
    >
  >
  /**@brief
   *  Concrete Visitor class 
   *  for abstract Host class whose 
   *  concrete subclasses
   *  are ConcreteHosts...
   */
: reifier_visit_concrete_pack
  < ReifierVisitor
  , mpl::package //store ConcreteHosts... for use in defining abstract visit methods.
    < ConcreteHosts...
    >
  , ConcreteHosts... //used for defining concrete visit methods.
  >
{
};  
  template
  < typename HostAbsract
  >
struct hosts_concrete
  /**@brief
   *  Metafunction returning mpl::package
   *  of Concrete classes corresponding
   *  to the HostAbstract abstract class.
   */
;
  template
  < typename HostConcrete
  >
struct host_abstract
  /**@brief
   *  Metafunction returning Abstract class
   *  of which HostConcrete is a member.
   *  IOW, does inverse of hosts_concrete.
   */
{
        typedef 
      HostConcrete 
    type
    //By default, HostConcrete is member
    //of a type hierarchy starting at
    //HostConcrete.
    ;   
};
  template
  < typename ReifyApply
  , typename ArgsConcreteAbstract
  >
struct reifier_visitor
;
  template
  < typename ReifyApply
  , typename... HeadConcrete
  , typename HeadAbstract
  , typename... TailAbstract
  >
struct reifier_visitor
  < ReifyApply
  , ptrs_target_source //container of pointers.
    < mpl::package
      < HeadConcrete...
      >
    , HeadAbstract
    , TailAbstract...
    >
  >
  /**@brief
   *  Uses visitor design pattern (in superclass)
   *  to convert HeadAbstract* to its concrete
   *  subclass pointer, store pointer in a 
   *  ptrs_target_source, and then call
   *  a ReifyApply with the modified ptrs_target_source.
   */
: reifier_visit_concrete_seq 
  //Uses CRTP:
  //  http://www.informit.com/articles/article.aspx?p=31473&seqNum=3
  //to implement concrete visitor in the visitor design pattern:
  //  http://www.tml.tkk.fi/~pnr/Tik-76.278/gof/html/Visitor.html
  //However, instead of the Element term used in Visitor.html,
  //the term, Host, is used here 
  //( because Visitor::visit(Host) seems more meaningful).
  //  
  < mpl::pair
    < typename ReifyApply::result_type
    , reifier_visitor
      < ReifyApply
      , ptrs_target_source
        < mpl::package
          < HeadConcrete...
          >
        , HeadAbstract
        , TailAbstract...
        >
      >
    >
  , typename hosts_concrete
    < typename host_abstract
      < typename remove_cv
        < HeadAbstract
        >::type
      >::type
    >::type
  >
{
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
    reifier_visitor
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
    
      result_type
    operator()
      ( void
      )const
    {
        return my_head_abstract.accept(*this);
    }

};

}//exit namespace multiple_dispatch
}//exit namespace pack

}//exit composite_storage namespace
}//exit boost namespace
#endif
