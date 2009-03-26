//////////////////////////////////////////////////////////////////////////////
// cluster.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_CLUSTER_HPP_ER_2009
#define BOOST_MATH_IFGT_CLUSTER_HPP_ER_2009
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>
#include <cmath>
#include <boost/range.hpp>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/bind.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/algorithm/l2_norm.hpp>
#include <boost/math/monomials.hpp>
#include <boost/math/multi_indexes_derived.hpp>
#include <boost/math/ifgt/detail/zscore.hpp>
#include <boost/math/ifgt/keyword.hpp>
#include <boost/math/ifgt/fast/detail/coefficients.hpp>
#include <boost/math/ifgt/truncation_degree/constant.hpp>
#include <boost/math/ifgt//detail/normal_kernel_properties.hpp>
namespace boost{namespace math{namespace ifgt{

    // A cluster is characterized by
    //  - A center
    //  - A bandwidth
    //  - A collection of coefficients (one for each weight sequence)

    /// apply<TruncationDegreePolicy,T>::type
    template<
        unsigned SourceSize,
        unsigned WeightSize,
        typename TruncationDegreePolicy
            = truncation_degree::constant<mpl::_1>,
        typename Cont = std::vector<double> >
    class cluster{
        typedef monomials<Cont>                         monoms_type;
        typedef typename monoms_type::result_type       monoms_result_type;
        typedef coefficients<SourceSize,Cont>           coeffs_type;
        typedef std::vector<coeffs_type>                coll_coeffs_type;
        typedef typename coll_coeffs_type::iterator     coll_coeffs_iter_type;

      public:
        // C++ Standard (now) allows initialization of const static data
        // members of an integral type inside their class.
        static const std::size_t source_size     = SourceSize;
        static const std::size_t weight_size     = WeightSize;
        typedef Cont                                    center_type;
        typedef const center_type&                      result_of_center_type;
        typedef typename monoms_type::value_type        value_type;
        typedef coeffs_type                    coefficients_type;
        typedef coll_coeffs_type               collection_coefficients_type;
        typedef const collection_coefficients_type&
                                  result_of_collection_coefficients_type;
        typedef std::size_t                             sources_count_type;
        typedef normal_kernel_properties<source_size,value_type> rpp_type;
        typedef typename mpl::apply<TruncationDegreePolicy,value_type>::type
                                            truncation_degree_policy_type;

        template<typename ArgPack>
        cluster(const ArgPack& arg)
        :center_(source_size),
        bandwidth_(arg[kwd<>::bandwidth]),
        max_radius_(arg[kwd<>::max_cluster_radius]),
        truncation_degree_policy_(arg),
        inv_nc_((value_type)(1)/rpp_type::normalizing_constant(bandwidth())),
        source_radius_((value_type)(0)),
        radius_((value_type)(0)),
        sources_count_(0),
        coll_coeffs_(weight_size){
            BOOST_ASSERT(size(arg[kwd<>::center])-source_size==0);
            copy(
                begin(arg[kwd<>::center]),
                end(arg[kwd<>::center]),
                begin(center_)
            );
        }

        template<typename R>
        cluster(
            const R& center,
            value_type bandwidth_val,//cannot name it bandiwth coz mf
            value_type max_radius,
            const truncation_degree_policy_type& truncation_degree_policy)
        :center_(source_size),
        bandwidth_(bandwidth_val),
        max_radius_(max_radius),
        truncation_degree_policy_(truncation_degree_policy),
        inv_nc_((value_type)(1)/rpp_type::normalizing_constant(bandwidth())),
        source_radius_((value_type)(0)),
        radius_((value_type)(0)),
        sources_count_(0),
        coll_coeffs_(weight_size){
            BOOST_ASSERT(size(center)-source_size==0);
            copy(
                begin(center),
                end(center),
                begin(center_)
            );
        }

        cluster(const cluster& that)
        :center_(that.source_size),
        bandwidth_(that.bandwidth_),
        max_radius_(that.max_radius_),
        truncation_degree_policy_(that.truncation_degree_policy_),
        inv_nc_(that.inv_nc_),
        source_radius_(that.source_radius_),
        radius_(that.radius_),
        sources_count_(that.sources_count_),
        coll_coeffs_(that.coll_coeffs_){
            copy(
                begin(that.center_),
                end(that.center_),
                begin(center_)
            );
        }

        cluster& operator=(const cluster& that){
            if(&that!=this){
                center_ = that.center_;
                bandwidth_ = that.bandwidth_;
                max_radius_ = that.max_radius_;
                truncation_degree_policy_ = that.truncation_degree_policy_;
                inv_nc_ = that.inv_nc_;
                source_radius_ = that.source_radius_;
                radius_ = that.radius_;
                sources_count_ = that.sources_count_;
                coll_coeffs_ = that.coll_coeffs_;
            }
            return *this;
        }

        result_of_center_type center()const{ return center_; }
        value_type bandwidth()const{ return bandwidth_; }
        /// Maximum allowed cluster radius
        value_type max_radius()const{ return max_radius_; }

        value_type inverse_normalizing_constant()const{ return inv_nc_; }

