//  Copyright (c) 2001-2009 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_SPIRIT_KARMA_RIGHT_ALIGNMENT_FEB_27_2007_1216PM)
#define BOOST_SPIRIT_KARMA_RIGHT_ALIGNMENT_FEB_27_2007_1216PM

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once      // MS compatible compilers support #pragma once
#endif

#include <boost/spirit/home/karma/meta_compiler.hpp>
#include <boost/spirit/home/karma/generator.hpp>
#include <boost/spirit/home/karma/domain.hpp>
#include <boost/spirit/home/karma/detail/output_iterator.hpp>
#include <boost/spirit/home/karma/detail/default_width.hpp>
#include <boost/spirit/home/karma/delimit_out.hpp>
#include <boost/spirit/home/karma/auxiliary/lazy.hpp>
#include <boost/spirit/home/support/unused.hpp>
#include <boost/spirit/home/support/common_terminals.hpp>
#include <boost/spirit/home/support/attributes.hpp>
#include <boost/spirit/home/support/info.hpp>
#include <boost/spirit/home/support/unused.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/integer_traits.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/utility/enable_if.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit
{
    ///////////////////////////////////////////////////////////////////////////
    // Enablers
    ///////////////////////////////////////////////////////////////////////////

    // enables right_align[]
    template <>
    struct use_directive<karma::domain, tag::right_align>
      : mpl::true_ {};

    // enables right_align(d)[g] and right_align(w)[g], where d is a generator
    // and w is a maximum width
    template <typename T>
    struct use_directive<karma::domain
          , terminal_ex<tag::right_align, fusion::vector1<T> > >
      : mpl::true_ {};

    // enables *lazy* delimit(d)[g], where d provides a generator
    template <>
    struct use_lazy_directive<karma::domain, tag::right_align, 1> 
      : mpl::true_ {};

    // enables right_align(w, d)[g], where d is a generator and w is a maximum 
    // width
    template <typename Width, typename Padding>
    struct use_directive<karma::domain
          , terminal_ex<tag::right_align, fusion::vector2<Width, Padding> > >
      : spirit::traits::matches<karma::domain, Padding> {};

    // enables *lazy* delimit(w, d)[g], where d provides a generator and w is 
    // a maximum width
    template <>
    struct use_lazy_directive<karma::domain, tag::right_align, 2> 
      : mpl::true_ {};

}}

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit { namespace karma
{
    using spirit::right_align;
    using spirit::right_align_type;

    namespace detail
    {
        ///////////////////////////////////////////////////////////////////////
        //  The right_align_generate template function is used for all the 
        //  different flavors of the right_align[] directive. 
        ///////////////////////////////////////////////////////////////////////
        template <typename OutputIterator, typename Context, typename Delimiter, 
            typename Attribute, typename Embedded, typename Padding>
        inline static bool 
        right_align_generate(OutputIterator& sink, Context& ctx, 
            Delimiter const& d, Attribute const& attr, Embedded const& e, 
            unsigned int const width, Padding const& p) 
        {
            // wrap the given output iterator to allow left padding
            detail::enable_buffering<OutputIterator> buffering(sink, width);
            bool r = false;

            // first generate the embedded output 
            {
                detail::disable_counting<OutputIterator> nocounting(sink);
                r = e.generate(sink, ctx, d, attr);
            }   // re-enable counting

            buffering.disable();    // do not perform buffering any more
            
            // generate the left padding
            detail::enable_counting<OutputIterator> counting(sink, buffering.buffer_size());
            while(r && counting.count() < width) 
                r = p.generate(sink, ctx, unused, unused);

            // copy the buffered output to the target output iterator
            if (r) 
                buffering.buffer_copy();
            return r;
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    //  The simple left alignment directive is used for right_align[...]
    //  generators. It uses default values for the generated width (defined via
    //  the BOOST_KARMA_DEFAULT_FIELD_LENGTH constant) and for the padding
    //  generator (always spaces).
    ///////////////////////////////////////////////////////////////////////////
    template <typename Subject, typename Width = detail::default_width>
    struct simple_right_alignment 
      : unary_generator<simple_right_alignment<Subject, Width> >
    {
        typedef mpl::true_ requires_buffering;
        typedef Subject subject_type;

        template <typename Context, typename Unused>
        struct attribute
        {
            typedef typename
                traits::attribute_of<subject_type, Context>::type
            type;
        };

        simple_right_alignment(Subject const& subject, Width width = Width())
          : subject(subject), width(width) {}

        template <typename OutputIterator, typename Context, typename Delimiter
          , typename Attribute>
        bool generate(OutputIterator& sink, Context& ctx, Delimiter const& d
          , Attribute const& attr) const
        {
            return detail::right_align_generate(sink, ctx, d, attr,
                subject, width, compile<karma::domain>(' '));
        }

        template <typename Context>
        info what(Context& context) const
        {
            return info("right_align", subject.what(context));
        }

        Subject subject;
        Width width;
    };

    ///////////////////////////////////////////////////////////////////////////
    //  The left alignment directive with padding, is used for generators like
    //  right_align(padding)[...], where padding is a arbitrary generator
    //  expression. It uses a default value for the generated width (defined
    //  via the BOOST_KARMA_DEFAULT_FIELD_LENGTH constant).
    ///////////////////////////////////////////////////////////////////////////
    template <typename Subject, typename Padding
      , typename Width = detail::default_width>
    struct padding_right_alignment 
      : unary_generator<padding_right_alignment<Subject, Padding, Width> >
    {
        typedef Subject subject_type;
        typedef Padding padding_type;

        template <typename Context, typename Unused>
        struct attribute
        {
            typedef typename
                traits::attribute_of<Subject, Context>::type
            type;
        };

        padding_right_alignment(Subject const& subject, Padding const& padding
              , Width width = Width())
          : subject(subject), padding(padding), width(width) {}

        template <typename OutputIterator, typename Context, typename Delimiter
          , typename Attribute>
        bool generate(OutputIterator& sink, Context& ctx, Delimiter const& d
          , Attribute const& attr) const
        {
            return detail::right_align_generate(sink, ctx, d, attr,
                subject, width, padding);
        }

        template <typename Context>
        info what(Context& context) const
        {
            return info("right_align", subject.what(context));
        }

        Subject subject;
        Padding padding;
        Width width;
    };

    ///////////////////////////////////////////////////////////////////////////
    // Generator generators: make_xxx function (objects)
    ///////////////////////////////////////////////////////////////////////////

    // creates right_align[] directive generator
    template <typename Subject, typename Modifiers>
    struct make_directive<tag::right_align, Subject, Modifiers>
    {
        typedef simple_right_alignment<Subject> result_type;
        result_type operator()(unused_type, Subject const& subject
          , unused_type) const
        {
            return result_type(subject);
        }
    };

    // creates right_align(width)[] directive generator
    template <typename Width, typename Subject, typename Modifiers>
    struct make_directive<
        terminal_ex<tag::right_align, fusion::vector1<Width> >
      , Subject, Modifiers
      , typename enable_if_c< integer_traits<Width>::is_integral >::type>
    {
        typedef simple_right_alignment<Subject, Width> result_type;

        template <typename Terminal>
        result_type operator()(Terminal const& term, Subject const& subject
          , unused_type) const
        {
            return result_type(subject, fusion::at_c<0>(term.args));
        }
    };

    // creates right_align(pad)[] directive generator
    template <typename Padding, typename Subject, typename Modifiers>
    struct make_directive<
        terminal_ex<tag::right_align, fusion::vector1<Padding> >
      , Subject, Modifiers
      , typename enable_if<spirit::traits::matches<karma::domain, Padding> >::type>
    {
        typedef typename
            result_of::compile<karma::domain, Padding, Modifiers>::type
        padding_type;

        typedef padding_right_alignment<Subject, padding_type> result_type;

        template <typename Terminal>
        result_type operator()(Terminal const& term, Subject const& subject
          , unused_type) const
        {
            return result_type(subject
              , compile<karma::domain>(fusion::at_c<0>(term.args)));
        }
    };

    // creates right_align(pad, width)[] directive generator
    template <typename Width, typename Padding, typename Subject
      , typename Modifiers>
    struct make_directive<
        terminal_ex<tag::right_align, fusion::vector2<Width, Padding> >
      , Subject, Modifiers>
    {
        typedef typename
            result_of::compile<karma::domain, Padding, Modifiers>::type
        padding_type;

        typedef padding_right_alignment<Subject, padding_type, Width> result_type;

        template <typename Terminal>
        result_type operator()(Terminal const& term, Subject const& subject
          , unused_type) const
        {
            return result_type(subject
              , compile<karma::domain>(fusion::at_c<1>(term.args))
              , fusion::at_c<0>(term.args));
        }
    };

}}} // namespace boost::spirit::karma

#endif


