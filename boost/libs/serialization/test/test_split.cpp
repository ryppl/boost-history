/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_split.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include <fstream>
#include <cstdio>

#include <boost/serialization/split_member.hpp>
#include <boost/serialization/split_free.hpp>

#include "test_tools.hpp"

class A
{
    friend class boost::serialization::access;
    template<class Archive>
    void save(
        Archive & /* ar */, 
        const unsigned int /* file_version */
    ) const {
        ++(const_cast<A &>(*this).count);
    }
    template<class Archive>
    void load(
        Archive & /* ar */, 
        const unsigned int /* file_version */
    ){
        --count;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    int count;
public:
    A() : count(0) {}
    ~A() {
        BOOST_CHECK(0 == count);
    }
};

class B
{
    friend class boost::serialization::access;
    template<class Archive>
    void save(
        Archive & /* ar */, 
        const unsigned int /* file_version */
    ) const {
        ++(const_cast<B &>(*this).count);
    }
    template<class Archive>
    void load(
        Archive & /* ar */, 
        const unsigned int /* file_version */
    ){
        --count;
    }
    int count;
public:
    B() : count(0) {}
    ~B() {
        BOOST_CHECK(0 == count);
    }
};

namespace boost { namespace serialization {

template<class Archive>
void serialize(
    Archive & ar,
    B & b,
    const unsigned int file_version
){ 
    split_member(ar, b, file_version);
} 

}} // namespace serialization // boost

class C
{
public:
    int count;
    C() : count(0) {}
    ~C() {
        BOOST_CHECK(0 == count);
    }
};

namespace boost { namespace serialization {

template<class Archive>
void save(
    Archive & ar /* ar */, 
    const C & c,
    const unsigned int file_version /* file_version */
){
    ++(const_cast<C &>(c).count);
}

template<class Archive>
void load(
    Archive & /* ar */, 
    C & c,
    const unsigned int /* file_version */
){
    --c.count;
}

} // namespace serialization 
} // namespace boost

BOOST_SERIALIZATION_SPLIT_FREE(C)

void out(const char *testfile, A & a, B & b, C & c)
{
    test_ostream os(testfile, TEST_STREAM_FLAGS);
    test_oarchive oa(os);
    oa << BOOST_SERIALIZATION_NVP(a);
    oa << BOOST_SERIALIZATION_NVP(b);
    oa << BOOST_SERIALIZATION_NVP(c);
}

void in(const char *testfile, A & a, B & b, C & c)
{
    test_istream is(testfile, TEST_STREAM_FLAGS);
    test_iarchive ia(is);
    ia >> BOOST_SERIALIZATION_NVP(a);
    ia >> BOOST_SERIALIZATION_NVP(b);
    ia >> BOOST_SERIALIZATION_NVP(c);
}

int
test_main( int /* argc */, char* /* argv */[] )
{
    const char * testfile = tmpnam(NULL);
    BOOST_REQUIRE(NULL != testfile);

    A a;
    B b;
    C c;

    out(testfile, a, b, c);
    in(testfile, a, b, c);
	std::remove(testfile);
    return boost::exit_success;
}

// EOF
