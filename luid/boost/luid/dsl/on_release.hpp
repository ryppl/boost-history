/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_DSL_ON_RELEASE_HPP_
#define BOOST_LUID_DSL_ON_RELEASE_HPP_

#if !defined(BOOST_PARAMETER_EXT_EXPR_ARITY)
#    define BOOST_PARAMETER_EXT_EXPR_ARITY 2
#elif BOOST_PARAMETER_EXT_EXPR_ARITY < 2
#    undef BOOST_PARAMETER_EXT_EXPR_ARITY
#    define BOOST_PARAMETER_EXT_EXPR_ARITY 2
#endif

#include "boost/parameter_ext/expr.hpp"
#include "boost/luid/iccl/posix_time_traits.hpp"
#include <boost/detail/is_xxx.hpp>
#include <boost/parameter/aux_/void.hpp>
#include <boost/parameter/parameters.hpp>

namespace boost {
namespace luid {
namespace dsl {

struct on_release_marker{};

template<typename T>
struct is_on_release:is_base_and_derived<on_release_marker,T>
{};

template<typename T=parameter::void_>
struct on_release: parameter::template_keyword<on_release<>,T>
{};

// Recovery When
struct recovery_when_marker{};

template<typename T>
struct is_recovery_when: is_base_and_derived<recovery_when_marker,T>
{};

template<typename T=parameter::void_>
struct recovery_when: parameter::template_keyword<recovery_when<>,T>
{};


template <typename dummy=parameter::void_> 
struct discard : on_release_marker {};
BOOST_DETAIL_IS_XXX_DEF(discard, discard, 1);
     
 
template <typename dummy=parameter::void_> 
struct immediate : recovery_when_marker {};
BOOST_DETAIL_IS_XXX_DEF(immediate, immediate, 1);

template <typename TimeTraits=iccl::posix_time_traits> 
struct delay : recovery_when_marker{
   typedef TimeTraits time_traits;
};
BOOST_DETAIL_IS_XXX_DEF(delay, delay, 1);

template <typename dummy=parameter::void_> 
struct freeze : recovery_when_marker {};
BOOST_DETAIL_IS_XXX_DEF(freeze, freeze, 1);

// Recovery How
struct recovery_how_marker{};

template<typename T>
struct is_recovery_how: is_base_and_derived<recovery_how_marker,T>
{};

template<typename T=parameter::void_>
struct recovery_how: parameter::template_keyword<recovery_how<>,T>
{};

template <typename dummy=parameter::void_> 
struct fifo : recovery_how_marker {};
      BOOST_DETAIL_IS_XXX_DEF(fifo, fifo, 1);

template <typename dummy=parameter::void_> 
struct undefined : recovery_how_marker {};
      BOOST_DETAIL_IS_XXX_DEF(undefined, undefined, 1);

template <
    typename Arg1=parameter::void_,
    typename Arg2=parameter::void_
> 
struct recover : on_release_marker, 
    boost::parameter_ext::expr<recover<>, 
        mpl::vector<
            boost::parameter_ext::optional<recovery_when<>, is_recovery_when<mpl::_>, immediate<> >,
            boost::parameter_ext::optional<recovery_how<>, is_recovery_how<mpl::_>, fifo<> >
           >, 
           mpl::vector<Arg1, Arg2> 
    >  
{};    

BOOST_DETAIL_IS_XXX_DEF(recover, recover, 2);


 
} // dsl
} // luid
} // boost
#endif // BOOST_LUID_DSL_ON_RELEASE_HPP_
