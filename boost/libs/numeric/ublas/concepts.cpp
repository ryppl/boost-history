#ifdef USE_MSVC

#pragma warning (disable: 4355)
#pragma warning (disable: 4503)
#pragma warning (disable: 4786)

#endif

#include <cassert>
#include <iostream>

#ifdef NUMERICS_PRIVATE
#include "config.h"
#include "vector.h"
#include "vector_sp.h"
#include "matrix.h"
#include "banded.h"
#include "triangular.h"
#include "symmetric.h"
#include "hermitean.h"
#include "matrix_sp.h"
#include "io.h"

#include "concepts.h"
#else
#include <boost/numeric/ublas/config.h>
#include <boost/numeric/ublas/vector.h>
#include <boost/numeric/ublas/vector_sp.h>
#include <boost/numeric/ublas/matrix.h>
#include <boost/numeric/ublas/banded.h>
#include <boost/numeric/ublas/triangular.h>
#include <boost/numeric/ublas/symmetric.h>
#include <boost/numeric/ublas/hermitean.h>
#include <boost/numeric/ublas/matrix_sp.h>
#include <boost/numeric/ublas/io.h>

#include <boost/numeric/ublas/concepts.h>
#endif

void test_norm () {
    std::complex<double> c (1, 1);
    std::cout << c << std::endl;
    std::cout << std::conj (c) << std::endl;
    std::cout << std::abs (c) << std::endl;
    std::cout << std::norm (c) << std::endl;
    std::cout << std::sqrt (c * std::conj (c)) << std::endl;
}

void test_storage () {
    try {
        int i;
        std::ostream_iterator<int> out (std::cout, " ");

        // Some vector storage tests
        std::vector<double> v1 (3), v2 (3);
        for (i = 0; i < 3; ++ i) 
            v2 [i] = i;
        v1 = v2;
        std::copy (v2.begin (), v2.end (), v1.begin ());
#if ! defined (USE_BCC) && ! defined (USE_ICC)
        std::copy (v2.rbegin (), v2.rend (), v1.rbegin ());
#endif
        std::swap (v1, v2);
#ifndef USE_GCC
        std::copy (v1.begin (), v1.end (), out); 
        std::cout << std::endl;
        std::copy (v2.begin (), v2.end (), out); 
        std::cout << std::endl;
#endif

        // Some unbounded array storage tests
        numerics::unbounded_array<double> ua1 (3), ua2 (3);
        for (i = 0; i < 3; ++ i) 
            ua2 [i] = i;
        ua1 = ua2;
        std::copy (ua2.begin (), ua2.end (), ua1.begin ());
#if ! defined (USE_BCC) && ! defined (USE_ICC)
        std::copy (ua2.rbegin (), ua2.rend (), ua1.rbegin ());
#endif
        std::swap (ua1, ua2);
#ifndef USE_GCC
        numerics::swap (ua1, ua2);
#endif
#ifndef USE_GCC
        std::copy (ua1.begin (), ua1.end (), out); 
        std::cout << std::endl;
        std::copy (ua2.begin (), ua2.end (), out); 
        std::cout << std::endl;
#endif

        // Some bounded array storage tests
        numerics::bounded_array<double, 3> ba1 (3), ba2 (3);
        for (i = 0; i < 3; ++ i) 
            ba2 [i] = i;
        ba1 = ba2;
        std::copy (ba2.begin (), ba2.end (), ba1.begin ());
#if ! defined (USE_BCC) && ! defined (USE_ICC)
        std::copy (ba2.rbegin (), ba2.rend (), ba1.rbegin ());
#endif
        std::swap (ba1, ba2);
#ifndef USE_GCC
        numerics::swap (ba1, ba2);
#endif
#ifndef USE_GCC
        std::copy (ba1.begin (), ba1.end (), out);
        std::cout << std::endl;
        std::copy (ba2.begin (), ba2.end (), out);
        std::cout << std::endl;
#endif
        // Some range tests
        numerics::range r (0, 3);
        std::copy (r.begin (), r.end (), out);
        std::cout << std::endl;
#if ! defined (USE_BCC) && ! defined (USE_GCC) && ! defined (USE_ICC)
        std::copy (r.rbegin (), r.rend (), out);
        std::cout << std::endl;
#endif

        // Some slice tests
        numerics::slice s (0, 2, 3);
        std::copy (s.begin (), s.end (), out);
        std::cout << std::endl;
#if ! defined (USE_BCC) && ! defined (USE_GCC) && ! defined (USE_ICC)
        std::copy (s.rbegin (), s.rend (), out);
        std::cout << std::endl;
#endif
    }
    catch (std::exception &e) {
        std::cout << e.what () << std::endl;
    }
    catch (...) {
        std::cout << "unknown exception" << std::endl;
    }
}

