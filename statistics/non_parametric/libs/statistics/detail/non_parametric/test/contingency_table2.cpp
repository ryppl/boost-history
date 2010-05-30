///////////////////////////////////////////////////////////////////////////////
// contingency_table2.cpp                                                    //
//                                                                           //
//  Copyright 2010 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include <string>

#include <boost/mpl/int.hpp>
#include <boost/mpl/vector/vector10.hpp>

#include <boost/assign/list_of.hpp>

#include <boost/fusion/container/map/detail/sequence_to_map.hpp>
#include <boost/fusion/include/make_map.hpp>
 
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/framework/parameters/weight.hpp>
#include <boost/accumulators/framework/accumulator_set.hpp>
#include <boost/accumulators/statistics/detail/weighted_count.hpp>

#include <boost/statistics/detail/non_parametric/contingency_table/include/factor.hpp>
#include <boost/statistics/detail/non_parametric/contingency_table/include/pearson_chisq/independence.hpp>

#include <libs/statistics/detail/non_parametric/test/contingency_table2.h>

/*
# Read the count data for this problem
count<-scan()
   12  34  23         4  47  11
   35  31  11        34  10  18
   12  32   9        18  13  19
   12  12  14         9  33  25 

# Create factor tags:r=rows, c=columns, t=tiers
r <- factor(gl(4, 2*3, 2*3*4, labels=c("r1","r2", "r3", "r4")));  r
c <- factor(gl(3, 1,   2*3*4, labels=c("c1","c2", "c3")));  c
t <- factor(gl(2, 3,   2*3*4, labels=c("t1","t2")));  t

# Cross-tabulation of counts:
xtabs(count~r+c+t)           # all three factors
xtabs(count~r+c)             # RC: sum over tiers
xtabs(count~r+t)             # RT: sum over columns
xtabs(count~c+t)             # CT: sum over rows
xtabs(count~r)       # R: sum over columns and tiers
xtabs(count~c)       # C: sum over rows and tiers
xtabs(count~t)       # T: sum over rows and columns

# 3-way Chi squared test of independence
summary(xtabs(count~r+c+t))
# 2-way Chi squared test of partial independence: R versus CT
summary(xtabs(count~c+t))
# 2-way Chi squared test of partial independence: C versus RT
summary(xtabs(count~r+t))
# 2-way Chi squared test of partial independence: T versus RC
summary(xtabs(count~r+c))
*/

