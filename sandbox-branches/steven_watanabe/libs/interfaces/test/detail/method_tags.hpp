// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_TEST_METHOD_TAGS_HPP_INCLUDED
#define BOOST_IDL_TEST_METHOD_TAGS_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

namespace boost { namespace interfaces { namespace test {

template< typename Class, typename Function, 
          typename Signature = int >
struct method { };

struct func_one_tag { };
struct func_two_tag { };
struct func_three_tag { };
struct func_four_tag { };
struct func_five_tag { };
struct func_six_tag { };
struct func_seven_tag { };
struct func_eight_tag { };

struct before_tag { };
struct after_tag { };
struct finally_tag { };
struct error_tag { };
struct cancel_tag { };
struct transform_tag { };

struct sig_a_tag { };
struct sig_b_tag { };
struct sig_c_tag { };

} } } // End namespace test, interfaces, boost.

#endif // #ifndef BOOST_IDL_TEST_METHOD_TAGS_HPP_INCLUDED
