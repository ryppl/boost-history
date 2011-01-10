
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

// Use all different parameter and binding permutations.

#include <boost/local/function.hpp>

struct c {
    void f(double p = 1.23, double q = -1.23) {
        { // No params, no const binds, no plain binds.
            BOOST_LOCAL_FUNCTION(
            (void) (l)( (void) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(l)
            l();
        }

        { // Only params.
            BOOST_LOCAL_FUNCTION(
            (void) (l)( (int)(x) (int)(y)(default)(0) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(l)
            l(1);
        }
        { // Only const binds.
            int a, b;
            BOOST_LOCAL_FUNCTION(
            (void) (l)( (const bind)((a)(&b)(&p)(q)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(l)
            l();

            BOOST_LOCAL_FUNCTION(
            (void) (t)( (const bind)((this)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(t)
            t();

            BOOST_LOCAL_FUNCTION(
            (void) (lt)( (const bind)((a)(&b)(&p)(q)(this)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(lt)
            lt();
        }
        { // Only plain binds.
            int c, d;
            BOOST_LOCAL_FUNCTION(
            (void) (l)( (bind)((c)(&d)(&p)(&q)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(l)
            l();

            BOOST_LOCAL_FUNCTION(
            (void) (t)( (bind)((this)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(t)
            t();

            BOOST_LOCAL_FUNCTION(
            (void) (lt)( (bind)((c)(&d)(&p)(&q)(this)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(lt)
            lt();
        }

        { // Both params and const binds.
            int a, b;
            BOOST_LOCAL_FUNCTION(
            (void) (l)( (int)(x) (int)(y)(default)(0)
                    (const bind)((a)(&b)(&p)(q)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(l)
            l(1);

            BOOST_LOCAL_FUNCTION(
            (void) (t)( (int)(x) (int)(y)(default)(0)
                    (const bind)((this)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(t)
            t(1);

            BOOST_LOCAL_FUNCTION(
            (void) (lt)( (int)(x) (int)(y)(default)(0)
                    (const bind)((a)(this)(&b)(&p)(q)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(lt)
            lt(1);
        }
        { // Both params and plain binds.
            int c, d;
            BOOST_LOCAL_FUNCTION(
            (void) (l)( (int)(x) (int)(y)(default)(0)
                    (bind)((c)(&d)(&p)(q)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(l)
            l(1);

            BOOST_LOCAL_FUNCTION(
            (void) (t)( (int)(x) (int)(y)(default)(0)
                    (bind)((this)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(t)
            t(1);
            
            BOOST_LOCAL_FUNCTION(
            (void) (lt)( (int)(x) (int)(y)(default)(0)
                    (bind)((c)(&d)(&p)(this)(q)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(lt)
            lt(1);
        }
        { // Both const and plain binds.
            int a, b, c, d;
            BOOST_LOCAL_FUNCTION(
            (void) (l)( (const bind)((a)(&b)(p)) (bind)((c)(&d)(q)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(l)
            l();

            BOOST_LOCAL_FUNCTION(
            (void) (ct)( (const bind)((this)) (bind)((c)(&d)(q)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(ct)
            ct();
            BOOST_LOCAL_FUNCTION(
            (void) (lct)( (const bind)((this)(a)(&b)(p)) (bind)((c)(&d)(q)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(lct)
            lct();

            BOOST_LOCAL_FUNCTION(
            (void) (pt)( (const bind)((a)(&b)(p)) (bind)((this)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(pt)
            pt();
            BOOST_LOCAL_FUNCTION(
            (void) (lpt)( (const bind)((a)(&b)(p)) (bind)((c)(this)(&d)(q)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(lpt)
            lpt();
        }

        { // All params, const binds, and plain binds.
            int a, b, c, d;
            BOOST_LOCAL_FUNCTION(
            (void) (l)( (int)(x) (int)(y)(default)(0)
                    (const bind)((a)(&b)(&p)) (bind)((c)(&d)(&q)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(l)
            l(1);

            BOOST_LOCAL_FUNCTION(
            (void) (ct)( (int)(x) (int)(y)(default)(0)
                    (const bind)((this)) (bind)((c)(&d)(&q)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(ct)
            ct(1);
            BOOST_LOCAL_FUNCTION(
            (void) (pt)( (int)(x) (int)(y)(default)(0)
                    (const bind)((a)(&b)(&p)) (bind)((this)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(pt)
            pt(1);

            BOOST_LOCAL_FUNCTION(
            (void) (lct)( (int)(x) (int)(y)(default)(0)
                    (const bind)((a)(this)(&b)(&p)) (bind)((c)(&d)(&q)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(lct)
            lct(1);
            BOOST_LOCAL_FUNCTION(
            (void) (lpt)( (int)(x) (int)(y)(default)(0)
                    (const bind)((a)(&b)(&p)) (bind)((c)(&d)(this)(&q)) )
            ) {
            } BOOST_LOCAL_FUNCTION_END(lpt)
            lpt(1);
        }
    }
};
    
int main() {
    c().f();
    return 0;
}

