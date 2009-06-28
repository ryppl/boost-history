/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_DSL_LIFETIME_HPP_
#define BOOST_LUID_DSL_LIFETIME_HPP_

#if !defined(BOOST_PARAMETER_EXT_EXPR_ARITY)
#    define BOOST_PARAMETER_EXT_EXPR_ARITY 2
#elif BOOST_PARAMETER_EXT_EXPR_ARITY < 2
#    undef BOOST_PARAMETER_EXT_EXPR_ARITY
#    define BOOST_PARAMETER_EXT_EXPR_ARITY 2
#endif


#include "boost/parameter_ext/expr.hpp"
#include <memory>
#include <boost/detail/is_xxx.hpp>
#include "boost/interprocess/allocators/allocator.hpp"
#include "boost/interprocess/managed_shared_memory.hpp"
#include "boost/interprocess/managed_mapped_file.hpp"
#include <boost/parameter/aux_/void.hpp>
#include <boost/parameter/parameters.hpp>

namespace boost {
namespace luid {
namespace dsl {
struct lifetime_marker{};

template<typename T>
struct is_lifetime:is_base_and_derived<lifetime_marker,T>
{};

template<typename T=parameter::void_>
struct lifetime: parameter::template_keyword<lifetime<>,T>,lifetime_marker {};

struct void_pointer_marker{};

template<typename T>
struct is_void_pointer:is_base_and_derived<void_pointer_marker,T>
{};

template<typename T=parameter::void_>
struct void_pointer: parameter::template_keyword<void_pointer<>,T>
        , void_pointer_marker {};

struct allocator_marker{};

template<typename T>
struct is_allocator_type:is_base_and_derived<allocator_marker,T> {};

template<typename T=parameter::void_>
struct allocator_type: parameter::template_keyword<allocator_type<>,T>,allocator_marker
{};


template <
    typename Arg1=parameter::void_,
    typename Arg2=parameter::void_
> 
struct process : lifetime_marker, 
    boost::parameter_ext::expr<process<>, 
        mpl::vector<
            boost::parameter_ext::optional<void_pointer<>, is_void_pointer<mpl::_>, 
                void* >,
            boost::parameter_ext::optional<allocator_type<>, is_allocator_type<mpl::_>, 
                std::allocator<int> >
           >, 
           mpl::vector<Arg1, Arg2> 
    >  
    {};    

BOOST_DETAIL_IS_XXX_DEF(process, process, 2);

template <
    typename Arg1=parameter::void_,
    typename Arg2=parameter::void_
> 
struct kernel : lifetime_marker, 
    boost::parameter_ext::expr<kernel<>, 
        mpl::vector<
            boost::parameter_ext::optional<void_pointer<>, is_void_pointer<mpl::_>, 
                boost::interprocess::offset_ptr<void> >,
            boost::parameter_ext::optional<allocator_type<>, is_allocator_type<mpl::_>, 
                boost::interprocess::allocator
                    <int, boost::interprocess::managed_shared_memory::segment_manager> >
           >, 
           mpl::vector<Arg1, Arg2> 
    >  
    {};    

BOOST_DETAIL_IS_XXX_DEF(kernel, kernel, 2);

template <
    typename Arg1=parameter::void_,
    typename Arg2=parameter::void_
> 
struct filesystem : lifetime_marker, 
    boost::parameter_ext::expr<filesystem<>, 
        mpl::vector<
            boost::parameter_ext::optional<void_pointer<>, is_void_pointer<mpl::_>, 
                boost::interprocess::offset_ptr<void> >,
            boost::parameter_ext::optional<allocator_type<>, is_allocator_type<mpl::_>, 
                boost::interprocess::allocator
                    <int, boost::interprocess::managed_mapped_file::segment_manager> >
           >, 
           mpl::vector<Arg1, Arg2> 
    >  
    {};    

BOOST_DETAIL_IS_XXX_DEF(filesystem, filesystem, 2);
    

 
} // dsl
} // luid
} // boost
#endif // BOOST_LUID_DSL_LIFETIME_HPP_
