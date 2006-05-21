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
  , template<typename>class Overhead
  >
  class
auto_overhead
;

template
  < typename Referent
  , template<typename>class Overhead
  >
  class 
auto_refwrap
: public reference_wrapper<auto_overhead<Referent,Overhead> >
//Purpose:
//  Allow use of auto_new::as_auto_refwrap()
{
 public:
        typedef
      Referent
    referent_type
    ;
        typedef
      auto_overhead<referent_type,Overhead>
    auto_ovhd_type
    ;    
        typedef 
      Overhead<referent_type>
    overhead_type
    ;
    
        typedef 
      reference_wrapper<auto_ovhd_type> 
    super_type
    ;
    
    auto_refwrap(auto_refwrap const& a_wrap)
    : super_type(a_wrap)
    {}
    
    auto_refwrap(auto_ovhd_type& a_new)
    : super_type(a_new)
    {
    }
    
    template
      < typename SubRef
      >
    auto_refwrap
      ( typename auto_overhead
        < SubRef
        , Overhead
        >::auto_refwrap& a_new
      )
    : super_type(a_new)
    {
    }
    
    template
      < typename Other
      >
      struct
    rebind
    {
            typedef
          typename auto_overhead
            < Other
            , Overhead
            >::auto_refwrap
        other
        ;
    };
    
    template
      < typename SuperRef
      >
        typename 
      rebind<SuperRef>::other
    as_super(void)
    ;
    
      referent_type* 
    referent()const
    //Purpose:
    //  alias for `operator referent_type*()` method
    ;

      overhead_type* 
    release_overhead()const
    ;

    operator bool()const
    ;

    operator referent_type*()const
    {
        return this->referent();
    }
    
};//end auto_refwrap class
    
template
  < typename Referent
  , template<typename>class Overhead
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
      Overhead<referent_type>
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
      overhead_referent_vals<Overhead,referent_type>
    basis_type
    ;
        typedef
      auto_refwrap<referent_type,Overhead>
    refwrap_type
    ;
    
    ~auto_overhead(void)
    {
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("~auto_overhead(void)");
      #endif
    }

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
      < typename SubRef //subtype of referent_type
      >
    auto_overhead(auto_overhead<SubRef,Overhead>& a_aoh)
    : super_type
      ( a_aoh.referent()
      , a_aoh.release_overhead()
      )
    {
    }

    auto_overhead(refwrap_type const& a_rw)
    : super_type(a_rw.get().referent(), a_rw.get().release_overhead())
    {
    }

    template
      < typename R
      >
    auto_overhead
      ( typename auto_overhead<R,Overhead>::refwrap const& a_aoh
      )
    : super_type(a_aoh.get().referent(), a_aoh.get().release_overhead())
    {
    }

    template
      < typename Other
      >
      struct
    rebind
    {
            typedef
          auto_overhead
            < Other
            , Overhead
            >
        other
        ;
    };
        
    template
      < typename SuperRef
      >
      typename rebind<SuperRef>::other
    as_super(void)
    {
        return typename rebind<SuperRef>::other(*this);
    }
        
    template
      < typename R
      >
    void operator=(auto_overhead<R,Overhead>& a_anm)
    {
        super_ref_type::reset(a_anm.referent());
        super_ovhd_type::operator=(a_anm);
    }

    template
      < typename R
      >
    void reset(overhead_referent_vals<Overhead,R>* a_basis)
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
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("auto_overhead::release_overhead()");
      #endif
        overhead_type* oh=this->overhead();
        super_ovhd_type::release();
        super_ref_type::reset();
      #ifdef TRACE_SCOPE_HPP
        mout()<<"overhead()="<<overhead()<<"\n";
        mout()<<"referent()="<<referent()<<"\n";
      #endif
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

    refwrap_type as_refwrap()
    //Purpose:
    //  Allow auto_new().as_refwrap() to be used as arg to
    //  template<typename U> smart_ptr(U const&).
    //  (can't use auto_new() because OwnerPolicy modifieds
    //  auto_new via auto_ptr::release).
    {
        return refwrap_type(*this);
    }

    operator refwrap_type()
    {
        return this->as_refwrap();
    }
    
 protected:
    auto_overhead(basis_type* a_basis)
    : super_type(&(a_basis->overhead_mut()),&(a_basis->referent_mut()))
    {}
    
 private:
    auto_overhead
      ( referent_type* a_ref
      , overhead_type* a_ovhd
      )
    : super_type(a_ovhd, a_ref)
    {}
};//end auto_overhead

template
  < typename Referent
  , template<typename>class Overhead
  >
    template
      < typename SuperRef
      >
        typename 
auto_refwrap< Referent, Overhead>::      
      rebind<SuperRef>::other
auto_refwrap< Referent, Overhead>::      
    as_super(void)
    {
        typename auto_ovhd_type::rebind<SuperRef>::other l_super_ovhd
        ( this->get().as_super<SuperRef>() );
        typename rebind<SuperRef>::other l_super_wrap(l_super_ovhd);
        return l_super_wrap;
    }
    
template
  < typename Referent
  , template<typename>class Overhead
  >
  typename
auto_refwrap< Referent, Overhead>::      
      referent_type* 
auto_refwrap< Referent, Overhead>::      
    referent()const
    //Purpose:
    //  alias for `operator referent_type*()` method
    {
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("auto_refwrap::referent()");
      #endif
        return super_type::get().referent();
    }

template
  < typename Referent
  , template<typename>class Overhead
  >
  typename
auto_refwrap< Referent, Overhead>::      
      overhead_type* 
auto_refwrap< Referent, Overhead>::      
    release_overhead()const
    {
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("auto_refwrap::release_overhead()");
      #endif
        return super_type::get().release_overhead();
    }

template
  < typename Referent
  , template<typename>class Overhead
  >
auto_refwrap< Referent, Overhead>::      
    operator bool()const
    {
        return super_type::get().is_valid();
    }

}}//exit boost::managed_ptr namespace
//////////////////////////////////////////////////////////////////////////////
// ChangeLog:
//   2004-06-22: Larry Evans
//     WHAT:
//       1)Mv'ed refwrap outside auto_overhead and added auto_overhead
//         template params to refwrap class to create essentially an
//         equivalent non-nested class.
//     WHY:
//       1)Allow refcycle_counted_curry_prox_visitor<ProxVisitor>::
//         owner templated CTOR to be used without non-deduced
//         context ( see c++ standard 14.8.2.4 
//         "Deducing template arguments from a type" paragraph 4 ).
//   2004-04-09: Larry Evans
//     WHAT:
//       1)Rm'ed auto_new and supplied forwarding functions with revised
//         MANAGED_PTR_CTOR_FORWARDER_OVERLOAD
//       2)renamed file
//     WHY:
//       1)Eliminate extra class since no longer needed.
//       2)Obvious
//   2003-12-05: Larry Evans
//     WHAT:
//       added conversion operator to return referent_type* const&.
//     WHY:
//       allow auto_new as arg to storage_policy(stored_type const& p).
//////////////////////////////////////////////////////////////////////////////
#endif
