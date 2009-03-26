///////////////////////////////////////////////////////////////////////////////
// improved_fast_gauss_transform
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///////////
/ Contact /
///////////

Please send questions or suggestions to erwann.rogard@gmail.com

/////////////
/ Overview  /
/////////////

This collection of C++ classes implements the Improved Fast Gauss
Transform (see Raykar2006a), and its exact counterpart. Applications
include the Rozenblatt--Parzen (RP) density estimator and the Nadaraya--
Watson (NW) conditional mean estimator.

This package offers the specificity  of allowing  sequential accumulation of
training data, and customization of data types and particulars of the
algorithm through templates.

Below is a description of the package, but individual *.hpp files contain
additional information.

Examples are in libs/ifgt/src/example.

//////////////////
/ Requirements   /
//////////////////

Compiles under

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1 (Apple Inc. build 5490)

The compiler search path must include
boost_1_37_0
sandbox/miscellanea_iterator_facade
sandbox/miscellanea_algorithm
sandbox/monomials_horner_rule
sandbox/improved_fast_gauss_transform
sandbox/conditionally_specified_distribution/utility

///////////////
/ Conventions /
///////////////

D       source size
a       multivariate power
|a|     total degree of a             |a| = sum{a(d):d=0,...,D-1}
p       a bound on |a|
b       coefficient

x       source                        dim(x)=D
y       target                        dim(y)=D
w       weight                        dim(w)=J
(x,w)   contribution
h       bandwith
i,N     contributions index           i=0,...,N-1
j,J     weight index                  j=0,...,J-1
m,M     target index                  m=0,...,M-1

rx   max cluster radius
ry   cutoff radius
c    cluster center
zx   zscore of x                    (x-c)/h
zy   zscore of y                    (t-c)/h
C    a cluster                      (h,{b:a},cc)
k,K  index  of clusters

{x:i} is short for {x_i:i=0,1,2,...,N}

Gu(y)       Gauss kernel (unnormalized)     exp(||x-y||/h)
nc          Normalizing constant            (2 * pi * h^2 )^(d/2)
G(y)        Gauss kernel                    Gu(y)/nc
gt(y)       Gauss transform (u) at y        sum{w G(y):i}
rp(y)       Rozenblatt Parzen estimate      gt(y)/N
rp(y)|w=1   RP estimate assuming w=1
nw(y)       Nadaray Watson estimate         rp(y) / rp(y)|w=1


The quantity nw(y) and rp(y) is an estimate of E[w|x=y].
In the special case that w=1, rp(y) is an estimate of pdf(x=y).
The quantity nw(y) and rp(y) is an estimate of E[w|x=y].
By convention, if we are to use nw, we will take w[0]=1.

Some literature defines the Gauss transform as sum{w Gu(y):i}.
Our definition provides the appropriate scaling in case h varies with i.

/////////////
/ Algorithm /
/////////////

* The basic idea:

First assume h_i = h, J=1 and fix {p_i:i}. The Fast Gauss transform
is based on a multivariate Taylor expansion centered at c, such that the
contribution of (x,y), can be separated into two components, one associated
with (x,c), and the other with (c,t):
  gta(y) = sum{b_a (1/h)^d exp|| (c-y)/h ||^2 (y-c/h)^a:|a|< p+1}
converges to g(y) as min{p_i:i} -> infty, where
  b_a = 2^a/a! sum{w_i exp(-||x_i-c||/h):i such that |a|<p_i+1}
which has complexity O(N). In particular, b_a is independent of y,
so to evaluate at {y:m=1,...,M}, total complexity is O(N+M).

* Space division

The greater ||x-c||, the greater p=min{p_i:i}, in oder
to keep the approximation error within a given bound. To keep p manageable,
the sources are split into K clusters, and the ifgt performed within each of
them.

Examples: Given a pre-set K, farthest point has complexity O(K N),
two phase (Feder and Greene, 1988) has complexity O(N log(K)).

* Degree truncation - the first source of error

