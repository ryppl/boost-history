// Boost.Bimap
//
// (C) Copyright Matias Capeletto 2006
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/test for the library home page.

// Boost.Test
#include <boost/test/minimal.hpp>

// Boost.MPL
#include <boost/mpl/list.hpp>
#include <boost/type_traits/is_same.hpp>

// Boost.Bimap
#include <boost/bimap/relation/detail/mutant.hpp>

using namespace boost::bimap::relation::detail;

// The mutant idiom is standard if only POD types are used.

typedef double  type_a;
typedef int     type_b;

const type_a value_a = 1.4;
const type_b value_b = 3;

struct Data
{
    type_a a;
    type_b b;
};

struct StdPairView
{
    typedef type_a first_type;
    typedef type_b second_type;
    type_a first;
    type_b second;
};

struct ReverseStdPairView
{
    typedef type_a second_type;
    typedef type_b first_type;
    type_a second;
    type_b first;
};


struct MutantData : public can_mutate_in< boost::mpl::list< StdPairView, ReverseStdPairView > >
{
    MutantData(type_a ap, type_b bp) : a(ap), b(bp) {}
    type_a a;
    type_b b;
};


void test_mutant_basic()
{

    // mutant test
    {
        typedef mutant< Data, boost::mpl::list< StdPairView, ReverseStdPairView > > mutant_pair;

        BOOST_CHECK( sizeof( mutant_pair ) == sizeof( StdPairView ) );

        mutant_pair m;
        m.a = value_a;
        m.b = value_b;

        BOOST_CHECK( mutate<StdPairView>(m).first  == value_a );
        BOOST_CHECK( mutate<StdPairView>(m).second == value_b );
        BOOST_CHECK( mutate<ReverseStdPairView>(m).first  == value_b );
        BOOST_CHECK( mutate<ReverseStdPairView>(m).second == value_a );

        ReverseStdPairView & rpair = mutate<ReverseStdPairView>(m);
        rpair.first = value_b;
        rpair.second = value_a;

        BOOST_CHECK( mutate<StdPairView>(m).first  == value_a );
        BOOST_CHECK( mutate<StdPairView>(m).second == value_b );

        BOOST_CHECK( &mutate<StdPairView>(m).first  == &m.a );
        BOOST_CHECK( &mutate<StdPairView>(m).second == &m.b );
    }

    // can_mutate test
    {
        MutantData m(value_a,value_b);

        BOOST_CHECK( sizeof( MutantData ) == sizeof( StdPairView ) );

        BOOST_CHECK( mutate<StdPairView>(m).first  == value_a );
        BOOST_CHECK( mutate<StdPairView>(m).second == value_b );
        BOOST_CHECK( mutate<ReverseStdPairView>(m).first  == value_b );
        BOOST_CHECK( mutate<ReverseStdPairView>(m).second == value_a );

        ReverseStdPairView & rpair = mutate<ReverseStdPairView>(m);
        rpair.first = value_b;
        rpair.second = value_a;

        BOOST_CHECK( mutate<StdPairView>(m).first  == value_a );
        BOOST_CHECK( mutate<StdPairView>(m).second == value_b );

        BOOST_CHECK( &mutate<StdPairView>(m).first  == &m.a );
        BOOST_CHECK( &mutate<StdPairView>(m).second == &m.b );
    }
}

int test_main( int, char* [] )
{
    test_mutant_basic();
    return 0;
}
