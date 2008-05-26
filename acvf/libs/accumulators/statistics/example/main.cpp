///////////////////////////////////////////////////////////////////////////////
// main.cpp
//
//  Copyright 2008 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <functional>
#include <fstream>
#include <boost/mpl/size_t.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/delay.hpp>
#include <boost/accumulators/statistics/acvf_moving_average.hpp>
#include <boost/accumulators/statistics/acvf.hpp>
#include <boost/accumulators/statistics/acf.hpp>
#include <boost/accumulators/statistics/integrated_acf.hpp>
#include <boost/accumulators/statistics/integrated_acvf.hpp>
#include <boost/accumulators/statistics/percentage_effective_sample_size.hpp>
#include <boost/accumulators/statistics/standard_error_autocorrelated.hpp>
#include <boost/accumulators/statistics/standard_error_iid.hpp>
#include <boost/accumulators/statistics/acvf_analysis.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/moving_average.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/numeric/conversion/converter.hpp>
int main(){

    const char* filepath = "./acvf_output";
    std::ofstream out(filepath);

    using namespace boost::accumulators;
    typedef boost::mt19937                                    urng_type;
    typedef boost::normal_distribution<>                      nd_type;
    typedef boost::variate_generator<urng_type&,nd_type>      gen_nd_type;
    typedef boost::random::moving_average<double>             ma_type;
    typedef std::vector<double>                               ma_vals_type;
    typedef default_delay_discriminator                       delaydisrc;
    typedef accumulator_set<
        double, stats<
            tag::acvf<>,
            tag::acf<>,
            tag::integrated_acvf<>,
            tag::percentage_effective_sample_size<>,
            tag::standard_error_autocorrelated<>,
            tag::standard_error_iid<>
            >
    >            acc_type;

    //model parameters and related quantities
    std::vector<double> coeffs;
    std::vector<unsigned int> lags;
    std::vector<double> true_acfs;
    double true_integrated_acvf = 0.0;
    std::size_t true_ess = 0;
    //with these coeffs, should expect ess% > 100
    {using namespace boost::assign; coeffs+=1.0,-0.5,0.2; lags+=0,1,2;}
    unsigned int  K = coeffs.size()-1;
    transform(lags.begin(),lags.end(),back_inserter(true_acfs),
        make_acvf_moving_average(coeffs));
    true_integrated_acvf
        = 2*std::accumulate(true_acfs.begin(),true_acfs.end(),0.0);
    true_integrated_acvf -=  *true_acfs.begin();
    true_ess = boost::numeric::converter<std::size_t,double>::convert(
        100.0*true_acfs[0]/true_integrated_acvf);

    out << "->true_acvf: ";
    copy(true_acfs.begin(),true_acfs.end(),
        std::ostream_iterator<double>(out," "));
    out << "<-" << std::endl;
    {   double div = 1.0/true_acfs[0];
        transform(true_acfs.begin(),true_acfs.end(),true_acfs.begin(),
            boost::bind(std::multiplies<double>(),_1,div));
    }
    out << "->true_acf: ";
    copy(true_acfs.begin(),true_acfs.end(),
        std::ostream_iterator<double>(out," ")); out << "<-" << std::endl;
    out << "->true var: " << true_integrated_acvf << "<-" << std::endl;
    out << "->true ess%: " << true_ess << "<-" << std::endl;

    //generation of a Moving Average of order K process
    const unsigned long N = 100000;
    urng_type urng(0);
    gen_nd_type gen_nd(urng,nd_type());
    ma_type ma(boost::make_iterator_range(coeffs.begin(),coeffs.end()));
    ma_vals_type ma_vals(N);
    for(ma_vals_type::iterator i=ma_vals.begin(); i<ma_vals.end(); i++)
    {(*i) = ma(gen_nd);}

    //estimation
    acc_type acc(tag::delay<>::cache_size=(K+1));
    for_each(ma_vals.begin(),ma_vals.end(),
        boost::bind<void>(boost::ref(acc),_1));
    out << "->sample size: " << N << std::endl;
    out << "->estimated acvf: ";
    copy(begin(acvf<delaydisrc>(acc)),end(acvf<delaydisrc>(acc)),
        std::ostream_iterator<double>(out," "));
    out<<"<-"<<std::endl;

    out << "->estimated acf: ";
    copy(begin(acf<delaydisrc>(acc)),end(acf<delaydisrc>(acc)),
        std::ostream_iterator<double>(out," "));
    out<<"<-"<<std::endl;

    out << "->estimated var: "
        << integrated_acvf<delaydisrc>(acc) << "<-" << std::endl;

    out << "->estimated ess%: "
        << percentage_effective_sample_size<delaydisrc>(acc)
        << "<-" << std::endl;

    out << "->estimated standard error assuming iid: "
        << standard_error_iid<delaydisrc>(acc) << "<-" << std::endl;

    out << "->estimated standard error assuming acf is zero after lag "
        << K << ": "
        << standard_error_autocorrelated<delaydisrc>(acc) << "<-" << std::endl;

    //the above bundled into one class:
    out << " --------- ";
    out << "output from acvf_analysis:" << std::endl;
    const unsigned int offset = 0;
    const unsigned int stride = 1;
    const unsigned int assumed_lag = 3;
    statistics::acvf_analysis acvf_x(assumed_lag);
    acvf_x(ma_vals,offset,stride);
    acvf_x.print(out);

    std::cout << "output libs/accumulators/main.cpp is in"
        << filepath << std::endl;

    return 0;
}