See, for example, Section 4.2 in Raykar2006a

* Cutoff radius  - the second source error

To further lessen computations, fix ry, and ignore clusters such that
||c_k-y||>ry. Note that their contribution is less than exp(-ry).

As per Raykar2006a, Section 4.1, to keep the error within eps:
  ry > rx + min(R,h sqrt(log(1/eps)),
where R=max{||x-y||:all i,m}

* Relaxing J=1

We can allow multiple weight sequences i.e. each training data point is
 (x,{w:j}). A separate approximation is needed for for each j, but common
computations are mutualized.

* Relaxing h_i=h

If h_i be piecewise constant, the ifgt is computed within each set, and the
sum taken over each of the set members. We call "active
range" the range of clusters of the current h.

* Accumulating data sequentially

The training data may be available all at once, or it may be necessary to
collect the sources' contributions sequentially (more general), whereby a new
cluster is created, if the nearest to that of the current input is
is farther than some pre--set value, rx.

* Monomials:
The IFGT coefficients and the approximation involve the monomials
{z^a:|a|< p+1}, for z=(x-c)/h, and z=(y-c)/h, respectively. For efficiency,
Hoerner's rule is used.

* Cluster radius (equivalently  K)

Given h, Raykar2006a, Section 4.3 provide an algorithm that find the optimal
K, equivalently rx. This relies on certain assumptions and we have not
implemented it yet (TODO). As of know, we fix max{p_i:i} and solve for rx.

* Choosing h

For w=1, under normality of x, the statistically optimal bandwidth is

h_{N}=sigma(N(2+D)/4)^{-1/(D+4)} * sqrt(2)

The sqrt(2) is needed because the kernel is
exp(-||(x-y)/h||^2), not the usual normal kernel exp(-(1/2)||(x-y)/h||^2)

A common approach is to transform x[i]<-x[i]/std(x[i]) allowing sigma = 1.

See Silverman1986, Section 4.3.2

* Subsetting
We may want to evaluate the gt only for a subset of {j=0,...,J-1}. For
that reason, evaluators take a subset range policy.

/////////
/ Usage /
/////////

An Accumulator (fast or exact) is constructed using Boost.Parameter:
     A a((
            kwd<>::bandwidth = h,
            kwd<>::max_cluster_radius = rx,
            kwd<>::degree = p
        )
    );
A new contribution is passed to an Accumulator (either exact or fast):
    a(source,weight)
For convenience, this operation can be vectorized:
    for_each_accumulate(sources,weights,a)
    for_each_weight_front_insert_1_accumulate(sources,weights,a)
The insertion of 1 is needed for NW.
An Evaluator (fast or exact) is created around an accumulator:
    e(a);
Whose interface comprises free functions such as:
for_each_gauss_transform(targets,ranges_out,e)
for_each_nadaraya_watson(targets,ranges_out,e)
for_each_rozenblatt_parzen(targets,ranges_out,e)

//////////////
/ Concepts   /
//////////////

Cluster :
Associated type:
    result_of_center


Accumulator (A)
Associated types:
    A derives from crtp::accumulator<A>
    A::value_type
    A::sources_count_type
Valid expressions:  let a an lvalue of A, and source and weight those
of a ForwardRange,
    a.source_size                           const std::size_t
    a.weight_size                           const std::size_t
    active_bandwidth()                      value_type
    sources_count()                         sources_count_type
    e(source, weight)                       void
Models: fast/accumulator.hpp, exact/accumulator.hpp

Evaluator (E), e an lvalue of E,  C a type,
    E has public base evaluator_base<C,E>
Associated types:
    E::sources_count_type
Valid expressions: let target and range_out model ForwarRange.
    e.sources_count()                       sources_count_type
    e(target,range_out)                     void
Models: fast_evaluator, exact_evaluator
Comment: If A is the type of the accumulator that E builds upon,
C = evaluator_common<A> takes care of sources_count.

CuttoffRadiusPolicy (CR): f an an lvalue,
Associated type:
    value_type
Valid expressions:
h and rx instances of value_type
    f(bandwidth,max_radius) returns a RealType

TruncateDegreePolicy: f an lvalue,
Associated type:
    value_type
Valid expression: rx a value_type,
    f(C,rx,w,p) assigns a value to each member of p (one for each weight).

FindClusterPolicy (F)
Associate types:
r and source lvalues of a forward range R
F f(r)     constructor
f(x)       returns an iterator pointing to an element of r.

SubsetRangePolicy (S),
Associated types: R0 a ForwardRange,
   R= result_of<S(R0)>::type (another ForwardRange)
Valid expressions:
    f(r0)    returns an lvalue of R

////////////////////
/ Files            /
////////////////////
/benchmark.hpp
/bandwdith
    /normal_plug_in.hpp
/crtp
    /accumulator.hpp
    /evaluator.hpp
/cluster_radius
    /at_max_degree.hpp
/cutoff_radius
    /none.hpp
    /rydg.hpp
/detail
    /normal_kernel_properties.hpp
    /evaluator_common.hpp
/exact
    /accumulator.hpp
    /detail
        /contribution.hpp
            var: x, {w:j}, h
            member: evaluate
                arg:    t, out
                effect: adds {gt(t):j} to out
        /contributions_evaluator.hpp
    /evaluator.hpp
/fast
    /detail
        /cluster.hpp
            vars:    x, h, {{b:a}:j}
            arg:     x, {w:j},{p:j}
            effect:  updates {{b:a}:j}
        /cluster_call_center.hpp
        /cluster_evaluator.hpp
            vars:   t, {out:j}
            arg:    C
            effect: adds {gt(t):j} to {out:j}
        /clusters_evaluator.hpp
            arg:    t, {C},{out:j}
            effect: iterates over {C} and adds {gt(t):j} to {out:j}
        /coefficients.hpp
            var:    {b:a}
            arg:    ({zx^a},w)
            effect: updates {b:a}
        /coefficients_evaluator.hpp
            var:   t
            arg:   r, cc,h, {b:a}
            out:   gt(t)

        /accumulator.hpp
        /evaluator.hpp
        /find_nearest_cluster.hpp:
            var: {cc}
            arg: x
            out: cc nearest to x
/for_each
    /accumulate.hpp
    /evaluate.hpp
/functor
    /evaluate.hpp
    /gauss_transform.hpp
    /nadaraya_watson_estimate.hpp
    /rozenblatt_parzen_estimate.hpp
/include.hpp
/keyword.hpp
/truncation_degree
    /constant.hpp
    /properties.hpp

////////
/ TODO /
////////
More testing.

Design:
- Add cutoff radius policy to exact::evaluator
- Predict how parameters affect resource
requirement (memory, in particular, easily becomes a problem) and precision.
- For now, the bandwidth is optimized only in a statistical sense as a
function of N. What is needed is a per-computation-time optimization.
- Optimal p and K, simulatneaously

///////////
/ History /
///////////

March 24th, 2009 : Simplified the interface.

/////////////
/ Reference /
/////////////

@techreport{Raykar2006a,
	title = {Fast computation of sums of gaussians in high dimensions},
	author = {Vikas Chandrakant Raykar and Changjiang Yang and Ramani
	Duraiswami and Nail Gumerov},
	institution = {Department of Computer Science and Institute for Advanced
	Computer
	Studies, University of Maryland},
	month = {April},
	number = {CS-TR-4767/UMIACS-TR-2005-69},
	url = {http://www.umiacs.umd.edu/~vikas/publications/CS-TR-4767.pdf},
	year = {2006},
	biburl =
    {http://www.bibsonomy.org/bibtex/21702c6cf15de590feaed72075225555a/
    marcoalvarez},
	abstract = {Evaluating sums of multivariate Gaussian kernels is a key
	computational
	task in many problems in computational statistics and machine learning.
	The computational cost of the direct evaluation of such sums scales
	as the product of the number of kernel functions and the evaluation
	points. The fast Gauss transform proposed by Greengard and Strain
	(1991) is a e-exact approximation algorithm that reduces the computational
	complexity of the evaluation of the sum of N Gaussians at M points
	in d dimensions from O(MN) to O(M+N). However, the constant factor
	in O(M+N) grows exponentially with increasing dimensionality d, which
	makes the algorithm impractical for dimensions greater than three.
	In this paper we present a new algorithm where the constant factor
	is reduced to asymptotically polynomial order. The reduction is based
	on a new multivariate Taylor's series expansion (which can act both
	as a local as well as a far field expansion) scheme combined with
	the efficient space subdivision using the k-center algorithm. The
	proposed method differs from the original fast Gauss transform in
	terms of a different factorization, effcient space subdivision, and
	the use of point-wise error bounds. Algorithm details, error bounds,
	procedure to choose the parameters and numerical experiments are
	presented. As an example we shows how the proposed method can be
	used for very fast e-exact multivariate kernel density estimation.},
	timestamp = {2007.11.15}, owner = {Marco},
	keywords = {Gaussian }
}

@book{Silverman1986,
 title =	"Density Estimation for Statistics and Data Analysis",
 author =	"B. W. Silverman and P. J. Green",
 publisher =	"Chapman and Hall",
 address =	"London",
 year = 	1986
}

/////////////////
/ Sample output /
/////////////////
pdf(x) = prod{N(x[d]|0,1),d=0,...,D} esimated by Rozenblatt-Parzen
f(x) = norm(x) = sqrt(x[0]^2+...+x[D]^2) estimated by Nadaraya-Watson

M: # evaluated
N: # accumulated
err0(a,b) = max {|a - b|:m=0,...,M-1 }
err1(a,b) = sqrt sum {|a - b|^2:m=0,...,M-1 } / M
e0_rp : err0(rp(y), pdf(y))
e1_rp : err1(rp(y), pdf(y))
e0_nw : err0(nw(y), w[1](y))
e1_nw : err1(nw(y), w[1](y))
Quantities below are scaled to reflect N=M=1e3
ta :  time accumulate {(x,w):i=0,...,N-1}
trp : time {rp(y):m = 0,..., M-1}
trp : time {nw(y):m = 0,..., M-1}



-> example_benchmark_exact
dim(x) = 3
dim(w) = 2
bandwidth = 0.547937
N          e0_rp      e1_rp     e0_nw      e1_nw
 2,500,000 1.004e-02  3.508e-04 3.901e-01  1.670e-02
 5,000,000 9.965e-03  3.487e-04 3.911e-01  1.672e-02
 7,500,000 9.996e-03  3.494e-04 3.914e-01  1.672e-02
10,000,000 9.988e-03  3.490e-04 3.928e-01  1.673e-02

N          ta         trp        tnw
 2,500,000 1.033e-02 1.257e+04  1.235e+04
 5,000,000 1.036e-02 2.514e+04  2.472e+04
 7,500,000 1.310e-02 3.772e+04  3.702e+04
10,000,000 7.707e-03 7.316e+03  6.385e+03
<-

 -> example_benchmark_fast
cutoff radius = 0
dim(x) = 3
dim(w) = 2
bandwidth =0.547937
max_rx=0.456257


N          e0_rp      e1_rp      e0_nw      e1_nw
 2,500,000 4.004e-02  1.793e-03 3.930e-01  1.672e-02
 5,000,000 4.002e-02  1.793e-03 3.925e-01  1.673e-02
 7,500,000 4.003e-02  1.793e-03 3.924e-01  1.673e-02
10,000,000 4.003e-02  1.793e-03 3.936e-01  1.673e-02

N          ta         trp        tnw        K
2500000    2.133e-01 2.252e+01  4.439e+01 2469
5000000    2.491e-01 2.540e+01  4.948e+01 2750
7500000    2.617e-01 2.686e+01  5.291e+01 2901
10000000   2.705e-01 2.978e+01  5.626e+01 3017

<-
