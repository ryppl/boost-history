//auto_ptr which creates pointee
#ifndef BOOST_MANAGED_PTR_AUTO_NEW_LJE20031009_HPP
#define BOOST_MANAGED_PTR_AUTO_NEW_LJE20031009_HPP
//Acknowlegements:
//  2003-10-09:
//    Special thanks to Fernando Cacciola
//    for the following post:
//      http://article.gmane.org/gmane.comp.lib.boost.devel/26211
//  2003-11-08:
//    The auto_overhead code appears to do about what David Abraham's 
//    auto_ptr_new<T> class in:
//      http://lists.boost.org/MailArchives/boost/msg42996.php
//    does, except that it also allocates an "overhead" with the referent.
//     I just read this post; so, I apologize for not acknowledging
//    earlier.

#include "boost/managed_ptr/referent_overhead_ptrs.hpp"
#include "boost/managed_ptr/typed_ptr.hpp"
#include "boost/ref.hpp"
#include <memory>//for auto_ptr

namespace boost{

namespace detail{

        template <class StoragePolicy, class OwnershipPolicy>
        class resource_manager
        ;

}//exit detail namespace

namespace managed_ptr{

    static
  mpl::vector<void>
default_ctor_tag
//signal's that auto_overhead use default Referent CTOR.
;

template
  < typename Referent
  , typename Overhead
    //REQUIRES:
    //  Overhead is a template class like:
    //
    //    template<typename R>class over_head{...};
    //
    //  with a templated member, rebind, such that:
    //
    //    overhead<R>::rebind<O>::other
    // 
    //  is the same as(e.g. typedef'ed to):
    //
    //    overhead<O>
    //
  >
  class 
auto_overhead
//Purpose:
//  Allocate Overhead<Referent> as well as Referent with single call
//  to new.  Overhead<Referent> is meant to store the "overhead"
//  of, for example, a reference counted garbaged collection smart
//  pointer.  In this case Overhead<Referent> would simply be the
//  reference count, e.g. a `long unsigned`.
  : private referent_overhead_ptrs
    < Referent
    , boost::managed_ptr::typed_ptr
    , Overhead
    , std::auto_ptr
    >
{
 public:
        typedef
      referent_overhead_ptrs
        < Referent
        , boost::managed_ptr::typed_ptr
        , Overhead
        , std::auto_ptr
        >
    super_type
    ;
        typedef
      Referent
    referent_type
    ;
        typedef
      Overhead
    overhead_type
    ;
        typedef
      std::auto_ptr<overhead_type>
    super_ovhd_type
    ;
        typedef
      typed_ptr<referent_type>
    super_ref_type
    ;
    
        typedef
      overhead_referent_vals<overhead_type,referent_type>
    basis_type
    ;
    
    class refwrap
    : public reference_wrapper<auto_overhead<Referent,Overhead> >
    //Purpose:
    //  Allow use of auto_new::as_refwrap()
    {
     public:
            typedef
          auto_overhead<Referent,Overhead>
        auto_ovhd_type
        ;    
            typedef 
          reference_wrapper<auto_ovhd_type> 
        super_type
        ;
        
        refwrap(refwrap const& a_wrap)
        : super_type(a_wrap)
        {}
        
        refwrap(auto_ovhd_type& a_new)
        : super_type(a_new)
        {
        }
        
        template<typename SubRef>
        refwrap
          ( typename auto_overhead
            < SubRef
            , typename Overhead::rebind<SubRef>::other
            >::refwrap& a_new
          )
        : super_type(a_new)
        {
        }
        
        referent_type* referent()const
        //Purpose:
        //  alias for `operator referent_type*()` method
        {
            utility::trace_scope ts("refwrap::referent()");
            return super_type::get().referent();
        }

        overhead_type* release_overhead()const
        {
            utility::trace_scope ts("refwrap::release_overhead()");
            return super_type::get().release_overhead();
        }

        operator referent_type*()const
        {
            return this->referent();
        }
        
        operator bool()const
        {
            return super_type::get().is_valid();
        }
    
    };//end refwrap class
    
    auto_overhead(void)
    {}

    MANAGED_PTR_CTOR_FORWARDER_OVERLOAD(MANAGED_PTR_CTOR_FORWARDER_MAX_ARGS,auto_overhead,(super_type)(new basis_type),1)

    template
      < template<typename>class StoragePolicy
      , template<typename>class OwnershipPolicy
      >
    auto_overhead(detail::resource_manager<StoragePolicy<referent_type>,OwnershipPolicy<referent_type*> >& a_rm)
    ;

    template
      < typename R
      >
    auto_overhead(auto_overhead<R,typename Overhead::rebind<R>::other>& a_aoh)
    : super_type(a_aoh)
    {
    }

    auto_overhead(refwrap const& a_rw)
    : super_type(a_rw.get())
    {
    }

    template
      < typename R
      >
    auto_overhead
      ( typename auto_overhead<R,typename Overhead::rebind<R>::other>::refwrap const& a_aoh
      )
    : super_type(a_aoh.get().referent(), a_aoh.get().release_overhead())
    {
    }

    template
      < typename R
      >
    void operator=(auto_overhead<R,typename Overhead::rebind<R>::other>& a_anm)
    {
        super_ref_type::reset(a_anm.referent());
        super_ovhd_type::operator=(a_anm);
    }

    template
      < typename R
      >
    void reset(overhead_referent_vals<typename Overhead::rebind<R>::other,R>* a_basis)
    //REQUIRES:
    //  a_basis points to heap
    {
        super_ref_type::reset(&(a_basis->referent_mut()));
        super_ovhd_type::reset(&(a_basis->overhead_mut()));
    }

    referent_type* referent()const
    {
        return super_type::referent_mut();
    }

    overhead_type* overhead()const
    {
        return super_type::overhead_mut();
    }

    overhead_type* release_overhead()
    {
        utility::trace_scope ts("auto_overhead::release_overhead()");
        overhead_type* oh=this->overhead();
        super_ovhd_type::release();
        super_ref_type::reset();
        mout()<<"overhead()="<<overhead()<<"\n";
        mout()<<"referent()="<<referent()<<"\n";
        return oh;
    }

    bool is_valid()const
    {
        return overhead() != 0;
    }
    
    operator bool()const
    {
        return is_valid();
    }

    refwrap as_refwrap()
    //Purpose:
    //  Allow auto_new().as_refwrap() to be used as arg to
    //  template<typename U> smart_ptr(U const&).
    //  (can't use auto_new() because OwnerPolicy modifieds
    //  auto_new via auto_ptr::release).
    {
        return refwrap(*this);
    }

    operator refwrap()
    {
        return this->as_refwrap();
    }
    
 protected:
    auto_overhead(basis_type* a_basis)
    : super_type(&(a_basis->overhead_mut()),&(a_basis->referent_mut()))
    {}
};//end auto_overhead
}}//exit boost::managed_ptr namespace
//////////////////////////////////////////////////////////////////////////////
// ChangeLog:
//   2004-04-09: Larry Evans
//     WHAT:
//       1)Rm'ed auto_new and supplied forwarding functions with revised
//         MANAGED_PTR_CTOR_FORWARDER_OVERLOAD
//       2)renamed file
//     WHY:
//       1)Eliminate extra class since no longer needed.
//       2)Obvious
// ChangeLog:
//   2003-12-05: Larry Evans
//     WHAT:
//       added conversion operator to return referent_type* const&.
//     WHY:
//       allow auto_new as arg to storage_policy(stored_type const& p).
//////////////////////////////////////////////////////////////////////////////
#endif