void test_sparse_storage () {
    try {
        int i;

        // Some map storage tests
        std::map<int, double> m1, m2;
//        m1.resize (3);
        for (i = 0; i < 3; ++ i) 
            m2 [i] = i;
        m1 = m2;
        m1.clear ();
        std::copy (m2.begin (), m2.end (), std::inserter (m1, m1.end ()));
        m1.clear ();
#if ! defined (USE_BCC) && ! defined (USE_ICC)
        std::copy (m2.rbegin (), m2.rend (), std::inserter (m1, m1.begin ()));
#endif
        std::swap (m1, m2);
        for (std::map<int, double>::const_iterator it_m1 = m1.begin (); it_m1 != m1.end (); ++ it_m1)
            std::cout << (*it_m1).first << " " << (*it_m1).second << " ";
        std::cout << std::endl;
        for (std::map<int, double>::const_iterator it_m2 = m2.begin (); it_m2 != m2.end (); ++ it_m2)
            std::cout << (*it_m2).first << " " << (*it_m2).second << " ";
        std::cout << std::endl;

        // Some compressed array storage tests
        numerics::compressed_array<int, double> ca1, ca2;
        ca1.resize (3);
        for (i = 0; i < 3; ++ i) 
            ca2 [i] = i;
        ca1 = ca2;
        ca1.clear ();
#if ! defined (USE_BCC) && ! defined (USE_ICC)
        std::copy (ca2.begin (), ca2.end (), std::inserter (ca1, ca1.end ()));
#endif
        ca1.clear ();
#if ! defined (USE_BCC) && ! defined (USE_ICC)
        std::copy (ca2.rbegin (), ca2.rend (), std::inserter (ca1, ca1.begin ()));
#endif
        std::swap (ca1, ca2);
#ifndef USE_GCC
        numerics::swap (ca1, ca2);
#endif
        for (numerics::compressed_array<int, double>::const_iterator it_ca1 = ca1.begin (); it_ca1 != ca1.end (); ++ it_ca1)
            std::cout << (*it_ca1).first << " " << (*it_ca1).second << " ";
        std::cout << std::endl;
        for (numerics::compressed_array<int, double>::const_iterator it_ca2 = ca2.begin (); it_ca2 != ca2.end (); ++ it_ca2)
            std::cout << (*it_ca2).first << " " << (*it_ca2).second << " ";
        std::cout << std::endl;
    }
    catch (std::exception &e) {
        std::cout << e.what () << std::endl;
    }
    catch (...) {
        std::cout << "unknown exception" << std::endl;
    }
}

void test_vector () {
    try {
        int i;
        std::ostream_iterator<int> out (std::cout, " ");

        // Some vector tests
        numerics::vector<double> v1 (3), v2 (3);
        for (i = 0; i < 3; ++ i) 
            v2 [i] = i;
        v1 = v2;
        std::copy (v2.begin (), v2.end (), v1.begin ());
        std::copy (v2.rbegin (), v2.rend (), v1.rbegin ());
        std::swap (v1, v2);
#ifndef USE_GCC
        numerics::swap (v1, v2);
        std::copy (v1.begin (), v1.end (), out); 
        std::cout << std::endl;
        std::copy (v2.begin (), v2.end (), out); 
        std::cout << std::endl;
#endif
    }
    catch (std::exception &e) {
        std::cout << e.what () << std::endl;
    }
    catch (...) {
        std::cout << "unknown exception" << std::endl;
    }
}

