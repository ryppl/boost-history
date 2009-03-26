///////////////////////////////////////////////////////////////////////////////
// benchmark.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_BENCHMARK_HPP_ER_2009
#define BOOST_MATH_IFGT_BENCHMARK_HPP_ER_2009
#include <ostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iterator>
#include <boost/timer.hpp>
#include <boost/static_assert.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/math/distributions/normal.hpp>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>

#include <boost/ref.hpp>
#include <boost/function.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/linfinity_distance.hpp>
#include <boost/algorithm/l2_distance_squared.hpp>
#include <boost/iterator/vector2matrix_iterator.hpp>
#include <boost/algorithm/l2_norm.hpp>
#include <boost/math/ifgt/for_each/accumulate.hpp>
#include <boost/math/ifgt/for_each/nadaraya_watson.hpp>
#include <boost/math/ifgt/for_each/gauss_transform.hpp>
#include <boost/math/ifgt/for_each/rozenblatt_parzen.hpp>
#include <boost/math/ifgt/for_each/weight_front_insert_1_accumulate.hpp>
#include <boost/math/ifgt/detail/product_pdf.hpp>
namespace boost{namespace math{namespace ifgt{
template<
typename RealType,
std::size_t test_count,
std::size_t SourceSize,
std::size_t WeightSize = 2,
typename Pdf_fun = product_pdf<math::normal_distribution<RealType> >,
typename W_fun   = l2_norm
>
class benchmark{
    //For now the implementation of benchmark only allows
    BOOST_STATIC_ASSERT(WeightSize == 2); //hint :W_fun = l2_norm

    public:
    typedef RealType                                    value_type;
    typedef std::vector<value_type>                     var_type;
    typedef linfinity_distance::type                    linf_dist_type;
    typedef l2_distance_squared::type                   l2_dist_sq_type;
    typedef Pdf_fun                                     pdf_fun_type;
    typedef W_fun                                       w_fun_type;
    typedef timer                                       timer_type;

    typedef std::size_t                                 size_type;
    typedef std::vector<size_type>                      sizes_type;

    static const size_type sdim = SourceSize;
    static const size_type wdim = WeightSize;

    template<typename G>
    benchmark(const G& g,const pdf_fun_type& pdf_fun,const w_fun_type& w_fun)
     :
        cumulative_train_count_(1),
        train_x_(0),//(train_count_ * sdim),
        train_w_(0),//(train_count_ * (wdim-1)),
        test_x_(test_count  * sdim),
        test_pdf_(test_count * 1),
        test_w_(test_count * (wdim-1)),
        est_pdf_(size(test_pdf_)),
        est_w_(size(test_w_)),
        pdf_fun_(pdf_fun),
        w_fun_(w_fun),
        acc_time_per_1e3_train_count_(0),
        eval_rp_time_per_1e3_test_count_(0),
        eval_nw_time_per_1e3_test_count_(0),
        e0_rp_((value_type)(0)),
        e0_nw_((value_type)(0)),
        e1_rp_((value_type)(0)),
        e1_nw_((value_type)(0)),
        linf_dist_(),
        l2_dist_sq_()
        {
            generate_data(g,size(test_x_),test_x_);
            update_pdf(test_x_,test_pdf_);
            update_w(test_x_,test_w_);
        }

    benchmark(const benchmark& that)
     :
        cumulative_train_count_(that.cumulative_train_count_),
        train_x_(that.train_x_),
        train_w_(that.train_w_),
        test_x_(that.test_x_),
        test_pdf_(that.test_pdf_),
        test_w_(that.test_w),
        est_pdf_(that.est_pdf_),
        est_w_(that.est_w_),
        pdf_fun_(that.pdf_fun_),
        w_fun_(that.w_fun_),
        acc_time_per_1e3_train_count_(that.acc_time_per_1e3_train_count_),
        eval_rp_time_per_1e3_test_count_(
            that.eval_rp_time_per_1e3_test_count_
        ),
        eval_nw_time_per_1e3_test_count_(
            that.eval_nw_time_per_1e3_test_count_
        ),
        e0_rp_(that.e0_rp_),
        e0_nw_(that.e0_nw_),
        e1_rp_(that.e1_rp_),
        e1_nw_(that.e1_nw),
        linf_dist_(that.linf_dist_),
        l2_dist_sq_(that.l2_dist_sq_)
        {}

