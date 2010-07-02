/*=============================================================================
    Copyright (c) 2001-2009 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_ALTERNATIVE_FUNCTION_APRIL_23_2007_1046AM)
#define SPIRIT_ALTERNATIVE_FUNCTION_APRIL_23_2007_1046AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/qi/domain.hpp>
#include <boost/spirit/home/support/unused.hpp>
#include <boost/spirit/home/support/attributes.hpp>
#include <boost/variant.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace spirit { namespace qi { namespace detail
{
    template <typename T>
    struct not_is_variant
      : mpl::true_ {};

    template <BOOST_VARIANT_ENUM_PARAMS(typename T)>
    struct not_is_variant<boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> >
      : mpl::false_ {};

    template <typename Iterator, typename Context, typename Skipper,
        typename Attribute>
    struct alternative_function
    {
        alternative_function(
            Iterator& first, Iterator const& last, Context& context,
            Skipper const& skipper, Attribute& attr)
          : first(first), last(last), context(context), skipper(skipper),
            attr(attr)
        {
        }

        template <typename Component>
        bool call(Component const& component, mpl::true_) const
        {
            // if Attribute is not a variant, then pass it as-is
            return component.parse(first, last, context, skipper, attr);
        }

        template <typename Component>
        bool call(Component const& component, mpl::false_) const
        {
            // if Attribute is a variant, then create an attribute for
            // the Component with its expected type.
            typename traits::attribute_of<Component, Context, Iterator>::type val;
            if (component.parse(first, last, context, skipper, val))
            {
                attr = val;
                return true;
            }
            return false;
        }

        template <typename Component>
        bool operator()(Component const& component) const
        {
            // return true if the parser succeeds
            return call(component, not_is_variant<Attribute>());
        }

        Iterator& first;
        Iterator const& last;
        Context& context;
        Skipper const& skipper;
        Attribute& attr;
    };

    template <typename Iterator, typename Context, typename Skipper>
    struct alternative_function<Iterator, Context, Skipper, unused_type const>
    {
        alternative_function(
            Iterator& first, Iterator const& last, Context& context,
            Skipper const& skipper, unused_type)
          : first(first), last(last), context(context), skipper(skipper)
        {
        }

        template <typename Component>
        bool operator()(Component const& component)
        {
            // return true if the parser succeeds
            return component.parse(first, last, context, skipper,
                unused);
        }

        Iterator& first;
        Iterator const& last;
        Context& context;
        Skipper const& skipper;
    };

}}}}

#endif