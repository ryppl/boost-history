////////////////////////////////////////////////////////////////////////////
// approximation_impl.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ADAPTIVE_REJECTION_SAMPLING_APPROXIMATION_IMPL_HPP_ER_2009
#define BOOST_ADAPTIVE_REJECTION_SAMPLING_APPROXIMATION_IMPL_HPP_ER_2009
#include <vector>
#include <ext/algorithm> // is_sorted
#include <algorithm>
#include <functional>
#include <iterator>
#include <cmath>
#include <boost/mpl/identity.hpp>
#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/range.hpp>
#include <boost/math/tools/precision.hpp>
#include <boost/math_limit/miscellanea.hpp>
#include <boost/utility/container_to_string.hpp>
#include <boost/adaptive_rejection_sampling/point.hpp>
namespace boost{
namespace adaptive_rejection_sampling{

        // approximation_impl keeps a set of of (n>2) points,
        // n+1 intersection abscicae and n approximate unnormalized cdfs
        //
        // Constructor
        //    approximation_impl( // Warning : must be followed by
        //        value_type min, // initialize_impl(...)
        //        value_type max
        //    )
        //    approximation_impl(
        //        value_type min,
        //       value_type max,
        //        const point_t& a,
        //        const point_t& b
        //   )
        // Const interface:
        //  min()const
        //  max()const
        //  lower(x)
        //  upper(x)
        //  total_unnormalized_cdf()const
        //  inverse_cdf(value_type u)const
        //
        // Modifier:
        //  void initialize_impl(const point_t& a,const point_t& b)
        //  update_impl(const point_t& p)
        template<
            typename RealType = double,
            template<typename,typename> class Cont = std::vector,
            template<typename> class Alloc = std::allocator
        >
        class approximation_impl{
            public:
            typedef RealType                                value_type;
            typedef math_limit::limits<value_type> limits_t;
            typedef point<value_type>                       point_t;
            typedef Alloc<point_t>                          alloc_t;
            typedef Cont<point_t,alloc_t>                   points_t;
            typedef std::vector<value_type>                 upper_t;
            typedef typename range_iterator<points_t>::type  iter_t;
            typedef typename range_iterator<upper_t>::type   upper_iter_t;
            typedef typename range_iterator<const points_t>::type
                                                            citer_t;
            typedef typename range_iterator<const upper_t>::type
                                                            cupper_iter_t;

            typedef typename range_size<points_t>::type     size_type;
            struct result_of_range_points : mpl::identity<const points_t&>{};
            struct result_of_range_upper : mpl::identity<const upper_t&>{};

            bool is_valid_leftmost_point(value_type dy){
                return (!limits_t::is_negative_infinity(min())) || (dy>0.0);
            }
            bool is_valid_rightmost_point(value_type dy){
                return (!limits_t::is_infinity(max())) || (dy<0.0);
            }

            approximation_impl(
                value_type min,
                value_type max
            ):min_(min),max_(max),subtract_from_y_(0.0)
            {
                //Warning not initialized.
            }

            approximation_impl(
                value_type min,
                value_type max,
                const point_t& a,
                const point_t& b
            ):min_(min),max_(max_),subtract_from_y_(0.0)
            {
                initialize_impl(a,b);
            }
            //TODO protected
            void initialize_impl(
                const point_t& a,
                const point_t& b
            ){
                BOOST_ASSERT(a.x()<b.x());
                BOOST_ASSERT(a.dy()>b.dy());
                BOOST_ASSERT(is_valid_leftmost_point(a.dy()));
                BOOST_ASSERT(is_valid_rightmost_point(b.dy()));
                points_.clear();
                points_.push_back(a);
                points_.push_back(b);
                upper_.clear();
                upper_.push_back(min());
                value_type ab = tangent_intersection(a,b);
                upper_.push_back(ab);
                upper_.push_back(max());
                assert_upper_points();
                assert_points_sorted();
                assert_range_dy_sorted();
                assert_upper_sorted();
                update_unnormalized_cdf();

            }
            approximation_impl(const approximation_impl& that)
            :min_(that.min_),
            max_(that.max_),
            points_(that.points_),
            upper_(that.upper_),
            range_unnormalized_cdf_(
                that.range_unnormalized_cdf_){}

            approximation_impl&
            operator=(const approximation_impl& that){
                if(&that!=this){
                    min_    = that.min_;
                    max_    = that.max_;
                    points_ = that.points_;
                    upper_  = that.upper_;
                    range_unnormalized_cdf_
                        = that.range_unnormalized_cdf_;
                    //unnormalized_cdf_ = that.unnormalized_cdf_;
                }
                return *this;
            }