void test_contingency_table2()
{

	namespace ct = boost::statistics::detail::contingency_table;
    namespace pearson_chisq = ct::pearson_chisq;
    using namespace boost::accumulators;

    typedef double val_;
    typedef boost::mpl::int_<0> r_; typedef std::string data_r_;
    typedef boost::mpl::int_<1> c_; typedef std::string data_c_;
    typedef boost::mpl::int_<2> t_; typedef std::string data_t_;
 
    typedef boost::fusion::detail::sequence_to_map<
        boost::mpl::vector6<r_,data_r_,c_,data_c_,t_,data_t_>
    >::type sample_;

    typedef boost::mpl::vector3<r_,c_,t_> all_three_factors_;
    typedef boost::mpl::vector2<r_,c_> sum_over_tiers_;
    typedef boost::mpl::vector2<r_,t_> sum_over_cols_;
    typedef boost::mpl::vector2<c_,t_> sum_over_rows_;

    typedef ct::pearson_chisq::tag::independence<all_three_factors_> indep_r_c_t_;
    typedef ct::pearson_chisq::tag::independence<sum_over_tiers_> indep_r_c_;
    typedef ct::pearson_chisq::tag::independence<sum_over_cols_> indep_r_t_;
    typedef ct::pearson_chisq::tag::independence<sum_over_rows_> indep_c_t_;

    typedef boost::accumulators::stats<
        indep_r_c_t_,
        indep_r_c_,
        indep_r_t_,
        indep_c_t_
    > stats_;
    typedef boost::accumulators::accumulator_set< sample_, stats_, long int > acc_;
        
    using namespace boost::assign;
    acc_ acc(( ct::factor::_map_of_levels = boost::fusion::make_map<r_,c_,t_>(
        list_of("r1")("r2")("r3")("r4"), list_of("c1")("c2")("c3"), list_of("t1")("t2") ) ));

    typedef boost::fusion::result_of::make_map<
        r_,c_,t_,data_r_,data_c_,data_t_>::type result_of_make_map_;
    typedef result_of_make_map_(*fp_)(const data_r_&,const data_c_&,const data_t_&);
    
    fp_ make_sample = boost::fusion::make_map<r_,c_,t_>;

    acc( make_sample( "r1", "c1", "t1" ), weight = 12 );
    acc( make_sample( "r1", "c1", "t2" ), weight =  4 );
    acc( make_sample( "r1", "c2", "t1" ), weight = 34 );
    acc( make_sample( "r1", "c2", "t2" ), weight = 47 );
    acc( make_sample( "r1", "c3", "t1" ), weight = 23 );
    acc( make_sample( "r1", "c3", "t2" ), weight = 11 );
    acc( make_sample( "r2", "c1", "t1" ), weight = 35 );
    acc( make_sample( "r2", "c1", "t2" ), weight = 34 );
    acc( make_sample( "r2", "c2", "t1" ), weight = 31 );
    acc( make_sample( "r2", "c2", "t2" ), weight = 10 );
    acc( make_sample( "r2", "c3", "t1" ), weight = 11 );
    acc( make_sample( "r2", "c3", "t2" ), weight = 18 );
    acc( make_sample( "r3", "c1", "t1" ), weight = 12 );
    acc( make_sample( "r3", "c1", "t2" ), weight = 18 );
    acc( make_sample( "r3", "c2", "t1" ), weight = 32 );
    acc( make_sample( "r3", "c2", "t2" ), weight = 13 );
    acc( make_sample( "r3", "c3", "t1" ), weight =  9 );
    acc( make_sample( "r3", "c3", "t2" ), weight = 19 );
    acc( make_sample( "r4", "c1", "t1" ), weight = 12 );
    acc( make_sample( "r4", "c1", "t2" ), weight =  9 );
    acc( make_sample( "r4", "c2", "t1" ), weight = 12 );
    acc( make_sample( "r4", "c2", "t2" ), weight = 33 );
    acc( make_sample( "r4", "c3", "t1" ), weight = 14 );
    acc( make_sample( "r4", "c3", "t2" ), weight = 25 );

    using namespace std;

    val_ stat;
    long df;

/*
#### Output and interpretation (at significance level alpha=0.05):
   # 3-way Chi squared test of independence
   > summary(xtabs(count~r+c+t))
   Call: xtabs(formula = count ~ r + c + t)
   Number of cases in table: 478
   Number of factors: 3
   Test for independence of all factors:
	   Chisq = 102.17, df = 17, p-value = 3.514e-14
#
# ==> reject H0 in favor of 
#            HA: r,c,t are NOT mutually independent
#
*/

    // #include <boost/test/unit_test.hpp>
    // #include <boost/test/floating_point_comparison.hpp>

    df = pearson_chisq::df(acc,indep_r_c_t_());
    stat = pearson_chisq::statistic<val_>( acc, indep_r_c_t_() );
    BOOST_ASSERT( df == 17 );
    BOOST_ASSERT( fabs(stat - 102.17) < 0.01 );

/*
   > # 2-way Chi squared test of partial independence: R versus CT
   > summary(xtabs(count~c+t))
   Call: xtabs(formula = count ~ c + t)
   Number of cases in table: 478
   Number of factors: 2
   Test for independence of all factors:
	   Chisq = 2.3704, df = 2, p-value = 0.3057
#
# ==> do not reject H0: c,t are mutually independent
#
*/

    df = pearson_chisq::df(acc,indep_c_t_());
    stat = pearson_chisq::statistic<val_>( acc, indep_c_t_() );
    BOOST_ASSERT( df == 2 );
    BOOST_ASSERT( fabs( stat == 2.3704 )< 0.001 );

/*
   >
   > # 2-way Chi squared test of partial independence: C versus RT
   > summary(xtabs(count~r+t))
   Call: xtabs(formula = count ~ r + t)
   Number of cases in table: 478
   Number of factors: 2
   Test for independence of all factors:
	   Chisq = 10.057, df = 3, p-value = 0.01809
#
# ==> reject H0 in favor of 
#            HA: r,t are NOT mutually independent

*/

    df = pearson_chisq::df(acc,indep_r_t_());
    stat = pearson_chisq::statistic<val_>( acc, indep_r_t_() );
    BOOST_ASSERT( df == 3 );
    BOOST_ASSERT( fabs( stat - 10.057 ) < 0.001 );

/*
   > # 2-way Chi squared test of partial independence: T versus RC
   > summary(xtabs(count~r+c))
   Call: xtabs(formula = count ~ r + c)
   Number of cases in table: 478
   Number of factors: 2
   Test for independence of all factors:
	   Chisq = 58.67, df = 6, p-value = 8.363e-11

#
# ==> reject H0 in favor of 
#            HA: r,c are NOT mutually independent
#
*/

    df = pearson_chisq::df(acc,indep_r_c_());
    stat = pearson_chisq::statistic<val_>( acc, indep_r_c_() );
    BOOST_ASSERT( df == 6 );
    BOOST_ASSERT( fabs( stat - 58.67 ) < 0.01 );

}