void test_unit_vector () {
#ifndef USE_GCC
    try {
        std::ostream_iterator<int> out (std::cout, " ");

        // Some unit vector tests
        numerics::unit_vector<double> cv1 (3, 0);
        std::cout << cv1 << std::endl;
        std::copy (cv1.begin (), cv1.end (), out); 
        std::cout << std::endl;
        numerics::unit_vector<double> cv2 (3, 1);
        std::cout << cv2 << std::endl;
        std::copy (cv2.begin (), cv2.end (), out); 
        std::cout << std::endl;
        numerics::unit_vector<double> cv3 (3, 2);
        std::cout << cv3 << std::endl;
        std::copy (cv3.begin (), cv3.end (), out); 
        std::cout << std::endl;
    }
    catch (std::exception &e) {
        std::cout << e.what () << std::endl;
    }
    catch (...) {
        std::cout << "unknown exception" << std::endl;
    }
#endif
}

void test_sparse_vector () {
    try {
        int i;
        std::ostream_iterator<int> out (std::cout, " ");

        // Some vector tests
        numerics::sparse_vector<double> v1 (3, 3), v2 (3, 3);
        for (i = 0; i < 3; ++ i) 
            v2 [i] = i;
        v1 = v2;
        std::copy (v2.begin (), v2.end (), v1.begin ());
        std::swap (v1, v2);
#ifndef USE_GCC
        numerics::swap (v1, v2);
        std::copy (v1.begin (), v1.end (), out); 
        std::cout << std::endl;
        std::copy (v2.begin (), v2.end (), out); 
        std::cout << std::endl;
#endif
    }
    catch (std::exception &e) {
        std::cout << e.what () << std::endl;
    }
    catch (...) {
        std::cout << "unknown exception" << std::endl;
    }
}

void test_matrix () {
    try {
        int i, j;
        std::ostream_iterator<int> out (std::cout, " ");

        // Some matrix tests
        numerics::matrix<double> m1 (3, 3), m2 (3, 3);
        for (i = 0; i < 3; ++ i) 
            for (j = 0; j < 3; ++ j) 
#ifdef NUMERICS_DEPRECATED
                m2 [i] [j] = i * j;
#else
                m2 (i, j) = i * j;
#endif
        m1 = m2;
        {
#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR) 
            numerics::matrix<double>::iterator1 it11 (m1.begin1 ());
            numerics::matrix<double>::const_iterator1 it12 (m2.begin1 ());
            while (it11 != m1.end1 () && it12 != m2.end1 ()) {
                std::copy (it12.begin (), it12.end (), it11.begin ());
                ++ it11, ++ it12;
            } 
            numerics::matrix<double>::iterator2 it21 (m1.begin2 ());
            numerics::matrix<double>::const_iterator2 it22 (m2.begin2 ());
            while (it21 != m1.end2 () && it22 != m2.end2 ()) {
                std::copy (it22.begin (), it22.end (), it21.begin ());
                ++ it21, ++ it22;
            } 
#endif
        }
        std::swap (m1, m2);
#ifndef USE_GCC
        numerics::swap (m1, m2);
        {
#if ! defined (NUMERICS_USE_CANONICAL_ITERATOR) && ! defined (NUMERICS_USE_INDEXED_ITERATOR) 
            numerics::matrix<double>::const_iterator1 it11 (m1.begin1 ());
            while (it11 != m1.end1 ()) {
                std::copy (it11.begin (), it11.end (), out);
                std::cout << std::endl;
                ++ it11;
            } 
            numerics::matrix<double>::const_iterator2 it21 (m1.begin2 ());
            while (it21 != m1.end2 ()) {
                std::copy (it21.begin (), it21.end (), out);
                std::cout << std::endl;
                ++ it21;
            }
#endif
        }
#endif
    }
    catch (std::exception &e) {
        std::cout << e.what () << std::endl;
    }
    catch (...) {
        std::cout << "unknown exception" << std::endl;
    }
}