            value_type min()const{return min_;}
            value_type max()const{return max_;}
            value_type
            total_unnormalized_cdf()const{
                BOOST_ASSERT(size(points_)>1);
                return range_unnormalized_cdf_.back();
                //warning : this is an approximation, whose magnitude
                // is compounded by subtract_y (Ctrl+F), but as n-->inf,
                // sutract_y should --->0

            }
            value_type
            lower(value_type x)const{
                BOOST_ASSERT(size(points_)>1);
                value_type res = 0.0;
                const point_t p(x,0.0,0.0);
                citer_t i = upper_bound(
                    begin(points_),
                    end(points_),
                    p,
                    less<value_type>()
                );
                if(i!=end(points_)){
                    if(begin(points_)!=i){
                        value_type x_h = i -> x();
                        value_type y_h = i -> y();
                        --i;
                        value_type x_l = i -> x();
                        value_type y_l = i -> y();
                        res = y_l + (x-x_l) * (y_h-y_l) / (x_h-x_l);
                    }else{
                        res = negative_infinity();
                    }
                }else{
                    res = negative_infinity();
                }
                return res;
            }
            value_type
            upper(value_type x)const{
                BOOST_ASSERT(size(points_)>1);

                    // Loops
                    //    a   b   c
                    //   / \ / \ / \
                    //  z  ab  bc   z     (tangent intersections)
                    // x is_in [ab,bc], we need tangent at b
                    cupper_iter_t u = upper_bound(
                        begin(upper_),
                        end(upper_),
                        x
                    );
                typedef typename range_difference<upper_t>::type u_diff_t;

                u_diff_t u_diff = std::distance(begin(upper_),u);
                BOOST_ASSERT(u_diff>0);
                citer_t i = begin(points_);
                std::advance(i,u_diff-1);
                BOOST_ASSERT((i->x())<*u);
                --u; BOOST_ASSERT(*u<=x);
                return tangent(*i,x);
            }
            value_type inverse_cdf(value_type u)const{
                BOOST_ASSERT(size(points_)>1);

            //        a---b---c---d
            //        /\ / \ / \ / \
            //      z-  ab  bc  cd  z+
            //          |   |   |   |

                const upper_t& rcdf
                    = range_unnormalized_cdf_;
                cupper_iter_t i_cdf = begin(rcdf);
                cupper_iter_t i_u = begin(upper_);
                citer_t i_p = begin(points_);
                value_type thresh = u * total_unnormalized_cdf();
                while(*i_cdf< thresh){
                      ++i_cdf;
                      ++i_u;
                      ++i_p;
                      BOOST_ASSERT(
                        i_cdf!=end(range_unnormalized_cdf_)
                      );
                }
                value_type x  = i_p->x();
                value_type y  = i_p->y() - subtract_from_y_;
                value_type dy = i_p->dy();
                //TODO distinguish based on is_negative_infinity(*i_u)
                value_type t = tangent(*i_p,*i_u) - subtract_from_y_;
                value_type cumul_cdf = 0.0;
                if(i_cdf!=begin(rcdf)){
                    --i_cdf;
                    cumul_cdf = *i_cdf;
                }
                value_type res
                    = math_limit::safer_exp(t)+ dy * (thresh-cumul_cdf);
                if(!(res>0.0)){
                    std::string str = "adaptive_rejection_sampling::";
                    str+= "approximation_impl::";
                    str+= "inverse_cdf : ";
                    str+= "!res=%1%>0.0";
                    str+= "x=%2% subtract_from_y_=%3% t=%4%";
                    format f(str);
                    f%res%x%subtract_from_y_;
                    f%t;
                    throw std::runtime_error(f.str());
                }else{ //safer_log instead?
                    if(!(res<limits_t::infinity())){
                        std::string str = "inverse_cdf";
                        str+= "!(res=%1%<limits_t::max_value()=%2%)";
                        str+= "x=%3%";
                        format f(str);
                        f%res%limits_t::infinity()%x;
                        throw std::runtime_error(f.str());
                    }else{
                        res = x + ( log(res) - y ) / dy;
                    }
                }
                return res;
            }

