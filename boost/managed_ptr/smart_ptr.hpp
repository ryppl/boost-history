//////////////////////////////////////////////////////////////////////////////
// The Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any
//     purpose is hereby granted without fee, provided that the above copy-
//     right notice appear in all copies and that both that copyright notice
//     and this permission notice appear in supporting documentation.
// The author or Addison-Welsey Longman make no representations about the
//     suitability of this software for any purpose. It is provided "as is"
//     without express or implied warranty.
//////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2002, David Held.
//////////////////////////////////////////////////////////////////////////////
// The above Copyrights apply to the following:
//   smart_ptr template class 
// as they appeared around 2003-12-03 22:22 in
//   http://cvs.sourceforge.net/viewcvs.py/boost-sandbox/boost-sandbox/
//     boost/policy_ptr/latest/smart_ptr.hpp
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_MANAGED_PTR_SMART_PTR_HPP_LJE20031009
#define BOOST_MANAGED_PTR_SMART_PTR_HPP_LJE20031009
#include <boost/config.hpp>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility.hpp>
#include <boost/checked_delete.hpp>
#include <boost/type_traits/same_traits.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/aux_/void_spec.hpp>
#include <boost/mpl/aux_/config/eti.hpp>

#include "optimally_inherit.hpp"

#include "boost/managed_ptr/auto_overhead.hpp"
#include "boost/managed_ptr/ref_counted.hpp"
#include "boost/managed_ptr/cycle_basis_mgr.hpp"

//////////////////////////////////////////////////////////////////////////////
// Configuration macros
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_MSVC6_MEMBER_TEMPLATES
# error This library requires member template support.
#endif

#ifdef BOOST_NO_TEMPLATE_TEMPLATES
# undef BOOST_SMART_POINTER_LEGACY_INTERFACE
#endif

//////////////////////////////////////////////////////////////////////////////

#ifdef BOOST_MSVC
# pragma warning(push)
# pragma warning(disable:4181) // qualifier applied to reference type ignored
#endif // BOOST_MSVC

//////////////////////////////////////////////////////////////////////////////

// Use boost::pool?
//#include "SmallObj.h"

#include <typeinfo>                     // std::bad_cast
#include <algorithm>                    // std::less<>, std::swap()
#include <functional>                   // std::binary_function<>
//////////////////////////////////////////////////////////////////////////////
// Used standard assert instead of boost::assert, because the implicit
// behaviour of a Checking Policy is to assert by default, and disable
// assertions with NDEBUG, while boost::assert turns off assertions by
// default, and only enables them with BOOST_DEBUG, which would probably
// suprise many people who select, say, assert_check.
//////////////////////////////////////////////////////////////////////////////
#include <cassert>

namespace boost
{

//////////////////////////////////////////////////////////////////////////////
// class template by_ref
// Transports a reference as a value
// Serves to implement the Colvin/Gibbons trick for smart_ptr
//////////////////////////////////////////////////////////////////////////////

    namespace detail
    {

        //////////////////////////////////////////////////////////////////////////////
        // class template by_ref
        // Transports a reference as a value
        // Serves to implement the Colvin/Gibbons trick for smart_ptr
        //////////////////////////////////////////////////////////////////////////////

        template <typename T>
        class by_ref
        {
        public:
            by_ref(T& value) : value_(value) { }
            operator T&() { return value_; }
            operator T const&() const { return value_; }
        private:
            T& value_;
        };

#ifndef BOOST_SHARED_PTR_HPP_INCLUDED

        struct static_cast_tag { };
        struct dynamic_cast_tag { };
        struct polymorphic_cast_tag { };

#endif

    }   // namespace detail


//////////////////////////////////////////////////////////////////////////////
// class template smart_ptr (declaration)
// The reason for all the fuss below
// Default policy declarations
//////////////////////////////////////////////////////////////////////////////

    struct empty_policy
    { typedef mpl::void_ policy_category; };

    template <typename> struct disallow_conversion;
    template <typename> struct assert_check;
    template <typename> class scalar_storage;

    template
    <
        typename Referent,

        class P1 = empty_policy,
        class P2 = empty_policy,
        class P3 = empty_policy,
        class P4 = empty_policy

    >
    class smart_ptr;
    
}//exit boost namespace

#include <boost/mpl/identity.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/apply_if.hpp>
#include <boost/mpl/is_placeholder.hpp>

namespace boost
{ 

    using mpl::_;

