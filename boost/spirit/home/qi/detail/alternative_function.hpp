/*=============================================================================
    Copyright (c) 2001-2011 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_ALTERNATIVE_FUNCTION_APRIL_23_2007_1046AM)
#define SPIRIT_ALTERNATIVE_FUNCTION_APRIL_23_2007_1046AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/qi/domain.hpp>
#include <boost/spirit/home/qi/detail/assign_to.hpp>
#include <boost/spirit/home/support/unused.hpp>
#include <boost/spirit/home/qi/detail/attributes.hpp>
#include <boost/variant.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace spirit { namespace qi { namespace detail
{
    template <typename Variant, typename Expected>
    struct find_substitute
    {
        // Get the typr from the variant that can be a substitute for Expected.
        // If none is found, just return Expected

        typedef Variant variant_type;
        typedef typename variant_type::types types;
        typedef typename mpl::end<types>::type end;

        typedef typename
            mpl::find_if<types, traits::is_substitute<mpl::_1, Expected> >::type
        iter;

        typedef typename
            mpl::eval_if<
                is_same<iter, end>,
                mpl::identity<Expected>,
                mpl::deref<iter>
            >::type
        type;
    };

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
        bool call_optional_or_variant(Component const& component, mpl::true_) const
        {
            // if Attribute is an optional, then create an attribute for
            // the Component with its expected type.
            typename traits::attribute_of<Component, Context, Iterator>::type val;
            if (component.parse(first, last, context, skipper, val))
            {
                traits::assign_to(val, attr);
                return true;
            }
            return false;
        }

        template <typename Component>
        bool call_optional_or_variant(Component const& component, mpl::false_) const
        {
#ifndef BOOST_SPIRIT_ALTERNATIVES_ALLOW_ATTR_COMPAT
            // if Attribute is a variant, then create an attribute for
            // the Component with its expected type.
            typename traits::attribute_of<Component, Context, Iterator>::type val;
            if (component.parse(first, last, context, skipper, val))
            {
                traits::assign_to(val, attr);
                return true;
            }
            return false;
#else
            // if Attribute is a variant, then search the variant types for a
            // suitable substitute type.

            typename
                find_substitute<Attribute,
                    typename traits::attribute_of<Component, Context, Iterator>::type
                >::type
            val;

            if (component.parse(first, last, context, skipper, val))
            {
                traits::assign_to(val, attr);
                return true;
            }
            return false;
#endif
        }

        template <typename Component>
        bool call(Component const& component, mpl::false_) const
        {
            return call_optional_or_variant(
                component, spirit::traits::not_is_variant<Attribute, qi::domain>());
        }

        template <typename Component>
        bool operator()(Component const& component) const
        {
            // return true if the parser succeeds
            return call(component,
                mpl::and_<
                    spirit::traits::not_is_variant<Attribute, qi::domain>,
                    spirit::traits::not_is_optional<Attribute, qi::domain>
                >());
        }

        Iterator& first;
        Iterator const& last;
        Context& context;
        Skipper const& skipper;
        Attribute& attr;

    private:
        // silence MSVC warning C4512: assignment operator could not be generated
        alternative_function& operator= (alternative_function const&);
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

    private:
        // silence MSVC warning C4512: assignment operator could not be generated
        alternative_function& operator= (alternative_function const&);
    };

}}}}

#endif