    benchmark& operator=(const benchmark& that){
        if(&that!=this){
            cumulative_train_count_ = that.cumulative_train_count_;
            train_x_    = that.train_x_;
            train_w_    = that.train_w_;
            test_x_     = that.test_x_;
            test_pdf_   = that.test_pdf_;
            test_w_     = that.test_w;
            est_pdf_    = that.est_pdf_;
            est_w_      = that.est_w_;
            pdf_fun_    = that.pdf_fun_;
            w_fun_      = that.w_fun_;
            acc_time_per_1e3_train_count_
                = that.acc_time_per_1e3_train_count_;
            eval_rp_time_per_1e3_test_count_
                =  that.eval_rp_time_per_1e3_test_count_;
            eval_nw_time_per_1e3_test_count_
                =  that.eval_nw_time_per_1e3_test_count_;
            e0_rp_ = that.e0_rp_;
            e0_nw_ = that.e0_nw_;
            e1_rp_ = that.e1_rp_;
            e1_nw_ = that.e1_nw_;
            linf_dist_ = that.linf_dist_;
            l2_dist_sq_ = that.l2_dist_sq_;
        }
        return *this;
    }

    template<typename Acc,typename G>
    void accumulate(G& g,size_type n,Acc& acc){
        train_x_.resize(sdim * n);
        train_w_.resize((wdim-1)*n);
        generate_data(g,n * sdim,train_x_);
        update_w(train_x_,train_w_);
        // accumulate
        timer_.restart();
        //w[1] --> {w[0]=1,w[1]}
        ifgt::for_each_weight_front_insert_1_accumulate(
            train_x_,
            train_w_,
            acc
        );
        acc_time_per_1e3_train_count_.push_back(
            timer_.elapsed() * 1e3 / n
        );
        std::size_t cn = cumulative_train_count_.back();
        cumulative_train_count_.push_back(n+cn);
    }
    template<typename Eval>
    void estimate_pdf(Eval& eval){//by Rozenblatt--Parzen
        timer_.restart();
        ifgt::for_each_rozenblatt_parzen(test_x_,est_pdf_,eval);
        eval_rp_time_per_1e3_test_count_.push_back(
            timer_.elapsed() * 1e3/test_count
        );

        value_type e0 = linf_dist_(test_pdf_,est_pdf_);
        value_type e1 = sqrt(l2_dist_sq_(test_pdf_,est_pdf_))
                /((value_type)(test_count));

        e0_rp_.push_back(e0);
        e1_rp_.push_back(e1);
    }

    template<typename Eval>
    void estimate_w(Eval& eval){//by Nadaraya--Watson
        timer_.restart();
        ifgt::for_each_nadaraya_watson(test_x_,est_w_,eval);
        eval_nw_time_per_1e3_test_count_.push_back(
                timer_.elapsed() * 1e3/test_count
        );
        value_type e0 = linf_dist_(test_w_,est_w_);
        value_type e1 = sqrt(l2_dist_sq_(test_w_,est_w_))
                /((value_type)(test_count));

        e0_nw_.push_back(e0);
        e1_nw_.push_back(e1);
    }