    namespace detail
    //Purpose:
    //  Provide template classes for template meta-programming
    //  purposes.  These templates will be used access policies
    //  by the policy's policy_category nested classes.
    {

# define AUX_NESTED_TYPE_ACCESSOR_DEF(name)                                  \
    template <typename T> struct name                                        \
    { typedef typename T::name type; };

# ifndef BOOST_MPL_MSVC_ETI_BUG
#  define AUX_NESTED_TYPE_ACCESSOR(name)                                     \
    AUX_NESTED_TYPE_ACCESSOR_DEF(name)
# else // BOOST_MPL_MSVC_ETI_BUG
#  define AUX_NESTED_TYPE_ACCESSOR(name)                                     \
    AUX_NESTED_TYPE_ACCESSOR_DEF(name)                                       \
    template <> struct name<int>                                             \
    { typedef int type; };
# endif // BOOST_MPL_MSVC_ETI_BUG

        AUX_NESTED_TYPE_ACCESSOR(pointer_type)
        AUX_NESTED_TYPE_ACCESSOR(const_pointer_type)
        AUX_NESTED_TYPE_ACCESSOR(stored_type)
        AUX_NESTED_TYPE_ACCESSOR(const_stored_type)
        AUX_NESTED_TYPE_ACCESSOR(reference_type)
        AUX_NESTED_TYPE_ACCESSOR(const_reference_type)
        AUX_NESTED_TYPE_ACCESSOR(ownership_category)
        AUX_NESTED_TYPE_ACCESSOR(result_type)

# undef AUX_NESTED_TYPE_ACCESSOR
# undef AUX_NESTED_TYPE_ACCESSOR_DEF

        template <class F, typename T>
        struct apply_lambda
        {
            typedef typename mpl::lambda<F>::type f_;
            typedef typename mpl::apply1<f_, T>::type type;
        };

        template <class Policy>
        struct get_category_impl
        {
            typedef typename Policy::policy_category type;
        };

        template <class Policy>
        struct get_category : public mpl::if_<
            mpl::is_placeholder<Policy>,
            mpl::identity<Policy>,
            get_category_impl<Policy>
        >::type
        {
            BOOST_MPL_AUX_LAMBDA_SUPPORT(1, get_category, (Policy))
        };

        template <class Sequence, class Category, class Default>
        struct get_policy
        {
            typedef typename mpl::find_if<
                Sequence, is_same<get_category<_>, Category>
            >::type iter_;
            typedef typename mpl::end<Sequence>::type last_;
            typedef typename mpl::apply_if<
                typename is_same<iter_, last_>::type,
                mpl::identity<Default>, iter_
            >::type type;
        };

        template <typename T, class Policies>
        struct storage_policy_
        {
            typedef typename apply_lambda<
                typename get_policy<
                            Policies, storage_policy_tag, scalar_storage<_>
                    >::type, T
            >::type type;
        };

        template <typename T, class Policies>
        struct ownership_policy_
        {
            typedef typename storage_policy_<T, Policies>::type policy_;
            typedef typename pointer_type<policy_>::type pointer_type_;
            typedef typename apply_lambda<
                typename get_policy<
                    Policies, ownership_policy_tag, ref_counted<_>
                >::type, pointer_type_
            >::type type;
        };

        template <typename T, class Policies>
        struct checking_policy_
        {
            typedef typename storage_policy_<T, Policies>::type policy_;
            typedef typename stored_type<policy_>::type stored_type_;
            typedef typename apply_lambda<
                typename get_policy<
                    Policies, checking_policy_tag, assert_check<_>
                >::type, stored_type_
            >::type type;
        };

        template <typename T, class Policies>
        struct conversion_policy_
        {
            typedef typename storage_policy_<T, Policies>::type policy_;
            typedef typename pointer_type<policy_>::type pointer_type_;
            typedef typename apply_lambda<
                typename get_policy<
                    Policies, conversion_policy_tag, disallow_conversion<_>
                >::type, pointer_type_
            >::type type;
        };
        
        
        template <class StoragePolicy, class OwnershipPolicy>
        class resource_manager
            : public optimally_inherit<StoragePolicy, OwnershipPolicy>::type
            , protected managed_ptr::cycle_basis_mgr<StoragePolicy, OwnershipPolicy>
        //***DIFF_POLICY_PTR : 
        // Added cycle_basis_mgr superclass
        {
        public:         // Types
            typedef typename optimally_inherit
              < StoragePolicy
              , OwnershipPolicy
              >::type                                 base_type;
            typedef StoragePolicy                     storage_policy;
            typedef OwnershipPolicy                   ownership_policy;
            typedef managed_ptr::cycle_basis_mgr
              < StoragePolicy, OwnershipPolicy>       cycle_basis_type;
        public:         // Structors

            resource_manager()
            : cycle_basis_type(this)
            { }

            template <class SP, class OP>
            resource_manager(resource_manager<SP, OP> const& rhs)
            : base_type(static_cast<
                typename optimally_inherit<SP, OP>::type const&
            >(rhs))
            , cycle_basis_type(this)
            { }

            template <typename U>
            resource_manager(U const& u)
            : base_type(u)
            , cycle_basis_type(this)
            { }

            template <typename U, typename V>
            resource_manager(U const& u, V const& v)
            : base_type(u, v)
            , cycle_basis_type(this)
            { }

            template <typename U, typename V>
            resource_manager(U const& u, V& v)
            : base_type(u, v)
            , cycle_basis_type(this)
            { }

            ~resource_manager()
            {
            #if defined(TRACE_SCOPE_HPP)
                utility::trace_scope ts("~resource_manager");
            #endif
                if (!ownership_policy::release(get_impl(*this)))
                {
                #if defined(TRACE_SCOPE_HPP)
                    utility::trace_scope ts("~resource_manager:storage_policy::release()");
                #endif
                    storage_policy::release();
                }
            }

            void swap(resource_manager& rhs)
            { base_type::swap(rhs); }
        };
        
        
    }//exit namespace detail

#include "BOOST_SMART_POINTER_PARAMS_POLICIES_prefix.hpp"        
# define BOOST_CONVERSION_PARAMETERS                                               \
    class Q1, class Q2, class Q3, class Q4
#define BOOST_CONVERSION_POLICIES                                                  \
    Q1, Q2, Q3, Q4

# define BOOST_STORAGE_POLICY                                                              \
    typename detail::storage_policy_<Referent, mpl::list<P1, P2, P3, P4> >::type
# define BOOST_OWNERSHIP_POLICY                                                            \
    typename detail::ownership_policy_<Referent, mpl::list<P1, P2, P3, P4> >::type
# define BOOST_CHECKING_POLICY                                                             \
    typename detail::checking_policy_<Referent, mpl::list<P1, P2, P3, P4> >::type
# define BOOST_CONVERSION_POLICY                                                           \
    typename detail::conversion_policy_<Referent, mpl::list<P1, P2, P3, P4> >::type

//////////////////////////////////////////////////////////////////////////////
// class template smart_ptr (definition)
//////////////////////////////////////////////////////////////////////////////

template
    < typename Referent
    , BOOST_SMART_POINTER_PARAMETERS
    >
class smart_ptr
    : public optimally_inherit
      < detail::resource_manager
        < BOOST_STORAGE_POLICY
        , BOOST_OWNERSHIP_POLICY
        >
      , typename optimally_inherit
        < BOOST_CHECKING_POLICY
        , BOOST_CONVERSION_POLICY
        >::type
      >::type
{
 public:     // Policy types & pointer_type
    typedef Referent                                    element_type;
    typedef BOOST_STORAGE_POLICY                        storage_policy;
    typedef BOOST_OWNERSHIP_POLICY                      ownership_policy;
    typedef BOOST_CHECKING_POLICY                       checking_policy;
    typedef BOOST_CONVERSION_POLICY                     conversion_policy;
    typedef detail::resource_manager
      < storage_policy
      , ownership_policy
      >                                                 resource_mgr_type;
    typedef typename optimally_inherit
    < resource_mgr_type
    , typename optimally_inherit
      < checking_policy
      , conversion_policy
      >::type
    >::type                                             base_type;
    typedef smart_ptr                                   this_type;
    
 public:     // Pointer/Reference types
    typedef typename storage_policy::pointer_type         pointer_type;
    typedef typename storage_policy::const_pointer_type   const_pointer_type;
    typedef typename storage_policy::stored_type          stored_type;
    typedef typename storage_policy::const_stored_type    const_stored_type;
    typedef typename storage_policy::reference_type       reference_type;
    typedef typename storage_policy::const_reference_type const_reference_type;
 public:  //? types
 
    typedef typename mpl::if_<
        ::boost::is_same<
            typename ownership_policy::ownership_category,
            move_semantics_tag
        >,
        this_type&, this_type const&
    >::type copy_arg;
    
    typedef typename mpl::if_<
        ::boost::is_same<
            typename ownership_policy::ownership_category,
            move_semantics_tag
        >,
        base_type&, base_type const&
    >::type copy_base;

    template <typename U>
    struct rebind
    {
        typedef smart_ptr
          <U, 
           BOOST_SMART_POINTER_POLICIES
          > 
         other;
    };

 public:     // Constructors/Destructor
 
    ~smart_ptr()
    {
    #if defined(TRACE_SCOPE_HPP)
        utility::trace_scope ts("~smart_ptr");
    #endif
    }
    
    smart_ptr()
    {
        checking_policy::on_default(get_impl(*this));
    }
    
    smart_ptr(copy_arg rhs)
        : base_type(static_cast<copy_base>(rhs))
    { 
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("smart_ptr::CTOR(copy_arg)");
      #endif
        element_type* et = get_impl(rhs);
        get_impl_ref(*this) = ownership_policy::clone(et); 
    }

    template <typename U, BOOST_CONVERSION_PARAMETERS>
    smart_ptr(smart_ptr<U, BOOST_CONVERSION_POLICIES> const& rhs)
    : base_type(static_cast<
        typename smart_ptr<U, BOOST_CONVERSION_POLICIES>::base_type const&
    >(rhs))
    { 
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("smart_ptr::CTOR(smart_ptr<U> const&)");
      #endif
        get_impl_ref(*this) = ownership_policy::clone(get_impl(rhs)); 
    }

    template <typename U, BOOST_CONVERSION_PARAMETERS>
    smart_ptr(smart_ptr<U, BOOST_CONVERSION_POLICIES>& rhs)
    : base_type(static_cast<
        typename smart_ptr<U, BOOST_CONVERSION_POLICIES>::base_type const&
    >(rhs))
    { 
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("smart_ptr::CTOR(smart_ptr<U>&)");
      #endif
        get_impl_ref(*this) = ownership_policy::clone(get_impl(rhs)); 
    }

    template <typename U, BOOST_CONVERSION_PARAMETERS>
    smart_ptr(smart_ptr<U, BOOST_CONVERSION_POLICIES> const& rhs,
        detail::static_cast_tag const&)
    : base_type(static_cast<
        typename smart_ptr<U, BOOST_CONVERSION_POLICIES>::base_type const&
    >(rhs))
    {
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("smart_ptr::CTOR(smart_ptr<U>&, static_cast_tag)");
      #endif
        pointer_type l_ptr = static_cast<pointer_type>(get_impl(rhs));
      #ifdef TRACE_SCOPE_HPP
        mout()<<"l_ptr ="<<l_ptr<<std::endl;
        mout()<<"before ownership_policy::clone:use_count="<<use_count(*this)<<std::endl;
      #endif
        get_impl_ref(*this) = ownership_policy::clone(l_ptr);
      #ifdef TRACE_SCOPE_HPP
        mout()<<"after ownership_policy::clone:use_count="<<use_count(*this)<<std::endl;
      #endif
    }

    template <typename U, BOOST_CONVERSION_PARAMETERS>
    smart_ptr(smart_ptr<U, BOOST_CONVERSION_POLICIES> const& rhs,
        detail::dynamic_cast_tag const&)
    : base_type(static_cast<
        typename smart_ptr<U, BOOST_CONVERSION_POLICIES>::base_type const&
    >(rhs))
    {
        get_impl_ref(*this) = ownership_policy::clone(
            dynamic_cast<pointer_type>(get_impl(rhs))
        );
        if (!*this)
        {
            // dynamic_cast<> failed; reset count
            this_type().swap(*this);
        }
    }

    smart_ptr(detail::by_ref<smart_ptr> rhs)
    : base_type(static_cast<base_type&>(static_cast<smart_ptr&>(rhs)))
    { }

        typedef 
      typename resource_mgr_type::cycle_basis_type::basis_spec_type 
    basis_spec_type
    ;
        typedef 
      typename basis_spec_type::basis_source_type 
    basis_source_type
    ;
        typedef 
      typename basis_spec_type::basis_sink_type 
    basis_sink_type
    //***DIFF_POLICY_PTR : Needed a type to substitute for stored_type in
    //  friend release and friend reset functions.  This is because, the
    //  cyclic reference counting ownership policy requires something other
    //  than stored_type.
    ;

    smart_ptr(basis_source_type const& p)
    //***DIFF_POLICY_PTR : Simply taking advantage of availability of the
    //  newly added basis_source_type to provide a more specific CTOR then
    //  the smart_ptr(U const&) one.
    : base_type(p, detail::init_first_tag())
    { 
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("smart_ptr(basis_source_type const&)");
      #endif
        checking_policy::on_init(get_impl(*this)); 
    }

    template
      < typename SubRef
      >
    smart_ptr
      ( typename basis_spec_type::rebind<SubRef>::other::basis_source_type const&p
      )
    //***DIFF_POLICY_PTR : Simply taking advantage of availability of the
    //  newly added basis_source_type to provide a more specific CTOR then
    //  the smart_ptr(U const&) one.
    : base_type(p, detail::init_first_tag())
    { 
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("smart_ptr(basis_spec_type::rebind<SubRef>... const&)");
      #endif
        checking_policy::on_init(get_impl(*this)); 
    }

    template <typename U>
    smart_ptr(U const& p)
    : base_type(p, detail::init_first_tag())
    { 
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("template<typename U>smart_ptr(U const&)");
      #endif
        checking_policy::on_init(get_impl(*this)); 
    }
    
    template <typename U, typename V>
    smart_ptr(U const& p, V const& v)
    : base_type(p, v, detail::init_first_tag())
    { checking_policy::on_init(get_impl(*this)); }

    template <typename U, typename V>
    smart_ptr(U const& p, V& v)
    : base_type(p, v, detail::init_first_tag())
    { checking_policy::on_init(get_impl(*this)); }

 public:     // Ownership modifiers
    operator detail::by_ref<this_type>()
    {
        return detail::by_ref<this_type>(*this);
    }

    smart_ptr& operator=(smart_ptr rhs)
    {
        swap(rhs);
        return *this;
    }

    template <typename U>
    smart_ptr& operator=(U const& u)
    {
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("template<U> smart_ptr::operator=(U const&)");
      #endif
        smart_ptr sp(u);
        swap(sp);
        return *this;
    }

    void swap(this_type& rhs)
    {
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("swap(this_type&)");
      #endif
        base_type::swap(rhs);
    }

    friend inline void release(this_type& sp, basis_sink_type& p)
    //***DIFF_POLICY_PTR : 
    // Moved most of code to super class in order to specialize
    // the code for ownership policy for collecting cycles.
    {
        checking_policy::on_release(get_impl(sp));
        resource_mgr_type& rmt=sp;
        resource_mgr_type::release_to_basis(rmt,p);
    }
    
    friend inline void reset(this_type& sp, basis_source_type p)
    {
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("reset(this_type&, basis_source_type)");
      #endif
        this_type(p).swap(sp);
    }

    template
      < typename SubRef
      >
    friend void reset
      ( this_type& sp
      , typename basis_spec_type::rebind<SubRef>::other::basis_source_type const&p
      //***DIFF_POLICY_PTR : 
      //   Since basis_souce_type depends on ownership policy, the above rebind
      //   is needed.  Unfortunated, this puts SubRef in a non-deduced context
      //   (see c++ standard 14.8.2.4 "Deducing template arguments from a type"
      //   paragraph 4) which, in turn, requires reset to be called with
      //   explicit template argument [e.g. reset<T>(sp,p) ].
      )
    {
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("reset(this_type&, basis_spec_type::rebind...source_type)");
      #endif
        this_type(p).swap(sp);
    }

 public:     // Dereference
    pointer_type operator->() const
    {
        return storage_policy::get_pointer();
    }

    reference_type operator*() const
    {
        return storage_policy::get_reference();
    }

 public:     // Comparison
    bool operator!() const // Enables "if (!sp) ..."
    { 
        return !storage_policy::is_valid(); 
    }
    
#ifndef __BORLANDC__
    inline friend bool operator==(smart_ptr const& lhs, element_type const* rhs)
    { return get_impl(lhs) == rhs; }

    inline friend bool operator==(element_type const* lhs, smart_ptr const& rhs)
    { return rhs == lhs; }

    inline friend bool operator!=(smart_ptr const& lhs, element_type const* rhs)
    { return !(lhs == rhs); }

    inline friend bool operator!=(element_type const* lhs, this_type const& rhs)
    { return rhs != lhs; }
#endif

    // Ambiguity buster
    template <typename U, BOOST_CONVERSION_PARAMETERS>
    bool operator==(smart_ptr<U, BOOST_CONVERSION_POLICIES> const& rhs) const
    { return get_impl(*this) == get_impl(rhs); }

    // Ambiguity buster
    template <typename U, BOOST_CONVERSION_PARAMETERS>
    bool operator!=(smart_ptr<U, BOOST_CONVERSION_POLICIES> const& rhs) const
    { return !(*this == rhs); }

    // Ambiguity buster
    template <typename U, BOOST_CONVERSION_PARAMETERS>
    bool operator<(smart_ptr<U, BOOST_CONVERSION_POLICIES> const& rhs) const
    { return get_impl(*this) < get_impl(rhs); }

 private:    // Helper for enabling 'if (sp)'
    class tester
    {
    public:
        void avoid_gcc_warning_about_all_private_members();
    private:
        void operator delete(void*);
    };

 public:     // enable 'if (sp)'
    operator tester*() const
    {
        if (!*this) return 0;
        static tester t;
        return &t;
    }

 private:
    typedef typename detail::result_type<conversion_policy>::type 
                                            automatic_conversion_result;

// VC6 gets horribly confused by the conversion operator
#ifndef BOOST_MSVC
 public:     // Implicit conversion
    operator automatic_conversion_result() const
    { return get_impl(*this); }
#endif
};

////////////////////////////////////////////////////////////////////////////////
// Exception classes
////////////////////////////////////////////////////////////////////////////////

