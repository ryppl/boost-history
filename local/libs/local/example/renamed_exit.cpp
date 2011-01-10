
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

// Implement simple local exit using local function renaming.

//[ renamed_exit_cpp
#include <boost/local/function.hpp>
#include <boost/local/function_ref.hpp>
#include <boost/preprocessor/cat.hpp>

#define EXIT(parenthesized_binding) \
    BOOST_LOCAL_FUNCTION( \
    (void) (BOOST_PP_CAT(exit_code, __LINE__))( parenthesized_binding ) \
    ) /* followed by exit body here */

// Local function renaming is necessary because `__LINE__` has a different
// value when `EXIT_END` macro expands respect to its value when the `EXIT()`
// macro expands.
#define EXIT_END \
    /* preceded by exit body here */ \
    BOOST_LOCAL_FUNCTION_END_RENAME( \
            BOOST_PP_CAT(exit_code, __LINE__)) \
    exit_guard BOOST_PP_CAT(exit_guardian, __LINE__)( \
            BOOST_PP_CAT(exit_code, __LINE__));

struct exit_guard {
    typedef boost::local::function_ref< void () > ref_type;
    explicit exit_guard(ref_type ref): ref_(ref) {}
    ~exit_guard() { ref_(); } // Execute local function code at scope exit.
private:
    ref_type ref_;
};

int main() {
    int* a = new int[3];
    EXIT( (bind)((a)) ) {
        // Body outside macros so retain complier error meaning.
        if (a) delete[] a;
    } EXIT_END

    return 0;
}
//]