void test_identity_matrix () {
#ifndef USE_GCC
#ifndef  NUMERICS_USE_CANONICAL_ITERATOR
    try {
        std::ostream_iterator<int> out (std::cout, " ");

        // Some identity matrix tests
        numerics::identity_matrix<double> im (3);
        std::cout << im << std::endl;
        numerics::identity_matrix<double>::const_iterator1 it1 (im.begin1 ());
        while (it1 != im.end1 ()) {
            std::copy (it1.begin (), it1.end (), out);
            std::cout << std::endl;
            ++ it1;
        } 
        numerics::identity_matrix<double>::const_iterator2 it2 (im.begin2 ());
        while (it2 != im.end2 ()) {
            std::copy (it2.begin (), it2.end (), out);
            std::cout << std::endl;
            ++ it2;
        } 
    }
    catch (std::exception &e) {
        std::cout << e.what () << std::endl;
    }
    catch (...) {
        std::cout << "unknown exception" << std::endl;
    }
#endif
#endif
}

void test_sparse_matrix () {
#ifndef  NUMERICS_USE_CANONICAL_ITERATOR
    try {
        int i, j;
        std::ostream_iterator<int> out (std::cout, " ");

        // Some matrix tests
        numerics::sparse_matrix<double> m1 (3, 3, 3 * 3), m2 (3, 3, 3 * 3);
        for (i = 0; i < 3; ++ i) 
            for (j = 0; j < 3; ++ j) 
#ifdef NUMERICS_DEPRECATED
                m2 [i] [j] = i * j;
#else
                m2 (i, j) = i * j;
#endif
        m1 = m2;
        {
            numerics::sparse_matrix<double>::iterator1 it11 (m1.begin1 ());
            numerics::sparse_matrix<double>::const_iterator1 it12 (m2.begin1 ());
            while (it11 != m1.end1 () && it12 != m2.end1 ()) {
                std::copy (it12.begin (), it12.end (), it11.begin ());
                ++ it11, ++ it12;
            } 
            numerics::sparse_matrix<double>::iterator2 it21 (m1.begin2 ());
            numerics::sparse_matrix<double>::const_iterator2 it22 (m2.begin2 ());
            while (it21 != m1.end2 () && it22 != m2.end2 ()) {
                std::copy (it22.begin (), it22.end (), it21.begin ());
                ++ it21, ++ it22;
            } 
        }
        std::swap (m1, m2);
#ifndef USE_GCC
        numerics::swap (m1, m2);
        {
            numerics::sparse_matrix<double>::const_iterator1 it11 (m1.begin1 ());
            while (it11 != m1.end1 ()) {
                std::copy (it11.begin (), it11.end (), out);
                std::cout << std::endl;
                ++ it11;
            } 
            numerics::sparse_matrix<double>::const_iterator2 it21 (m1.begin2 ());
            while (it21 != m1.end2 ()) {
                std::copy (it21.begin (), it21.end (), out);
                std::cout << std::endl;
                ++ it21;
            } 
        }
#endif
    }
    catch (std::exception &e) {
        std::cout << e.what () << std::endl;
    }
    catch (...) {
        std::cout << "unknown exception" << std::endl;
    }
#endif
}

void test_io () {
    std::cout << std::complex<double> (1, 1) << std::endl;
    numerics::vector<std::complex<double> > v (2);
    v (0) = std::complex<double > (1, 0);
    v (1) = std::complex<double > (0, 1);
    std::cout << v << std::endl;
    std::cin >> v;
    std::cout << v << std::endl;
    numerics::matrix<std::complex<double> > m (2, 2);
    m (0, 0) = std::complex<double > (0, 0);
    m (0, 1) = std::complex<double > (0, 1);
    m (1, 0) = std::complex<double > (1, 0);
    m (1, 1) = std::complex<double > (1, 1);
    std::cout << m << std::endl;
    std::cin >> m;
    std::cout << m << std::endl;
}

void test_conversion () {
    numerics::vector<double> dv (1);
    numerics::vector<float> fv (1);
    dv = fv;
    fv = dv;
    numerics::matrix<double> dm (1, 1);
    numerics::matrix<float> fm (1, 1);
    dm = fm;
    fm = dm;
}

