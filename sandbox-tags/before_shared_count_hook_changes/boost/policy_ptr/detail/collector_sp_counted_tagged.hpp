//tagged collector for refcounted pointers based on sp_counted_tagged
#ifndef BOOST_POLICY_PTR_DETAIL_COLLECTOR_SP_COUNTED_TAGGED_HPP_LJE20050523
#define BOOST_POLICY_PTR_DETAIL_COLLECTOR_SP_COUNTED_TAGGED_HPP_LJE20050523
#include "boost/policy_ptr/detail/collector.hpp"
#include "boost/policy_ptr/detail/sp_counted_tagged.hpp"

namespace boost
{
namespace policy_ptr
{
namespace detail
{

  template
  <
  >
  class
collector
  < sp_counted_tagged
  >
/**@brief
 *  Collector for smart pointers with detached refcounts of type
 *  sp_counted_tagged.
 */
{
 public:
        static
      void
    collect(void)
    {
        free_unreachable_objects();
    }
        static
      std::size_t 
    find_unreachable_objects(bool report)
    ;
        static
      void
    free_unreachable_objects(void)
    ;
        static
      void 
    sp_scalar_constructor_hook
      ( void* referent_ptr
      , std::size_t referent_size
      , sp_counted_tagged const* pn
      )
    /**@brief
     *  Essentially the same as like named function in
     *  libs/smart_ptr/src/sp_collector.cpp
     */
    ;
        static
      void 
    sp_scalar_destructor_hook
      ( sp_counted_tagged * pn
      )
    /**@brief
     *  Essentially the same as like named and signatured function in
     *  libs/smart_ptr/src/sp_collector.cpp
     */
    ;
    
};//end collector<sp_counted_tagged> class
        
}//exit detail namespace
}//exit policy_ptr namespace
}//exit boost namespace
#endif
