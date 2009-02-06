//////////////////////////////////////////////////////////////////////////////
// multi_indexes_derived.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MULTI_INDEXES_DERIVED_HPP_ER_2009
#define BOOST_MULTI_INDEXES_DERIVED_HPP_ER_2009
#include <numeric>
#include <cmath>
#include <boost/math/multi_indexes.hpp>
#include <boost/math/special_functions/factorials.hpp>

namespace boost{namespace math{

// Multi indexes derived are functions of multi_indexes
// The particular function is controled by a policy:
// multi_factorial_op :    (a,b,c) --> a! b! c!
// multi_degree_op:        (a,b,c) --> a + b + c (also called total degree)
// multi_power2divfact_op  (a,b,c) --> 2^{a+b+c}/(a! b! c!)

            struct multi_factorial_op{
                typedef unsigned                      result_type;
                typedef unsigned                      first_argument_type;
                typedef unsigned                      second_argument_type;
                static result_type initial_value(){
                    static result_type iv = (result_type)(1);
                    return iv;
                }
                multi_factorial_op(){}
                result_type operator()(
                    first_argument_type a1,
                    second_argument_type a2)const{
                        return a1 * (result_type)(factorial<double>(a2));
                }
            };
            struct multi_degree_op:public std::plus<unsigned>{
                static unsigned initial_value(){
                    static unsigned iv = (unsigned)(0);
                    return iv;
                }
            };
            struct multi_power2divfact_op{
                typedef double                      result_type;
                typedef double                      first_argument_type;
                typedef unsigned                    second_argument_type;
                static result_type initial_value(){
                    static result_type iv = (result_type)(1);
                    return iv;
                }
                multi_power2divfact_op(){}
                result_type operator()(
                    first_argument_type a1,
                    second_argument_type a2)const{
                        result_type d = std::pow(2.0,(result_type)(a2));
                        result_type f = factorial<result_type>(a2);
                        result_type r = d/f;
                        return a1 * ((result_type)(r));
                }
            };


    template<unsigned int SourceSz,typename Policy>
    class multi_indexes_derived{
      public:
        typedef typename Policy::result_type            value_type;
        typedef std::vector<value_type>                 storage_type;
        typedef typename storage_type::iterator         iter_type;
        typedef iterator_range<iter_type>               iter_range_type;
        typedef typename storage_type::size_type        size_type;

       static iter_range_type get(unsigned int degree){
            static unsigned max_degree = 0;
            static storage_type storage(1,Policy::initial_value());
            size_type dist
                = monomials_properties<>::number_degree_less_than(
                    degree,SourceSz);
            if(max_degree<degree){
                typedef multi_indexes<SourceSz>            mi_type;
                typedef
                    typename mi_type::iter_range_type      mi_iter_range_type;
                mi_iter_range_type ir = mi_type::get(degree);
                storage.clear();
                transform(
                    begin(ir),
                    end(ir),
                    back_inserter(storage),
                    unary_op()
                );
                max_degree = degree;
            }

            iter_type b(begin(storage));
            iter_type e(b); std::advance(e,dist);

            return iter_range_type(b,e);
        }
      private:
        multi_indexes_derived(); //do not implement

        typedef multi_indexes<SourceSz>  mi_type;
        typedef typename mi_type::iter_range_type       mi_iter_range_type;
        typedef typename mi_type::iter_type             mi_iter_type;
        typedef typename mi_type::value_type            mi_value_type;
        typedef typename mi_type::base_iter_type        mi_base_iter_type;
        typedef typename mi_type::base_value_type       mi_base_value_type;

        struct unary_op{
            typedef mi_value_type                       argument_type;
            typedef value_type                          result_type;
            unary_op(){}
            result_type operator()(argument_type ir){
                return accumulate(
                    begin(ir),
                    end(ir),
                    Policy::initial_value(), //initial value
                    Policy()
                );
            }
            iter_type write_begin;
        };
    };
}}

#endif
