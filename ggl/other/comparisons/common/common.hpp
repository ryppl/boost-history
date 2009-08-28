// Generic Geometry Library - COMPARISONS
//
// Copyright Barend Gehrels, 2008, 2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef COMPARE_COMMON_HPP_INCLUDED
#define COMPARE_COMMON_HPP_INCLUDED

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <boost/timer.hpp>
#include <boost/math/constants/constants.hpp>

//#define OVERLAY_SYMDIFF


/*
To put results into a (e.g. MySQL) database:

drop table if exists measurements;
create table measurements(id integer auto_increment primary key,
    lib varchar(16), comp varchar(16), dset varchar(16), alg varchar(16), val double, cnt integer, sec double);

To get the results, do something like:
select lib,min(cnt) as c,round(min(val),5) as a,round(min(sec),2) as s from measurements
group by lib
order by s


To get the results in the form which can be pasted into the Wiki-page:
select alg,concat('||', lib, '||',round(min(val),5),'||',round(min(sec),3),'||') from measurements
group by alg,lib
order by alg,lib


*/


//#define NLP4 // if defined, DUTCH dataset (private not for sale) else PUBLIC US COUNTY dataset

// BEGIN DEFINES for testing ggl-sorting only
//#define USE_SMOOTH_SORT
//#define USE_MERGE_SORT
//#define CONVEX_HULL_USE_INSERTION
#if defined(USE_MERGE_SORT)
static const int thread_count = 1;
#endif
// END


// #define OUTPUT_TEXT // if defined, outputs in text format. If not, outputs SQL insert statements



namespace compare
{

    std::string g_name;
    std::string g_compiler;
    std::string g_dataset;

    std::string filename(int argc, char** argv)
    {
        return argc > 1 ? argv[1] :
#ifdef NLP4
         "c:/data/spatial/shape/nl/nlp4_r98"
#else
        // Can be downloaded for free from http://www.weather.gov/geodata/catalog/county/html/county100k.htm
         "c:/data/spatial/shape/c100000jan2902"
#endif
    ;
    }

    std::string fieldname()
    {
    // todo: argc,argv
#ifdef NLP4
        return "pc4nr";
#else
        return "fips";
#endif
    }


#ifdef NLP4
    const int AREA_COUNT = 1000;
    const double AREA_DIVISOR = 1000000.0; // km^2

    const double LENGTH_DIVISOR = 1000.0;

    const double SIMPLIFY_DISTANCE = 91.7; // results in simplified length of 99%, vertex reduction of ca. 50%

    const int CENTROID_COUNT = 1000;

    const double CLIP_DISTANCE = 10.0;

    const int OVERLAY_COUNT = 10;
    const int CLIP_COUNT = 10;

    const double INTEGER_FACTOR = 100.0;

#else
    const int AREA_COUNT = 10;

    const double AREA_DIVISOR = 1.0;
    const double LENGTH_DIVISOR = 1.0;

    const double SIMPLIFY_DISTANCE = 0.0008975; // results in simplified length of 99%, vertex reduction of ca. 10%

    const int CENTROID_COUNT = 10;


    const double CLIP_DISTANCE = 0.1;

    const int OVERLAY_COUNT = 1;
    const int CLIP_COUNT = 1;

    const double INTEGER_FACTOR = 10000.0;

#endif


//#define OVERLAY_UNION

    const int OVERLAY_ELLIPSE_COUNT = 101;
    const double delta = boost::math::constants::pi<double>() * 2.0 / (compare::OVERLAY_ELLIPSE_COUNT - 1);
    const double OVERLAY_ELLIPSE_FACTOR1 = 1.1; // 1.1
    const double OVERLAY_ELLIPSE_FACTOR2 = 0.2; // 0.2
    const double CLIP_FACTOR = 0.9;

    const bool MEASURE_AREA = true;
    const bool MEASURE_CENTROID = true;
    const bool MEASURE_CLIP = true;
    const bool MEASURE_CONVEX_HULL = true;
    const bool MEASURE_OVERLAY = true;
    const bool MEASURE_SIMPLIFY = true;
    const bool MEASURE_TOUCH = false; // currently only for GEOS, not further worked out
    const bool MEASURE_WITHIN = true;

    // Variables to check the results
    const bool CLIP_AREA = true;
    const bool HULL_AREA = true;
    const bool OVERLAY_AREA = true;
    const bool SIMPLIFY_LENGTH = true;



    // Wait a while, after reading shapefiles/generating ellipses, etc
    // until all buffers/memory allocation/swapping is done.
    inline void wait(bool first = true)
    {
        int const s = first ? 3 : 1;
        boost::timer t;
        while (t.elapsed() < s) ;
    }