    struct null_pointer_error : public std::runtime_error
    {
        null_pointer_error() : std::runtime_error("Null Pointer Exception")
        { }
    };

//////////////////////////////////////////////////////////////////////////////
// Default policies
// These are the default policies provided for smart_ptr
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// class template scalar_storage
// Implementation of the StoragePolicy used by smart_ptr
//////////////////////////////////////////////////////////////////////////////

    template <class Policy>
    struct storage_policy_base
    {
        typedef Policy type;
        typedef storage_policy_tag policy_category;
    };

    template <typename T>
    class scalar_storage : public storage_policy_base< scalar_storage<T> >
    {
    public:
        typedef T*          stored_type;        // the type of the pointee_
        typedef T const*    const_stored_type;  //   object
        typedef T*          pointer_type;       // type returned by operator->
        typedef T const*    const_pointer_type;
        typedef typename add_reference<T>::type
                            reference_type;     // type returned by operator*
        typedef typename add_reference<T const>::type
                            const_reference_type;

    protected:
        scalar_storage()
        : pointee_(default_value())
        { }

        // The storage policy doesn't initialize the stored pointer
        //     which will be initialized by the OwnershipPolicy's Clone fn
        template <typename U>
        scalar_storage(scalar_storage<U> const& rhs)
        : pointee_(default_value())
        { }

