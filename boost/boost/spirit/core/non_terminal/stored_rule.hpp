/*=============================================================================
    Spirit v1.7.0
    Copyright (c) 1998-2003 Joel de Guzman
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#if !defined(BOOST_SPIRIT_STORED_RULE_HPP)
#define BOOST_SPIRIT_STORED_RULE_HPP

///////////////////////////////////////////////////////////////////////////////
#include <boost/spirit/core/non_terminal/stored_rule.hpp>
#include <boost/smart_ptr.hpp> 
#include <boost/weak_ptr.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit {

    template <
        typename T0 = nil_t
      , typename T1 = nil_t
      , typename T2 = nil_t
    >
    class stored_rule; 

    ///////////////////////////////////////////////////////////////////////////
    //
    //  stored_rule_ref class
    //
    //      <<< For framework use only. Not for public consumption. >>>
    //
    ///////////////////////////////////////////////////////////////////////////
    namespace impl
    {
        template <
            typename T0 = nil_t
          , typename T1 = nil_t
          , typename T2 = nil_t
        >
        class stored_rule_ref 
            : public impl::rule_base<
                stored_rule_ref<T0, T1, T2>
              , stored_rule_ref<T0, T1, T2> const&
              , T0, T1, T2>
        {
        public:   
    
            typedef impl::rule_base<
                stored_rule_ref<T0, T1, T2>
              , stored_rule_ref<T0, T1, T2> const&
              , T0, T1, T2>
            base_t;
            
            typedef typename base_t::scanner_t scanner_t;
            typedef typename base_t::attr_t attr_t;
            typedef impl::abstract_parser<scanner_t, attr_t> abstract_parser_t;
    
            stored_rule_ref() : ptr() {}
            ~stored_rule_ref() {}
    
            stored_rule_ref(stored_rule_ref const& r)
            : ptr(r.ptr) {} // copy constructor 
            
            stored_rule_ref(stored_rule<T0, T1, T2> const& r)
            : ptr(r.ptr) {} // copy from stored_rule
            
            ::boost::smart_ptr<abstract_parser_t>
            get() const
            {
                return ptr.lock();
            }
            
            bool
            defined() const
            {
                return !ptr.expired();
            }
        
        private:
        
            stored_rule_ref& operator=(stored_rule_ref const& r); // not allowed
            ::boost::weak_ptr<abstract_parser_t> ptr;
        };

    } // namespace impl
    
    ///////////////////////////////////////////////////////////////////////////
    //
    //  stored_rule class
    //
    ///////////////////////////////////////////////////////////////////////////
    template <
        typename T0 = nil_t
      , typename T1 = nil_t
      , typename T2 = nil_t
    >
    class stored_rule 
        : public impl::rule_base<
            stored_rule<T0, T1, T2>
          , impl::stored_rule_ref<T0, T1, T2>
          , T0, T1, T2>
    {
    public:   

        typedef impl::rule_base<
            stored_rule<T0, T1, T2>
          , stored_rule<T0, T1, T2> const&
          , T0, T1, T2>
        base_t;
        
        typedef typename base_t::scanner_t scanner_t;
        typedef typename base_t::attr_t attr_t;
        typedef impl::abstract_parser<scanner_t, attr_t> abstract_parser_t;

        stored_rule() : ptr() {}
        ~stored_rule() {}

        stored_rule(stored_rule const& r)
        : ptr(r.ptr) {}

        template <typename ParserT>
        stored_rule(ParserT const& p)
        : ptr(new impl::concrete_parser<ParserT, scanner_t, attr_t>(p)) {}

        template <typename ParserT>
        stored_rule& operator=(ParserT const& p)
        {
            ptr = r.ptr;
            return *this;
        }

        stored_rule& operator=(stored_rule const& r)
        {
            //  If this is placed above the templatized assignment
            //  operator, VC6 incorrectly complains ambiguity with
            //  r1 = r2, where r1 and r2 are both rules.
            ptr = r.ptr;
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
    
        friend class stored_rule_ref<T0, T1, T2>;
        ::boost::smart_ptr<abstract_parser_t> ptr;
    };

///////////////////////////////////////////////////////////////////////////////
}} // namespace boost::spirit

#endif
