//  Copyright (c) 2001-2009 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_SPIRIT_KARMA_STREAM_MAY_01_2007_0310PM)
#define BOOST_SPIRIT_KARMA_STREAM_MAY_01_2007_0310PM

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once      // MS compatible compilers support #pragma once
#endif

#include <boost/spirit/home/support/common_terminals.hpp>
#include <boost/spirit/home/support/info.hpp>
#include <boost/spirit/home/support/common_terminals.hpp>
#include <boost/spirit/home/support/detail/hold_any.hpp>
#include <boost/spirit/home/support/detail/get_encoding.hpp>
#include <boost/spirit/home/karma/domain.hpp>
#include <boost/spirit/home/karma/meta_compiler.hpp>
#include <boost/spirit/home/karma/delimit_out.hpp>
#include <boost/spirit/home/karma/auxiliary/lazy.hpp>
#include <boost/spirit/home/karma/stream/detail/format_manip.hpp>
#include <boost/spirit/home/karma/stream/detail/iterator_sink.hpp>
#include <boost/spirit/home/karma/detail/get_casetag.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/cons.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#include <iosfwd>

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit
{
    ///////////////////////////////////////////////////////////////////////////
    // Enablers
    ///////////////////////////////////////////////////////////////////////////
    template <>
    struct use_terminal<karma::domain, tag::stream>     // enables stream
      : mpl::true_ {};

    template <>
    struct use_terminal<karma::domain, tag::wstream>    // enables wstream
      : mpl::true_ {};

    template <typename A0>
    struct use_terminal<karma::domain                   // enables stream(...)
      , terminal_ex<tag::stream, fusion::vector1<A0> >
    > : mpl::true_ {};

    template <typename A0>
    struct use_terminal<karma::domain                   // enables wstream(...)
      , terminal_ex<tag::wstream, fusion::vector1<A0> >
    > : mpl::true_ {};

    template <>                                         // enables stream(f)
    struct use_lazy_terminal<
        karma::domain
      , tag::stream
      , 1 /*arity*/
    > : mpl::true_ {};

    template <>                                         // enables wstream(f)
    struct use_lazy_terminal<
        karma::domain
      , tag::wstream
      , 1 /*arity*/
    > : mpl::true_ {};

}}

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit { namespace karma
{
    using spirit::stream;
    using spirit::wstream;

    ///////////////////////////////////////////////////////////////////////////
    template <typename Char, typename CharEncoding, typename Tag>
    struct stream_generator
      : primitive_generator<stream_generator<Char, CharEncoding, Tag> >
    {
        template <typename Context, typename Unused>
        struct attribute
        {
            typedef spirit::hold_any type;
        };

        // stream_generator has an attached attribute 
        template <
            typename OutputIterator, typename Context, typename Delimiter
          , typename Attribute
        >
        static bool generate(OutputIterator& sink, Context&
          , Delimiter const& d, Attribute const& attr)
        {
            typedef karma::detail::iterator_sink<
                OutputIterator, Char, CharEncoding, Tag
            > sink_device;

            // use existing operator<<()
            boost::iostreams::stream<sink_device> ostr(sink);
            ostr << attr << std::flush;

            if (ostr.good()) 
                return karma::delimit_out(sink, d);   // always do post-delimiting
            return false;
        }

        // this is a special overload to detect if the output iterator has been
        // generated by a format_manip object.
        template <
            typename T, typename Traits, typename Context, typename Delimiter
          , typename Attribute
        >
        static bool generate(
            karma::detail::output_iterator<
                karma::detail::ostream_iterator<T, Char, Traits>
            >& sink, Context&, Delimiter const& d, Attribute const& attr)
        {
            typedef karma::detail::output_iterator<
                karma::detail::ostream_iterator<T, Char, Traits>
            > output_iterator;
            typedef karma::detail::iterator_sink<
                output_iterator, Char, CharEncoding, Tag
            > sink_device;

            // use existing operator<<()
            boost::iostreams::stream<sink_device> ostr(sink);
            ostr.imbue(sink.get_ostream().getloc());
            ostr << attr << std::flush;

            if (ostr.good()) 
                return karma::delimit_out(sink, d);  // always do post-delimiting
            return false;
        }

        // this any_stream has no parameter attached, it needs to have been
        // initialized from a value/variable
        template <typename OutputIterator, typename Context
          , typename Delimiter>
        static bool
        generate(OutputIterator&, Context&, Delimiter const&, unused_type)
        {
            BOOST_SPIRIT_ASSERT_MSG(false, stream__not_usable_without_attribute,
                (OutputIterator, Delimiter));
            return false;
        }

        template <typename Context>
        info what(Context& /*context*/) const
        {
            return info("stream");
        }
    };

    
    template <typename T, typename Char, typename CharEncoding, typename Tag>
    struct any_stream_generator
      : primitive_generator<any_stream_generator<T, Char, CharEncoding, Tag> >
    {
        template <typename Context, typename Unused>
        struct attribute
        {
            typedef typename remove_const<T>::type type;
        };

        any_stream_generator(typename add_reference<T>::type t)
          : t_(t)
        {}

        // any_stream_generator has an attached parameter

        // this overload will be used in the normal case (not called from
        // format_manip).
        template <
            typename OutputIterator, typename Context, typename Delimiter
          , typename Attribute>
        bool generate(OutputIterator& sink, Context&, Delimiter const& d
          , Attribute const&)
        {
            typedef karma::detail::iterator_sink<
                OutputIterator, Char, CharEncoding, Tag
            > sink_device;

            boost::iostreams::stream<sink_device> ostr(sink);
            ostr << t_ << std::flush;             // use existing operator<<()

            if (ostr.good()) 
                return karma::delimit_out(sink, d); // always do post-delimiting
            return false;
        }

        // this is a special overload to detect if the output iterator has been
        // generated by a format_manip object.
        template <
            typename T1, typename Traits
          , typename Context, typename Delimiter, typename Attribute>
        bool generate(
            karma::detail::output_iterator<
                karma::detail::ostream_iterator<T1, Char, Traits>
            >& sink, Context&, Delimiter const& d, Attribute const&)
        {
            typedef karma::detail::output_iterator<
                karma::detail::ostream_iterator<T1, Char, Traits>
            > output_iterator;
            typedef karma::detail::iterator_sink<
                output_iterator, Char, CharEncoding, Tag
            > sink_device;

            boost::iostreams::stream<sink_device> ostr(sink);
            ostr.imbue(sink.get_ostream().getloc());
            ostr << t_ << std::flush;             // use existing operator<<()

            if (ostr.good()) 
                return karma::delimit_out(sink, d); // always do post-delimiting
            return false;
        }

        template <typename Context>
        info what(Context& /*context*/) const
        {
            return info("any-stream");
        }

        T t_;
    };

    ///////////////////////////////////////////////////////////////////////////
    // Parser generators: make_xxx function (objects)
    ///////////////////////////////////////////////////////////////////////////
    template <typename Char, typename Modifiers>
    struct make_stream
    {
        static bool const lower =
            has_modifier<Modifiers, tag::char_code_base<tag::lower> >::value;

        static bool const upper =
            has_modifier<Modifiers, tag::char_code_base<tag::upper> >::value;

        typedef stream_generator<
            Char
          , typename spirit::detail::get_encoding<
                Modifiers, unused_type, lower || upper>::type
          , typename detail::get_casetag<Modifiers, lower || upper>::type
        > result_type;

        result_type operator()(unused_type, unused_type) const
        {
            return result_type();
        }
    };

    // stream
    template <typename Modifiers>
    struct make_primitive<tag::stream, Modifiers> 
      : make_stream<char, Modifiers> {};

    // wstream
    template <typename Modifiers>
    struct make_primitive<tag::wstream, Modifiers> 
      : make_stream<wchar_t, Modifiers> {};

    ///////////////////////////////////////////////////////////////////////////
    template <typename Char, typename A0, typename Modifiers>
    struct make_any_stream
    {
        static bool const lower =
            has_modifier<Modifiers, tag::char_code_base<tag::lower> >::value;

        static bool const upper =
            has_modifier<Modifiers, tag::char_code_base<tag::upper> >::value;

        typedef typename add_const<A0>::type const_attribute;
        typedef any_stream_generator<
            const_attribute, Char
          , typename spirit::detail::get_encoding<
                Modifiers, unused_type, lower || upper>::type
          , typename detail::get_casetag<Modifiers, lower || upper>::type
        > result_type;

        template <typename Terminal>
        result_type operator()(Terminal const& term, unused_type) const
        {
            return result_type(fusion::at_c<0>(term.args));
        }
    };

    // stream(...)
    template <typename Modifiers, typename A0>
    struct make_primitive<
            terminal_ex<tag::stream, fusion::vector1<A0> >, Modifiers>
      : make_any_stream<char, A0, Modifiers> {};

    // wstream(...)
    template <typename Modifiers, typename A0>
    struct make_primitive<
            terminal_ex<tag::wstream, fusion::vector1<A0> >, Modifiers>
      : make_any_stream<wchar_t, A0, Modifiers> {};

}}}

#endif
