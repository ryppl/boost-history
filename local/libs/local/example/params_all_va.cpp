
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

#include <boost/config.hpp>
#if defined(BOOST_NO_VARIADIC_MACROS) || defined(BOOST_LOCAL_CONFIG_COMPLIANT)
#include <iostream>
int main() {
    std::cerr << "Error: This program requires variadic macros" << std::endl;
    return 0;
}
#else

//[params_all_va_cpp
#include <boost/local/function.hpp>

struct s {
    void f(double p = 1.23, double q = -1.23) {
        { // Only params.
            void BOOST_LOCAL_FUNCTION_PARAMS(int x, int y, default 0) {
            } BOOST_LOCAL_FUNCTION_NAME(l)
            l(1);
        }
        { // Only const binds.
            int a, b;

            const int& BOOST_LOCAL_FUNCTION_PARAMS(const bind a,
                    const bind& b, const bind& p, const bind q) {
                return b;
            } BOOST_LOCAL_FUNCTION_NAME(l)
            l();

            const s& BOOST_LOCAL_FUNCTION_PARAMS(const bind this) {
                return *this_;
            } BOOST_LOCAL_FUNCTION_NAME(t)
            t();

            const int BOOST_LOCAL_FUNCTION_PARAMS(const bind a,
                    const bind& b, const bind& p, const bind q,
                    const bind this) {
                return a;
            } BOOST_LOCAL_FUNCTION_NAME(lt)
            lt();
        }
        { // Only plain binds.
            int c, d;

            int& BOOST_LOCAL_FUNCTION_PARAMS(bind c, bind& d,
                    bind& p, bind& q) {
                return d;
            } BOOST_LOCAL_FUNCTION_NAME(l)
            l();

            s& BOOST_LOCAL_FUNCTION_PARAMS(bind this) {
                return *this_;
            } BOOST_LOCAL_FUNCTION_NAME(t)
            t();

            int BOOST_LOCAL_FUNCTION_PARAMS(bind c, bind& d,
                    bind& p, bind& q, bind this) {
                return c;
            } BOOST_LOCAL_FUNCTION_NAME(lt)
            lt();
        }

        { // Both params and const binds.
            int a, b;

            void BOOST_LOCAL_FUNCTION_PARAMS(int x, int y, default 0,
                    const bind a, const bind& b,
                    const bind& p, const bind q) {
            } BOOST_LOCAL_FUNCTION_NAME(l)
            l(1);

            void BOOST_LOCAL_FUNCTION_PARAMS(int x, int y, default 0,
                    const bind this) {
            } BOOST_LOCAL_FUNCTION_NAME(t)
            t(1);

            void BOOST_LOCAL_FUNCTION_PARAMS(int x, int y, default 0,
                    const bind a, const bind this,
                    const bind& b, const bind& p, const bind q) {
            } BOOST_LOCAL_FUNCTION_NAME(lt)
            lt(1);
        }
        { // Both params and plain binds.
            int c, d;

            void BOOST_LOCAL_FUNCTION_PARAMS(int x, int y, default 0,
                    bind c, bind& d, bind& p, bind q) {
            } BOOST_LOCAL_FUNCTION_NAME(l)
            l(1);

            void BOOST_LOCAL_FUNCTION_PARAMS(int x, int y, default 0,
                    bind this) {
            } BOOST_LOCAL_FUNCTION_NAME(t)
            t(1);
            
            void BOOST_LOCAL_FUNCTION_PARAMS(int x, int y, default 0,
                    bind c, bind& d,
                    bind& p, bind this, bind q) {
            } BOOST_LOCAL_FUNCTION_NAME(lt)
            lt(1);
        }
        { // Both const and plain binds.
            int a, b, c, d;

            void BOOST_LOCAL_FUNCTION_PARAMS(const bind a, const bind& b,
                    const bind p, bind c, bind& d, bind q) {
            } BOOST_LOCAL_FUNCTION_NAME(l)
            l();

            void BOOST_LOCAL_FUNCTION_PARAMS(const bind this,
                    bind c, bind& d, bind q) {
            } BOOST_LOCAL_FUNCTION_NAME(ct)
            ct();
            void BOOST_LOCAL_FUNCTION_PARAMS(const bind this,
                    const bind a, const bind& b, const bind p,
                    bind c, bind& d, bind q) {
            } BOOST_LOCAL_FUNCTION_NAME(lct)
            lct();

            void BOOST_LOCAL_FUNCTION_PARAMS(const bind a, const bind& b,
                    const bind p, bind this) {
            } BOOST_LOCAL_FUNCTION_NAME(pt)
            pt();
            void BOOST_LOCAL_FUNCTION_PARAMS(const bind a, const bind& b,
                    const bind p, bind c, bind this, bind& d, bind q) {
            } BOOST_LOCAL_FUNCTION_NAME(lpt)
            lpt();
        }

        { // All params, const binds, and plain binds.
            int a, b, c, d;
            
            void BOOST_LOCAL_FUNCTION_PARAMS(int x, int y, default 0,
                    const bind a, const bind& b, const bind& p,
                    bind c, bind& d, bind& q) {
            } BOOST_LOCAL_FUNCTION_NAME(l)
            l(1);

            void BOOST_LOCAL_FUNCTION_PARAMS(int x, int y, default 0,
                    const bind this, bind c, bind& d, bind& q) {
            } BOOST_LOCAL_FUNCTION_NAME(ct)
            ct(1);
            void BOOST_LOCAL_FUNCTION_PARAMS(int x, int y, default 0,
                    const bind a, const bind& b, const bind& p,
                    bind this) {
            } BOOST_LOCAL_FUNCTION_NAME(pt)
            pt(1);

            void BOOST_LOCAL_FUNCTION_PARAMS(int x, int y, default 0,
                    const bind a, const bind this, const bind& b,
                    const bind& p, bind c, bind& d, bind& q) {
            } BOOST_LOCAL_FUNCTION_NAME(lct)
            lct(1);
            void BOOST_LOCAL_FUNCTION_PARAMS(int x, int y, default 0,
                    const bind a, const bind& b, const bind& p,
                    bind c, bind& d, bind this, bind& q) {
            } BOOST_LOCAL_FUNCTION_NAME(lpt)
            lpt(1);
        }
    }
};
    
int main() {
    s().f();
    return 0;
}
//]

#endif

