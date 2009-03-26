//////////////////////////////////////////////////////////////////////////////
// coefficients.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_SOURCE_COEFFICIENTS_HPP_ER_2009
#define BOOST_MATH_IFGT_SOURCE_COEFFICIENTS_HPP_ER_2009
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>
#include <cmath>
#include <boost/range.hpp>
#include <boost/range/value_type.hpp>
#include <boost/static_assert.hpp>
#include <boost/bind.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/math/monomials.hpp>
#include <boost/math/multi_indexes_derived.hpp>
namespace boost{namespace math{namespace ifgt{

    // coefficients keeps a set of coefficients and a max_degree
    // Each time (zsqn, monoms, w,p) is passed to the object,
    // the coefficients {C_\alpha:\alpha_0+...+\alpha_{SourceSz-1}<=p}
    // are updated.
    // Specifically,
    //  C_{\alpha} +=
    //   2^{\alpha}/\alpha! * w exp(-zsqn ) * z^alpha
    // The z^alpha's are the elements in monoms
    // zsqn represents || z ||_2^2
    //
    // The reason for this set up (zsqn and monoms passed to the object)
    // is that this operation may be repeated
    // for different weight sequences (one coefficients for each).

    template<unsigned SourceSz,typename Cont = std::vector<double> >
    class coefficients{
        typedef typename Cont::value_type                    value_type;
        typedef multi_indexes_derived<SourceSz,multi_power2divfact_op>
                                                             mult_p2df;
        typedef typename mult_p2df::iter_range_type          p2df_ir_type;
        typedef
            typename range_iterator<p2df_ir_type>::type      p2df_iter_type;
        typedef
            typename range_value<p2df_ir_type>::type         p2df_value_type;
     public:
        typedef Cont                                        coefficients_type;
        typedef const coefficients_type&                    result_type;

        coefficients():max_degree_(0){}
        coefficients(const coefficients& that)
        :max_degree_(that.max_degree_),coeffs_(that.coeffs_){}
        coefficients& operator=(const coefficients& that){
            if(&that!=this){
                max_degree_ = that.max_degree_;
                coeffs_     = that.coeffs_;
            }
            return *this;
        }

        unsigned max_degree()const{ return max_degree_; }

        result_type operator()()const{ return coeffs_; }

        template<typename R>
        void operator()(
            value_type zscore_sqnorm,
            const R& monomials,
            value_type weight,
            unsigned degree
        ){
            typedef typename range_iterator<const R>::type monoms_iter_type;
            typedef typename range_value<R>::type monoms_value_type;

            value_type c = exp(-zscore_sqnorm) * weight;
            p2df_ir_type p2df_ir = mult_p2df::get(degree);
            std::size_t adv_by
                = monomials_properties<>::number_degree_less_than(
                    degree,SourceSz);
            if(adv_by>((std::size_t)(size(coeffs_)))){
                coeffs_.resize(adv_by,(value_type)(0));
            }

            BOOST_ASSERT( adv_by <= ((std::size_t)(size(monomials))) );
            BOOST_ASSERT( adv_by <= ((std::size_t)(size(p2df_ir))) );
            monoms_iter_type m_e =  begin(monomials);
            p2df_iter_type  p2df_e   =
                            begin(p2df_ir);
            std::advance(m_e,adv_by);
            std::advance(p2df_e,adv_by);

            inner_op<monoms_value_type> io(c);

            transform(
                make_zip_iterator(
                    make_tuple(
                        begin(monomials),
                        begin(p2df_ir)
                    )
                ),
                make_zip_iterator(
                    make_tuple(
                        m_e,
                        p2df_e
                    )
                ),
                begin(coeffs_),
                begin(coeffs_),
                bind(
                    std::plus<value_type>(),
                    bind(
                        io,//ref?
                        _1
                    ),
                    _2
                 )
            );

            if(max_degree()<degree){max_degree_ = degree;}
        }

     private:
        template<typename V>
        struct inner_op{
            typedef tuple<
                const V&,const p2df_value_type&> argument_type;

            typedef V                                  result_type;
            explicit inner_op(result_type c_):c(c_){}
            inner_op(const inner_op& that):c(that.c){}
            inner_op& operator=(const inner_op& that){
                if(&that!=this){ c=that.c;}
                return *this;}
            result_type operator()(argument_type t){
                const V&                mon  = t.template get<0>();
                const p2df_value_type&  p2df = t.template get<1>();

                return mon * ((result_type)(p2df)) * c;
            }
            result_type c;
        };//inner_op

        unsigned                max_degree_;
        coefficients_type       coeffs_;
    };
}}}
#endif
