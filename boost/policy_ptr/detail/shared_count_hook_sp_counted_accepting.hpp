//specialization of shared_count_hook for collecting cycles using precise pointer location
#ifndef BOOST_POLICY_PTR_DETAIL_SHARED_COUNT_HOOK_SP_COUNTED_ACCEPTING_HPP_LJE20050514
#define BOOST_POLICY_PTR_DETAIL_SHARED_COUNT_HOOK_SP_COUNTED_ACCEPTING_HPP_LJE20050514
#include "boost/policy_ptr/policy/std.hpp"
#include "boost/policy_ptr/detail/sp_counted_accepting.hpp"
#include "boost/fields_visitor/container_extern/single.hpp"
#include "boost/fields_visitor/container_intern/field_iterator_of_container_intern_single.hpp"
#ifdef CONTAINER_SINGLE_OBJ_ID
#include "boost/utility/obj_id.hpp"
#include "boost/utility/trace_scope.hpp"
#endif

namespace boost
{
namespace policy_ptr
{
namespace detail
{

template<class Referent>
class sp_counted_impl_hook<Referent,sp_counted_accepting>
  : public sp_counted_accepting
/**@brief
 *  Specialization for collecting cycles (must be declared before
 *  used [indirectly] in next declaration ).
 */
{
    
 protected:
 
    typedef Referent ref_type;
    
    virtual ref_type* typed_ref_start(void)const
    =0
    ;
    
    sp_counted_impl_hook( Referent* p )
    {}
    
    //void dispose_hook()
    //  is defined in sp_counted_accepting
    
      void
    accept_each(shared_count_visitor_abs& a_visitor)const
    ;
};
  
template<>
class shared_count_hook<sp_counted_accepting>
  : public shared_count_base<sp_counted_accepting>
#ifdef CONTAINER_SINGLE_OBJ_ID
  , public utility::obj_id
#endif
  , public fields_visitor::field_registrar<shared_count_visitor_abs>
/**@brief
 *  Same as general template (defined in shared_count.hpp)
 *  but, in addition, allows collection of cycles by precisely
 *  locating smart pointers via the field_registrar superclass.
 */
{
    friend class shared_count_visitor_abs;
    
        typedef 
      shared_count_base<sp_counted_accepting>
    count_super_type;
    
      typedef fields_visitor::field_registrar<shared_count_visitor_abs> 
    registrar_super_type;
    
 public:
    shared_count_hook(void)
      : count_super_type()
      , registrar_super_type(this)
    { }
    
    shared_count_hook(shared_count_hook const & r)
      : count_super_type(r)
      , registrar_super_type(this)
    { }
    
    template<class Y>
    shared_count_hook(Y* p)
      : count_super_type(p, checked_deleter<Y>() )
      , registrar_super_type(this)
    { }

};

class shared_count_visitor_abs
{
 public:
        typedef
      shared_count_hook<sp_counted_accepting>
    field_type
    ;
        virtual
    ~shared_count_visitor_abs(void)
    {}
    
      template
      < template
          < typename //Element
          , typename //Allocator with default value
          >
        class Container
      , typename Element
      , typename VisitorSequence
      , typename Allocator
      >
      void
    visit_field
      ( fields_visitor::container_extern::single
        < Container
        , Element
        , VisitorSequence
        , Allocator
        >& a_field
      )
    {
        visit_with_iterator(a_field);
    }
    
      template
      < typename VisitorSequence 
        //^ Sequence of fields_visitor's for use as arg to ../registrar_heirarchy.hpp.
      , template
          < typename //Element
          , typename //Allocator
          >
          class Single //the modified stl container
      , typename Element
      >
      void
    visit_field
      ( Single
        < Element
        , fields_visitor::container_intern::allocator 
          < Element
          , VisitorSequence 
            //^ enables iteration over Single's data, if != empty_registrar
            //with specialization of prox_indirect, as in:
            //   prox_indirect_registrar_heirarchy.hpp
          >
        >& a_field
      )
    {
        visit_with_iterator(a_field);
    }
      
      template
      < typename Container
      >
      void
    visit_with_iterator
      ( Container& a_container
      )
    {
          typedef typename fields_visitor::field_iterator_of
          < shared_count_visitor_abs
          , Container
          > 
        iter_type;
        iter_type a_iter(&a_container);
        for(; !a_iter.empty(); a_iter.increment())
        {
            a_iter.accept(*this);
        }
    }
    
      template
      < typename Referent
      >
      void
    visit_field
      ( shared_storage_<Referent,sp_counted_accepting>& a_field
      )
    {
        field_type& l_count=a_field.count_prox_mut();
        visit_field(l_count);
    }
    
      void
    visit_field
      ( field_type& a_field
      )
    {
        if(a_field.is_valid())
        {
            visit_count(a_field);
        }
    }

 protected:
 
         static 
      sp_counted_accepting*
    get_counter(field_type& a_field)
    {
        return a_field.count_ptr_mut();
    }  
    
 private:    
 
        virtual
      void
    visit_count(field_type& a_field)
    =0
    ;
    
    
};

template<class Referent>
      void
sp_counted_impl_hook<Referent,sp_counted_accepting>::
    accept_each(shared_count_visitor_abs& a_visitor)const
    {
          typedef typename fields_visitor::field_iterator_of
          < shared_count_visitor_abs
          , Referent
          > 
        iter_type;
        Referent* pref=this->typed_ref_start();
        iter_type a_iter(pref);
        for(; !a_iter.empty(); a_iter.increment())
        {
            a_iter.accept(a_visitor);
        }
    }
    
}//exit detail namespace
}//exit policy_ptr namespace
}//exit boost namespace
//----------------------------------------
//ChangeLog:
//  2005-06-23: Larry Evans
//    WHAT:
//      Added:
//        a) shared_count_visitor_abs::visit_field(...container_extern...)
//        b) shared_count_visitor_abs::visit_field(...container_intern...)
//    WHY:
//      Enable collection using containers such as vector or list.
//    PROBLEMS:
//      This version allows my current (1.12) *local* version of:
//        libs/policy_ptr/test/std_ptr_binary_node_test.cpp
//      to pass; however, if the Elements of the container were *not*
//      derived from shared_storage_<*>, then there's no visit_field which
//      would accept them.  One might be tempted to just rename visit_with_iterator
//      as visit_field; however, doing that causes, AFAICT, that renamed
//      visit_field to accept smart_ptr<...>::to<T>& which has the default
//      field_iterator_of defined for it, and the default has 0 length (because
//      it has default selected_fields_description_of, see:
//        boost/fields_visitor/fields_visitor.hpp around comment:
//          @class selected_fields_description_of
//      ).  The problem can't be solved by defining a new templated visit_field
//      as:
//
//         template<class P1,..,class P4, class T>void visit_field
//         ( smart_ptr<P1,..,P4>::to<T>&)
//
//      because of the non-deduced context of <P1,..,P4>::to<T> eliminates that
//      visit_field as a candidate for overload resolution, AFAICT.
//  2005-06-17: Larry Evans
//    WHAT:
//      Added public shared_count_hook<...>::get_counter()const
//    WHY:
//      To provide debug print information in collector_sp_counted_accepting.hpp.
//  2005-06-15: Larry Evans
//    WHAT:
//      Renamed from shared_count_sp_counted_accepting and renamed
//      several classes.
//    WHY:
//      Part of changes described in shared_count.hpp ChangeLog.
//      These changed were needed because of, amoung other changes,
//      a change in the inheritance heirarchy.
#endif
