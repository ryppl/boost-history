/////////////////////////////////////////////////////////////////////////////
// survival_data
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////////////

///////////
/ Contact /
///////////

Please send suggestions or questions to erwann.rogard@gmail.com

////////////////
/ Requirements /
////////////////

Compiles fine under

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

The compiler search path must include
boost_1_37_0
sandbox/conditionally_specified_distribution/survival_data
sandbox/conditionally_specified_distribution/utility
The folder that contains
    iterator/cycle_iterator.hpp
(currently in the Vault).
////////////
/ Overview /
////////////

This library provides C++ classes for managing and generating survival data
(staggered entry, right censored, fixed covariate).

Files:
unit.hpp                            a data-unit
unit_at_time.hpp                    a data-unit analysed at time t
transform_if_to_unit_at_time.hpp    make a dataset at time t from data-units
generate.hpp                        tool for simulating data unit
stream_to_dataset.hpp               generates data-units from a data file

//////////////
/ Discussion /
//////////////

Copying of of a data unit is "shallow" i.e. the copy and the original
share the same data (through shared_ptr). This generally more convenient
and saves memory if multiple clients need access to the same data.

This may not be suitable, however, for very large datasets.

////////////////////////
/ Output from main.cpp /
////////////////////////

->example_as_random_distribution
1 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9
2 2.1 2.2 2.3 2.4 2.5 2.6 2.7 2.8 2.9
 <-
->example_as_random_regression
1 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9
 <-
->example_surival_data_unit
 covariate=-1 0 1
unit : 1.5
 entry_time = 1
 time_on_study(1.5)=0.5
 event_time(1.5)=0.5
 is_at_risk(0.5,1.5)=1
 is_at_risk(1.0,1.5)=0
 event_time(2.5)=1
 is_failure(2.5)=1
unit_at_time at : 1.5
 entry_time = 1
 time_on_study()=0.5
 event_time()=0.5
 is_at_risk(0.5)=1
 is_at_risk(1.0)=0
<-
-> example_survival_data_generate
 generation separately
entry=0
covariate=1 0
failure time =0.879577
censoring time =0.993063
entry=0.1
covariate=0 1
failure time =1.02827
censoring time =1.52252
entry=0.2
covariate=1 0
failure time =1.02032
censoring time =2.15679
entry=0.3
covariate=0 1
failure time =0.644506
censoring time =1.53837
entry=0.4
covariate=1 0
failure time =0.609003
censoring time =1.07976
entry=0.5
covariate=0 1
failure time =0.849973
censoring time =0.397189
entry=0.6
covariate=1 0
failure time =0.636047
censoring time =0.390301
entry=0.7
covariate=0 1
failure time =1.82047
censoring time =0.0478013
entry=0.8
covariate=1 0
failure time =3.66354
censoring time =0.351838
entry=0.9
covariate=0 1
failure time =0.39594
censoring time =0.531722
 generation simulatneously
entry=1
covariate=1 0
event time=0.879577
entry=1.1
covariate=0 1
event time=1.02827
entry=1.2
covariate=1 0
event time=1.02032
entry=1.3
covariate=0 1
event time=0.644506
entry=1.4
covariate=1 0
event time=0.609003
entry=1.5
covariate=0 1
event time=0.849973
entry=1.6
covariate=1 0
event time=0.636047
entry=1.7
covariate=0 1
event time=1.82047
entry=1.8
covariate=1 0
event time=3.66354
entry=1.9
covariate=0 1
event time=0.39594
<-
-> example_survival_data_generate_fixed_design_no_censoring
fail times:
0.83668 0.773398 1.45918 1.76891 0.970558 1.67971 0.914597 1.78733 0.579301
0.840917 1.20617 0.461468 0.605027 0.303967 2.58337 0.0555372 3.48487
0.274012 0.561865 0.617772 1.64934 1.43929 0.874483 0.621992 0.882471
0.429383 3.01874 1.72017 0.0774633 0.354248 0.105914 0.993666 0.0214728
0.395279 2.07676 2.99653 1.58299 0.130166 2.37051 1.94136 4.04237 0.552324
1.86502 1.53529 0.650663 0.63259 1.76196 1.08054 0.132328 1.0976 1.18673
0.829778 0.162663 0.663464 3.36284 1.35204 0.775657 0.096353 0.622239
0.610399 0.323035 0.177481 1.72911 1.27017 0.640311 0.210054 0.97633
0.138193 0.0199411 0.3372 1.11697 0.154229 0.99555 0.216418 1.11484
0.46473 3.02547 2.00451 1.33046 0.568594 0.468361 0.817238 0.667511
2.21289 1.25743 0.0899967 0.0721677 3.32951 1.15525 0.911347 1.29033
0.178285 0.248317 0.381641 0.160366 1.3213 0.398392 0.805676 0.525268
0.37394  OK <-
-> example_survival_data_transform_if_to_unit_at_time
OK <-
-> example_survival_data_stream_to_dataset()
unit # =0
covariate=1 0 0 0 1 12 is failure=1 event time=12
unit # =20
covariate=0 1 0 0 1 32 is failure=1 event time=32
unit # =40
covariate=0 0 1 0 1 22 is failure=1 event time=22
unit # =60
covariate=0 0 0 1 1 27 is failure=1 event time=27
<-
