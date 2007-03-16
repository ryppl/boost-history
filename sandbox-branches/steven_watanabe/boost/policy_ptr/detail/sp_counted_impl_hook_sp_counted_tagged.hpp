//specialization sp_counted_impl_p_rmptr on sp_counted_tagged
#ifndef BOOST_POLICY_PTR_DETAIL_SP_COUNTED_IMPL_HOOK_SP_COUNTED_TAGGED_HPP_LJE20050526
#define BOOST_POLICY_PTR_DETAIL_SP_COUNTED_IMPL_HOOK_SP_COUNTED_TAGGED_HPP_LJE20050526
#include "boost/policy_ptr/detail/collector_sp_counted_tagged.hpp"
#include "boost/policy_ptr/detail/sp_counted_impl.hpp"
namespace boost
{
namespace policy_ptr
{
namespace detail
{

template<class Referent>
class sp_counted_impl_hook<Referent,sp_counted_tagged>
  : public sp_counted_tagged
/**@brief
 *  Specialization for collecting cycles using tagged location of pointers.
 */
{
 protected:
 
    sp_counted_impl_hook( Referent* p )
    { 
        collector<sp_counted_tagged>::
          sp_scalar_constructor_hook( p, sizeof(Referent), this );
    }
    
    Referent* typed_ref_start(void)const
    {
        return static_cast<Referent*>( this->void_ref_start() );
    }
    
    void dispose_hook()
    { 
     //2005-06-09 Larry Evans
     //   SHARED_PTR_DIFF_NOTE:  
     //     The original code in boost/detail/sp_counted_impl.hpp,
     //     had the sp_scalar_destructor_hook in sp_counted_impl_p::dispose;
     //     however, it was absent in sp_counted_impl_pd::dispose.  In 
     //     order for the hook code to be called by the dispose of both sp_counted_impl_p
     //     and sp_counted_impl_pd, it was moved to this class, a common
     //     superclass of those classes.
     //     
        collector<sp_counted_tagged>::
          sp_scalar_destructor_hook( this );
    }
};
 
}//exit detail namespace
}//exit policy_ptr namespace
}//exit boost namespace
#endif