        scalar_storage(scalar_storage const&)
        : pointee_(default_value())
        { }

        scalar_storage(stored_type p) : pointee_(p)
        { }
        
        ~scalar_storage()
        { 
        #if defined(TRACE_SCOPE_HPP)
            utility::trace_scope ts("~scalar_storage");
            mout()<<"pointee_="<<pointee_<<"\n";
        #endif
            {
            #if defined(TRACE_SCOPE_HPP)
                utility::trace_scope ts("~scalar_storage.checked_delete(pointee_)");
            #endif
                boost::checked_delete(pointee_); 
            }
        }

        void swap(scalar_storage& rhs)
        { 
            std::swap(pointee_, rhs.pointee_);
        }

        pointer_type get_pointer() const
        { return pointee_; }

        reference_type get_reference() const
        { return *pointee_; }

        bool is_valid() const
        { return pointee_ != default_value(); }
        
    public:
        operator bool() const
        { return is_valid(); }
        
        friend inline pointer_type get_impl(scalar_storage const& sp)
        { return sp.pointee_; }

        friend inline stored_type& get_impl_ref(scalar_storage& sp)
        { return sp.pointee_; }

        static stored_type default_value()
        { return 0; }

    protected:
        void release()
        { pointee_ = 0; }

    private:
        stored_type pointee_;

