//specialization of shared_count_hook for collecting cycles using "tagged" shared_count
#ifndef BOOST_POLICY_PTR_DETAIL_SHARED_COUNT_HOOK_SP_COUNTED_TAGGED_HPP_INCLUDED
#define BOOST_POLICY_PTR_DETAIL_SHARED_COUNT_HOOK_SP_COUNTED_TAGGED_HPP_INCLUDED
#include "boost/policy_ptr/detail/shared_count.hpp"
#include "boost/policy_ptr/detail/sp_counted_impl_hook_sp_counted_tagged.hpp"

namespace boost
{
namespace policy_ptr
{
namespace detail
{

int const shared_count_id = 0x2C35F101; //value of "tag"

template<>
class shared_count_hook<sp_counted_tagged>
  : public shared_count_base<sp_counted_tagged>
/**@brief
 *  Same as general template (defined in shared_count.hpp)
 *  but, in addition, add's a "tag" (shared_count_id),
 *  which is zeroed in DTOR.
 */
{
        typedef 
      shared_count_base<sp_counted_tagged>
    count_super_type;
    
 protected:
 
    int id_; //the "tag"

    shared_count_hook(void)
      : count_super_type()
      , id_(shared_count_id)
   { 
   }
    
    shared_count_hook(shared_count_hook const & r)
      : count_super_type(r)
      , id_(shared_count_id)
    { 
    }
    
    template<class Y>
    shared_count_hook(Y* p)
      : count_super_type(p, checked_deleter<Y>() )
      , id_(shared_count_id)
    {
    }

    template<class P, class D> 
    shared_count_hook(P p, D d)
      : count_super_type(p,d)
      , id_(shared_count_id)
    { }
    
    ~shared_count_hook()
    {
        id_ = 0;
    }
    
};

}//exit detail namespace
}//exit policy_ptr namespace
}//exit boost namespace
//----------------------------------------
//ChangeLog:
//  2005-06-15: Larry Evans
//    WHAT:
//      renamed from shared_count_sp_counted_tagged.
//    WHY:
//      part of changes described in shared_count.hpp ChangeLog.
#endif
