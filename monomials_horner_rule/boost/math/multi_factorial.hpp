//////////////////////////////////////////////////////////////////////////////
// multi_factorial.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_MULTI_FACTORIAL_HPP_ER_2009
#define BOOST_MATH_MULTI_FACTORIAL_HPP_ER_2009
#include <numeric>
#include <boost/math/multi_indexes.hpp>
#include <boost/math/special_functions/factorials.hpp>

namespace boost{namespace math{


    // This is deprecated
    // Use multi_indexed_derived instead
    template<unsigned int SourceSz>
    class multi_factorial{
      public:
        typedef std::vector<unsigned>                   storage_type;
        typedef typename storage_type::iterator         iter_type;
        typedef iterator_range<iter_type>               iter_range_type;
        typedef typename storage_type::value_type       value_type;
        typedef typename storage_type::size_type        size_type;

       static iter_range_type get(unsigned int degree){
            static unsigned max_degree = 0;
            static storage_type storage(1,(value_type)(0)); //initial value
            size_type dist
                = monomials_properties<>::number_degree_less_than(
                    degree,SourceSz);
            if(max_degree<degree){
                typedef multi_indexes<SourceSz>                      mi_type;
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
        multi_factorial(); //do not implement

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
                    (value_type)(1), //initial value
                    inner_op()
                );
            }
            iter_type write_begin;
            struct inner_op{
                typedef value_type                      result_type;
                typedef value_type                      first_argument_type;
                typedef value_type                      second_argument_type;
                inner_op(){}
                result_type operator()(
                    first_argument_type a1,
                    second_argument_type a2)const{
                        return a1 * (result_type)(factorial<double>(a2));
                }
            };
        };
    };
}}

#endif
