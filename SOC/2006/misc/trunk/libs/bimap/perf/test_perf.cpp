// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This program is based (it is almost equal) in the performance testing program
// of the Boost.MultiIndex Library developped by Joaquin Lopez Muñoz.
// See it in /libs/multi_index/perf/test_perf.cpp

//***************************************************************************//
//                          In construction                                  //
//***************************************************************************//

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */

#include <algorithm>
#include <cassert>
#include <climits>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>

#include <boost/bimap/bimap.hpp>

using namespace std;
using namespace boost::multi_index;
using namespace boost::bimaps;

// Measurement harness by Andrew Koenig, extracted from companion code to
//   Stroustrup, B.: "Wrapping C++ Member Function Calls", The C++ Report,
//     June 2000, Vol 12/No 6.
// Original code retrievable at: http://www.research.att.com/~bs/wrap_code.cpp

// How many clock units does it take to interrogate the clock?

static double clock_overhead()
{
    clock_t k = clock(), start, limit;

    // Wait for the clock to tick

    do start = clock();
    while (start == k);

    // interrogate the clock until it has advanced at least a second
    // (for reasonable accuracy)

    limit = start + CLOCKS_PER_SEC;

    unsigned long r = 0;
    while ((k = clock()) < limit)
    {
        ++r;
    }

    return double(k - start) / r;
}

// We'd like the odds to be factor:1 that the result is
// within percent% of the median

const int factor = 10;
const int percent = 20;

// Measure a function (object) factor*2 times,
// appending the measurements to the second argument

template< class F >
void measure_aux(F f, vector<double>& mv)
{
    static double ovhd = clock_overhead();

    // Ensure we don't reallocate in mid-measurement

    mv.reserve(mv.size() + factor*2);

    // Wait for the clock to tick

    clock_t k = clock();
    clock_t start;

    do start = clock();
    while (start == k);

    // Do 2*factor measurements

    for (int i = 2 * factor; i; --i)
    {
        unsigned long count = 0, limit = 1, tcount = 0;

        // Original code used CLOCKS_PER_SEC/100

        const clock_t clocklimit = start + CLOCKS_PER_SEC/10;
        clock_t t;

        do
        {
            while (count < limit)
            {
                f();
                ++count;
            }
            limit *= 2;
            ++ tcount;

        } while ((t = clock()) < clocklimit);

        // Wait for the clock to tick again;

        clock_t t2;
        do ++tcount;
        while ((t2 = clock()) == t);

        // Append the measurement to the vector

        mv.push_back(((t2 - start) - (tcount * ovhd)) / count);

        // Establish a new starting point

        start = t2;
    }
}

// Returns the number of clock units per iteration
// With odds of factor:1, the measurement is within percent% of
// the value returned, which is also the median of all measurements.

template< class F >
double measure(F f)
{
    vector<double> mv;

    int n = 0; // iteration counter

    do
    {
        ++n;

        // Try 2*factor measurements

        measure_aux(f, mv);
        assert(mv.size() == 2*n*factor);

        // Compute the median.  We know the size is even, so we cheat.

        sort(mv.begin(), mv.end());
        double median = (mv[n*factor] + mv[n*factor-1])/2;

        // If the extrema are within threshold of the median, we're done

        if (mv[n] > (median * (100-percent))/100 &&
            mv[mv.size() - n - 1] < (median * (100+percent))/100)
            return median;

    } while (mv.size() < factor * 200);

    // Give up!

    clog << "Help!\n\n";
    exit(1);
}

// measure and compare two routines for several numbers of elements
// and also estimates relative memory consumption.

template< class BimapTest, class MultiIndexTest >
void run_tests(
    const char* title
    BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(MultiIndexTest)
    BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(BimapTest))
{
    cout << fixed << setprecision(2);
    cout << title << endl;
    unsigned int n = 1000;

    for(unsigned int i=0 ; i<3 ; ++i)
    {
        double bimap_t       = measure( BimapTest(n)  );
        double multi_index_t = measure( MultiIndexTest(n) );

        cout << "  10^" << i + 3 << " elmts: "
             << setw(6) << 100.0 * bimap_t / multi_index_t << "% "
             << "("
             << setw(6) << 1000.0 * bimap_t / CLOCKS_PER_SEC << " ms / "
             << setw(6) << 1000.0 * multi_index_t / CLOCKS_PER_SEC << " ms)"
             << endl;

        n *= 10;
    }
}

// insert

template< class MultiIndexType >
struct insert_multi_index_test
{
    insert_multi_index_test(unsigned int n) : number(n) {}
    void operator()()
    {
        for( unsigned int i = 0; i < number; ++i )
        {
            m.get<1>().insert( typename MultiIndexType::value_type(i,i) );
        }
    }
    private:
    MultiIndexType m;
    unsigned int number;
};

template< class BimapType >
struct insert_bimap_test
{
    insert_bimap_test(unsigned int n) : number(n) {}
    void operator()()
    {
        for( unsigned int i = 0; i < number; ++i )
        {
            b.right.insert( typename BimapType::right_value_type(i,i) );
        }
    }
    private:
    BimapType b;
    unsigned int number;
};

// find

template< class MultiIndexType >
struct find_multi_index_test
{
    find_multi_index_test(unsigned int n) : number(n) 
    {
        for( unsigned int i = 0; i < number; ++i )
        {
            m.get<1>().insert( typename MultiIndexType::value_type(i,i) );
        }
    }
    void operator()()
    {
        for( unsigned int i = 0; i < number; ++i )
        {
            m.get<1>().find(i);
        }
    }
    private:
    MultiIndexType m;
    unsigned int number;
};

template< class BimapType >
struct find_bimap_test
{
    find_bimap_test(unsigned int n) : number(n) 
    {
        for( unsigned int i = 0; i < number; ++i )
        {
            b.right.insert( typename BimapType::right_value_type(i,i) );
        }
    }
    void operator()()
    {
        for( unsigned int i = 0; i < number; ++i )
        {
            b.right.find(i);
        }
    }
    private:
    BimapType b;
    unsigned int number;
};


template< class BimapType, class MultiIndexType >
void compare_structures(const string & title)
{
    cout << title << endl << endl;

    run_tests<
        insert_bimap_test<BimapType>,
        insert_multi_index_test<MultiIndexType> 
    >("insert");

    run_tests<
        find_bimap_test<BimapType>,
        find_multi_index_test<MultiIndexType>
    >("find");
};

int main()
{
    // Set Set
    {
        typedef std::pair<int,int> pair_type;

        compare_structures<

            bimap<int,int>,

            multi_index_container<
                pair_type,
                indexed_by<
                    ordered_unique< member<pair_type,int,&pair_type::first > >,
                    ordered_unique< member<pair_type,int,&pair_type::second> >
                >
            >
        >("set_of - set_of");
    }

    return 0;
}