        public:
        BOOST_MPL_AUX_LAMBDA_SUPPORT(1, scalar_storage, (T))
    };

#ifdef __BORLANDC__
    namespace mpl { BOOST_MPL_AUX_VOID_SPEC(1, scalar_storage) }
#endif

//////////////////////////////////////////////////////////////////////////////
// class template array_storage
// Implementation of an array-based StoragePolicy for smart_ptr
//////////////////////////////////////////////////////////////////////////////

    template <typename T>
    class array_storage : public storage_policy_base< array_storage<T> >
    {
    public:
        typedef T*          stored_type;        // the type of the pointee_
        typedef T const*    const_stored_type;  //   object
        typedef T*          pointer_type;       // type returned by operator->
        typedef T const*    const_pointer_type;
        typedef typename add_reference<T>::type
                            reference_type;     // type returned by operator*
        typedef typename add_reference<T const>::type
                            const_reference_type;

    protected:
        array_storage() : pointee_(default_value())
        { }

        // The storage policy doesn't initialize the stored pointer
        //     which will be initialized by the OwnershipPolicy's Clone fn
        template <typename U>
        array_storage(array_storage<U> const&)
        { }

        array_storage(array_storage const&)
        { }

        array_storage(stored_type const& p) : pointee_(p)
        { }

