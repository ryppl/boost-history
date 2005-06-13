//derivation of sp_counted_base enabling cycle collection via tagged pointer location.
#ifndef BOOST_POLICY_PTR_DETAIL_SP_COUNTED_TAGGED_HPP_LJE20050526
#define BOOST_POLICY_PTR_DETAIL_SP_COUNTED_TAGGED_HPP_LJE20050526
#include "boost/policy_ptr/detail/sp_counted_base.hpp"
#include <cstddef>
#include <utility>
namespace boost
{
namespace policy_ptr
{
namespace detail
{

  class 
sp_counted_tagged
  : public sp_counted_base
/**@brief
 *  Provide a dummy class to enable specialization of sp_counted_impl_p_rmptr
 */
{
 public:
    sp_counted_tagged(void)
    {
    }
    
  private:
 
    sp_counted_tagged( sp_counted_tagged const & );
    sp_counted_tagged & operator= ( sp_counted_tagged const & );
};

}//exit detail namespace
}//exit policy_ptr namespace
}//exit boost namespace
#endif
