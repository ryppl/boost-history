/*=============================================================================
    Spirit v1.6.1
    Copyright (c) 1998-2003 Joel de Guzman
    Copyright (c) 2001 Daniel Nuffer
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#if !defined(BOOST_SPIRIT_DIRECTIVES_HPP)
#define BOOST_SPIRIT_DIRECTIVES_HPP

///////////////////////////////////////////////////////////////////////////////
#include <algorithm>

#if !defined(BOOST_SPIRIT_PARSER_HPP)
#include "boost/spirit/core/parser.hpp"
#endif

#if !defined(BOOST_SPIRIT_PRIMITIVES_HPP)
#include "boost/spirit/core/primitives/primitives.hpp"
#endif

#if !defined(BOOST_SPIRIT_COMPOSITE_HPP)
#include "boost/spirit/core/composite/composite.hpp"
#endif

#if !defined(BOOST_SPIRIT_DIRECTIVES_IPP)
#include "boost/spirit/core/composite/impl/directives.ipp"
#endif

namespace boost { namespace spirit {

    ///////////////////////////////////////////////////////////////////////////
    //
    //  no_skipper_iteration_policy class
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename BaseT>
    struct no_skipper_iteration_policy : public BaseT
    {
        typedef BaseT base_t;

        no_skipper_iteration_policy()
        : BaseT() {}

        template <typename PolicyT>
        no_skipper_iteration_policy(PolicyT const& other)
        : BaseT(other) {}

        template <typename ScannerT>
        void
        skip(ScannerT const& /*scan*/) const {}
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  contiguous class
    //
    ///////////////////////////////////////////////////////////////////////////
    struct lexeme_parser_gen;

    template <typename ParserT>
    struct contiguous
    :   public unary<ParserT, parser<contiguous<ParserT> > >
    {
        typedef contiguous<ParserT>             self_t;
        typedef unary_parser_category           parser_category_t;
        typedef lexeme_parser_gen               parser_generator_t;
        typedef unary<ParserT, parser<self_t> > base_t;

        template <typename ScannerT>
        struct result
        {
            typedef typename parser_result<ParserT, ScannerT>::type type;
        };

        contiguous()
        : base_t(ParserT()) {}

        contiguous(ParserT const& p)
        : base_t(p) {}

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan) const
        {
            typedef typename parser_result<self_t, ScannerT>::type result_t;
            return impl::contiguous_parser_parse<result_t>
                (this->subject(), scan, scan);
        }
    };

    struct lexeme_parser_gen
    {
        template <typename ParserT>
        struct result {

            typedef contiguous<ParserT> type;
        };

        template <typename ParserT>
        static contiguous<ParserT>
        generate(parser<ParserT> const& subject)
        {
            return contiguous<ParserT>(subject.derived());
        }

        template <typename ParserT>
        contiguous<ParserT>
        operator[](parser<ParserT> const& subject) const
        {
            return contiguous<ParserT>(subject.derived());
        }
    };

    //////////////////////////////////
    const lexeme_parser_gen lexeme_d = lexeme_parser_gen();

    ///////////////////////////////////////////////////////////////////////////
    //
    //  lexeme_scanner
    //
    //      Given a Scanner, return the correct scanner type that
    //      the lexeme_d uses. Scanner is assumed to be a phrase
    //      level scanner (see skipper.hpp)
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ScannerT>
    struct lexeme_scanner
    {
        typedef scanner_policies<
            no_skipper_iteration_policy<
                typename ScannerT::iteration_policy_t>,
            typename ScannerT::match_policy_t,
            typename ScannerT::action_policy_t
        > policies_t;

        typedef typename
            rebind_scanner_policies<ScannerT, policies_t>::type type;
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  inhibit_case_iteration_policy class
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename BaseT>
    struct inhibit_case_iteration_policy : public BaseT
    {
        typedef BaseT base_t;

        inhibit_case_iteration_policy()
        : BaseT() {}

        template <typename PolicyT>
        inhibit_case_iteration_policy(PolicyT const& other)
        : BaseT(other) {}

        template <typename CharT>
        CharT filter(CharT ch) const
        { return impl::tolower_(ch); }
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  inhibit_case class
    //
    ///////////////////////////////////////////////////////////////////////////
    struct inhibit_case_parser_gen;

    template <typename ParserT>
    struct inhibit_case
    :   public unary<ParserT, parser<inhibit_case<ParserT> > >
    {
        typedef inhibit_case<ParserT>           self_t;
        typedef unary_parser_category           parser_category_t;
        typedef inhibit_case_parser_gen         parser_generator_t;
        typedef unary<ParserT, parser<self_t> > base_t;

        template <typename ScannerT>
        struct result
        {
            typedef typename parser_result<ParserT, ScannerT>::type type;
        };

        inhibit_case()
        : base_t(ParserT()) {}

        inhibit_case(ParserT const& p)
        : base_t(p) {}

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan) const
        {
            typedef typename parser_result<self_t, ScannerT>::type result_t;
            return impl::inhibit_case_parser_parse<result_t>
                (this->subject(), scan, scan);
        }
    };

    template <int N>
    struct inhibit_case_parser_gen_base
    {
        //  This hack is needed to make borland happy.
        //  If these member operators were defined in the
        //  inhibit_case_parser_gen class, or if this class
        //  is non-templated, borland ICEs.

        static inhibit_case<strlit<char const*> >
        generate(char const* str)
        { return inhibit_case<strlit<char const*> >(str); }

        static inhibit_case<strlit<wchar_t const*> >
        generate(wchar_t const* str)
        { return inhibit_case<strlit<wchar_t const*> >(str); }

        static inhibit_case<chlit<char> >
        generate(char ch)
        { return inhibit_case<chlit<char> >(ch); }

        static inhibit_case<chlit<wchar_t> >
        generate(wchar_t ch)
        { return inhibit_case<chlit<wchar_t> >(ch); }

        template <typename ParserT>
        static inhibit_case<ParserT>
        generate(parser<ParserT> const& subject)
        { return inhibit_case<ParserT>(subject.derived()); }

        inhibit_case<strlit<char const*> >
        operator[](char const* str) const
        { return inhibit_case<strlit<char const*> >(str); }

        inhibit_case<strlit<wchar_t const*> >
        operator[](wchar_t const* str) const
        { return inhibit_case<strlit<wchar_t const*> >(str); }

        inhibit_case<chlit<char> >
        operator[](char ch) const
        { return inhibit_case<chlit<char> >(ch); }

        inhibit_case<chlit<wchar_t> >
        operator[](wchar_t ch) const
        { return inhibit_case<chlit<wchar_t> >(ch); }

        template <typename ParserT>
        inhibit_case<ParserT>
        operator[](parser<ParserT> const& subject) const
        { return inhibit_case<ParserT>(subject.derived()); }
    };

    //////////////////////////////////
    struct inhibit_case_parser_gen : public inhibit_case_parser_gen_base<0>
    {
        inhibit_case_parser_gen() {}
    };

    //////////////////////////////////
    //  Depracated
    const inhibit_case_parser_gen nocase_d = inhibit_case_parser_gen();

    //  Preferred syntax
    const inhibit_case_parser_gen as_lower_d = inhibit_case_parser_gen();

    ///////////////////////////////////////////////////////////////////////////
    //
    //  as_lower_scanner
    //
    //      Given a Scanner, return the correct scanner type that
    //      the as_lower_d uses. Scanner is assumed to be a scanner
    //      with an inhibit_case_iteration_policy.
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ScannerT>
    struct as_lower_scanner
    {
        typedef scanner_policies<
            inhibit_case_iteration_policy<
                typename ScannerT::iteration_policy_t>,
            typename ScannerT::match_policy_t,
            typename ScannerT::action_policy_t
        > policies_t;

        typedef typename
            rebind_scanner_policies<ScannerT, policies_t>::type type;
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  longest_alternative class
    //
    ///////////////////////////////////////////////////////////////////////////
    struct longest_parser_gen;

    template <typename A, typename B>
    struct longest_alternative
    :   public binary<A, B, parser<longest_alternative<A, B> > >
    {
        typedef longest_alternative<A, B>       self_t;
        typedef binary_parser_category          parser_category_t;
        typedef longest_parser_gen              parser_generator_t;
        typedef binary<A, B, parser<self_t> >   base_t;

        longest_alternative()
        : base_t(A(), B()) {}

        longest_alternative(A const& a, B const& b)
        : base_t(a, b) {}

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan) const
        {
            typedef typename parser_result<self_t, ScannerT>::type result_t;
            typename ScannerT::iterator_t save = scan.first;
            result_t l = this->left().parse(scan);
            std::swap(scan.first, save);
            result_t r = this->right().parse(scan);

            if (l || r)
            {
                if (l.length() > r.length())
                {
                    scan.first = save;
                    return l;
                }
                return r;
            }

            return scan.no_match();
        }
    };

    struct longest_parser_gen
    {
        template <typename A, typename B>
        struct result {

            typedef typename
                impl::to_longest_alternative<alternative<A, B> >::result_t
            type;
        };

        template <typename A, typename B>
        static typename
        impl::to_longest_alternative<alternative<A, B> >::result_t
        generate(alternative<A, B> const& alt)
        {
            return impl::to_longest_alternative<alternative<A, B> >::
                convert(alt);
        }

        template <typename A, typename B>
        typename impl::to_longest_alternative<alternative<A, B> >::result_t
        operator[](alternative<A, B> const& alt) const
        {
            return impl::to_longest_alternative<alternative<A, B> >::
                convert(alt);
        }
    };

    const longest_parser_gen longest_d = longest_parser_gen();

    ///////////////////////////////////////////////////////////////////////////
    //
    //  shortest_alternative class
    //
    ///////////////////////////////////////////////////////////////////////////
    struct shortest_parser_gen;

    template <typename A, typename B>
    struct shortest_alternative
    :   public binary<A, B, parser<shortest_alternative<A, B> > >
    {
        typedef shortest_alternative<A, B>      self_t;
        typedef binary_parser_category          parser_category_t;
        typedef shortest_parser_gen             parser_generator_t;
        typedef binary<A, B, parser<self_t> >   base_t;

        shortest_alternative()
        : base_t(A(), B()) {}

        shortest_alternative(A const& a, B const& b)
        : base_t(a, b) {}

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan) const
        {
            typedef typename parser_result<self_t, ScannerT>::type result_t;
            typename ScannerT::iterator_t save = scan.first;
            result_t l = this->left().parse(scan);
            std::swap(scan.first, save);
            result_t r = this->right().parse(scan);

            if (l || r)
            {
                if (l.length() < r.length())
                {
                    scan.first = save;
                    return l;
                }
                return r;
            }

            return scan.no_match();
        }
    };

    struct shortest_parser_gen
    {
        template <typename A, typename B>
        struct result {

            typedef typename
                impl::to_shortest_alternative<alternative<A, B> >::result_t
            type;
        };

        template <typename A, typename B>
        static typename
        impl::to_shortest_alternative<alternative<A, B> >::result_t
        generate(alternative<A, B> const& alt)
        {
            return impl::to_shortest_alternative<alternative<A, B> >::
                convert(alt);
        }

        template <typename A, typename B>
        typename impl::to_shortest_alternative<alternative<A, B> >::result_t
        operator[](alternative<A, B> const& alt) const
        {
            return impl::to_shortest_alternative<alternative<A, B> >::
                convert(alt);
        }
    };

    const shortest_parser_gen shortest_d = shortest_parser_gen();

    ///////////////////////////////////////////////////////////////////////////
    //
    //  min_bounded class
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename BoundsT>
    struct min_bounded_gen;

    template <typename ParserT, typename BoundsT>
    struct min_bounded
    :   public unary<ParserT, parser<min_bounded<ParserT, BoundsT> > >
    {
        typedef min_bounded<ParserT, BoundsT>   self_t;
        typedef unary_parser_category           parser_category_t;
        typedef min_bounded_gen<BoundsT>        parser_generator_t;
        typedef unary<ParserT, parser<self_t> > base_t;

        template <typename ScannerT>
        struct result
        {
            typedef typename parser_result<ParserT, ScannerT>::type type;
        };

        min_bounded()
        : base_t(ParserT()) {}

        min_bounded(ParserT const& p, BoundsT const& min__)
        : base_t(p)
        , min_(min__) {}

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan) const
        {
            typedef typename parser_result<self_t, ScannerT>::type result_t;
            result_t hit = this->subject().parse(scan);
            if (hit.value() < min_)
                return scan.no_match();
            return hit;
        }

        BoundsT min_;
    };

    template <typename BoundsT>
    struct min_bounded_gen
    {
        min_bounded_gen(BoundsT const& min__)
        : min_(min__) {}

        template <typename DerivedT>
        min_bounded<DerivedT, BoundsT>
        operator[](parser<DerivedT> const& p) const
        { return min_bounded<DerivedT, BoundsT>(p.derived(), min_); }

        BoundsT min_;
    };

    template <typename BoundsT>
    inline min_bounded_gen<BoundsT>
    min_limit_d(BoundsT const& min_)
    { return min_bounded_gen<BoundsT>(min_); }

    ///////////////////////////////////////////////////////////////////////////
    //
    //  max_bounded class
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename BoundsT>
    struct max_bounded_gen;

    template <typename ParserT, typename BoundsT>
    struct max_bounded
    :   public unary<ParserT, parser<max_bounded<ParserT, BoundsT> > >
    {
        typedef max_bounded<ParserT, BoundsT>   self_t;
        typedef unary_parser_category           parser_category_t;
        typedef max_bounded_gen<BoundsT>        parser_generator_t;
        typedef unary<ParserT, parser<self_t> > base_t;

        template <typename ScannerT>
        struct result
        {
            typedef typename parser_result<ParserT, ScannerT>::type type;
        };

        max_bounded()
        : base_t(ParserT()) {}

        max_bounded(ParserT const& p, BoundsT const& max__)
        : base_t(p)
        , max_(max__) {}

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan) const
        {
            typedef typename parser_result<self_t, ScannerT>::type result_t;
            result_t hit = this->subject().parse(scan);
            if (hit.value() > max_)
                return scan.no_match();
            return hit;
        }

        BoundsT max_;
    };

    template <typename BoundsT>
    struct max_bounded_gen
    {
        max_bounded_gen(BoundsT const& max__)
        : max_(max__) {}

        template <typename DerivedT>
        max_bounded<DerivedT, BoundsT>
        operator[](parser<DerivedT> const& p) const
        { return max_bounded<DerivedT, BoundsT>(p.derived(), max_); }

        BoundsT max_;
    };

    //////////////////////////////////
    template <typename BoundsT>
    inline max_bounded_gen<BoundsT>
    max_limit_d(BoundsT const& max_)
    { return max_bounded_gen<BoundsT>(max_); }

    ///////////////////////////////////////////////////////////////////////////
    //
    //  bounded class
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename BoundsT>
    struct bounded_gen;

    template <typename ParserT, typename BoundsT>
    struct bounded
    :   public unary<ParserT, parser<bounded<ParserT, BoundsT> > >
    {
        typedef bounded<ParserT, BoundsT>       self_t;
        typedef unary_parser_category           parser_category_t;
        typedef bounded_gen<BoundsT>            parser_generator_t;
        typedef unary<ParserT, parser<self_t> > base_t;

        template <typename ScannerT>
        struct result
        {
            typedef typename parser_result<ParserT, ScannerT>::type type;
        };

        bounded()
        : base_t(ParserT()) {}

        bounded(ParserT const& p, BoundsT const& min__, BoundsT const& max__)
        : base_t(p)
        , min_(min__)
        , max_(max__) {}

        template <typename ScannerT>
        typename parser_result<self_t, ScannerT>::type
        parse(ScannerT const& scan) const
        {
            typedef typename parser_result<self_t, ScannerT>::type result_t;
            result_t hit = this->subject().parse(scan);
            if (hit.value() < min_ || hit.value() > max_)
                return scan.no_match();
            return hit;
        }

        BoundsT min_, max_;
    };

    template <typename BoundsT>
    struct bounded_gen
    {
        bounded_gen(BoundsT const& min__, BoundsT const& max__)
        : min_(min__)
        , max_(max__) {}

        template <typename DerivedT>
        bounded<DerivedT, BoundsT>
        operator[](parser<DerivedT> const& p) const
        { return bounded<DerivedT, BoundsT>(p.derived(), min_, max_); }

        BoundsT min_, max_;
    };

    template <typename BoundsT>
    inline bounded_gen<BoundsT>
    limit_d(BoundsT const& min_, BoundsT const& max_)
    { return bounded_gen<BoundsT>(min_, max_); }

}} // namespace boost::spirit

#endif