        /// Maximum radius among collected sources
        value_type radius()const{ return radius_; }
        /// Number of sources collected
        sources_count_type sources_count()const{ return sources_count_; }

        /// Radius of the source in the last argument that was passed
        value_type source_radius()const{ return source_radius_; }
        bool source_radius_is_zero()const{
            return source_radius() == (value_type)(0);
        }
        bool source_radius_exceeds_max_radius()const{
            return (source_radius() > max_radius());
        }

        result_of_collection_coefficients_type
        collection_coefficients()const{return coll_coeffs_; }

        template<typename R0,typename R1>
        bool operator()(const R0& source, const R1& weight){
            typedef std::vector<unsigned> degrees_type;
            typedef zscore<center_type>            zscore_type;
            typedef l2_norm_squared                sqnorm_type;
            static degrees_type degrees(weight_size,0);
            //static zscore_type zscore(center());//not safe
            static Cont zscore_val(source_size);
            static sqnorm_type sqnorm;
            zscore_type zscore(center());
            zscore(source,bandwidth(),zscore_val);

            value_type zscore_sqnorm = sqnorm(zscore_val);
            source_radius_ = sqrt(zscore_sqnorm) * bandwidth();
            bool do_collect = !(
                source_radius_is_zero()
                        || source_radius_exceeds_max_radius() );

            if(do_collect){
                truncation_degree_policy_(
                    *this, source_radius(), weight, degrees);
                (*this)(zscore_val, zscore_sqnorm, weight, degrees);
            }
            return do_collect;
        }

        // TODO check that
        // range_iterator<R2>::type::value_type == unsigned
        /// Returns true if source collected, false otherwise
        template<typename R0,typename R1,typename R2>
        void operator()(
            const R0& zscore_val,
            value_type zscore_sqnorm,
            const R1& weight,
            const R2& degrees
        ){
            typedef range_iterator<R0>       ir0_type;
            typedef range_iterator<R1>       ir1_type;
            typedef range_iterator<R2>       ir2_type;
            typedef typename ir0_type::type         iter0_type;
            typedef typename ir1_type::type         iter1_type;
            typedef typename ir2_type::type         iter2_type;
            typedef typename iter0_type::value_type value0_type;
            typedef typename iter1_type::value_type value1_type;
            typedef typename iter2_type::value_type value2_type;
            BOOST_MPL_ASSERT((is_same<value0_type,value_type>));
            BOOST_MPL_ASSERT((is_same<value1_type,value_type>));
            BOOST_MPL_ASSERT((is_same<value2_type,unsigned>));
            //static Cont zscore_val(source_size);
            static Cont consts(source_size);
            static monomials<Cont> monoms;
            BOOST_ASSERT(size(zscore_val)-source_size==0);
            BOOST_ASSERT(size(weight)-weight_size==0);
            BOOST_ASSERT(size(degrees)-weight_size==0);
            BOOST_ASSERT(zscore_sqnorm!=((value_type)(0)));

                unsigned local_max_degree
                    = (*std::max_element(
                        begin(degrees),
                        end(degrees)));

                monoms(zscore_val,local_max_degree);

                for_each(
                    make_zip_iterator(
                        make_tuple(
                            begin(weight),
                            begin(coll_coeffs_),
                            begin(degrees)
                        )
                    ),
                    make_zip_iterator(
                        make_tuple(
                            end(weight),
                            end(coll_coeffs_),
                            end(degrees)
                        )
                    ),
                op_factory::make(monoms(),zscore_sqnorm)
                );

                radius_
                    = (radius()<source_radius())
                        ? source_radius() : radius();

                ++sources_count_;
        }
        //TODO remove. Necessary for temporary fix in copy constructor
        // of derived classes
        const truncation_degree_policy_type& truncation_degree_policy()const{
            return truncation_degree_policy_;}

        private:
        template<typename Mons>
        class op{
            public:
            typedef tuple<
                const value_type&,
                coeffs_type&,
                const unsigned&>                                argument_type;
            typedef void                                        result_type;
            typedef
                typename range_iterator<const Mons>::type mons_ir_type;
            op(const Mons& mons,value_type zscore_sqnorm)
                :zscore_sqnorm_(zscore_sqnorm),mons_(mons){}
            op(const op& that)
                :zscore_sqnorm_(that.zscore_sqnorm_),mons_(that.mons_){}

            void operator()(argument_type t){
                value_type w            = t.template get<0>();
                coeffs_type& coeffs     = t.template get<1>();
                unsigned degree         = t.template get<2>();
                coeffs(zscore_sqnorm_,mons_,w,degree);
            }
            private:
            op& operator=(const op& that);
            value_type  zscore_sqnorm_;
            const Mons& mons_;
        };//op
        struct op_factory{
            template<typename Mons>
            static op<Mons> make(const Mons& mons,value_type zscore_sqnorm){
                return op<Mons>(mons,zscore_sqnorm);
            }
        };
        center_type             center_;
        value_type              bandwidth_;
        value_type              max_radius_;
        truncation_degree_policy_type      truncation_degree_policy_;
        value_type              inv_nc_;

        value_type              source_radius_;
        value_type              radius_;
        sources_count_type      sources_count_;
        coll_coeffs_type        coll_coeffs_;
   };


}}}


#endif