        ~array_storage()
        { boost::checked_array_delete(pointee_); }

        void swap(array_storage& rhs)
        { std::swap(pointee_, rhs.pointee_); }

        const_pointer_type get_pointer() const
        { return pointee_; }

        pointer_type get_pointer()
        { return pointee_; }

        const_reference_type get_reference() const
        { return *pointee_; }

        bool is_valid() const
        { return pointee_ != default_value(); }

    public:
        friend inline pointer_type get_impl(array_storage const& sp)
        { return sp.pointee_; }

        friend inline stored_type& get_impl_ref(array_storage& sp)
        { return sp.pointee_; }

        const_reference_type operator[](int i) const
        {
            BOOST_ASSERT(i >= 0);
            return get_pointer()[i];
        }

        reference_type operator[](int i)
        {
            BOOST_ASSERT(i >= 0);
            return get_pointer()[i];
        }

        static stored_type default_value()
        { return 0; }

    protected:
        void release()
        { pointee_ = 0; }

    private:
        stored_type pointee_;

	public:
        BOOST_MPL_AUX_LAMBDA_SUPPORT(1, array_storage, (T))
    };

#ifdef __BORLANDC__
    namespace mpl { BOOST_MPL_AUX_VOID_SPEC(1, array_storage) }
#endif

////////////////////////////////////////////////////////////////////////////////
// Ownership Policies
////////////////////////////////////////////////////////////////////////////////

