// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_INTERFACE_BASE_HPP_INCLUDED
#define BOOST_IDL_DETAIL_INTERFACE_BASE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <stdexcept>
#include <utility>          // pair.
#include <boost/config.hpp> // BOOST_MSVC.
#include <boost/interfaces/detail/constants.hpp>
#include <boost/interfaces/detail/empty_base.hpp>
#include <boost/interfaces/detail/fn_ptr.hpp>
#include <boost/interfaces/detail/generate.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/transform_view.hpp>

#include <boost/mpl/size.hpp>
#include <boost/interfaces/detail/proxy.hpp>

#ifdef BOOST_MSVC
# pragma warning(push)          // reinterpret_cast: conversion to type of 
# pragma warning(disable:4312)  // greater size
#endif

namespace boost { namespace interfaces { namespace detail {

template<typename Index, int Size>
struct sequence_from_index {
    typedef mpl::range_c<int, 0, Size>                 numeric_range;
    typedef mpl::transform_view<numeric_range, Index>  generators;
    typedef mpl::filter_view<
                generators, 
                mpl::not_< is_void<mpl::_> > 
            >                                          type;
};

template<typename Advice>
struct null_generator {
    template< typename Derived, 
              typename Offset = mpl::int_<0>, 
              typename Flags = flags::default_, 
              typename Base = empty_base >
    struct apply {
        struct type : Advice {
            struct interface_metadata {
                typedef Derived         derived_type;
                typedef Offset          offset_type;
                typedef Flags           flags_type;
                typedef Advice          base_type;
                typedef mpl::vector0<>  signatures;
                typedef mpl::true_      implements;
                static const int size = 0;
                struct proxy { };
                template<typename X>
                static void initialize(fn_ptr* table) { }
                std::pair<const char*, fn_ptr>
                lookup(int n, fn_ptr* table)
                { throw std::logic_error("no such function"); }
            };
        };
    };
};

struct get_generator {
    template<typename Interface>
    struct apply {
        typedef typename Interface::interface_generator type;
    };
};

template<typename Interface, typename InterfaceImpl, int Lines>
struct interface_base {
    struct base_generator {
        template< typename Derived, 
                  typename Offset = offset::default_, 
                  typename Flags = flags::default_,
                  typename Base = empty_base >
        struct apply {
            typedef typename 
                    InterfaceImpl::template
                    inner<
                        Base
                    >                                      impl_type;
            typedef typename impl_type::supers_idl_::type  superinterfaces;
            typedef mpl::transform_view<
                        superinterfaces,
                        get_generator
                    >                                      generators;
            typedef typename 
                    mpl::fold<
                        generators, 
                        null_generator<impl_type>,
                        generate
                    >::type                                generator;
            typedef typename 
                    generator::template
                    apply<
                        Derived, 
                        mpl::int_<Offset::value + 1>, 
                        Flags, 
                        Base
                    >::type                                base_type;
            struct type : base_type { };
        };
    };

    struct generator {
        template< typename Derived, 
                  typename Offset = offset::default_, 
                  typename Flags = flags::default_,
                  typename Base = empty_base >
        struct apply {
            typedef typename
                    InterfaceImpl::template
                    inner<Base>                      impl_type;
            typedef typename 
                    impl_type::supers_idl_::type     superinterfaces;
            typedef typename impl_type::index_idl_   index;
            typedef typename
                    sequence_from_index<
                        index,
                        Lines
                    >::type                          function_generators;
            typedef typename 
                    mpl::fold<
                        function_generators, 
                        base_generator, 
                        generate
                    >::type                          generator;
            typedef typename 
                    generator::template
                    apply<
                        Derived, 
                        Offset, 
                        Flags, 
                        Base
                    >::type                          base_type;
            struct type : base_type {
                typedef typename base_type::interface_metadata base_metadata;
                struct interface_metadata : base_metadata {
                    static const int size = base_metadata::size + 1;
                    typedef superinterfaces base_list;
                    struct proxy
                        : boost::interfaces::detail::proxy<base_list>::type
                        { };
                    template<typename X>
                    static void initialize(fn_ptr* table)
                    {
                        static const int offset = Offset::value - 1;
                        *(table + offset) = 
                            reinterpret_cast<fn_ptr>(
                                offset + offset::initial::value
                            );
                        base_metadata::template initialize<X>(table);
                    }
                };
            };
        };
    };

    struct type : generator::template apply<Interface, mpl::int_<1> >::type {
        typedef generator interface_generator;
    };
};

} } } // End namespace detail, interfaces, boost.

#ifdef BOOST_MSVC
# pragma warning(pop)
#endif

#endif // #ifndef BOOST_IDL_DETAIL_INTERFACE_BASE_HPP_INCLUDED