            // For testing only.
            typename result_of_range_points::type
            range_point()const{ return points_; }
            typename result_of_range_upper::type
            range_upper()const{ return upper_; }
            //TODO protected
            void update_impl(value_type x,value_type y,value_type dy){
                return update_impl(point_t(x,y,dy));
            }
            //TODO protected
            void update_impl(const point_t& p){
                BOOST_ASSERT(size(points_)>1);

// lower_bound returns furthermost iterator i in [first, last) such that,
// for every j in [first, i), *j < value or (comp(*j, value) is true)
// upper_bound returns the furthermost iterator i in [first, last) such that
//, for every iterator j in [first, i),  value < *j is false.
// or comp(value, *j) is false.

                iter_t i = upper_bound(
                    begin(points_),
                    end(points_),
                    p,
                    less<value_type>()
                );
                bool is_not_largest    = (i!=end(points_));
                i = points_.insert(i,p);
                bool is_not_smallest  = (i!=begin(points_));
                value_type z_l,z_r = 0.0; //left

                if(is_not_smallest){
                    iter_t l = i; --l;
                    z_l = tangent_intersection(*l,*i);
                }
                if(is_not_largest){
                    iter_t r = i; ++r;
                    z_r = tangent_intersection(*i,*r);
                }
                // z[0] = m, z[1],...., z[n-1]=M, end
                if(!is_not_smallest){
                    upper_iter_t j = begin(upper_); ++j; //*j = z[1]
                    upper_.insert(j,z_r);
                }else{
                    if(is_not_largest){
                       //   a b c
                       //  / \ / \
                       // z-  ac  z+
                        upper_iter_t j = begin(upper_);
                        std::advance(j,distance(begin(points_),i)); //*j=ac
                        *j = z_r; //*j = bc
                       //   a b c
                       //  / \ / \
                       // z-  bc  z+
                        upper_.insert(j,z_l);
                       //   a   b   c
                       //  / \ / \ / \
                       // z-  ab  bc  z+
                    }else{
                        upper_iter_t j = begin(upper_);
                        std::advance(j,size(upper_)-1); //*j = z+
                        upper_.insert(j,z_l);
                    }
                }

                assert_upper_points();
                assert_points_sorted();
                assert_range_dy_sorted();
                assert_upper_sorted();
                update_unnormalized_cdf();
            }

            std::string as_string()const{
                typedef utility::container_to_string to_str_t;
                std::string str;
                str+= "[ upper_ = %1%\n";
                str+= " range_unnormalized_cdf_ = %2%\n";
                str+= " {x} = %3%\n";
                str+= " {y} = %4%\n";
                str+= " {dy} = %5% ]";
                format f(str);
                f%to_str_t::get_indexed(upper_);
                f%to_str_t::get_indexed(range_unnormalized_cdf_);
                f%to_str_t::get_indexed(range_x());
                f%to_str_t::get_indexed(range_y());
                f%to_str_t::get_indexed(range_dy());
                return f.str();
            }
            protected:
                value_type
                negative_infinity()const{
                    return limits_t::negative_infinity();
                }
                value_type infinity()const{
                    return limits_t::infinity();
                }

            private:

