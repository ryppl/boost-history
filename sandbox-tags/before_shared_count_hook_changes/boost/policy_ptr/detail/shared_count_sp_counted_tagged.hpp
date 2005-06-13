//specialization of shared_count for collecting cycles using "tagged" shared_count
#ifndef BOOST_POLICY_PTR_DETAIL_SHARED_COUNT_SP_COUNTED_TAGGED_HPP_INCLUDED
#define BOOST_POLICY_PTR_DETAIL_SHARED_COUNT_SP_COUNTED_TAGGED_HPP_INCLUDED
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
class shared_count<sp_counted_tagged>
  : public shared_count_base<sp_counted_tagged>
/**@brief
 *  Same as general template (defined in shared_count.hpp)
 *  but, in addition, add's a "tag" (shared_count_id).
 */
{
        typedef 
      shared_count_base<sp_counted_tagged>
    count_super_type;
    
 protected:
 
    int id_; //the "tag"

 public:
 
    shared_count(void)
      : count_super_type()
      , id_(shared_count_id)
   { 
   }
    
    shared_count(shared_count const & r)
      : count_super_type(r)
      , id_(shared_count_id)
    { 
    }
    
    template<class Y>
    shared_count(Y* p)
      : count_super_type(p, checked_deleter<Y>() )
      , id_(shared_count_id)
    {
    }

    ~shared_count() // nothrow
    {
     //2005-06-09 Larry Evans
     //   SHARED_PTR_DIFF_NOTE:  
     //     ~shared_count_base does pi_->release();
     //     AFTER the following statement is executed.
     //     In the original code, boost/detail/shared_count.hpp,
     //     this order was reversed.  I don't think it makes
     //     any difference though.
        id_ = 0;
    }
    
};

}//exit detail namespace
}//exit policy_ptr namespace
}//exit boost namespace
#endif
