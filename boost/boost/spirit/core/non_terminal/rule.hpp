/*=============================================================================
    Spirit v1.7.0
    Copyright (c) 1998-2003 Joel de Guzman
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#if !defined(BOOST_SPIRIT_RULE_HPP)
#define BOOST_SPIRIT_RULE_HPP

///////////////////////////////////////////////////////////////////////////////
//
//  Spirit predefined maximum number of simultaneously usable different scanner 
//  types. 
//
//  This limit defines the maximum number of of possible different scanner 
//  types for which a specific rule<> may be used. If this isn't defined, a
//  rule<> may be used with one scanner type only (multiple scanner support is
//  disabled).
//
///////////////////////////////////////////////////////////////////////////////
#if !defined(BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT)
#define BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT 1
#endif

//  This feature is available only to compilers that support partial template 
//  specialization.
#if defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
#undef BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT
#define BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT 1
#endif

//  Ensure a meaningful maximum number of simultaneously usable scanner types
BOOST_STATIC_ASSERT(BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT > 0);

///////////////////////////////////////////////////////////////////////////////
#include <boost/scoped_ptr.hpp> 
#include <boost/spirit/core/parser.hpp>
#include <boost/spirit/core/scanner/scanner.hpp>
#include <boost/spirit/core/non_terminal/parser_context.hpp>
#include <boost/spirit/core/non_terminal/parser_id.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/spirit/core/non_terminal/impl/rule.ipp>

#if BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT > 1
#include <boost/preprocessor/enum_params.hpp>
#endif

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit {

    ///////////////////////////////////////////////////////////////////////////
    //
    //  rule_base class
    //
    //      The rule_base class implements the basic plumbing for rules
    //      minus the storage mechanism. It is up to the derived class
    //      to actually store the definition somewhere. The rule_base
    //      class assumes that the derived class provides a get() function
    //      that will return a pointer to a parser. The get() function
    //      may return NULL. See rule below for details.
    //
    //      <<< For framework use only. Not for public consumption. >>>
    //
    ///////////////////////////////////////////////////////////////////////////
    namespace impl
    {
        template <
            typename DerivedT       // derived class
          , typename EmbedT         // how derived class is embedded
          , typename T0 = nil_t     // see rule class
          , typename T1 = nil_t     // see rule class
          , typename T2 = nil_t     // see rule class
        >
        class rule_base
            : public parser<DerivedT>
            , public impl::get_context<T0, T1, T2>::type::base_t
            , public context_aux<
                typename impl::get_context<T0, T1, T2>::type, DerivedT>
            , public impl::get_tag<T0, T1, T2>::type
        {
        public:
    
            typedef typename impl::get_scanner<T0, T1, T2>::type scanner_t;
            typedef typename impl::get_context<T0, T1, T2>::type context_t;
            typedef typename impl::get_tag<T0, T1, T2>::type tag_t;
    
            typedef EmbedT embed_t;
            typedef typename context_t::context_linker_t linked_context_t;
            typedef typename linked_context_t::attr_t attr_t;
    
            template <typename ScannerT>
            struct result 
            { 
                typedef typename match_result<ScannerT, attr_t>::type type; 
            };
        
            template <typename ScannerT>
            typename parser_result<DerivedT, ScannerT>::type
            parse(ScannerT const& scan) const
            {
                typedef parser_scanner_linker<ScannerT> linked_scanner_t;
                typedef typename parser_result<DerivedT, ScannerT>::type result_t;
                BOOST_SPIRIT_CONTEXT_PARSE(
                    scan, *this, linked_scanner_t, linked_context_t, result_t);
            }
        
            template <typename ScannerT>
            typename parser_result<DerivedT, ScannerT>::type
            parse_main(ScannerT const& scan) const
            {
                typename parser_result<DerivedT, ScannerT>::type hit;
                if (static_cast<DerivedT const&>(*this).defined())
                {
                    typename ScannerT::iterator_t s(scan.first);
                    hit = static_cast<DerivedT const&>(*this)
                        .get()->do_parse_virtual(scan);
                    scan.group_match(hit, id(), s, scan.first);
                }
                else
                {
                    hit = scan.no_match();
                }
                return hit;
            }
        };
        
    } // namespace impl
    
    ///////////////////////////////////////////////////////////////////////////
    //
    //  rule class
    //
    //      The rule is a polymorphic parser that acts as a named place-
    //      holder capturing the behavior of an EBNF expression assigned to
    //      it. 
    //
    //      The rule is a template class parameterized by:
    //
    //          1) scanner (scanner_t, see scanner.hpp), 
    //          2) the rule's context (context_t, see parser_context.hpp)
    //          3) an arbitrary tag (tag_t, see parser_id.hpp) that allows 
    //             a rule to be tagged for identification. 
    //
    //      These template parameters may be specified in any order. The 
    //      scanner will default to scanner<> when it is not specified. 
    //      The context will default to parser_context when not specified. 
    //      The tag will default to parser_address_tag when not specified.  
    //
    //      The definition of the rule (its right hand side, RHS) held by the
    //      rule through a ::boost::scoped_ptr. When a rule is seen in the
    //      the RHS of an assignment or copy construction EBNF expression,
    //      the rule is held by the LHS rule by reference.
    //
    ///////////////////////////////////////////////////////////////////////////
    template <
        typename T0 = nil_t
      , typename T1 = nil_t
      , typename T2 = nil_t
    >
    class rule 
        : public impl::rule_base<
            rule<T0, T1, T2>
          , rule<T0, T1, T2> const&
          , T0, T1, T2>
    {
    public:   

        typedef impl::rule_base<
            rule<T0, T1, T2>
          , rule<T0, T1, T2> const&
          , T0, T1, T2>
        base_t;
        
        typedef typename base_t::scanner_t scanner_t;
        typedef typename base_t::attr_t attr_t;
        typedef impl::abstract_parser<scanner_t, attr_t> abstract_parser_t;

        rule() : ptr() {}
        ~rule() {}

        rule(rule const& r)
        : ptr(new impl::concrete_parser<rule, scanner_t, attr_t>(r)) {}

        template <typename ParserT>
        rule(ParserT const& p)
        : ptr(new impl::concrete_parser<ParserT, scanner_t, attr_t>(p)) {}

        template <typename ParserT>
        rule& operator=(ParserT const& p)
        {
            ptr.reset(new impl::concrete_parser<ParserT, scanner_t, attr_t>(p));
            return *this;
        }

        rule& operator=(rule const& r)
        {
            //  If this is placed above the templatized assignment
            //  operator, VC6 incorrectly complains ambiguity with
            //  r1 = r2, where r1 and r2 are both rules.
            ptr.reset(new impl::concrete_parser<rule, scanner_t, attr_t>(r));
            return *this;
        }
        
        abstract_parser_t*
        get() const
        {
            return ptr.get();
        }
        
        bool
        defined() const
        {
            return ptr.get() != 0;
        }
      
    private:

        ::boost::scoped_ptr<abstract_parser_t> ptr;
    };

#if BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT > 1
    ///////////////////////////////////////////////////////////////////////////
    //
    //  scanner_list
    //
    //      Typically, rules are tied to a specific scanner type and
    //      a particular rule cannot be used with anything else. Sometimes
    //      there's a need for rules that can accept more than one scanner
    //      type. The scanner_list<S0, ...SN> can be used as a template
    //      parameter to the rule class to specify up to the number of 
    //      scanner types defined by the BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT 
    //      constant. Example:
    //
    //          rule<scanner_list<ScannerT0, ScannerT1> > r;
    //
    //      *** This feature is available only to compilers that support
    //      partial template specialization. ***
    //
    ///////////////////////////////////////////////////////////////////////////
    template <
        BOOST_PP_ENUM_PARAMS(
            BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT, 
            typename ScannerT
        )
    >
    struct scanner_list : scanner_base {}; // a proxy scanner 

#endif
    
///////////////////////////////////////////////////////////////////////////////
}} // namespace boost::spirit

#endif