    template <typename T1, typename T2>
    inline void report_common(double s, int n, const std::string& algorithm, T1 value1, T2 value2 = 0)
    {
        #ifdef OUTPUT_TEXT
        std::cout << algorithm << " " << n
            << " total: " << value1
            << " other: " << value2
            << " time: "<< s << "s" << std::endl;
        #else

        std::cout << "insert into measurements(lib, comp, dset, alg, val, cnt, sec)"
            << " values("
            << "'" << g_name << "'"
            << ", '" << g_compiler << "'"
            << ", '" << g_dataset << "'"
            << ", '" << algorithm << "'"
            << ", " << value1
            << ", " << n
            << ", " << s
            << ");" << std::endl;
        #endif


        wait(false);
    }

    inline void report_area(const boost::timer& t, int n, double area)
    {
        double s = t.elapsed();
        area /= double(compare::AREA_COUNT);
        area /= compare::AREA_DIVISOR;
        report_common(s, n, "AREA", area, 0);
    }

    inline void report_centroid(const boost::timer& t, int n, double sum_x, double sum_y)
    {
        double s = t.elapsed();
        sum_x /= double(compare::CENTROID_COUNT);
        sum_y /= double(compare::CENTROID_COUNT);
        sum_x /= double(n);
        sum_y /= double(n);
        report_common(s, n, "CENTROID", sum_x, sum_y);
    }

    inline void report_hull(const boost::timer& t, int n, double area)
    {
        double s = t.elapsed();
        report_common(s, n, "HULL", area / compare::AREA_DIVISOR, 0);
    }
    inline void report_simplify(const boost::timer& t, int n, double length1, double length2, int count1, int count2)
    {
        double s = t.elapsed();
        length1 /= compare::LENGTH_DIVISOR;
        length2 /= compare::LENGTH_DIVISOR;

#ifdef OUTPUT_TEXT
        std::cout
            << "original : length=" << length1 << ", count=" << count1
            << " -> "
            << "simplified : length=" << length2 << ", count=" << count2
            << std::endl;
#endif

        report_common(s, n, "SIMPLIFY", length2, length1);
    }

    inline void report_within(const boost::timer& t, int n, int count, int count_boundary)
    {
        double s = t.elapsed();
            //<< " " << count << " (" << count_boundary << ")"
        report_common(s, n, "WITHIN", count, count_boundary);
    }

    inline void report_touch(const boost::timer& t, int n, int count, int count_box)
    {
        double s = t.elapsed();
            //<< " " << count << " (" << count_boundary << ")"
        report_common(s, n, "TOUCH", count, count_box);
    }

    inline void report_overlay(const boost::timer& t, int n, double area1, double area2)
    {
        double s = t.elapsed();

        area1 /= compare::OVERLAY_COUNT;
        area2 /= compare::OVERLAY_COUNT;
        area1 /= compare::AREA_DIVISOR;
        area2 /= compare::AREA_DIVISOR;
        report_common(s, OVERLAY_ELLIPSE_COUNT,
#if defined(OVERLAY_UNION)
            "UNION"
#elif defined(OVERLAY_SYMDIFF)
            "SYM_DIFFERENCE"
#else
            "INTERSECTION"
#endif
            , area2, area1);
    }

    inline void report_clip(const boost::timer& t, int n, double area1, double area2)
    {
        double s = t.elapsed();

        area1 /= compare::CLIP_COUNT;
        area2 /= compare::CLIP_COUNT;
        area1 /= compare::AREA_DIVISOR;
        area2 /= compare::AREA_DIVISOR;
        report_common(s, n, "CLIP", area2, area1);
    }



    inline void debug_within(int i, int count)
    {
        std::cout << ".";
        if ((i + 1) % 70 == 0)
        {
            std::cout << count << std::endl;
        }
    }


    void version_info(const std::string& name)
    {
        g_name = name;

        #ifdef _STLPORT_VERSION
            std::cout << "STLPORT " << _STLPORT_VERSION << std::endl;
        #endif

        std::ostringstream out;

        #ifdef _MSC_VER
            out << "MSC " << _MSC_VER;
        #endif

        #if defined(__GNUC__)
            out << "GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__;
        #endif

        g_compiler = out.str();

        #ifdef NLP4
            g_dataset = "NLP4";
        #else
            g_dataset = "US Counties";
        #endif

        #ifdef OUTPUT_TEXT
        std::cout << g_compiler << std::endl;
        std::cout << g_dataset << std::endl;
        #endif


        #if defined(CONVEX_HULL_USE_INSERTION)
            std::cout << "insertions" << std::endl;
        #elif defined(USE_SMOOTH_SORT)
            std::cout << "smoothsort::sort" << std::endl;
        #elif defined(USE_MERGE_SORT)
            std::cout << "MergeSort::sort (" << thread_count << " thread"
                << (thread_count == 1 ? "" : "s") << ")" << std::endl;
        #else
            //std::cout << "std::sort" << std::endl;
        #endif
        std::cout << std::setprecision(16) << std::endl;
    }
}


#endif // COMPARE_COMMON_HPP_INCLUDED

