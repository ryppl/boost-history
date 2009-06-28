//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/luid for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_LUID_DSL_HPP_
#define BOOST_LUID_DSL_HPP_

#if !defined(BOOST_PARAMETER_EXT_EXPR_ARITY)
#    define BOOST_PARAMETER_EXT_EXPR_ARITY 8
#elif BOOST_PARAMETER_EXT_EXPR_ARITY < 8
#    undef BOOST_PARAMETER_EXT_EXPR_ARITY
#    define BOOST_PARAMETER_EXT_EXPR_ARITY 8
#endif

#include "boost/parameter_ext/expr.hpp"
#include "boost/luid/dsl/numeric_type.hpp"
#include "boost/luid/dsl/on_release.hpp"
#include "boost/luid/dsl/on_overflow.hpp"
#include "boost/luid/dsl/scope.hpp"
#include "boost/luid/dsl/lifetime.hpp"
#include "boost/luid/dsl/coherency.hpp"
#include "boost/luid/dsl/optimization.hpp"
#include "boost/unspecified.hpp"
#include <boost/detail/is_xxx.hpp>
#include <boost/parameter/aux_/void.hpp>
#include <boost/parameter/parameters.hpp>

namespace boost {
namespace luid {
namespace dsl {

// introspection
struct introspection_marker{};

template<typename T>
struct is_introspection:is_base_and_derived<introspection_marker,T>
{};

template<typename T=parameter::void_>
struct introspection: parameter::template_keyword<introspection<>,T>
                    , introspection_marker {};

template <typename dummy=parameter::void_> 
struct none: introspection_marker {
};

template <typename dummy=parameter::void_> 
struct count: introspection_marker {
};


// luidg
template <
 typename Arg1=parameter::void_,
 typename Arg2=parameter::void_,
 typename Arg3=parameter::void_,
 typename Arg4=parameter::void_,
 typename Arg5=parameter::void_,
 typename Arg6=parameter::void_,
 typename Arg7=parameter::void_,
 typename Arg8=parameter::void_
     > 
 struct luidg 
{
    typedef     boost::parameter_ext::expr<luidg<>,
    mpl::vector<
        boost::parameter_ext::optional<numeric_type<>, is_numeric_type<mpl::_>, dsl::dynamic_lower_bound<> >,
        boost::parameter_ext::optional<on_overflow<>, is_on_overflow<mpl::_>, dsl::ignore_error<> >,
        boost::parameter_ext::optional<on_release<>, is_on_release<mpl::_>, dsl::discard<> >,
        boost::parameter_ext::optional<scope<>, is_scope<mpl::_>, mono_threaded<> >,
        boost::parameter_ext::optional<lifetime<>, is_lifetime<mpl::_>, process<> >,
        boost::parameter_ext::optional<coherency<>, is_coherency<mpl::_>, ignore<> >,
        boost::parameter_ext::optional<introspection<>, is_introspection<mpl::_>, none<> >,
        boost::parameter_ext::optional<optimization<>, is_optimization<mpl::_>, speed<> >
       >,
       mpl::vector<Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8>
    >  base_type;

   typedef typename base_type::template get<dsl::numeric_type<> >::type numeric_type_kind_;
   typedef typename base_type::template get<dsl::on_release<> >::type on_release;                
   typedef typename base_type::template get<dsl::on_overflow<> >::type on_overflow;                
   typedef typename base_type::template get<dsl::scope<> >::type scope;                
   typedef typename base_type::template get<dsl::lifetime<> >::type lifetime;                
   typedef typename base_type::template get<dsl::coherency<> >::type coherency;                
   typedef typename base_type::template get<dsl::introspection<> >::type introspection;                
   typedef typename base_type::template get<dsl::optimization<> >::type optimization;                  
        
//    typedef typename numeric_type_kind__::numeric_type numeric_type;                        
//    typedef typename numeric_type_kind__::lower_bound_type_ lower_bound_type_;                        
    typedef typename mpl::if_<boost::is_unsigned<numeric_type_kind_>, 
                        numeric_type_kind_, 
                        typename numeric_type_kind_::numeric_type >::type numeric_type;
    typedef typename mpl::if_<boost::is_unsigned<numeric_type_kind_>, 
                        mpl::size_t<boost::integer_traits<numeric_type_kind_>::const_min >, 
                        mpl::size_t<numeric_type_kind_::lower_bound> >::type lower_bound_type_;
    typedef typename mpl::eval_if<boost::is_unsigned<numeric_type_kind_>, 
                        mpl::identity<dsl::dynamic_lower_bound<numeric_type> >, 
                        mpl::identity<numeric_type_kind_> >::type numeric_type_kind;

                        
    static const numeric_type lower_bound = lower_bound_type_::value;
    typedef typename mpl::if_<dsl::is_recover<on_release>, 
                    on_release, 
                    dsl::recover<> >::type recover;
    typedef typename recover::template get<dsl::recovery_when<> >::type recovery_when;                
    typedef typename recover::template get<dsl::recovery_how<> >::type recovery_how;
    typedef typename mpl::if_<dsl::is_freeze<recovery_when>, 
                    recovery_when, 
                    dsl::freeze<> >::type freeze;
    typedef typename mpl::if_<dsl::is_delay<recovery_when>, 
                    recovery_when, 
                    dsl::delay<> >::type delay;
    typedef typename delay::time_traits time_traits;
    
    typedef typename scope::locking_strategy locking_strategy;

    typedef typename locking_strategy::locking_traits locking_traits_;
    typedef typename mpl::if_<boost::is_unspecified<locking_traits_>, 
        typename mpl::if_<
            boost::luid::dsl::is_mono_threaded<scope>, 
            boost::luid::iccl::null_locking_traits,
            typename mpl::if_<
                boost::luid::dsl::is_multi_threaded<scope>, 
                boost::luid::iccl::thread_locking_traits,
                boost::luid::iccl::process_locking_traits
            >::type
        >::type,
        locking_traits_
    >::type locking_traits;
    
    
    typedef typename lifetime::template  get<dsl::void_pointer<> >::type
                        void_pointer_type;
     typedef typename lifetime::template  get<dsl::allocator_type<> >::type
                         allocator_type;
 };

 BOOST_DETAIL_IS_XXX_DEF(luidg, luidg, 4);

} // dsl
} // luid
} // boost
#endif // BOOST_LUID_DSL_HPP_
