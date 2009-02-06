//////////////////////////////////////////////////////////////////////////////
// multi_indexes.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MULTI_INDEXES_HPP_ER_2009
#define BOOST_MULTI_INDEXES_HPP_ER_2009
#include <vector>
#include <functional>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/range.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/bind.hpp>
#include <boost/assert.hpp>
#include <boost/iterator/vector2matrix_iterator.hpp>
#include <boost/math/monomials_properties.hpp>


namespace boost{namespace math{

    // Example of multi indexes for SourceSz = 3, degree =2
    // >(0,0,0)
    // >(1,0,0) >(0,1,0) >(0,0,1)
    // >(2,0,0) (1,1,0) (1,0,1) >(0,2,0) (0,1,1) >(0,0,2)
    // Note that
    // {(2,0,0) (1,1,0) (1,0,1)} = {(1,0,0) (0,1,0) (0,0,1)} + (1,0,0)
    // {(0,2,0) (0,1,1)} = {(0,1,0) (0,0,1)} + (0,1,0) etc.

    template<unsigned int SourceSz>
    class multi_indexes{
      public:
        typedef std::vector<unsigned>                   storage_type;
        typedef typename storage_type::iterator         base_iter_type;
        typedef typename storage_type::size_type        size_type;
        typedef storage_type::value_type                base_value_type;

        typedef vector2matrix_iterator<base_iter_type>  iter_type;
        typedef std::vector<iter_type>                  iters_type;
        typedef typename iter_type::value_type          value_type;
        typedef iterator_range<iter_type>               iter_range_type;
        typedef typename iters_type::iterator           iters_iter_type;
        typedef typename iter_type::difference_type     iter_diff_type;
        typedef std::vector<iter_diff_type>             iters_diff_type;
        typedef typename iters_diff_type::iterator      iters_diff_iter_type;

        static iter_range_type get(unsigned int degree){
            static unsigned max_degree = 0;
            static storage_type storage(SourceSz,(base_value_type)(0));
            static iters_diff_type read_begins_dists(SourceSz,0);

// // keeping read_begins rather than read_begins_dists causes
// // error 'gliblc detected' at runtime
//            static iters_type   read_begins(
//                SourceSz,
//                make_vector2matrix_iterator(
//                    begin(storage),
//                    SourceSz
//            ));

            //e.g. SourceSz=3: (1,0,0, 0,1,0, 0,0,1)
            static storage_type variables = variables_init();

            if(degree>max_degree){
                unsigned i = max_degree+1;
                unsigned n = degree+1;
                size_type sz_old = size(storage)/SourceSz;
                size_type sz_new
                    = properties::number_degree_less_than(degree,SourceSz);
                storage.resize(sz_new*SourceSz);
                iter_type write_begin = make_vector2matrix_iterator(
                    begin(storage),
                    SourceSz
                );
                std::advance(write_begin,sz_old);

                iters_type read_begins;

//old implem, do not uncomment
//                iters_diff_iter_type b = begin(read_begins_dists);
//                iters_diff_iter_type e = end(read_begins_dists);
//                for(iters_diff_iter_type i = b; i<e; i++){
//                    iter_type it(begin(storage),SourceSz);
//                    std::advance(it,*i);
//                    read_begins.push_back(it);
//                }

                transform(
                    begin(read_begins_dists),
                    end(read_begins_dists),
                    back_inserter(read_begins),
                    unary_op(
                        make_vector2matrix_iterator(
                            begin(storage),
                            SourceSz
                        )
                    )
                );

                while(i<n){
                    BOOST_ASSERT(size(read_begins_dists)==SourceSz);
                    BOOST_ASSERT(std::distance(
                        make_vector2matrix_iterator(
                            begin(variables),
                            SourceSz
                        ),
                        make_end_vector2matrix_iterator(
                                begin(variables),
                                end(variables),
                                SourceSz
                        )
                    )==SourceSz);

                    //(***) bug
                    write_begin = for_each(
                        make_zip_iterator(
                            make_tuple(
                                begin(read_begins),
                                make_vector2matrix_iterator(
                                    begin(variables),
                                    SourceSz
                                )
                            )
                        ),
                        make_zip_iterator(
                            make_tuple(
                                end(read_begins),
                                make_end_vector2matrix_iterator(
                                    begin(variables),
                                    end(variables),
                                    SourceSz
                                )
                            )
                        ),
                        zip_func(write_begin)
                    ).write_begin;
                    ++i;
                }
                read_begins_dists.clear();
                std::transform(
                    begin(read_begins),
                    end(read_begins),
                    back_inserter(read_begins_dists),
                    bind(
                        std::distance<iter_type>,
                        make_vector2matrix_iterator(
                            begin(storage),
                            SourceSz
                        ),
                        _1
                    )
                );
                max_degree = degree;
            }//if(degree>max_degree)

            size_type dist
                = properties::number_degree_less_than(degree,SourceSz);
            iter_type b(begin(storage),SourceSz);
            iter_type e = b;
            std::advance(e,dist);
            iter_range_type(b,e);
            return iter_range_type(b,e);
        }//get(unsigned degree)

