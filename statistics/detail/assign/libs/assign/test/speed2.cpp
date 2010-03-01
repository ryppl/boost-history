//////////////////////////////////////////////////////////////////////////////
// test::speed.cpp                                                          //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <string>
#include <boost/progress.hpp>
#include <boost/bind.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assign/auto_size/ref_list_of.hpp>

#define BOOST_ASSIGN_CSV_SIZE 90
#include <boost/assign/auto_size/ref_list_of_csv.hpp>
#undef BOOST_ASSIGN_CSV_SIZE

#include <libs/assign/test/speed_common.h>
#include <libs/assign/test/speed.h>

void test_speed2(std::ostream& os)
{
	os << "-> test_speed : " << std::endl;
    
    const int m = 100;							// max size of vec
    const int n = 100 * 1000 * 1000;			// number of loops

	os << "Each arg is a vec of max size  : " << m <<  std::endl;
	os << "Number of repeated function calls : " << n <<  std::endl;
    
    std::vector<int>    
        a00 = rand_vec(m), 
        a01 = rand_vec(m), 
        a02 = rand_vec(m), 
        a03 = rand_vec(m), 
        a04 = rand_vec(m), 
        a05 = rand_vec(m), 
        a06 = rand_vec(m), 
        a07 = rand_vec(m), 
        a08 = rand_vec(m), 
        a09 = rand_vec(m), 
        a10 = rand_vec(m), 
        a11 = rand_vec(m), 
        a12 = rand_vec(m), 
        a13 = rand_vec(m), 
        a14 = rand_vec(m), 
        a15 = rand_vec(m), 
        a16 = rand_vec(m), 
        a17 = rand_vec(m), 
        a18 = rand_vec(m), 
        a19 = rand_vec(m), 
        a20 = rand_vec(m), 
        a21 = rand_vec(m), 
        a22 = rand_vec(m), 
        a23 = rand_vec(m), 
        a24 = rand_vec(m), 
        a25 = rand_vec(m), 
        a26 = rand_vec(m), 
        a27 = rand_vec(m), 
        a28 = rand_vec(m), 
        a29 = rand_vec(m), 
        a30 = rand_vec(m), 
        a31 = rand_vec(m), 
        a32 = rand_vec(m), 
        a33 = rand_vec(m), 
        a34 = rand_vec(m), 
        a35 = rand_vec(m), 
        a36 = rand_vec(m), 
        a37 = rand_vec(m), 
        a38 = rand_vec(m), 
        a39 = rand_vec(m), 
        a40 = rand_vec(m), 
        a41 = rand_vec(m), 
        a42 = rand_vec(m), 
        a43 = rand_vec(m), 
        a44 = rand_vec(m), 
        a45 = rand_vec(m), 
        a46 = rand_vec(m), 
        a47 = rand_vec(m), 
        a48 = rand_vec(m), 
        a49 = rand_vec(m),
        a50 = rand_vec(m), 
        a51 = rand_vec(m), 
        a52 = rand_vec(m), 
        a53 = rand_vec(m), 
        a54 = rand_vec(m), 
        a55 = rand_vec(m), 
        a56 = rand_vec(m), 
        a57 = rand_vec(m), 
        a58 = rand_vec(m), 
        a59 = rand_vec(m),
        a60 = rand_vec(m), 
        a61 = rand_vec(m), 
        a62 = rand_vec(m), 
        a63 = rand_vec(m), 
        a64 = rand_vec(m), 
        a65 = rand_vec(m), 
        a66 = rand_vec(m), 
        a67 = rand_vec(m), 
        a68 = rand_vec(m), 
        a69 = rand_vec(m),
        a70 = rand_vec(m), 
        a71 = rand_vec(m), 
        a72 = rand_vec(m), 
        a73 = rand_vec(m), 
        a74 = rand_vec(m), 
        a75 = rand_vec(m), 
        a76 = rand_vec(m), 
        a77 = rand_vec(m), 
        a78 = rand_vec(m), 
        a79 = rand_vec(m),
        a80 = rand_vec(m), 
        a81 = rand_vec(m), 
        a82 = rand_vec(m), 
        a83 = rand_vec(m), 
        a84 = rand_vec(m), 
        a85 = rand_vec(m), 
        a86 = rand_vec(m), 
        a87 = rand_vec(m), 
        a88 = rand_vec(m), 
        a89 = rand_vec(m);

	typedef boost::progress_timer timer_;
        
    {
        int const N = 1;
        os << "cref_list_of(" << N << ") => ";
        {
            timer_ timer;
            for(int i = 0; i < n; ++i)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of(a00));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
        }
        os << "cref_list_of_csv(" << N << ") => ";
        {
            timer_ timer;
            for(int i = 0; i < n; ++i)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of_csv(a00));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
        }
        os << "cref_list_of<>(" << N << ") => ";
        {
 			timer_ timer;
            for(int i = 0; i < n; ++i)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of<N>(a00));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
        }
		// For N > 1, list_of is too slow to be included in this test.
        os << "list_of(" << N << ") => ";
        {
 			timer_ timer;
            for(int i = 0; i < n; ++i)
            {
                BOOST_AUTO(rng, boost::assign::list_of(a00));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
        }
    }
    {
        int const N = 3;
        os << "cref_list_of(" << N << ") => ";
        {
 			timer_ timer;
            for(int i = 0; i < n; ++i)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of(a00)(a01)(a02));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
        }
        os << "cref_list_of_csv(" << N << ") => ";
        {
 			timer_ timer;
            for(int i = 0; i < n; ++i)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of_csv(a00,a01,a02));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
        }
        os << "cref_list_of<>(" << N << ") => ";
        {
 			timer_ timer;
            for(int i = 0; i < n; ++i)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of<3>(a00)(a01)(a02));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
        }
    }
    
    {
        int const N = 10;
        os << "cref_list_of(" << N << ") => ";
        {
 			timer_ timer;
            for(int idx = 0; idx < n; ++idx)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of(a00)(a01)(a02)(a03)(a04)
                                                           (a05)(a06)(a07)(a08)(a09));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
        }
        os << "cref_list_of_csv(" << N << ") => ";
        {
 			timer_ timer;
            for(int idx = 0; idx < n; ++idx)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of_csv(
                											a00,a01,a02,a03,a04
                                                           	,a05,a06,a07,a08,a09));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
        }
        os << "cref_list_of<>(" << N << ") => ";
        {
 			timer_ timer;
            for(int idx = 0; idx < n; ++idx)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of<10>
                                                            (a00)(a01)(a02)(a03)(a04)
                                                            (a05)(a06)(a07)(a08)(a09));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
        }
    }
    {
        int const N = 30;
        os << "cref_list_of(" << N << ") => ";
        {
 			timer_ timer;
            for(int idx = 0; idx < n; ++idx)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of(a00)(a01)(a02)(a03)(a04)
                                                            (a05)(a06)(a07)(a08)(a09)
                                                            (a10)(a11)(a12)(a13)(a14)
                                                            (a15)(a16)(a17)(a18)(a19)
                                                            (a20)(a21)(a22)(a23)(a24)
                                                            (a25)(a26)(a27)(a28)(a29));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
        }
        os << "cref_list_of_csv(" << N << ") => ";
        {
 			timer_ timer;
            for(int idx = 0; idx < n; ++idx)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of_csv(
                											a00,a01,a02,a03,a04
                                                            ,a05,a06,a07,a08,a09
                                                            ,a10,a11,a12,a13,a14
                                                            ,a15,a16,a17,a18,a19
                                                            ,a20,a21,a22,a23,a24
                                                            ,a25,a26,a27,a28,a29));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
        }
        os << "cref_list_of<>(" << N << ") => ";
        {
 			timer_ timer;
            for(int idx = 0; idx < n; ++idx)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of<N>
                                                            (a00)(a01)(a02)(a03)(a04)
                                                            (a05)(a06)(a07)(a08)(a09)
                                                            (a10)(a11)(a12)(a13)(a14)
                                                            (a15)(a16)(a17)(a18)(a19)
                                                            (a20)(a21)(a22)(a23)(a24)
                                                            (a25)(a26)(a27)(a28)(a29));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
        }
    }	

    {
        int const N = 90;
        os << "cref_list_of(" << N << ") => ";
        {
 			timer_ timer;
            for(int idx = 0; idx < n; ++idx)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of(a00)(a01)(a02)(a03)(a04)
                                                            (a05)(a06)(a07)(a08)(a09)
                                                            (a10)(a11)(a12)(a13)(a14)
                                                            (a15)(a16)(a17)(a18)(a19)
                                                            (a20)(a21)(a22)(a23)(a24)
                                                            (a25)(a26)(a27)(a28)(a29)
                                                            (a30)(a31)(a32)(a33)(a34)
                                                            (a35)(a36)(a37)(a38)(a39)
                                                            (a40)(a41)(a42)(a43)(a44)
                                                            (a45)(a46)(a47)(a48)(a49)
                                                            (a50)(a51)(a52)(a53)(a54)
                                                            (a55)(a56)(a57)(a58)(a59)
                                                            (a60)(a61)(a62)(a63)(a64)
                                                            (a65)(a66)(a67)(a68)(a69)
                                                            (a70)(a71)(a72)(a73)(a74)
                                                            (a75)(a76)(a77)(a78)(a79)
                                                            (a80)(a81)(a82)(a83)(a84)
                                                            (a85)(a86)(a87)(a88)(a89));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
        }
        os << "cref_list_of_csv(" << N << ") => ";
        {
 			timer_ timer;
            for(int idx = 0; idx < n; ++idx)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of_csv(
                											a00,a01,a02,a03,a04
                                                            ,a05,a06,a07,a08,a09
                                                            ,a10,a11,a12,a13,a14
                                                            ,a15,a16,a17,a18,a19
                                                            ,a20,a21,a22,a23,a24
                                                            ,a25,a26,a27,a28,a29
                                                            ,a30,a31,a32,a33,a34
                                                            ,a35,a36,a37,a38,a39
                                                            ,a40,a41,a42,a43,a44
                                                            ,a45,a46,a47,a48,a49
                                                            ,a50,a51,a52,a53,a54
                                                            ,a55,a56,a57,a58,a59
                                                            ,a60,a61,a62,a63,a64
                                                            ,a65,a66,a67,a68,a69
                                                            ,a70,a71,a72,a73,a74
                                                            ,a75,a76,a77,a78,a79
                                                            ,a80,a81,a82,a83,a84
                                                            ,a85,a86,a87,a88,a89));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
        }
        os << "cref_list_of<>(" << N << ") => ";
        {
 			timer_ timer;
            for(int idx = 0; idx < n; ++idx)
            {
                 BOOST_AUTO(rng, boost::assign::cref_list_of<N>
                                                            (a00)(a01)(a02)(a03)(a04)
                                                            (a05)(a06)(a07)(a08)(a09)
                                                            (a10)(a11)(a12)(a13)(a14)
                                                            (a15)(a16)(a17)(a18)(a19)
                                                            (a20)(a21)(a22)(a23)(a24)
                                                            (a25)(a26)(a27)(a28)(a29)
                                                            (a30)(a31)(a32)(a33)(a34)
                                                            (a35)(a36)(a37)(a38)(a39)
                                                            (a40)(a41)(a42)(a43)(a44)
                                                            (a45)(a46)(a47)(a48)(a49)
                                                            (a50)(a51)(a52)(a53)(a54)
                                                            (a55)(a56)(a57)(a58)(a59)
                                                            (a60)(a61)(a62)(a63)(a64)
                                                            (a65)(a66)(a67)(a68)(a69)
                                                            (a70)(a71)(a72)(a73)(a74)
                                                            (a75)(a76)(a77)(a78)(a79)
                                                            (a80)(a81)(a82)(a83)(a84)
                                                            (a85)(a86)(a87)(a88)(a89));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
        }
    }	


    os << "<- " << std::endl;
};
