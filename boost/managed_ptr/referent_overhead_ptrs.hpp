//combined referent and "overhead" pointers
#ifndef BOOST_MANAGED_PTR_REFERENT_OVERHEAD_PTRS_HPP_LJE20040327
#define BOOST_MANAGED_PTR_REFERENT_OVERHEAD_PTRS_HPP_LJE20030327
#include "boost/managed_ptr/overhead_referent_vals.hpp"
namespace boost{namespace managed_ptr{

template
  < typename Referent
  , template<typename>class RefrProxy
  , typename Overhead
  , template<typename>class OvhdProxy
  >
class referent_overhead_ptrs
: protected RefrProxy<Referent>
, protected OvhdProxy<Overhead>
//Purpose:
//  Contains detached smart_ptr overhead and
//  referent pointers.
{
 public:
    typedef Referent referent_type;
    typedef Overhead overhead_type;
    
    ~referent_overhead_ptrs()
    {}
        
    referent_overhead_ptrs()
    {}
       
    referent_overhead_ptrs
    ( referent_type* ref
    , overhead_type* ovhd
    )
    : RefrProxy<referent_type>(ref)
    , OvhdProxy<overhead_type>(ovhd)
    {}
       
    referent_overhead_ptrs(overhead_referent_vals<overhead_type,referent_type>* vals_ptr)
    : RefrProxy<referent_type>(vals_ptr)
    , OvhdProxy<overhead_type>(vals_ptr)
    {}
       
    overhead_type* overhead_mut()const
    {
        return OvhdProxy<overhead_type>::get();
    }
    
    referent_type* referent_mut()const
    {
        return RefrProxy<referent_type>::get();
    }
        
};


}}//exit boost::managed_ptr namespace
#endif
