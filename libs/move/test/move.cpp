// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/move.hpp>
#include "say.hpp"
#include "x.hpp"
#include "move_only.hpp"

//
// Some functions we can use to test the passing of Xs in and out of
// functions.
//
X source()
{
    return X();
}

X const csource()
{
    return X();
}

void sink(X)
{
    SAY("in rvalue sink");
}

void sink2(X&)
{
    SAY("in non-const lvalue sink2");
}

void sink2(X const&)
{
    SAY("in const lvalue sink2");
}

void sink3(X&)
{
    SAY("in non-const lvalue sink3");
}

template <class T>
void tsink(T)
{
    SAY("in templated rvalue sink");
}

move_only test_returnable() { move_only x; return boost::move(x); }

int main()
{
    SAY(" ------ test 1, direct init from rvalue ------- ");
#ifdef __GNUC__ // GCC having trouble parsing the extra parens
    X z2((0, X() ));
#else
    X z2((X()));
#endif 

    SAY(" ------ test 2, copy init from rvalue ------- ");
    X z4 = X();

    SAY(" ------ test 3, copy init from lvalue ------- ");
    X z5 = z4;

    SAY(" ------ test 4, direct init from lvalue ------- ");
    X z6(z4);

    SAY(" ------ test 5, construct const ------- ");
    X const z7;

    SAY(" ------ test 6, copy init from lvalue ------- ");
    X z8 = z7;

    SAY(" ------ test 7, direct init from lvalue ------- ");
    X z9(z7);

    SAY(" ------ test 8, pass rvalue by-value ------- ");
    sink(source());

    SAY(" ------ test 9, pass const rvalue by-value ------- ");
    sink(csource());

    SAY(" ------ test 10, pass rvalue by overloaded reference ------- ");
    // This one fails in Comeau's strict mode due to 8.5.3/5.  GCC 3.3.1 passes it.
    sink2(source());

    SAY(" ------ test 11, pass const rvalue by overloaded reference ------- ");
    sink2(csource());

    SAY(" ------ test 13, pass lvalue by-value ------- ");
    sink(z5);

    SAY(" ------ test 14, pass const lvalue by-value ------- ");
    sink(z7);

    SAY(" ------ test 15, pass lvalue by-reference ------- ");
    sink2(z4);

    SAY(" ------ test 16, pass const lvalue by const reference ------- ");
    sink2(z7);

    SAY(" ------ test 17, pass rvalue by value to template param ------- ");
    tsink(source());

    SAY(" ------ test 18, direct initialize a const A with an A ------- ");
    typedef X const XC;
    sink2(XC(X()));

    SAY(" ------ test 19, assign from non-const lvalue ------- ");
    z4 = z5;

    SAY(" ------ test 20, assign from const lvalue ------- ");
    z4 = z7;

    SAY(" ------ test 21, assign from rvalue ------- ");
    z4 = source();

    SAY(" ------ test 22, move construct from movable lvalue ------- ");
    X z10(boost::move(z2));
    
    SAY(" ------ test 23, move assign from movable lvalue ------- ");
    z10 = boost::move(z4, 0); // Using 0 to see that the msvc6-generic form works too

    SAY(" ------ test 24, request move construct from non-movable lvalue ------- ");
    std::string s1("hello");
    std::string s2(boost::move(s1, 0));

    SAY(" ------ test 25, movable but noncopyable test ------- ");
    
    move_only quack(test_returnable());
    
    SAY("----- done -----");
    return 0;
}
