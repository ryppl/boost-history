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
    std::vector<int>    // 50 items
        a00 = rand_vec(), 
        a01 = rand_vec(), 
        a02 = rand_vec(), 
        a03 = rand_vec(), 
        a04 = rand_vec(), 
        a05 = rand_vec(), 
        a06 = rand_vec(), 
        a07 = rand_vec(), 
        a08 = rand_vec(), 
        a09 = rand_vec(), 
        a10 = rand_vec(), 
        a11 = rand_vec(), 
        a12 = rand_vec(), 
        a13 = rand_vec(), 
        a14 = rand_vec(), 
        a15 = rand_vec(), 
        a16 = rand_vec(), 
        a17 = rand_vec(), 
        a18 = rand_vec(), 
        a19 = rand_vec(), 
        a20 = rand_vec(), 
        a21 = rand_vec(), 
        a22 = rand_vec(), 
        a23 = rand_vec(), 
        a24 = rand_vec(), 
        a25 = rand_vec(), 
        a26 = rand_vec(), 
        a27 = rand_vec(), 
        a28 = rand_vec(), 
        a29 = rand_vec(), 
        a30 = rand_vec(), 
        a31 = rand_vec(), 
        a32 = rand_vec(), 
        a33 = rand_vec(), 
        a34 = rand_vec(), 
        a35 = rand_vec(), 
        a36 = rand_vec(), 
        a37 = rand_vec(), 
        a38 = rand_vec(), 
        a39 = rand_vec(), 
        a40 = rand_vec(), 
        a41 = rand_vec(), 
        a42 = rand_vec(), 
        a43 = rand_vec(), 
        a44 = rand_vec(), 
        a45 = rand_vec(), 
        a46 = rand_vec(), 
        a47 = rand_vec(), 
        a48 = rand_vec(), 
        a49 = rand_vec(),
        a50 = rand_vec(), 
        a51 = rand_vec(), 
        a52 = rand_vec(), 
        a53 = rand_vec(), 
        a54 = rand_vec(), 
        a55 = rand_vec(), 
        a56 = rand_vec(), 
        a57 = rand_vec(), 
        a58 = rand_vec(), 
        a59 = rand_vec(),
        a60 = rand_vec(), 
        a61 = rand_vec(), 
        a62 = rand_vec(), 
        a63 = rand_vec(), 
        a64 = rand_vec(), 
        a65 = rand_vec(), 
        a66 = rand_vec(), 
        a67 = rand_vec(), 
        a68 = rand_vec(), 
        a69 = rand_vec(),
        a70 = rand_vec(), 
        a71 = rand_vec(), 
        a72 = rand_vec(), 
        a73 = rand_vec(), 
        a74 = rand_vec(), 
        a75 = rand_vec(), 
        a76 = rand_vec(), 
        a77 = rand_vec(), 
        a78 = rand_vec(), 
        a79 = rand_vec(),
        a80 = rand_vec(), 
        a81 = rand_vec(), 
        a82 = rand_vec(), 
        a83 = rand_vec(), 
        a84 = rand_vec(), 
        a85 = rand_vec(), 
        a86 = rand_vec(), 
        a87 = rand_vec(), 
        a88 = rand_vec(), 
        a89 = rand_vec();

	typedef boost::progress_timer timer_;
    
    const int n = 100 * 1000 * 1000;
    
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
