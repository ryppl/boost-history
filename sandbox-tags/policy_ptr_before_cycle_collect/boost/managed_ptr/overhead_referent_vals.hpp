//combined "overhead" and referent with varying number of templated ctor args
#ifndef BOOST_MANAGED_PTR_OVERHEAD_REFERENT_VALS_HPP_LJE20031020
#define BOOST_MANAGED_PTR_OVERHEAD_REFERENT_VALS_HPP_LJE20031020
#include "boost/managed_ptr/managed_ptr_ctor_forwarder.hpp"

namespace boost{namespace managed_ptr{

template
  < template<typename>class Overhead
  , typename Referent
  >
class overhead_referent_vals
: public Overhead<Referent>
, public Referent
//Purpose:
//  Enable "intrusive allocation" of gc'ed objects.
//  This is in contrast to "intrusive definition" of gc'ed
//  objects where the gc overhead (i.e. the Overhead
//  superclass) is part of the gc'ed object (i.e. the Referent
//  object).
{
    typedef Overhead<Referent> overhead_type;
 public:
    ~overhead_referent_vals(void)
    {}
        
    MANAGED_PTR_CTOR_FORWARDER_OVERLOAD(MANAGED_PTR_CTOR_FORWARDER_MAX_ARGS,overhead_referent_vals,(Referent),0)

    overhead_type& overhead_mut()
    {
        return *this;
    }
    
    Referent& referent_mut()
    {
        return *this;
    }
        
};


}}//exit boost::managed_ptr namespace
#endif
