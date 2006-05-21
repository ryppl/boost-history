//derivation of sp_counted_base enabling cycle collection via boost/fields_visitor.
#ifndef BOOST_POLICY_PTR_DETAIL_SP_COUNTED_ACCEPTING_HPP_LJE20050525
#define BOOST_POLICY_PTR_DETAIL_SP_COUNTED_ACCEPTING_HPP_LJE20050525
#include "boost/policy_ptr/detail/sp_counted_base.hpp"
#include "boost/policy_ptr/detail/refcycle_status.hpp"
#include <set>
namespace boost
{
namespace policy_ptr
{
namespace detail
{

  class 
shared_count_visitor_abs
;

  class 
sp_counted_accepting
  : public sp_counted_base
/**@brief
 *  Add method (accept_each) and class variable (our_instances)
 *  and instance variable (status_) to enable collection of cycles 
 *  in pointer graph.
 */
{
 public:
    sp_counted_accepting(void)
    {
        our_instances.insert(this); 
    }
    
    void dispose_hook(void)
    {
        our_instances.erase(this); 
    }
    
        virtual
      void
    accept_each(shared_count_visitor_abs& a_visitor)const
    /**@brief
     *  Have all children of smart pointer referent "accept" 
     *  a_visitor.  The referent is stored by derived class.
     */
    =0
    ;
        typedef
      std::set<sp_counted_accepting*>
    this_set_type
    ;
        static
      this_set_type
    our_instances
    ;
 private:

    sp_counted_accepting( sp_counted_accepting const & );
    sp_counted_accepting & operator= ( sp_counted_accepting const & );
    
 public:
    refcycle_status::flags status_;

};

}//exit detail namespace
}//exit policy_ptr namespace
}//exit boost namespace
#endif
