//specialization of shared_count for collecting cycles using precise pointer location
#ifndef BOOST_POLICY_PTR_DETAIL_SHARED_COUNT_SP_COUNTED_ACCEPTING_HPP_LJE20050514
#define BOOST_POLICY_PTR_DETAIL_SHARED_COUNT_SP_COUNTED_ACCEPTING_HPP_LJE20050514
#include "boost/policy_ptr/detail/shared_count.hpp"
#include "boost/policy_ptr/detail/sp_counted_accepting.hpp"
#include "boost/fields_visitor/fields_visitor.hpp"
#ifdef CONTAINER_SINGLE_OBJ_ID
#include "boost/utility/obj_id.hpp"
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
class shared_count<sp_counted_accepting>
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
    shared_count(void)
      : count_super_type()
      , registrar_super_type(this)
    { }
    
    shared_count(shared_count const & r)
      : count_super_type(r)
      , registrar_super_type(this)
    { }
    
    template<class Y>
    shared_count(Y* p)
      : count_super_type(p, checked_deleter<Y>() )
      , registrar_super_type(this)
    { }

 private:
      sp_counted_accepting*
    get_counter(void)
    {
        return this->pi_;
    }   
};

class shared_count_visitor_abs
{
 public:
        virtual
    ~shared_count_visitor_abs(void)
    {}
      void
    visit_field(shared_count<sp_counted_accepting>& a_count)
    {
        if(a_count.is_valid())
        {
            visit_count(a_count);
        }
    }

      template
      < class X
      >    
      void
    visit_field(X& a_field)
    {
          typedef typename fields_visitor::field_iterator_of
          < shared_count_visitor_abs
          , X
          > 
        iter_type;
        iter_type a_iter(&a_field);
        for(; !a_iter.empty(); a_iter.increment())
        {
            a_iter.accept(*this);
        }
    }
    
 protected:
 
         static 
      sp_counted_accepting*
    get_counter(shared_count<sp_counted_accepting>& a_count)
    {
        return a_count.get_counter();
    }  
    
 private:    
 
        virtual
      void
    visit_count(shared_count<sp_counted_accepting>& a_count)
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
#endif