void test_sparse_prod () {
    numerics::sparse_matrix<double, numerics::column_major> mc (3, 3, 9), mc1 (3, 3, 9), mc2 (3, 3, 9);
    numerics::sparse_matrix<double, numerics::row_major> mr (3, 3, 9), mr1 (3, 3, 9), mr2 (3, 3, 9);
    numerics::sparse_vector<double> v1 (3, 3), v2 (3, 3);

#ifdef USE_MSVC
    v2 = numerics::prod (mc, v1, numerics::sparse_column_major_tag ());
#else
    v2 = numerics::prod (mc, v1);
#endif
    std::cout << v2 << std::endl;
#ifdef USE_MSVC
    v2 = numerics::prec_prod (mc, v1, numerics::sparse_column_major_tag ());
#else
    v2 = numerics::prec_prod (mc, v1);
#endif
    std::cout << v2 << std::endl;

#ifdef USE_MSVC
    v2 = numerics::prod (v1, mr, numerics::sparse_row_major_tag ());
#else
    v2 = numerics::prod (v1, mr);
#endif
    std::cout << v2 << std::endl;
#ifdef USE_MSVC
    v2 = numerics::prec_prod (v1, mr, numerics::sparse_row_major_tag ());
#else
    v2 = numerics::prec_prod (v1, mr);
#endif
    std::cout << v2 << std::endl;

#ifdef USE_MSVC
    mr = numerics::prod (mc1, mc2, numerics::sparse_column_major_tag ());
#else
    mr = numerics::prod (mc1, mc2);
#endif
    std::cout << mr << std::endl;
#ifdef USE_MSVC
    mr = numerics::prec_prod (mc1, mc2, numerics::sparse_column_major_tag ());
#else
    mr = numerics::prec_prod (mc1, mc2);
#endif
    std::cout << mr << std::endl;

#ifdef USE_MSVC
    mc = numerics::prod (mr1, mr2, numerics::sparse_row_major_tag ());
#else
    mc = numerics::prod (mr1, mr2);
#endif
    std::cout << mc << std::endl;
#ifdef USE_MSVC
    mc = numerics::prec_prod (mr1, mr2, numerics::sparse_row_major_tag ());
#else
    mc = numerics::prec_prod (mr1, mr2);
#endif
    std::cout << mc << std::endl;
}

void test_diagonal () {
    int imax = 2;
    int jmax = 3;
    int i = 0, j = 0;
    std::cout << i << " " << j << std::endl;
    do {
        if (i < imax) {
            ++ i;
            std::cout << i << " " << j << std::endl;
            while (i > 0 && j < jmax) {
                ++ j;
                -- i;
                std::cout << i << " " << j << std::endl;
            }
        }
        if (j < jmax) {
            ++ j;
            std::cout << i << " " << j << std::endl;
            while (j > 0 && i < imax) {
                ++ i;
                -- j;
                std::cout << i << " " << j << std::endl;
            }
        }
    } while (i != imax || j != jmax);
}

