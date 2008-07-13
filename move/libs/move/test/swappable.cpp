// Copyright David Abrahams 2004. Distributed under the Boost
// Copyright Daniel James 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define DEBUG_MOVE
#include <boost/move.hpp>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include "say.hpp"

//
// A sample swappable class.
//
// Note that when it says 'move' here, it can mean that the class will be moved
// by swapping.
//
// TODO: I need to distinguish between classes constructed just to be swapped into,
// and value classes.

class Swappable
{
 public: // "Ordinary" stuff
    Swappable() : resource(++cnt)
    {
        SAY("Swappable() #" << resource);
    }

    Swappable(Swappable const& rhs) : resource(++cnt)
    {
        copy(rhs);
    }

    Swappable& operator=(Swappable const& rhs)
    {
        copy(rhs);
        resource = rhs.resource;
        return *this;
    }

    ~Swappable()
    {
        release();
    }
    
    static int copies;  // count the number of copies
    static int suboptimal_copies; // count the number of copies that should've been avoidable
    
    static void expect_copy() { copy_expected = true; move_expected = false; }
    static void expect_move() { copy_expected = false; move_expected = true; }

    friend void swap(Swappable& x, Swappable& y) {
        SAY("SWAP #" << x.resource << ", " << y.resource);
        BOOST_ASSERT(move_expected);
        std::swap(x.resource, y.resource);
    }
    
 private: // helper functions
    void release()
    {
        BOOST_ASSERT(resource <= cnt); // check for garbage
        if (!resource)
            SAY("destroy empty");
        else
            SAY("destroy #" << resource);
        resource = 0;
    }

    void copy(Swappable const& rhs)
    {
        BOOST_ASSERT(rhs.resource <= cnt); // check for garbage
        SAY("copy #" << this->resource << " <- #" << rhs.resource);
        if (!copy_expected)
        {
            SAY("***** SUBOPTIMAL COPY ******");
            ++suboptimal_copies;
        }
        ++copies;
    }
    
 private:   // Data members
    int resource;
    
    static int cnt;  // count the number of resources
    static bool copy_expected;
    static bool move_expected;
};

int Swappable::cnt;
int Swappable::copies;
int Swappable::suboptimal_copies;
bool Swappable::copy_expected;
bool Swappable::move_expected;

//
// Some functions we can use to test the passing of Ys in and out of
// functions.
//
Swappable source()
{
    return Swappable();
}

Swappable const csource()
{
    return Swappable();
}

void sink(Swappable)
{
    SAY("in rvalue sink");
}

void sink2(Swappable&)
{
    SAY("in non-const lvalue sink2");
}

void sink2(Swappable const&)
{
    SAY("in const lvalue sink2");
}

void sink3(Swappable&)
{
    SAY("in non-const lvalue sink3");
}

template <class T>
void tsink(T)
{
    SAY("in templated rvalue sink");
}

int main()
{
    BOOST_STATIC_ASSERT((boost::is_same<boost::swap_tag,
            boost::move_type<Swappable>::type>::value));

    SAY(" ------ test 1, direct init from rvalue ------- ");
    Swappable::expect_move();
    Swappable z2(source());

    SAY(" ------ test 2, copy init from rvalue ------- ");
    Swappable::expect_move();
    Swappable z4 = Swappable();

    SAY(" ------ test 3, copy init from lvalue ------- ");
    Swappable::expect_copy();
    Swappable z5 = z4;

    SAY(" ------ test 4, direct init from lvalue ------- ");
    Swappable::expect_copy();
    Swappable z6(z4);

    SAY(" ------ test 5, construct const ------- ");
    Swappable const z7;

    SAY(" ------ test 6, copy init from lvalue ------- ");
    Swappable::expect_copy();
    Swappable z8 = z7;

    SAY(" ------ test 7, direct init from lvalue ------- ");
    Swappable::expect_copy();
    Swappable z9(z7);

    SAY(" ------ test 8, pass rvalue by-value ------- ");
    Swappable::expect_move();
    sink(source());

    SAY(" ------ test 9, pass const rvalue by-value ------- ");
    Swappable::expect_move();
    sink(csource());

    SAY(" ------ test 10, pass rvalue by overloaded reference ------- ");
    Swappable::expect_move();
    sink2(source());

    SAY(" ------ test 11, pass const rvalue by overloaded reference ------- ");
    Swappable::expect_move();
    sink2(csource());

    SAY(" ------ test 13, pass lvalue by-value ------- ");
    Swappable::expect_copy();
    sink(z5);

    SAY(" ------ test 14, pass const lvalue by-value ------- ");
    Swappable::expect_copy();
    sink(z7);

    SAY(" ------ test 15, pass lvalue by-reference ------- ");
    Swappable::expect_copy();
    sink2(z4);

    SAY(" ------ test 16, pass const lvalue by const reference ------- ");
    Swappable::expect_copy();
    sink2(z7);

    SAY(" ------ test 17, pass rvalue by value to template param ------- ");
    Swappable::expect_move();
    tsink(source());

    SAY(" ------ test 18, direct initialize a const Swappable with an Swappable ------- ");
    Swappable::expect_move();
    typedef Swappable const YC;
    sink2(YC(Swappable()));

    SAY(" ------ test 19, assign from non-const lvalue ------- ");
    Swappable::expect_copy();
    z4 = z5;

    SAY(" ------ test 20, assign from const lvalue ------- ");
    Swappable::expect_copy();
    z4 = z7;

    SAY(" ------ test 21, assign from rvalue ------- ");
    Swappable::expect_copy();
    z4 = source();

    SAY(" ------ test 22, explicit move direct init from movable lvalue ------- ");
    BOOST_STATIC_ASSERT(boost::is_movable<Swappable>::value);
    Swappable::expect_move();
    Swappable z10(boost::move(z2));
    
    SAY(" ------ test 23, explicit move copy init from movable lvalue ------- ");
    Swappable::expect_move();
    Swappable z11 = boost::move(z9);
    
    SAY(" ------ test 24, move assign from movable lvalue ------- ");
    Swappable::expect_move();
    z10 = boost::move(z8);
    
    SAY(" ------ test 25, request move construct from non-movable lvalue ------- ");
    //BOOST_STATIC_ASSERT(!boost::is_movable<std::string>::value);
    std::string s1("hello");
    std::string s2(boost::move(s1));

    SAY("----- done, with " << Swappable::suboptimal_copies << " suboptimal copies -----");

    return Swappable::suboptimal_copies == 0 ? 0 : -1;
}