    void notation(std::ostream& os)const{
        os << "M: # evaluated"                              << std::endl;
        os << "N: # accumulated"                            << std::endl;
        os << "err0(a,b) = max {|a - b|:m=0,...,M-1 }"      << std::endl;
        os << "err1(a,b) = sqrt sum {|a - b|^2:m=0,...,M-1 } / M"<< std::endl;
        os << "e0_rp : err0(rp(y), pdf(y)) "                << std::endl;
        os << "e1_rp : err1(rp(y), pdf(y)) "                << std::endl;
        os << "e0_nw : err0(nw(y), w[1](y)) "               << std::endl;
        os << "e1_nw : err1(nw(y), w[1](y)) "               << std::endl;
        os << "Times are per  N=M=1e3"                      << std::endl;
        os << "ta :  time accumulate {(x,w):i=0,...,N-1}"   << std::endl;
        os << "trp : time {rp(y):m = 0,..., M-1} "          << std::endl;
        os << "trp : time {nw(y):m = 0,..., M-1} "          << std::endl;
    }

    void header(std::ostream& os)const{
        std::string str = "%|1$-10| %|2$-10| %|3$-10| %|4$-10| %|5$-10|";
        str+= "%|6$-10| %|7$-10| %|8$-10| ";
        os << boost::format(str)
            % "N"
            % "e0_rp"
            % "e1_rp"
            % "e0_nw"
            % "e1_nw"
            % "ta"
            % "trp"
            % "tnw";
    }
    void statistics(std::ostream& os, unsigned i){
        BOOST_ASSERT(size(e0_rp_)-i>0);
        std::string str = "%|1$-10.3| %|2$-10.3e| %|3$-10.3e|";
        str+= "%|4$-10.3e| %|5$-10.3e| %|6$-10.3e|";
        str+= "%|7$-10.3e| %|8$-10.3e|";
        os << boost::format(str)
            % cumulative_train_count_[i+1]           // 1
            % e0_rp_[i]                              // 2
            % e1_rp_[i]                              // 3
            % e0_nw_[i]                              // 4
            % e1_nw_[i]                              // 5
            % acc_time_per_1e3_train_count_[i]       // 6
            % eval_rp_time_per_1e3_test_count_[i]    // 7
            % eval_nw_time_per_1e3_test_count_[i] ;  // 8
    }
    private:
        template<typename G,typename R>
        void generate_data(G& g, size_type n,R& x){
            std::generate_n(
                begin(x),
                n,
                g
            );
        };
        template<typename R0,typename R1>
        void update_pdf(const R0& x,R1& pdf_out){
            std::transform(
                make_vector2matrix_iterator(begin(x),sdim),
                make_end_vector2matrix_iterator(
                    begin(x),
                    end(x),
                    sdim
                ),
                begin(pdf_out),
                bind<value_type>(
                    pdf_fun_,
                    _1
                )
            );
        }
        template<typename R0,typename R1>
        void update_w(const R0& x, R1& w_out){
            std::transform(
                make_vector2matrix_iterator(begin(x),sdim),
                make_end_vector2matrix_iterator(
                    begin(x),
                    end(x),
                    sdim
                ),
                begin(w_out),
                bind<value_type>(
                    w_fun_,
                    _1
                )
            );
        }

    sizes_type   cumulative_train_count_;

   // train data set
    //w[1] = f(x) + e. here e=0, f(x) = ||x|| = (x[0]^2+...+x[D-1]^2)^(1/2)
    var_type train_x_;
    var_type train_w_;

    // test data
    var_type test_x_;
    var_type test_pdf_;
    var_type test_w_;

    // estimates
    var_type est_pdf_;      // estimate of pdf
    var_type est_w_;        // estimate of {E[w[j]|x]:j>0}

    //
    pdf_fun_type    pdf_fun_;
    w_fun_type      w_fun_;

    //
    timer_type      timer_;

    //
    var_type    acc_time_per_1e3_train_count_ ;
    var_type    eval_rp_time_per_1e3_test_count_;
    var_type    eval_nw_time_per_1e3_test_count_;

    //
    var_type    e0_rp_;
    var_type    e0_nw_;
    var_type    e1_rp_;
    var_type    e1_nw_;

    linf_dist_type      linf_dist_;
    l2_dist_sq_type     l2_dist_sq_;

};

}}}

#endif
