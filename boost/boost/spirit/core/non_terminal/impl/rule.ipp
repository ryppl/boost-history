/*=============================================================================
    Spirit v1.7.0
    Copyright (c) 1998-2003 Joel de Guzman
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#if !defined(BOOST_SPIRIT_RULE_IPP)
#define BOOST_SPIRIT_RULE_IPP

#if BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT > 1
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/repeat_from_to.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/enum_params_with_defaults.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#endif

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit {

#if BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT > 1

        template <
            BOOST_PP_ENUM_BINARY_PARAMS(
                BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT, 
                typename ScannerT, = mpl::void_ BOOST_PP_INTERCEPT
            )
        >
        struct scanner_list;
        
#endif // BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT > 1

    ///////////////////////////////////////////////////////////////////////////
    namespace impl
    {
        template <typename BaseT, typename DefaultT
            , typename T0, typename T1, typename T2>
        struct get_param
        {
            typedef typename mpl::if_<
                is_base_and_derived<BaseT, T0>
              , T0
              , typename mpl::if_<
                    is_base_and_derived<BaseT, T1>
                  , T1
                  , typename mpl::if_<
                        is_base_and_derived<BaseT, T2>
                      , T2
                      , DefaultT
                    >::type
                >::type
            >::type type;
        };
        
        template <typename T0, typename T1, typename T2>
        struct get_context
        {
            typedef typename get_param<
                parser_context_base, parser_context, T0, T1, T2>::type
            type;
        };
        
        template <typename T0, typename T1, typename T2>
        struct get_tag
        {
            typedef typename get_param<
                parser_tag_base, parser_address_tag, T0, T1, T2>::type
            type;
        };
        
        template <typename T0, typename T1, typename T2>
        struct get_scanner
        {
            typedef typename get_param<
                scanner_base, scanner<>, T0, T1, T2>::type
            type;
        };
        
        ///////////////////////////////////////////////////////////////////////
        //
        //  abstract_parser class
        //
        ///////////////////////////////////////////////////////////////////////
        template <typename ScannerT, typename AttrT>
        struct abstract_parser
        {
            abstract_parser() {}
            virtual ~abstract_parser() {}

            virtual typename match_result<ScannerT, AttrT>::type
            do_parse_virtual(ScannerT const& scan) const = 0;
        };

        ///////////////////////////////////////////////////////////////////////
        //
        //  concrete_parser class
        //
        ///////////////////////////////////////////////////////////////////////
        template <typename ParserT, typename ScannerT, typename AttrT>
        struct concrete_parser : abstract_parser<ScannerT, AttrT>
        {
            concrete_parser(ParserT const& p_) : p(p_) {}
            virtual ~concrete_parser() {}

            virtual typename match_result<ScannerT, AttrT>::type
            do_parse_virtual(ScannerT const& scan) const
            { return p.parse(scan); }

            typename ParserT::embed_t p;
        };
        
#if BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT > 1

        ///////////////////////////////////////////////////////////////////////
        //
        //  This generates partial specializations for the class 
        //
        //          abstract_parser 
        //
        //  with an increasing number of different ScannerT template parameters
        //  and corresponding do_parse_virtual function declarations for each 
        //  of the different required scanner types:
        //
        //      template <typename ScannerT0, ..., typename AttrT>
        //      struct abstract_parser<scanner_list<ScannerT0, ...>, AttrT>
        //      {
        //          abstract_parser() {} 
        //          virtual ~abstract_parser() {} 
        //
        //          virtual typename match_result<ScannerT0, AttrT>::type
        //          do_parse_virtual(ScannerT0 const &scan) const = 0;
        //
        //          ...
        //      };
        //
        ///////////////////////////////////////////////////////////////////////
        #define BOOST_SPIRIT_RULE_ENUM_DOPARSE_A(z, N, _)                       \
                virtual typename match_result<                                  \
                    BOOST_PP_CAT(ScannerT, N), AttrT                            \
                >::type                                                         \
                do_parse_virtual(                                               \
                    BOOST_PP_CAT(ScannerT, N) const& scan) const = 0;           \

        #define BOOST_SPIRIT_ENUM_ABSTRACT_PARSERS(z, N, _)                     \
            template <                                                          \
                BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(N), typename ScannerT),  \
                typename AttrT                                                  \
            >                                                                   \
            struct abstract_parser<                                             \
                scanner_list<                                                   \
                    BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(N), ScannerT)        \
                >,                                                              \
                AttrT                                                           \
            >                                                                   \
            {                                                                   \
                abstract_parser() {}                                            \
                virtual ~abstract_parser() {}                                   \
                                                                                \
                BOOST_PP_REPEAT_ ## z(                                          \
                    BOOST_PP_INC(N), BOOST_SPIRIT_RULE_ENUM_DOPARSE_A, _)       \
            };                                                                  \

        BOOST_PP_REPEAT_FROM_TO(1, BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT, 
            BOOST_SPIRIT_ENUM_ABSTRACT_PARSERS, _)

        #undef BOOST_SPIRIT_RULE_ENUM_DOPARSE_A
        #undef BOOST_SPIRIT_ENUM_ABSTRACT_PARSERS
        ///////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////////////////
        //
        //  This generates partial specializations for the class
        //
        //          concrete_parser 
        //
        //  with an increasing number of different ScannerT template parameters
        //  and corresponding do_parse_virtual function declarations for each 
        //  of the different required scanner types:
        //
        //      template <
        //          typename ParserT, typename ScannerT0, ..., typename AttrT
        //      >
        //      struct concrete_parser<
        //          ParserT, scanner_list<ScannerT0, ...>, AttrT
        //      >
        //      :   public abstract_parser<scanner_list<ScannerT0, ...>, AttrT>
        //      {
        //          concrete_parser(ParserT const& p_) : p(p_) {} 
        //          virtual ~concrete_parser() {}
        //
        //          virtual typename match_result<ScannerT0, AttrT>::type
        //          do_parse_virtual(ScannerT0 const &scan) const
        //          { return p.parse(scan); }
        //
        //          ...
        //        
        //          typename ParserT::embed_t p;
        //      };
        //
        ///////////////////////////////////////////////////////////////////////
        #define BOOST_SPIRIT_RULE_ENUM_DOPARSE_C(z, N, _)                       \
                virtual typename match_result<                                  \
                    BOOST_PP_CAT(ScannerT, N), AttrT                            \
                >::type                                                         \
                do_parse_virtual(                                               \
                    BOOST_PP_CAT(ScannerT, N) const& scan) const                \
                { return p.parse(scan); }                                       \

        #define BOOST_SPIRIT_ENUM_CONCRETE_PARSERS(z, N, _)                     \
            template <                                                          \
                typename ParserT,                                               \
                BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(N), typename ScannerT),  \
                typename AttrT                                                  \
            >                                                                   \
            struct concrete_parser<                                             \
                ParserT,                                                        \
                scanner_list<                                                   \
                    BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(N), ScannerT)        \
                >,                                                              \
                AttrT                                                           \
            >                                                                   \
            :   abstract_parser<                                                \
                    scanner_list<                                               \
                        BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(N), ScannerT)    \
                    >,                                                          \
                    AttrT                                                       \
                >                                                               \
            {                                                                   \
                concrete_parser(ParserT const& p_) : p(p_) {}                   \
                virtual ~concrete_parser() {}                                   \
                                                                                \
                BOOST_PP_REPEAT_ ## z(                                          \
                    BOOST_PP_INC(N), BOOST_SPIRIT_RULE_ENUM_DOPARSE_C, _)       \
                                                                                \
                typename ParserT::embed_t p;                                    \
            };                                                                  \

        BOOST_PP_REPEAT_FROM_TO(1, BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT, 
            BOOST_SPIRIT_ENUM_CONCRETE_PARSERS, _)

        #undef BOOST_SPIRIT_ENUM_CONCRETE_PARSERS
        #undef BOOST_SPIRIT_RULE_ENUM_DOPARSE_C
        ///////////////////////////////////////////////////////////////////////

#endif // BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT > 1

    } // namespace impl

}} // namespace boost::spirit

#endif