            void update_unnormalized_cdf(){
                upper_t upper_ys;
                value_type max_t     = negative_infinity();
                //min_nni_t = infinity(); // no use, for now.
                subtract_from_y_ = 0.0;
                {
                    citer_t       i = begin(points_);
                    citer_t       e = end(points_);
                    cupper_iter_t u = begin(upper_);
                    value_type    t = tangent(*i,*u);
                    upper_ys.push_back(t);
                    ++u;
                    while(i < e){
                        BOOST_ASSERT(u!=end(upper_));
                        t = tangent(*i,*u);
                        upper_ys.push_back(t);
                        max_t = (t<max_t)?max_t : t;
                        //min_nni_t
                        //    = ( (t<min_nni_t) && (!is_negative_infinity(t)) )
                        //        ?t : min_nn_t;
                        ++i, ++u;
                    }
                    BOOST_ASSERT(max_t > negative_infinity());
                }

                { // narrows support to [log_min_value,log_max_value]
                    value_type max_allowed = limits_t::log_max_value();
                    subtract_from_y_ = max_t - max_allowed;
                    subtract_from_y_
                        = (subtract_from_y_>0.0)? subtract_from_y_ : 0.0;
                    upper_iter_t b = begin(upper_ys);
                    upper_iter_t e = end(upper_ys);
                    if(subtract_from_y_>0.0){
                        for(upper_iter_t i = b; i<e; i++){
                            if(
                              *i>subtract_from_y_+limits_t::log_min_value()
                            ){
                                (*i) -= subtract_from_y_;
                            }
                        }
                    }
                }

                range_unnormalized_cdf_.clear();
                value_type unnormalized_cdf = 0.0;
                citer_t  i = begin(points_);
                citer_t  e = end(points_);
                cupper_iter_t i_t = begin(upper_ys);
                value_type t_prior  = *i_t; //z-
                value_type et_prior = math_limit::safer_exp(t_prior);
                ++i_t;
                value_type t, et, dy, delta = 0.0;
                while(i < e){
                    BOOST_ASSERT(i_t<end(upper_ys));
                    t = *i_t;
                    et = math_limit::safer_exp(t);
                    dy = i->dy();
                    // sometimes et-et_prior = 0 even if (t-t_prior) !=0
                    // due to rounding
                    delta = (t - t_prior);
                    if(
                        ((dy>0.0) && (delta >0.0))
                            || ((dy<0.0) && (delta<0.0))
                    )
                    {
                        //Warning. Do not un-comment as leads to failure
                        //if(delta<limits_t::min_value() * dy){
                        //    delta = limits_t::min_value();
                        //}else{
                            delta = (et-et_prior)/dy;
                        //}
                    }else{

                        //math_limit::safer_exp(t); //debugging only
                        typedef utility::container_to_string to_str_t;
                        std::string str = "adaptive_rejection_sampling::";
                        str+= "approximation_impl";
                        str+= "!(delta=%1%>0.0)\n";
                        str+= "subtract_from_y_=%2%\n, t_prior = %3%";
                        str+= " t = %4%, dy = %5%, et_prior = %6%";
                        str+= " et = %7%\n";
                        str+= " upper_ys = %8%\n";
                        str+= this->as_string();
                        format f(str);
                        f%delta%subtract_from_y_;
                        f%t_prior%t%dy%et_prior%et;
                        f%to_str_t::get_indexed(upper_ys);
                        throw std::runtime_error(f.str());
                    }
                    t_prior = t;
                    et_prior = et;
                    unnormalized_cdf += delta;
                    range_unnormalized_cdf_.push_back(unnormalized_cdf);
                    ++i; ++i_t;
                } //while
                assert_range_unnormalized_cdf_points();
                assert_range_unnormalized_cdf_sorted();
            }

            /// --> For testing only
            const upper_t&
            range_dy()const{
                static upper_t result;
                result.clear();
                transform(
                    begin(points_),
                    end(points_),
                    back_inserter(result),
                    bind(
                        &point_t::dy,
                        _1
                    )
                );
                return result;
            }
            const upper_t&
            range_x()const{
                static upper_t result;
                result.clear();
                transform(
                    begin(points_),
                    end(points_),
                    back_inserter(result),
                    bind(
                        &point_t::x,
                        _1
                    )
                );
                return result;
            }
            const upper_t&
            range_y()const{
                static upper_t result;
                result.clear();
                transform(
                    begin(points_),
                    end(points_),
                    back_inserter(result),
                    bind(
                        &point_t::y,
                        _1
                    )
                );
                return result;
            }
            /// <--- For Testing only
            void assert_upper_points()const{
                BOOST_ASSERT(
                    size(upper_) - (size(points_)+1) ==0
                );
            }
            void assert_points_sorted()const{
                BOOST_ASSERT(
                    is_sorted(
                        begin(points_),
                        end(points_),
                        less<value_type>()
                    )
                );
            }
            void assert_upper_sorted()const{
                BOOST_ASSERT(
                    is_sorted(begin(upper_),end(upper_))
                );
            }

            void assert_range_dy_sorted()const{
                BOOST_ASSERT(
                    is_sorted(
                        begin(range_dy()),
                        end(range_dy()),
                        std::greater<value_type>()
                    )
                );
            }


            void assert_range_unnormalized_cdf_points()const{
                BOOST_ASSERT(size(range_unnormalized_cdf_)==size(points_));
            }
            void assert_range_unnormalized_cdf_sorted()const{
                BOOST_ASSERT(
                    is_sorted(
                        begin(range_unnormalized_cdf_),
                        end(range_unnormalized_cdf_)
                    )
                );
            }

            value_type                      min_;
            value_type                      max_;
            points_t                        points_;
            upper_t                         upper_;
            upper_t                         range_unnormalized_cdf_;
            //value_type                      unnormalized_cdf_;
            value_type                      subtract_from_y_;
        };

}//adaptive rejection sampling
}//boost


#endif // UPPER_HULL_HPP_INCLUDED