    //***DIFF_POLICY_PTR : the ownership policies have been moved to separate
    // .hpp files.  However, several policies in policy_ptr are not available
    // simply because they were not needed to demonstrate cycle collection.
         
////////////////////////////////////////////////////////////////////////////////
// class template destructive_copy
// Implementation of the OwnershipPolicy used by smart_ptr
// Implements destructive copy semantics (a la std::auto_ptr)
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    struct destructive_copy
    {
        typedef ownership_policy_tag policy_category;
        typedef move_semantics_tag ownership_category;

        destructive_copy()
        { }

        template <typename U>
        destructive_copy(destructive_copy<U> const&)
        { }

        destructive_copy(destructive_copy const&)
        { }

        template <typename U>
        destructive_copy(U const&)
        { }

        template <typename U>
        static P clone(U& val)
        {
            P result(val);
            val = U();
            return result;
        }

        static bool release(P const&)
        { return true; }

        template <typename S>
        static void on_release(S const&)
        { }

        template <typename S>
        static void reset(S const&)
        { }

        static void swap(destructive_copy&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template deep_copy
// Implementation of the OwnershipPolicy used by smart_ptr
// Implements deep copy semantics, assumes existence of a clone() member
//     function of the pointee type
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    struct deep_copy
    {
        typedef ownership_policy_tag policy_category;
        typedef copy_semantics_tag ownership_category;

        deep_copy()
        { }

        template <typename U>
        deep_copy(deep_copy<U> const&)
        { }

        deep_copy(deep_copy const&)
        { }

        template <typename U>
        deep_copy(U const&)
        { }

        static P clone(P const& val)
        { return val->clone(); }

        static bool release(P const&)
        { return true; }

        template <typename S>
        static void on_release(S const&)
        { }

        template <typename S>
        static void reset(S const&)
        { }

        static void swap(deep_copy&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template no_copy
// Implementation of the OwnershipPolicy used by smart_ptr
// Implements a policy that doesn't allow copying objects
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    struct no_copy
    {
        typedef ownership_policy_tag policy_category;
        typedef no_copy_semantics_tag ownership_category;

        no_copy()
        { }

        template <typename U>
        no_copy(no_copy<U> const&)
        { }

        no_copy(no_copy const&)
        { }

        template <typename U>
        no_copy(U)
        { }

        static P clone(P const&)
        {
            // This Policy Disallows Value Copying
            BOOST_STATIC_ASSERT(false);
        }

        static bool release(P const&)
        { return true; }

        template <typename S>
        static void on_release(S const&)
        { }

        template <typename S>
        static void reset(S const&)
        { }

        static void swap(no_copy&)
        { }
    };

    struct no_copy_
    {
        typedef ownership_policy_tag policy_category;
        template <typename P>
        struct apply { typedef no_copy<P> type; };
    };

////////////////////////////////////////////////////////////////////////////////
// class template allow_conversion
// Implementation of the ConversionPolicy used by smart_ptr
// Allows implicit conversion from smart_ptr to the pointee type
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    struct allow_conversion
    {
        typedef conversion_policy_tag policy_category;

        typedef P result_type;

        allow_conversion()
        { }

        template <typename U>
        allow_conversion(allow_conversion<U> const&)
        { }

        allow_conversion(allow_conversion const&)
        { }

        static void swap(allow_conversion&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template disallow_conversion
// Implementation of the ConversionPolicy used by smart_ptr
// Does not allow implicit conversion from smart_ptr to the pointee type
// You can initialize a disallow_conversion with an allow_conversion
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    struct disallow_conversion
    {
        typedef disallow_conversion type;
        typedef conversion_policy_tag policy_category;

        struct disallow_conversion_result
        {
            disallow_conversion_result(P const&)
            { }
        };

        typedef disallow_conversion_result result_type;

        disallow_conversion()
        { }

        template <typename U>
        disallow_conversion(disallow_conversion<U> const&)
        { }

        disallow_conversion(disallow_conversion const&)
        { }

        template <typename U>
        disallow_conversion(allow_conversion<U> const&)
        { }

        static void swap(disallow_conversion&)
        { }

        BOOST_MPL_AUX_LAMBDA_SUPPORT(1, disallow_conversion, (P))
    };

#ifdef __BORLANDC__
    namespace mpl { BOOST_MPL_AUX_VOID_SPEC(1, disallow_conversion) }
#endif

////////////////////////////////////////////////////////////////////////////////
// class template no_check
// Implementation of the CheckingPolicy used by smart_ptr
// Well, it's clear what it does :o)
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    struct no_check
    {
        typedef checking_policy_tag policy_category;

        no_check()
        { }

        template <typename U>
        no_check(no_check<U> const&)
        { }

        no_check(no_check const&)
        { }

        static void on_default(P const&)
        { }

        static void on_init(P const&)
        { }

        static void on_dereference(P const&)
        { }

        static void on_release(P const&)
        { }

        static void swap(no_check&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template assert_check
// Implementation of the CheckingPolicy used by smart_ptr
// Checks the pointer before dereference
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    struct assert_check
    {
        typedef checking_policy_tag policy_category;
        typedef assert_check type;

        assert_check()
        { }

        template <typename U>
        assert_check(assert_check<U> const&)
        { }

        assert_check(assert_check const&)
        { }

        template <typename U>
        assert_check(no_check<U> const&)
        { }

        static void on_default(P const&)
        { }

        static void on_init(P const&)
        { }

        static void on_dereference(P const& val)
        { assert(val); }

        static void on_release(P const&)
        { }

        static void swap(assert_check&)
        { }

    public:
        BOOST_MPL_AUX_LAMBDA_SUPPORT(1, assert_check, (P))
    };

#ifdef __BORLANDC__
    namespace mpl { BOOST_MPL_AUX_VOID_SPEC(1, assert_check) }
#endif

////////////////////////////////////////////////////////////////////////////////
// class template assert_check_strict
// Implementation of the CheckingPolicy used by smart_ptr
// Checks the pointer against zero upon initialization and before dereference
// You can initialize an assert_check_strict with an assert_check
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    struct assert_check_strict
    {
        typedef checking_policy_tag policy_category;

        assert_check_strict()
        { }

        template <typename U>
        assert_check_strict(assert_check_strict<U> const&)
        { }

        assert_check_strict(assert_check_strict const&)
        { }

        template <typename U>
        assert_check_strict(assert_check<U> const&)
        { }

        template <typename U>
        assert_check_strict(no_check<U> const&)
        { }

        static void on_default(P const& val)
        { assert(val); }

        static void on_init(P const& val)
        { assert(val); }

        static void on_dereference(P const& val)
        { assert(val); }

        static void on_release(P const&)
        { }

        static void swap(assert_check_strict&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template reject_null_static
// Implementation of the CheckingPolicy used by smart_ptr
// Checks the pointer upon initialization and before dereference
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    struct reject_null_static
    {
        typedef checking_policy_tag policy_category;

        reject_null_static()
        { }

        template <typename U>
        reject_null_static(reject_null_static<U> const&)
        { }

        reject_null_static(reject_null_static const&)
        { }

        template <typename U>
        reject_null_static(no_check<U> const&)
        { }

        template <typename U>
        reject_null_static(assert_check<U> const&)
        { }

        template <typename U>
        reject_null_static(assert_check_strict<U> const&)
        { }

        static void on_default(P const&)
        {
            // This Policy Does Not Allow Default Initialization
            BOOST_STATIC_ASSERT(false);
        }

        static void on_init(P const& val)
        { if (!val) throw null_pointer_error(); }

        static void on_dereference(P const& val)
        { if (!val) throw null_pointer_error(); }

        static void on_release(P const&)
        { }

        static void swap(reject_null_static&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template reject_null
// Implementation of the CheckingPolicy used by smart_ptr
// Checks the pointer before dereference
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    struct reject_null
    {
        typedef checking_policy_tag policy_category;

        reject_null()
        { }

        template <typename U>
        reject_null(reject_null<U> const&)
        { }

        reject_null(reject_null const&)
        { }

        static void on_default(P const&)
        { }

        static void on_init(P const&)
        { }

        static void on_dereference(P const& val)
        { if (!val) throw null_pointer_error(); }

        static void on_release(P const&)
        { }

        static void swap(reject_null&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// class template reject_null_strict
// Implementation of the CheckingPolicy used by smart_ptr
// Checks the pointer upon initialization and before dereference
////////////////////////////////////////////////////////////////////////////////

    template <typename P>
    struct reject_null_strict
    {
        typedef checking_policy_tag policy_category;

        reject_null_strict()
        { }

        template <typename U>
        reject_null_strict(reject_null_strict<U> const&)
        { }

        reject_null_strict(reject_null_strict const&)
        { }

        template <typename U>
        reject_null_strict(reject_null<U> const&)
        { }

        static void on_init(P const& val)
        { if (!val) throw null_pointer_error(); }

        static void on_default(P const& val)
        {
            // This Policy Does Not Allow Default Initialization
            BOOST_STATIC_ASSERT(false);
        }

        static void on_dereference(P const& val)
        { if (!val) throw null_pointer_error(); }

        static void on_release(P const&)
        { }

        static void swap(reject_null_strict&)
        { }
    };

////////////////////////////////////////////////////////////////////////////////
// free comparison operators for class template smart_ptr
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// operator== for lhs = smart_ptr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline bool operator==(smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& lhs, U const* rhs)
    {
        return get_impl(lhs) == rhs;
    }

////////////////////////////////////////////////////////////////////////////////
// operator== for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline bool operator==(U const* lhs, smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& rhs)
    {
        return rhs == lhs;
    }

////////////////////////////////////////////////////////////////////////////////
// operator!= for lhs = smart_ptr, rhs = raw pointer
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline bool operator!=(smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& lhs, U const* rhs)
    {
        return !(lhs == rhs);
    }

////////////////////////////////////////////////////////////////////////////////
// operator!= for lhs = raw pointer, rhs = smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline bool operator!=(U const* lhs, smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& rhs)
    {
        return rhs != lhs;
    }

////////////////////////////////////////////////////////////////////////////////
// Smart Pointer Concept Interface
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// C++-style cast operators
// These were shamelessly ripped off from boost::shared_ptr
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// static_pointer_cast
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline smart_ptr<T, BOOST_SMART_POINTER_POLICIES>
        static_pointer_cast(smart_ptr<U, BOOST_SMART_POINTER_POLICIES> const& p)
    {
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("static_pointer_cast");
      #endif
        return smart_ptr<T, BOOST_SMART_POINTER_POLICIES>(
            p, detail::static_cast_tag()
        );
    }

////////////////////////////////////////////////////////////////////////////////
// dynamic_cast
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS, typename U>
    inline smart_ptr<T, BOOST_SMART_POINTER_POLICIES>
        dynamic_pointer_cast(smart_ptr<U, BOOST_SMART_POINTER_POLICIES> const& p)
    {
        return smart_ptr<T, BOOST_SMART_POINTER_POLICIES>(
            p, detail::dynamic_cast_tag()
        );
    }

////////////////////////////////////////////////////////////////////////////////
// get_pointer() enables boost::mem_fn to recognize smart_ptr
////////////////////////////////////////////////////////////////////////////////

    template <typename T, BOOST_SMART_POINTER_PARAMETERS>
    inline typename smart_ptr<T, BOOST_SMART_POINTER_POLICIES>::pointer_type
        get_pointer(smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& p)
    {
        return get_impl(p);
    }

    template <typename T, BOOST_SMART_POINTER_PARAMETERS>
    struct less
        : public std::binary_function<
            smart_ptr<T, BOOST_SMART_POINTER_POLICIES>,
            smart_ptr<T, BOOST_SMART_POINTER_POLICIES>,
            bool
        >
    {
        bool operator()(smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& lhs,
                        smart_ptr<T, BOOST_SMART_POINTER_POLICIES> const& rhs) const
        {
            return std::less<typename
                smart_ptr<T, BOOST_SMART_POINTER_POLICIES>::pointer_type
            >()(get_impl(lhs), get_impl(rhs));
        }
    };

    template <typename T, BOOST_SMART_POINTER_PARAMETERS>
    void swap(smart_ptr<T, BOOST_SMART_POINTER_POLICIES>& lhs,
              smart_ptr<T, BOOST_SMART_POINTER_POLICIES>& rhs)
    {
        lhs.swap(rhs);
    }
    
    namespace managed_ptr
    {
    
    template
      < typename Referent
      , class StoragePolicy
      , class OwnershipPolicy
      >
//            static 
          void 
    cycle_basis_mgr_nester
      < Referent
      , StoragePolicy
      , OwnershipPolicy
      , void
      >::
        release_to_basis
          ( 
            cycle_basis_mgr_nester
            < Referent
            , StoragePolicy
            , OwnershipPolicy
            , void
            >::
            resource_mgr_type& source //of Referent*
          , 
            cycle_basis_mgr_nester
            < Referent
            , StoragePolicy
            , OwnershipPolicy
            , void
            >::
            basis_sink_type& target //for Referent*
          )
        {
            OwnershipPolicy::on_release(source);
            target = get_impl_ref(source);
            get_impl_ref(source) = StoragePolicy::default_value();
            OwnershipPolicy::reset(source);
        }
    
    } //exit managed_ptr namespace


}//exit boost namespace

#ifdef BOOST_SMART_POINTER_ORDERING_OPERATORS
# include "ordering_operators.hpp"
#endif

#undef BOOST_STORAGE_POLICY
#undef BOOST_OWNERSHIP_POLICY
#undef BOOST_CHECKING_POLICY
#undef BOOST_CONVERSION_POLICY
#undef BOOST_SMART_CONVERSION_POLICIES
#undef BOOST_SMART_CONVERSION_PARAMETERS
#include "BOOST_SMART_POINTER_PARAMS_POLICIES_suffix.hpp"
#ifdef BOOST_MSVC
# pragma warning(pop)
#endif // BOOST_MSVC

//////////////////////////////////////////////////////////////////////////////
// ChangeLog:
//   2003-12-04: Larry Evans
//     WHAT:
//       "merged" with download from policy_ptr latest.  This involved:
//        1) rm'ing ~smart_ptr and release_me.
//     WHY:
//        1) now handled by ~resource_manager.
//   2003-12-06: Larry Evans
//     WHAT:
//       Changed template args to resource_manager
//     WHY:
//       See resource_manager.hpp ChangeLog for this date.
//   2004-03-05: Larry Evans
//     WHAT:
//       Re-incorporated resource_manager in this file.
//     WHY:
//       Delegated the specialization's to superclasses in order to
//       minimize differences with original policy_ptr resource_manager.
//////////////////////////////////////////////////////////////////////////////
#endif
