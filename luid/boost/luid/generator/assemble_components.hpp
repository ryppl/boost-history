/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_NUIDG_ASSEMBLE_COMPONENTS_HPP_
#define BOOST_NUIDG_ASSEMBLE_COMPONENTS_HPP_

#include "boost/luid/dsl.hpp"
#include "boost/luid/iccl.hpp"
#include <memory>
#include <boost/detail/allocator_utilities.hpp>
#include <boost/intrusive/detail/pointer_to_other.hpp>
#include "boost/mpl/eval_if.hpp"

namespace boost
{
namespace luid
{
namespace detail 
{
template< typename CompleteDSLDescription>
struct assemble_components {
    typedef assemble_components<CompleteDSLDescription> generator;
    typedef CompleteDSLDescription dsl_features;
    
    typedef typename dsl_features::numeric_type numeric_type_;
    static const numeric_type_ lower_bound_ = dsl_features::lower_bound;
    typedef typename dsl_features::time_traits time_traits_;
    typedef typename dsl_features::allocator_type allocator_type_;
//    typedef typename dsl_features::node_type node_type_;
    
    typedef iccl::base<generator> base;
    typedef typename mpl::if_<dsl::is_process<typename dsl_features::lifetime>, 
            iccl::raw_storage<base>,
            typename mpl::if_<dsl::is_kernel<typename dsl_features::lifetime>,
                iccl::shm_storage<base>,
                iccl::shm_storage<base>
                   >::type        
               >::type alt_stortage;

    typedef iccl::storage<alt_stortage> storage;
   
    typedef typename mpl::if_<
             dsl::is_dynamic_lower_bound<typename dsl_features::numeric_type_kind>,
             iccl::dynamic_lower_bound<iccl::upper_bound<storage> >,
             iccl::static_lower_bound<iccl::upper_bound<storage> >
         >::type bounds_type;
             
    typedef iccl::counter<bounds_type> counter;
            
    typedef typename mpl::if_<
             dsl::is_discard<typename dsl_features::on_release>, 
            counter,
             typename mpl::if_<
                 dsl::is_delay<typename dsl_features::recovery_when>,
                 iccl::rec_fifo_delay<iccl::fifo<counter> >,
                 typename mpl::if_<
                     dsl::is_freeze<typename dsl_features::recovery_when>,
                     iccl::rec_fifo_delay<iccl::fifo<counter> >,
                     typename mpl::if_<
                         dsl::is_fifo<typename dsl_features::recovery_how>,
                         iccl::fifo<counter>, 
                         iccl::set<bounds_type>
                     >::type
                 >::type
             >::type
         >::type on_release;
    
    typedef typename mpl::if_<
          dsl::is_ignore<typename dsl_features::coherency>,
         on_release,
          typename mpl::if_<
             dsl::is_discard<typename dsl_features::on_release>, 
             iccl::coherency<iccl::coherency_bitset<on_release> >,
             typename mpl::if_<
                 dsl::is_delay<typename dsl_features::recovery_when>,
                 iccl::coherency<iccl::coherency_bitset<on_release> >,
                 typename mpl::if_<
                     dsl::is_freeze<typename dsl_features::recovery_when>,
                     iccl::coherency<iccl::coherency_bitset<on_release> >,
                     typename mpl::if_<
                         dsl::is_fifo<typename dsl_features::recovery_how>,
                         iccl::coherency<iccl::coherency_bitset<on_release> >, 
                         on_release
                     >::type
                 >::type
            >::type
        >::type
    >::type opt_coherency;

            
    typedef typename mpl::if_<
            boost::luid::dsl::is_ignore_error<typename dsl_features::on_overflow>, 
            iccl::return_error_value_on_overflow<opt_coherency>,
            typename mpl::if_<
                        boost::luid::dsl::is_errno_on_error<typename dsl_features::on_overflow>,
                luid::iccl::errno_on_overflow<opt_coherency>,
                luid::iccl::throw_exception_on_overflow<opt_coherency>
                >::type
            >::type alt_on_overflow;

    typedef alt_on_overflow reentrant;

    typedef typename mpl::if_<
            boost::luid::dsl::is_mono_threaded<typename dsl_features::scope>, 
            alt_on_overflow,
            typename mpl::if_<
                boost::luid::dsl::is_externally_locked<typename dsl_features::locking_strategy>, 
                alt_on_overflow, 
                iccl::internally_locked<alt_on_overflow> 
            >::type
            >::type opt_locking;

    typedef iccl::facade<opt_locking> facade;

    typedef typename mpl::if_<
        boost::luid::dsl::is_mono_threaded<typename dsl_features::scope>, 
        facade,
        typename mpl::if_<
            boost::luid::dsl::is_externally_locked<typename dsl_features::locking_strategy>, 
            iccl::externally_locked<facade>, 
            facade 
        >::type
        >::type opt_external_locking;
        
    typedef iccl::constructor<opt_external_locking> luidg_type;
    
public:
  struct config {
    typedef numeric_type_ numeric_type;
    static const numeric_type lower_bound = lower_bound_;
    typedef time_traits_ time_traits;
//    typedef multi_threaded_locking_traits_ multi_threaded_locking_traits;
    //    typedef externally_locked_mutex_type_ externally_locked_mutex_type;
    typedef typename dsl_features::allocator_type allocator_type;
    
    typedef typename luidg_type::node_type node_type;
    typedef typename luidg_type::header_type header_type;

//    typedef typename boost::detail::allocator::rebind_to<
//                      allocator_type,
//                      node_type 
//                  >::type    node_allocator_type;
//    typedef node_allocator_type node_allocator;
    
    typedef typename dsl_features::void_pointer_type void_pointer_type;
//    typedef typename dsl_features::internally_locking_traits internally_locking_traits;
    typedef typename dsl_features::locking_traits locking_traits;

    
//    typedef typename mpl::if_<
//            matches<typename dsl_features::lifetime, dsl::process<> >, 
//            node_type*,
//            boost::interprocess::offset_ptr<node_type>
//            >::type node_pointer_type;
    
    typedef typename boost::pointer_to_other<void_pointer_type, node_type>
                ::type node_pointer_type;
    
    typedef typename luidg_type::type final;
  };
  typedef typename luidg_type::type type;
};

}}}
#endif /*ASSEMBLE_COMPONENTS_HPP_*/
