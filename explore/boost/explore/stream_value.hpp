//
// stream_value.hpp - streaming function objects for different value types
//
// Copyright (C) 2007, Jeffrey Faust
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_EXPLORE_STREAM_VALUE_H
#define BOOST_EXPLORE_STREAM_VALUE_H

#include "container_stream_state.hpp"

#include <ostream>

// should be the last #include
#include <boost/type_traits/detail/bool_trait_def.hpp>

namespace boost { namespace explore
{
    namespace detail
    {
        BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_string, T, false)

        BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_string, char*, true)
        BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_string, const char*, true)
        BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_string, wchar_t*, true)
        BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_string, const wchar_t*, true)
        BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_string, std::string, true)
        BOOST_TT_AUX_BOOL_TRAIT_CV_SPEC1(is_string, std::wstring, true)

        template<typename T>
        struct value_holder_base
        {
            explicit value_holder_base(const T& t, bool quotestrings) : m_value(t), m_quotestrings(quotestrings) {}
            const T& m_value;
            bool m_quotestrings;
        };

        template<typename T, int S>
        struct value_holder;

        template<typename T>
        struct value_holder<T, boost::false_type::value> : public value_holder_base<T>
        {
            explicit value_holder(const T& value, bool quotestrings) : value_holder_base<T>(value, quotestrings) {}

            template<typename Elem, typename Tr>
            friend std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const value_holder_base<T>& v)
            {
                return ostr << v.m_value;
            }
        };

        template<typename T>
        struct value_holder<T, boost::true_type::value> : public value_holder_base<T>
        {
            explicit value_holder(const T& value, bool quotestrings) : value_holder_base<T>(value, quotestrings) {}

            template<typename Elem, typename Tr>
            friend std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const value_holder_base<T>& v)
            {
                if( v.m_quotestrings )
                {
                    return ostr << "\"" << v.m_value << "\"";
                }

                return ostr << v.m_value;
            }
        };

        template<typename T>
        struct value : public value_holder<T, is_string<T>::value>
        {
            explicit value(const T& value, bool quotestrings) : value_holder<T, is_string<T>::value>(value, quotestrings) {}
        };
    }

    struct stream_normal_value
    {
        template<typename Elem, typename Tr, typename T>
        void operator()(std::basic_ostream<Elem, Tr>& ostr, const T& val, container_stream_state<Elem>* state, container_common_stream_state* common_state)
        {
            const bool qs = common_state->quote_strings();
            ostr.width(common_state->itemwidth());
            ostr << detail::value<T>(val, qs);
        }
    };

    // stream value from associative container
    struct stream_associative_value
    {
        template<typename Elem, typename Tr, typename T>
        void operator()(std::basic_ostream<Elem, Tr>& ostr, const T& val, container_stream_state<Elem>* state, container_common_stream_state* common_state)
        {
            const bool qs = common_state->quote_strings();
            ostr << state->assoc_item_start()     << detail::value<typename T::first_type>(val.first, qs)
                 << state->assoc_item_separator() << detail::value<typename T::second_type>(val.second, qs)
                 << state->assoc_item_end();
        }
    };
}}

#endif
