// Copyright David Abrahams 2004. Distributed under the Boost
// Copyright Daniel James 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define DEBUG_MOVE
#include <boost/move/move.hpp>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <memory>
#include "say.hpp"
#include "./generic_tests.hpp"

//
// A noncopyable class to be stored in an auto_ptr.
//

class Noncopyable
{
    Noncopyable(Noncopyable const& rhs);
    Noncopyable& operator=(Noncopyable const& rhs);
 public: // "Ordinary" stuff
    Noncopyable(int t = 0) : tag(t), resource(++cnt)
    {
        SAY("Noncopyable(" << tag << ") #" << resource);
    }

    ~Noncopyable()
    {
        release();
    }
    
    static int copies;  // count the number of copies

    int tag;
    int resource;
    
 private: // helper functions

    void release()
    {
        BOOST_ASSERT(resource <= cnt); // check for garbage
        SAY("destroy(" << tag << ") #" << resource);
        resource = 0;
    }

 private:   // Data members
    static int cnt;  // count the number of resources
};

int Noncopyable::cnt;
int Noncopyable::copies;

typedef std::auto_ptr<Noncopyable> Ptr;

struct extract_auto_ptr_value {
    int operator()(Ptr const& x) {
        return x->resource;
    }
};

typedef test::generic_tests<Ptr, extract_auto_ptr_value> generic_ptr_tests;

//
// Some functions we can use to test the passing of Ys in and out of
// functions.
//
Ptr source(int tag)
{
    return Ptr(new Noncopyable(tag));
}

Ptr const csource(int tag)
{
    return Ptr(new Noncopyable(tag));
}

void sink(Ptr)
{
    SAY("in rvalue sink");
}

void sink2(Ptr&)
{
    SAY("in non-const lvalue sink2");
}

void sink2(Ptr const&)
{
    SAY("in const lvalue sink2");
}

void sink3(Ptr&)
{
    SAY("in non-const lvalue sink3");
}

template <class T>
void template_sink(T)
{
    SAY("in templated rvalue sink");
}

int main()
{
    generic_ptr_tests::move_tests();

    BOOST_STATIC_ASSERT((boost::is_same<boost::custom_move_tag,
            boost::move_type<Ptr>::type>::value));

    SAY(" ------ test 1, direct init from rvalue ------- ");
    Ptr z2(source(2));

    SAY(" ------ test 2, copy init from rvalue ------- ");
    Ptr z4 = Ptr(new Noncopyable(4));

    SAY(" ------ test 3, copy init from lvalue ------- ");
    Ptr z5 = z4;
    BOOST_ASSERT(z5->tag == 4 && !z4.get());

    SAY(" ------ test 4, direct init from lvalue ------- ");
    Ptr z6(z5);
    BOOST_ASSERT(z6->tag == 4 && !z5.get());


    SAY(" ------ test 5, construct const ------- ");
    Ptr const z7;

    //SAY(" ------ test 6, copy init from lvalue ------- ");
    //Ptr::expect_copy();
    //Ptr z8 = z7;

    //SAY(" ------ test 7, direct init from lvalue ------- ");
    //Ptr::expect_copy();
    //Ptr z9(z7);

    SAY(" ------ test 8, pass rvalue by-value ------- ");
    sink(source(8));

    //SAY(" ------ test 9, pass const rvalue by-value ------- ");
    //sink(csource(9));

    SAY(" ------ test 10, pass rvalue by overloaded reference ------- ");
    sink2(source(10));

    //SAY(" ------ test 11, pass const rvalue by overloaded reference ------- ");
    //sink2(csource(11));

    SAY(" ------ test 13, pass lvalue by-value ------- ");
    sink(z5);

    //SAY(" ------ test 14, pass const lvalue by-value ------- ");
    //sink(z7);

    SAY(" ------ test 15, pass lvalue by-reference ------- ");
    Ptr z15(new Noncopyable(15));
    sink2(z4);
    sink2(z15);

    //SAY(" ------ test 16, pass const lvalue by const reference ------- ");
    //sink2(z7);

    SAY(" ------ test 17, pass rvalue by value to template param ------- ");
    template_sink(source(17));

    //SAY(" ------ test 18, direct initialize a const Ptr with an Ptr ------- ");
    //typedef Ptr const YC;
    //sink2(YC(Ptr()));

    SAY(" ------ test 19, assign from non-const lvalue ------- ");
    z4 = z5;
    Ptr z19(new Noncopyable(19));
    z4 = z19;
    BOOST_ASSERT(z4->tag == 19 && !z19.get());

    //SAY(" ------ test 20, assign from const lvalue ------- ");
    //Ptr::expect_copy();
    //z4 = z7;

    SAY(" ------ test 21, assign from rvalue ------- ");
    z4 = source(21);

    SAY(" ------ test 22, explicit move direct init from movable lvalue ------- ");
    BOOST_STATIC_ASSERT(boost::is_movable<Ptr>::value);
    Ptr z10(boost::move(z2));
    
    SAY(" ------ test 23, explicit move copy init from movable lvalue ------- ");
    Ptr z23(new Noncopyable(23));
    Ptr z11 = boost::move(z23);
    
    SAY(" ------ test 24, move assign from movable lvalue ------- ");
    Ptr z24(new Noncopyable(24));
    z10 = boost::move(z24);
    BOOST_ASSERT(z10.get() && z10->tag == 24 && !z24.get());

    return 0;
}