void test_iterator () {
#ifndef NUMERICS_USE_CANONICAL_ITERATOR
    numerics::unbounded_array<double> a (3);
    {
        for (numerics::unbounded_array<double>::iterator ita = a.begin ();
                ita != a.end ();
                    ++ ita) {
            *ita = 0;
        }
    }
    {
        for (numerics::unbounded_array<double>::const_iterator ita = a.begin ();
                ita != a.end ();
                    ++ ita) {
            std::cout << *ita << std::endl;
        }
    }
    numerics::vector<double> v (3);
    {
        for (numerics::vector<double>::iterator itv = v.begin ();
                itv != v.end ();
                    ++ itv) {
            *itv = 0;
        }
    }
    {
#if defined (USE_MSVC) || defined (USE_ICC)
        const numerics::vector<double> &cv = v;
        for (numerics::vector<double>::const_iterator itv = cv.begin ();
                itv != cv.end ();
                    ++ itv) {
            std::cout << *itv << std::endl;
        }
#else
        for (numerics::vector<double>::const_iterator itv = v.begin ();
                itv != v.end ();
                    ++ itv) {
            std::cout << *itv << std::endl;
        }
#endif
    }
    {
        for (numerics::vector<double>::reverse_iterator itv = v.rbegin ();
                itv != v.rend ();
                    ++ itv) {
            *itv = 0;
        }
    }
    {
#ifndef NUMERICS_LWG_ISSUE_280
        const numerics::vector<double> &cv = v;
        for (numerics::vector<double>::const_reverse_iterator itv = cv.rbegin ();
                itv != cv.rend ();
                    ++ itv) {
#else
        for (numerics::vector<double>::const_reverse_iterator itv = v.rbegin ();
                itv != v.rend ();
                    ++ itv) {
#endif
            std::cout << *itv << std::endl;
        }
    }
    numerics::matrix<double> m (3, 3);
    {
        for (numerics::matrix<double>::iterator1 itmr = m.begin1 ();
                itmr != m.end1 ();
                    ++ itmr) {
             for (numerics::matrix<double>::iterator2 itmc = itmr.begin ();
                    itmc != itmr.end ();
                        ++ itmc) {
                *itmc = 0;
            }
        }
    }
    {
#if defined (USE_MSVC) || defined (USE_ICC)
        const numerics::matrix<double> &cm = m;
        for (numerics::matrix<double>::const_iterator1 itmr = cm.begin1 ();
                itmr != cm.end1 ();
                    ++ itmr) {
             for (numerics::matrix<double>::const_iterator2 itmc = itmr.begin ();
                    itmc != itmr.end ();
                        ++ itmc) {
                            std::cout << *itmc << std::endl;
            }
        }
#else
        for (numerics::matrix<double>::const_iterator1 itmr = m.begin1 ();
                itmr != m.end1 ();
                    ++ itmr) {
             for (numerics::matrix<double>::const_iterator2 itmc = itmr.begin ();
                    itmc != itmr.end ();
                        ++ itmc) {
                            std::cout << *itmc << std::endl;
            }
        }
#endif
    }
    {
        for (numerics::matrix<double>::reverse_iterator1 itmr = m.rbegin1 ();
                itmr != m.rend1 ();
                    ++ itmr) {
             for (numerics::matrix<double>::reverse_iterator2 itmc = itmr.rbegin ();
                    itmc != itmr.rend ();
                        ++ itmc) {
                *itmc = 0;
            }
        }
    }
    {
#ifndef NUMERICS_LWG_ISSUE_280
        const numerics::matrix<double> &cm = m;
        for (numerics::matrix<double>::const_reverse_iterator1 itmr = cm.rbegin1 ();
                itmr != cm.rend1 ();
                    ++ itmr) {
             for (numerics::matrix<double>::const_reverse_iterator2 itmc = itmr.rbegin ();
                    itmc != itmr.rend ();
                        ++ itmc) {
                            std::cout << *itmc << std::endl;
            }
        }
#else
         for (numerics::matrix<double>::const_reverse_iterator1 itmr = m.rbegin1 ();
                 itmr != m.rend1 ();
                     ++ itmr) {
              for (numerics::matrix<double>::const_reverse_iterator2 itmc = itmr.rbegin ();
                     itmc != itmr.rend ();
                         ++ itmc) {
                             std::cout << *itmc << std::endl;
             }
         }
#endif
    }
#endif
}

void test_project () {
#ifndef USE_GCC
     numerics::vector<double> v (1), v1 (1), v2 (1);
     v (0) = v1 (0) = v2 (0) = 0;
	 std::cout << numerics::project (numerics::project (v, numerics::range (0, 1)), numerics::range (0, 1)) << std::endl;
     std::cout << (v1 + v2) (0) << std::endl;

     numerics::matrix<double> m (1, 1), m1 (1, 1), m2 (1, 1);
     m1 (0, 0) = m2 (0, 0) = 0;
	 std::cout << numerics::project (numerics::project (m, numerics::range (0, 1), numerics::range (0, 1)), numerics::range (0, 1), numerics::range (0, 1)) << std::endl;
     std::cout << (m1 + m2) (0, 0) << std::endl;
#endif
}

void test_const_project () {
     numerics::vector<double> v1 (1);
#ifdef NUMERICS_DEPRECATED
     numerics::vector_range<numerics::vector<double> > vr1 (v1.project (0, 1));
#else
     numerics::vector_range<numerics::vector<double> > vr1 (numerics::project (v1, numerics::range (0, 1)));
#endif
     vr1 (0) = 0;
     std::cout << vr1 (0) << std::endl;

     const numerics::vector<double> v2 (v1);
#ifdef NUMERICS_DEPRECATED
     numerics::vector_range<const numerics::vector<double> > vr2 (v2.project (0, 1)); // Compile time error: 2 overloads have no legal conversion for 'this' pointer
#else
     numerics::vector_range<const numerics::vector<double> > vr2 (numerics::project (v2, numerics::range (0, 1))); // Compile time error: 2 overloads have no legal conversion for 'this' pointer
#endif
     // vr2 (0) = 0; // Conversion loses qualifiers
     // std::cout << vr2 (0) << std::endl; // Conversion loses qualifiers

     const numerics::vector<double> v3 (v1);
#ifdef NUMERICS_DEPRECATED
     const numerics::vector_range<const numerics::vector<double> > vr3 (v3.project (0, 1));
#else
     const numerics::vector_range<const numerics::vector<double> > vr3 (numerics::project (v3, numerics::range (0, 1)));
#endif
     // vr3 (0) = 0; // Compile time error: left operand must be l-value
     std::cout << vr3 (0) << std::endl;
}

#ifdef NUMERICS_DEPRECATED

void test_project_equivalences () {
    numerics::vector<double> v;
    numerics::vector_range<numerics::vector<double> > vr;
    numerics::vector_expression_range<numerics::vector_const_reference<numerics::vector<double> > > ver;

    numerics::vector_unary<numerics::vector_const_reference<numerics::vector<double> >, numerics::scalar_identity<double> > vu;
    numerics::vector_expression_range<numerics::vector_unary<numerics::vector_const_reference<numerics::vector<double> >, numerics::scalar_identity<double> > > vuer;
    numerics::vector_range<numerics::vector_unary<numerics::vector_const_reference<numerics::vector<double> >, numerics::scalar_identity<double> > > vur;

    numerics::matrix<double> m;
    numerics::matrix_row<numerics::matrix<double> > mr;
    numerics::matrix_expression_row<numerics::matrix_const_reference<numerics::matrix<double> > > mer;
    numerics::matrix_column<numerics::matrix<double> > mc;
    numerics::matrix_expression_column<numerics::matrix_const_reference<numerics::matrix<double> > > mec;

    numerics::matrix_unary1<numerics::matrix_const_reference<numerics::matrix<double> >, numerics::scalar_identity<double> > mu;
    numerics::matrix_expression_row<numerics::matrix_unary1<numerics::matrix_const_reference<numerics::matrix<double> >, numerics::scalar_identity<double> > > muer;
    numerics::matrix_row<numerics::matrix_unary1<numerics::matrix_const_reference<numerics::matrix<double> >, numerics::scalar_identity<double> > > mur;
    numerics::matrix_expression_column<numerics::matrix_unary1<numerics::matrix_const_reference<numerics::matrix<double> >, numerics::scalar_identity<double> > > muec;
    numerics::matrix_column<numerics::matrix_unary1<numerics::matrix_const_reference<numerics::matrix<double> >, numerics::scalar_identity<double> > > muc;
}

#endif

int main () {
    test_storage ();
    test_sparse_storage ();
    test_vector ();
    test_unit_vector ();
    test_sparse_vector ();
    test_matrix ();
    test_identity_matrix ();
    test_sparse_matrix ();
    test_iterator ();
    test_sparse_prod ();
    test_project ();
    test_const_project ();
#ifdef NUMERICS_DEPRECATED
    test_project_equivalences ();
#endif
    numerics::concept_checks ();
    return 0;
}

















