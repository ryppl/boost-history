/*=============================================================================
    Spirit V1.5.2
    Copyright (c) 2002-2003 Joel de Guzman
    Copyright (c) 2002-2003 Hartmut Kaiser
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#if !defined(BOOST_SPIRIT_PARSER_CONTEXT_HPP)
#define BOOST_SPIRIT_PARSER_CONTEXT_HPP

///////////////////////////////////////////////////////////////////////////////
namespace boost
{
    namespace spirit
    {

    ///////////////////////////////////////////////////////////////////////////
    //
    //  default_parser_context_base class { default context base }
    //
    ///////////////////////////////////////////////////////////////////////////
    struct default_parser_context_base
    {
        template <typename DerivedT>
        struct aux {};
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  parser_context class { default context }
    //
    ///////////////////////////////////////////////////////////////////////////
    struct nil_t;
    struct parser_context
    {
        typedef nil_t attr_t;
        typedef default_parser_context_base base_t;

        template <typename ParserT>
        parser_context(ParserT const&) {}

        template <typename ParserT, typename ScannerT>
        void
        pre_parse(ParserT const&, ScannerT const&) {}

        template <typename ResultT, typename ParserT, typename ScannerT>
        ResultT&
        post_parse(ResultT& hit, ParserT const&, ScannerT const&)
        { return hit; }
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  context_aux class
    //
    //      context_aux<ContextT, DerivedT> is a class derived from the
    //      ContextT's nested base_t::base<DerivedT> template class. (see
    //      default_parser_context_base::aux for an example).
    //
    //      Basically, this class provides ContextT dependent optional
    //      functionality to the derived class DerivedT through the CRTP
    //      idiom (Curiously recurring template pattern).
    //
    ///////////////////////////////////////////////////////////////////////////
    #if defined(BOOST_MSVC) && (BOOST_MSVC <= 1300)
    BOOST_SPIRIT_DEPENDENT_TEMPLATE_WRAPPER(context_base_wrapper, aux);

    //////////////////////////////////
    template <typename ContextT, typename DerivedT,
        typename AuxT =
            impl::context_base_wrapper<ContextT::base_t>
                ::template result_<DerivedT> >
    struct context_aux : public AuxT {};

    #else

    template <typename ContextT, typename DerivedT>
    struct context_aux : public ContextT::base_t::template aux<DerivedT> {};

    #endif

    ///////////////////////////////////////////////////////////////////////////
    //
    //  parser_scanner_linker and parser_scanner_linker classes
    //  { helper templates for the rule extendability }
    //
    //      This classes can be 'overloaded' (defined elsewhere), to plug
    //      in additional functionality into the rule parsing process.
    //
    ///////////////////////////////////////////////////////////////////////////
    #if !defined(BOOST_SPIRIT_PARSER_SCANNER_LINKER_DEFINED)
    #define BOOST_SPIRIT_PARSER_SCANNER_LINKER_DEFINED

    template<typename ScannerT>
    struct parser_scanner_linker : public ScannerT
    {
        parser_scanner_linker(ScannerT const scan_) : ScannerT(scan_) {}
    };

    #endif // !defined(BOOST_SPIRIT_PARSER_SCANNER_LINKER_DEFINED)

    //////////////////////////////////
    #if !defined(BOOST_SPIRIT_PARSER_CONTEXT_LINKER_DEFINED)
    #define BOOST_SPIRIT_PARSER_CONTEXT_LINKER_DEFINED

    template<typename ContextT>
    struct parser_context_linker : public ContextT
    {
        template <typename ParserT>
        parser_context_linker(ParserT const& p)
        : ContextT(p) {}

        template <typename ParserT, typename ScannerT>
        void pre_parse(ParserT const& p, ScannerT const& scan)
        { ContextT::pre_parse(p, scan); }

        template <typename ResultT, typename ParserT, typename ScannerT>
        ResultT&
        post_parse(ResultT& hit, ParserT const& p, ScannerT const& scan)
        { return ContextT::post_parse(hit, p, scan); }
    };

    #endif // !defined(BOOST_SPIRIT_PARSER_CONTEXT_LINKER_DEFINED)

    ///////////////////////////////////////////////////////////////////////////
    //
    //  BOOST_SPIRIT_CONTEXT_PARSE helper macro
    //
    //      The original implementation uses a template class. However, we
    //      need to lessen the template instantiation depth to help inferior
    //      compilers that sometimes choke on deep template instantiations.
    //      The objective is to avoid code redundancy. A macro, in this case
    //      is an obvious solution. Sigh!
    //
    //      WARNING: INTERNAL USE ONLY. NOT FOR PUBLIC CONSUMPTION.
    //
    ///////////////////////////////////////////////////////////////////////////
    #define BOOST_SPIRIT_CONTEXT_PARSE(scan, this_, scanner_t, context_t, result_t) \
            scanner_t scan_wrap(scan);                                              \
            context_t context_wrap(this_);                                          \
            context_wrap.pre_parse(this_, scan_wrap);                               \
            result_t hit = parse_main(scan);                                        \
            return context_wrap.post_parse(hit, this_, scan_wrap);

    } // namespace spirit
} // namespace boost

#endif
