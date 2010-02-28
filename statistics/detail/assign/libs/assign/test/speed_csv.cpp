//////////////////////////////////////////////////////////////////////////////
// test::speed_csv.cpp                                                      //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <boost/progress.hpp>
#include <boost/bind.hpp>
#include <boost/typeof/typeof.hpp>
#include <libs/assign/test/speed_common.h>

#define BOOST_ASSIGN_CSV_SIZE 160
#include <boost/assign/auto_size/ref_list_of_csv.hpp>
#undef BOOST_ASSIGN_CSV_SIZE

#include <libs/assign/test/speed.h>

// This file was provided by M.P.G 

void test_speed_csv(std::ostream& os)
{
	os << "-> test_speed_csv : " << std::endl;

	typedef boost::progress_timer timer_;
    
    typedef std::vector<std::string> Vec;
    
    int const str_len = 100, // Irrelevant bec. we don't copy them
    max_N = 200;
    
    const int n = 1000 * 1000;
    int N;
    Vec v;
    std::generate_n(std::back_inserter(v), max_N, boost::bind(&rand_str, str_len));
    
    {
        N = 1;
        assert(N < max_N);
        {
            timer_ timer;
            for(int i = 0; i < n; ++i)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of_csv(v[0]));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
            os << "cref_list_of_csv(" << N << ") => ";
        }
        {
 			timer_ timer;
            for(int i = 0; i < n; ++i)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of<1>(v[0]));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
 			os << "cref_list_of<>(" << N << ") => ";
        }
    }
    {
        N = 3;
        assert(N < max_N);
        {
 			timer_ timer;
            for(int i = 0; i < n; ++i)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of_csv(v[0],v[1],v[2]));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
            os << "cref_list_of_csv(" << N << ") => ";
        }
        {
 			timer_ timer;
            for(int i = 0; i < n; ++i)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of<3>(v[0])(v[1])(v[2]));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
 			os << "cref_list_of<>(" << N << ") => ";
        }
    }
    
    {
        N = 10;
        assert(N < max_N);
        {
 			timer_ timer;
            for(int i = 0; i < n; ++i)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of_csv(v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9]));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
 			os << "cref_list_of_csv(" << N << ") => ";
        }
        {
 			timer_ timer;
            for(int i = 0; i < n; ++i)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of<10>(v[0])(v[1])(v[2])(v[3])(v[4])(v[5])(v[6])(v[7])(v[8])(v[9]));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
 			os << "cref_list_of<>(" << N << ") => ";
        }
    }
    
    {
        N = 30;
        assert(N < max_N);
        {
 			timer_ timer;
            for(int i = 0; i < n; ++i)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of_csv(v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9]
                           ,v[10],v[11],v[12],v[13],v[14],v[15],v[16],v[17],v[18],v[19]
                           ,v[20],v[21],v[22],v[23],v[24],v[25],v[26],v[27],v[28],v[29]));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
 			os << "cref_list_of_csv(" << N << ") => ";
        }
        {
 			timer_ timer;
            for(int i = 0; i < n; ++i)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of<30>(v[0])(v[1])(v[2])(v[3])(v[4])(v[5])(v[6])(v[7])(v[8])(v[9])
                           (v[10])(v[11])(v[12])(v[13])(v[14])(v[15])(v[16])(v[17])(v[18])(v[19])
                           (v[20])(v[21])(v[22])(v[23])(v[24])(v[25])(v[26])(v[27])(v[28])(v[29]));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
 			os << "cref_list_of<>(" << N << ") => ";
        }
    }    
    
    {
        N = 80;
        assert(N < max_N);
        {
            timer_ timer;
            for(int i = 0; i < n; ++i)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of_csv( 
                			v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9]
                           ,v[10],v[11],v[12],v[13],v[14],v[15],v[16],v[17],v[18],v[19]
                           ,v[20],v[21],v[22],v[23],v[24],v[25],v[26],v[27],v[28],v[29]
                           ,v[30],v[31],v[32],v[33],v[34],v[35],v[36],v[37],v[38],v[39]
                           ,v[40],v[41],v[42],v[43],v[44],v[45],v[46],v[47],v[48],v[49]
                           ,v[50],v[51],v[52],v[53],v[54],v[55],v[56],v[57],v[58],v[59]
                           ,v[60],v[61],v[62],v[63],v[64],v[65],v[66],v[67],v[68],v[69]
                           ,v[70],v[71],v[72],v[73],v[74],v[75],v[76],v[77],v[78],v[79]));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
 			os << "cref_list_of_csv(" << N << ") => ";
        }
        {
 			timer_ timer;
            for(int i = 0; i < n; ++i)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of<80>( v[0])( v[1])( v[2])( v[3])( v[4])( v[5])( v[6])( v[7])( v[8])( v[9])
                           (v[10])(v[11])(v[12])(v[13])(v[14])(v[15])(v[16])(v[17])(v[18])(v[19])
                           (v[20])(v[21])(v[22])(v[23])(v[24])(v[25])(v[26])(v[27])(v[28])(v[29])
                           (v[30])(v[31])(v[32])(v[33])(v[34])(v[35])(v[36])(v[37])(v[38])(v[39])
                           (v[40])(v[41])(v[42])(v[43])(v[44])(v[45])(v[46])(v[47])(v[48])(v[49])
                           (v[50])(v[51])(v[52])(v[53])(v[54])(v[55])(v[56])(v[57])(v[58])(v[59])
                           (v[60])(v[61])(v[62])(v[63])(v[64])(v[65])(v[66])(v[67])(v[68])(v[69])
                           (v[70])(v[71])(v[72])(v[73])(v[74])(v[75])(v[76])(v[77])(v[78])(v[79]));
                int sz = (int)rng.size();   
                if(sz != N)
                    os << "ERROR\n";
            }
 			os << "cref_list_of<>(" << N << ") => ";
        }
    }  
    
    {
        N = 160;
        assert(N < max_N);
        {
 			timer_ timer;
            for(int i = 0; i < n; ++i)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of_csv( 
                			v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9]
                           ,v[10],v[11],v[12],v[13],v[14],v[15],v[16],v[17],v[18],v[19]
                           ,v[20],v[21],v[22],v[23],v[24],v[25],v[26],v[27],v[28],v[29]
                           ,v[30],v[31],v[32],v[33],v[34],v[35],v[36],v[37],v[38],v[39]
                           ,v[40],v[41],v[42],v[43],v[44],v[45],v[46],v[47],v[48],v[49]
                           ,v[50],v[51],v[52],v[53],v[54],v[55],v[56],v[57],v[58],v[59]
                           ,v[60],v[61],v[62],v[63],v[64],v[65],v[66],v[67],v[68],v[69]
                           ,v[70],v[71],v[72],v[73],v[74],v[75],v[76],v[77],v[78],v[79]
                           ,v[80],v[88],v[82],v[83],v[84],v[85],v[86],v[87],v[88],v[89]
                           ,v[90],v[91],v[92],v[93],v[94],v[95],v[96],v[97],v[98],v[99]
                           ,v[100],v[101],v[102],v[103],v[104],v[105],v[106],v[107],v[108],v[109]
                           ,v[110],v[111],v[112],v[113],v[114],v[115],v[116],v[117],v[118],v[119]
                           ,v[120],v[121],v[122],v[123],v[124],v[125],v[126],v[127],v[128],v[129]
                           ,v[130],v[131],v[132],v[133],v[134],v[135],v[136],v[137],v[138],v[139]
                           ,v[140],v[141],v[142],v[143],v[144],v[145],v[146],v[147],v[148],v[149]
                           ,v[150],v[151],v[152],v[153],v[154],v[155],v[156],v[157],v[158],v[159]));
                int sz = (int)rng.size();
                if(sz != N)
                    os << "ERROR\n";
            }
 			os << "cref_list_of_csv(" << N << ") => ";
        }
        {
 			timer_ timer;
            for(int i = 0; i < n; ++i)
            {
                BOOST_AUTO(rng, boost::assign::cref_list_of<160>( v[0])( v[1])( v[2])( v[3])( v[4])( v[5])( v[6])( v[7])( v[8])( v[9])
                           (v[10])(v[11])(v[12])(v[13])(v[14])(v[15])(v[16])(v[17])(v[18])(v[19])
                           (v[20])(v[21])(v[22])(v[23])(v[24])(v[25])(v[26])(v[27])(v[28])(v[29])
                           (v[30])(v[31])(v[32])(v[33])(v[34])(v[35])(v[36])(v[37])(v[38])(v[39])
                           (v[40])(v[41])(v[42])(v[43])(v[44])(v[45])(v[46])(v[47])(v[48])(v[49])
                           (v[50])(v[51])(v[52])(v[53])(v[54])(v[55])(v[56])(v[57])(v[58])(v[59])
                           (v[60])(v[61])(v[62])(v[63])(v[64])(v[65])(v[66])(v[67])(v[68])(v[69])
                           (v[70])(v[71])(v[72])(v[73])(v[74])(v[75])(v[76])(v[77])(v[78])(v[79])
                           (v[80])(v[88])(v[82])(v[83])(v[84])(v[85])(v[86])(v[87])(v[88])(v[89])
                           (v[90])(v[91])(v[92])(v[93])(v[94])(v[95])(v[96])(v[97])(v[98])(v[99])
                           (v[100])(v[101])(v[102])(v[103])(v[104])(v[105])(v[106])(v[107])(v[108])(v[109])
                           (v[110])(v[111])(v[112])(v[113])(v[114])(v[115])(v[116])(v[117])(v[118])(v[119])
                           (v[120])(v[121])(v[122])(v[123])(v[124])(v[125])(v[126])(v[127])(v[128])(v[129])
                           (v[130])(v[131])(v[132])(v[133])(v[134])(v[135])(v[136])(v[137])(v[138])(v[139])
                           (v[140])(v[141])(v[142])(v[143])(v[144])(v[145])(v[146])(v[147])(v[148])(v[149])
                           (v[150])(v[151])(v[152])(v[153])(v[154])(v[155])(v[156])(v[157])(v[158])(v[159]));
                int sz = (int)rng.size();   
                if(sz != N)
                    os << "ERROR\n";
            }
 			os << "cref_list_of<>(" << N << ") => ";
        }
    }       
	os << "<- " << std::endl;
    
};
