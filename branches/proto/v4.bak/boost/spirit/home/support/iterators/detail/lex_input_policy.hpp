//  Copyright (c) 2001, Daniel C. Nuffer
//  Copyright (c) 2001-2008, Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_SPIRIT_ITERATOR_LEX_INPUT_POLICY_MAR_16_2007_1205PM)
#define BOOST_SPIRIT_ITERATOR_LEX_INPUT_POLICY_MAR_16_2007_1205PM

#include <boost/spirit/home/support/iterators/multi_pass_fwd.hpp>
#include <boost/spirit/home/support/iterators/detail/multi_pass.hpp>

namespace boost { namespace spirit { namespace multi_pass_policies
{
    ///////////////////////////////////////////////////////////////////////////////
    //  class lex_input
    //  Implementation of the InputPolicy used by multi_pass
    // 
    //  The lex_input class gets tokens (integers) from yylex()
    ///////////////////////////////////////////////////////////////////////////
    struct lex_input
    {
        ///////////////////////////////////////////////////////////////////////
        template <typename T>
        class unique : public detail::default_input_policy
        {
        public:
            typedef int value_type;
            typedef std::ptrdiff_t difference_type;
            typedef std::ptrdiff_t distance_type;
            typedef int* pointer;
            typedef int& reference;

        protected:
            unique() {}
            explicit unique(T x) {}

        public:
            template <typename MultiPass>
            static void advance_input(MultiPass& mp, value_type& t)
            {
                // if mp.shared is NULL then this instance of the multi_pass 
                // represents a end iterator, so no advance functionality is 
                // needed
                if (0 != mp.shared) 
                {
                    extern int yylex();
                    t = yylex();
                }
            }

            // test, whether we reached the end of the underlying stream
            template <typename MultiPass>
            static bool input_at_eof(MultiPass const& mp, value_type const& t) 
            {
                return 0 == t;
            }

            template <typename MultiPass>
            static bool input_is_valid(MultiPass const& mp, value_type const& t) 
            {
                return -1 != t;
            }
        };

        ///////////////////////////////////////////////////////////////////////
        template <typename T>
        struct shared
        {
            explicit shared(T x) {}

            // no shared data elements
        };
    };

}}}

#endif

