//////////////////////////////////////////////////////////////////////////////
// ifgt/evaluator_base.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_EVALUATOR_BASE_HPP_ER_2009
#define BOOST_MATH_IFGT_EVALUATOR_BASE_HPP_ER_2009
#include <functional>
#include <cmath>
#include <vector>
#include <algorithm>
#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/utility.hpp>
#include <boost/iterator/range_left.hpp>
#include <boost/iterator/range_right.hpp>
#include <boost/iterator/range_same.hpp>
#include <boost/math/ifgt/tag.hpp>
namespace boost{namespace math{namespace ifgt{

template<typename Common,typename Derived>
class evaluator_base : public Common{
    typedef Common                                          common_type;

    public:
    typedef typename Common::value_type                     value_type;
    static const std::size_t weight_size = Common::weight_size;

    static const value_type default_nadaraya_watson_check_tolerance(){
        static value_type val = 1e-15;
        return val; };

    template<typename ArgPack>
    evaluator_base(const ArgPack& arg)
    :common_type(arg),
    nadaraya_watson_check_tolerance_(arg[
        tag::nadaraya_watson_check_tolerance
            |default_nadaraya_watson_check_tolerance()]){}

    evaluator_base(const evaluator_base& that)
    :common_type(that),nadaraya_watson_check_tolerance_(
        that.nadaraya_watson_check_tolerance_){};

    evaluator_base& operator=(const evaluator_base& that){
        if(&that!=this){
            common_type::operator=(that);
            nadaraya_watson_check_tolerance_
                = that.nadaraya_watson_check_tolerance_;
        }
        return *this;
    }

    /// \warning adds to range_out
    template<typename R0,typename R1>
    void gauss_transform(const R0& target,R1& range_out)const{
        static range_same policy;
        return this->gauss_transform(target,range_out,policy);
    };

    /// \warning adds to range_out
    template<typename R0,typename R1,typename SubsetRangePolicy>
    void gauss_transform(
            const R0& target,
            R1& range_out,
            const SubsetRangePolicy& policy
    )const{
        const Derived& self = static_cast<const Derived&>(*this);
        return self.call_impl(target,range_out,policy);
    };

    template<typename R0>
    value_type rozenblatt_parzen_estimate(const R0& target)const{
        typedef std::vector<value_type> range_type;
        typedef range_left<1> policy_type;
        static range_type range(1);
        static policy_type policy;
        range[0] = (value_type)(0);
        this->gauss_transform(target,range,policy);
        value_type count = (value_type)(this->sources_count());
        range[0]/=count;
        return range[0];
    };

    /// Requires weight[0]==1
    /// Initializes range_out to zero and writes NW to it
    template<typename R0,typename R1>
    void nadaraya_watson_estimate(
        const R0& target,R1& range_out)const{
        typedef std::vector<value_type> range_type;
        typedef range_right<weight_size-1> policy_type;
        typedef typename result_of<policy_type(const range_type&)>::type
                                                            range_right_type;
        static range_type range(weight_size);
        static policy_type policy;
        BOOST_ASSERT(size(range_out)-(weight_size-1)==0);
        std::fill(
            begin(range),
            end(range),
            (value_type)(0)
        );
        this->gauss_transform(target,range);
        value_type denom = range[0];

        range_right_type range_right = policy(range);
        BOOST_ASSERT(size(range_right)-(weight_size-1)==0);
        std::transform(
            begin(range_right), end(range_right),
            begin(range_out),
            bind(std::divides<value_type>(), _1, denom)
        );
    };

    /// Writes rozenblatt_parzen to range_out[0]
    /// and NW to {range_out[j]:j=1,...,J-1}
    /// Less practical but more efficient than each separately.
    template<typename R0,typename R1>
    void rozenblatt_parzen_and_nadaraya_watson_estimate(
        const R0& target,R1& range_out)const{
        typedef std::vector<value_type>                     range_type;
        typedef range_right<weight_size-1>                  policy_type;
        typedef typename range_iterator<R1>::type           iter1_type;
        typedef typename result_of<policy_type(const range_type&)>::type
                                                            range_right_type;
        static range_type  range(weight_size);
        static policy_type policy;
        std::fill(begin(range),end(range),(value_type)(0));
        this->gauss_transform(target,range);
        BOOST_ASSERT(size(range_out)-weight_size==0);
        value_type denom = range[0];
        value_type count = (value_type)(this->sources_count());;
        iter1_type iter1 = begin(range_out);
        (*iter1) = denom / count;
        std::advance(iter1,1);
        range_right_type range_right = policy(range);
        std::transform(
            begin(range_right), end(range_right), iter1,
            bind(std::divides<value_type>(), _1, denom)
        );
    };

    private:
    evaluator_base();
    value_type nadaraya_watson_check_tolerance_;

};

}}}
#endif