      private:
        typedef monomials_properties<> properties;
        multi_indexes(); //do not implement

        static storage_type variables_init(){
            storage_type variables(SourceSz*SourceSz,0);
            for(unsigned i=0; i<SourceSz; i++){
                variables[i*SourceSz+i]=1;
            }
            return variables;
        }

        struct unary_op{
            iter_type b;
            unary_op(iter_type b_):b(b_){}
            unary_op(const unary_op& that):b(that.b){}
            unary_op& operator=(const unary_op& that){
                if(&that!=this){b=that.b;}
                return *this;
            }
            typedef iter_type                   result_type;
            typedef const iter_diff_type&       argument_type;
            result_type operator()(argument_type diff)const{
                result_type it = b;
                std::advance(it,diff);
                return it;
            }
        };

        struct zip_func
        {
            typedef const tuple<
                iter_type&,const value_type&>&                  argument_type;
            typedef void                                        result_type;

            iter_type write_begin;
            iter_type read_end;
            unsigned cnt;//TODO remove

            zip_func(iter_type write_begin_)
            :write_begin(write_begin_),read_end(write_begin),cnt(0){}

            zip_func(const zip_func& that)
            :write_begin(that.write_begin),
            read_end(that.read_end),
            cnt(that.cnt){}

            zip_func& operator=(const zip_func& that){
                if(&that!=this){
                    write_begin = that.write_begin;
                    read_end = that.read_end;
                    cnt = that.cnt;
                }
                return *this;
            }

            result_type operator()(argument_type t){
                value_type variable_ir = (t.template get<1>());
                iter_type read_begin = (t.template get<0>());
                t.template get<0>() = write_begin;
                write_begin =
                    for_each(
                        read_begin,
                        read_end,
                        unary_op(write_begin,variable_ir)
                    ).write_begin;
            }

            struct unary_op{
                unary_op(iter_type write_begin_,value_type variable_ir)
                :write_begin(write_begin_),variable(variable_ir){}

                unary_op(const unary_op& that)
                :write_begin(that.write_begin),variable(that.variable){}

                unary_op& operator=(const unary_op& that)
                {
                    if(&that!=this){
                        write_begin = that.write_begin;
                        variable = that.variable;
                    }
                    return *this;
                }

                typedef typename iter_type::value_type     argument_type;
                typedef void                               result_type;
                result_type operator()(argument_type ir){

                    BOOST_ASSERT(size(*write_begin)==size(ir));
                    BOOST_ASSERT(size(ir)==size(variable));
                    std::transform(
                        begin(ir),
                        end(ir),
                        begin(variable),
                        begin(*write_begin),
                        bind(
                            std::plus<base_value_type>(),
                            _1,
                            _2
                        )
                    );
                    ++write_begin;
                }
                iter_type   write_begin;
                value_type  variable;
            };//unary_op
        };//zip_func
    };//multi_indexes
}}


#endif
