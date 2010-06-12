/*=============================================================================
    Copyright (c) 2009 Daniel James
    Copyright (c) 2002 2004 2006 Joel de Guzman
    Copyright (c) 2004 Eric Niebler
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_QUICKBOOK_AS_STRING_HPP)
#define BOOST_SPIRIT_QUICKBOOK_AS_STRING_HPP

#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/qi_nonterminal.hpp>
#include <string>

namespace quickbook
{
    namespace spirit = boost::spirit;
    namespace ph = boost::phoenix;

    // member_assign - action to assign the attribute to a member of the
    //                 rule's attributte.

    template <typename Struct, typename Member>
    struct member_assign_type {
        member_assign_type(Member Struct::*mem_ptr) : mem_ptr_(mem_ptr) {}
    
        template <typename Context>
        void operator()(Member& attrib, Context& context, bool& pass) const {
            (ph::bind(mem_ptr_, spirit::_val) = attrib)(attrib, context, pass);
        }
        
        template <typename Attrib, typename Context>
        void operator()(Attrib& attrib, Context& context, bool& pass) const {
            (ph::bind(mem_ptr_, spirit::_val) = attrib)(attrib, context, pass);
        }

        Member Struct::*mem_ptr_;
    };
    
    template <typename Struct>
    struct member_assign_type<Struct, std::string> {
        member_assign_type(std::string Struct::*mem_ptr) : mem_ptr_(mem_ptr) {}
    
        template <typename Context>
        void operator()(std::string& attrib, Context& context, bool& pass) const {
            (ph::bind(mem_ptr_, spirit::_val) = attrib)(attrib, context, pass);
        }

        template <typename Attrib, typename Context>
        void operator()(Attrib& attrib, Context& context, bool& pass) const {
            (ph::bind(mem_ptr_, spirit::_val) = std::string(attrib.begin(), attrib.end()))(attrib, context, pass);
        }
        
        std::string Struct::*mem_ptr_;
    };
    
    template <typename Struct, typename Member>
    member_assign_type<Struct, Member> member_assign(Member Struct::*mem_ptr) {
        return member_assign_type<Struct, Member>(mem_ptr);
    }

    // member_push - action to push the attribute to a member of the
    //               rule's attributte.

    template <typename Struct, typename Member>
    struct member_push_type {
        member_push_type(Member Struct::*mem_ptr) : mem_ptr_(mem_ptr) {}
    
        template <typename Context>
        void operator()(Member& attrib, Context& context, bool& pass) const {
            ph::bind(mem_ptr_, spirit::_val)(attrib, context, pass)
                .push_back(attrib);
        }
        
        template <typename Attrib, typename Context>
        void operator()(Attrib& attrib, Context& context, bool& pass) const {
            ph::bind(mem_ptr_, spirit::_val)(attrib, context, pass)
                .push_back(typename Member::value_type(attrib));
        }

        Member Struct::*mem_ptr_;
    };
    
    template <typename Struct>
    struct member_push_type<Struct, std::vector<std::string> > {
        member_push_type(std::vector<std::string> Struct::*mem_ptr) : mem_ptr_(mem_ptr) {}
    
        template <typename Context>
        void operator()(std::string& attrib, Context& context, bool& pass) const {
            ph::bind(mem_ptr_, spirit::_val)(attrib, context, pass)
                .push_back(attrib);
        }

        template <typename Attrib, typename Context>
        void operator()(Attrib& attrib, Context& context, bool& pass) const {
            ph::bind(mem_ptr_, spirit::_val)(attrib, context, pass)
                .push_back(std::string(attrib.begin(), attrib.end()));
        }
        
        std::vector<std::string> Struct::*mem_ptr_;
    };
    
    template <typename Struct, typename Member>
    member_push_type<Struct, Member> member_push(Member Struct::*mem_ptr) {
        return member_push_type<Struct, Member>(mem_ptr);
    }


}

#endif
