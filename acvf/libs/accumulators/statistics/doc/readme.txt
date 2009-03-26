////////////////////////////////////////////////////////////////////////////
// acvf
//  (C) Copyright 2008 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////

///////////
/ Contact /
///////////

Please send questions or suggestions to erwann.rogard@gmail.com

////////////////
/ Requirements /
////////////////

Compiles fine under

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

The compiler search path must include
boost_1_37_0
sandbox/acvf
sandbox/accumulators_filters

////////////
/ Overview /
////////////

Provides autocovariance and related statistics within the Boost.Accumulator
framework.


/////////
/ Usage /
/////////

    typedef mpl::void_ discr_t;
    typedef accumulator_set<
        value_type, stats<
            tag::acvf<discr_t>,
            tag::acf<discr_t>,
            tag::integrated_acvf<discr_t>,
            tag::percentage_effective_sample_size<discr_t>,
            tag::standard_error_autocorrelated<discr_t>,
            tag::standard_error_iid<discr_t>
            >
    >            acc_type;
    std::size_t max_lag = 3;
    acc_type acc(max_lag);
    acc(-1.0); acc(+1.0); acc(-1.0); acc(1.0);
    acvf<discr_t>(acc);
    percentage_effective_sample_size(acc);

    Bundled version:
    typedef acvf_analysis<value_type,discr_t> bundled_t;
    std::ofstream out;
    bundle_t bundle(max_lag);
    bundle.acvf();
    bundle.percentage_effective_sample_size();
    bundle.print(out);

////////////////////////
/ Output from main.cpp /
////////////////////////
->true_acvf: 1.29 -0.6 0.2 <-
->true_acf: 1 -0.465116 0.155039 <-
->true var: 0.49<-
->true ess%: 263<-
->sample size: 100000
->estimated acvf: 1.27563 -0.58848 0.190071 <-
->estimated acf: 1 -0.461325 0.149002 <-
->estimated var: 0.478814<-
->estimated ess%: 266<-
->estimated standard error assuming iid: 0.0035716<-
->estimated standard error assuming acf is zero after lag 2: 0.00218818<-
 --------- output from acvf_analysis:
count :100000
acf : 1 -0.461325 0.149002 0.00284592
integrated_acvf : 0.486075
ess% : 262
mean : 0.00306125
standard error :
 assuming iid : 0.0035716
 assuming acf is zero after lag 3: 0.00220471


