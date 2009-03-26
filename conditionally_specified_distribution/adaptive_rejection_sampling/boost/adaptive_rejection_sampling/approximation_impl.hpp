////////////////////////////////////////////////////////////////////////////
// approximation_impl.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ADAPTIVE_REJECTION_SAMPLING_APPROXIMATION_IMPL_HPP_ER_2009
#define BOOST_ADAPTIVE_REJECTION_SAMPLING_APPROXIMATION_IMPL_HPP_ER_2009
#include <vector>
#include <list>
#include <ext/algorithm> // is_sorted
#include <algorithm>
#include <functional>
#include <iterator>
#include <cmath>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/range.hpp>
#include  <boost/math/special_functions/fpclassify.hpp>
#include <boost/math/tools/precision.hpp>
#include <boost/math/special_functions/next.hpp>
#include <boost/math_limit/miscellanea.hpp>
#include <boost/utility/container_to_string.hpp>
#include <boost/adaptive_rejection_sampling/point.hpp>
//TODO : make use of
//<boost/math/special_functions/fpclassify.hpp>
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
        //
    template<
        typename RealType = double,
        template<typename,typename> class Cont = std::list,
        template<typename> class Alloc = std::allocator
    >
    class approximation_impl{
    public:
    typedef RealType                                value_type;
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
        return ( (min()>lowest()) || (dy>0.0) );
    }
    bool is_valid_rightmost_point(value_type dy){
        return ( (max()<highest()) || (dy<0.0) );
    }

    approximation_impl(
        value_type min,
        value_type max
    ):min_(min),max_(max)
    {
        //Warning not initialized.
    }

    approximation_impl(
        value_type min,
        value_type max,
        const point_t& a,
        const point_t& b
    ):min_(min),max_(max_)
    {
        initialize_impl(a,b);
    }

    // Optional, before initialize
    void reserve(size_type n){
            //Cont == vector only
            points_.reserve(n);
            upper_.reserve(n);
            range_unnormalized_cdf_.reserve(n);
    }

    size_type size_range_point()const{
        return size(points_);
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
    range_unnormalized_cdf_(that.range_unnormalized_cdf_),
    shift_(that.shift_){}

    approximation_impl&
    operator=(const approximation_impl& that){
        if(&that!=this){
            min_    = that.min_;
            max_    = that.max_;
            points_ = that.points_;
            upper_  = that.upper_;
            range_unnormalized_cdf_
                        = that.range_unnormalized_cdf_;
            shift_ = that.shift_;
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
        // Warning : This is a shifted normalizing constant i.e. in
        // general it will not approach the true normalizing const
    }
    value_type
    lower(value_type x)const{
        BOOST_ASSERT(size(points_)>1);
        try{
        const point_t p(x,0.0,0.0);
        citer_t i = upper_bound(
            begin(points_),
            end(points_),
            p,
            less<value_type>()
        );
        if(i!=end(points_)){
            if(begin(points_)!=i){
                citer_t prior = i; --prior;
                return linear_approximation(*prior,*i,x);
            }else{
                //TODO That, or lower_bound?
                return lowest();
            }
        }else{
            return lowest();
        }
        }catch(std::exception& e){
            std::string str = "lower(%1%)";
            format f(str); f%x;
            throw std::runtime_error(f.str());
        }
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
        return tangent_impl(*i,x);
    }
    value_type inverse_cdf(value_type u)const{
        BOOST_ASSERT(size(points_)>1);
//        x   x0  x1  x
//        /\ / \ / \ / \
//      z-  z  z0  z1  z+
//          |   |   |   |
//             c0  c1 c
//                ^
//               c*u
// where c1 = (exp(t1-shift)-exp(t0-shift)) / dy1 > 0.
// Let a = exp(t0 - shift) + dy1 (c u - c0)
// The quantile is
// q = x1 + ( log ( a ) - (y1-shift) ) / dy1
// We have: (c u - c0) < c1 hence |dy1 (cu - c0)| < |dy1| c1
// if dy1>0, 0< exp(t0-shift) < a < exp(t1-shift)
// if dy1<0, 0< exp(t1-shift) < a < exp(t0-shift)
// Hence we want shift such that
// exp(t1-shift) < inf
// exp(t0-shift) < inf
//
// From the above, we have:
// dy1>0 : x1 + (1/dy) * (t0-y1) < q < x1 + (1/dy) * (t1-y1)
// dy1>0 : x1 + (1/dy) * (t0-y1) < q < x1 + (1/dy) * (t1-y1)

        value_type thresh, x, y, dy, t, cumul_cdf, a, res;
        try{

            const upper_t& rcdf = range_unnormalized_cdf_;
            cupper_iter_t i_cdf = begin(rcdf);
            cupper_iter_t i_u = begin(upper_);
            citer_t i_p = begin(points_);
            thresh = u * total_unnormalized_cdf();
            while(*i_cdf< thresh){
                ++i_cdf;
                ++i_u;
                ++i_p;
                BOOST_ASSERT(
                        i_cdf!=end(range_unnormalized_cdf_)
                );
            }
            x  = i_p->x();
            y  = i_p->y();
            dy = i_p->dy();
            t = tangent_impl(*i_p,*i_u);

            cumul_cdf = 0.0;
            if(i_cdf!=begin(rcdf)){
                --i_cdf;
                cumul_cdf = *i_cdf;
            }

            //value_type e = exp_shift(t-log(fabs(dy)));
            //a = e;
            //if(dy<0.0){
            //    a -= (thresh-cumul_cdf);
            //}else{
            //    a += (thresh-cumul_cdf);
            //}
            //a *= fabs(dy);

            value_type e = exp_shift(t);
            a = e + dy * (thresh-cumul_cdf);

            if(!(a>0.0)){
                format f("!a=%1%>0.0 \n"); f%a;
                throw std::runtime_error(f.str());
            }else{
                if(!(a<highest())){
                    format f("!(a=%1%<highest()=%2%)");
                    f%1%highest();
                    throw std::runtime_error(f.str());
                }else{
                    res = x + ( log(a) - shift(y) ) / dy;

                    if((math::isnan)(res)){
                        std::string str = "!isnan( x + ( log(a) - y ) / dy)";
                        str+= "x = %1%, a = %2%, y = %3%, dy = %4%";
                        format f(str); f%x%a%y%dy;
                        throw std::runtime_error(f.str());
                    }
                }
            }
        }catch(std::exception& e){
            std::string str = "adaptive_rejection_sampling::";
            str+= "approximation_impl::";
            str+= "inverse_cdf(%1%) : ";
            str+= "thresh = %2%, x = %3%, y = %4%, dy = %5%, t = %6%";
            str+= "cumul_cdf = %7%, res = %8%";
            format f(str); f%u%thresh%x%y%dy%t%cumul_cdf%res;
            str+= e.what();
            str+= as_string();
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
        try{
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
        bool is_not_lowest  = (i!=begin(points_));
        value_type z_l,z_r = 0.0;

        if(is_not_lowest){
            iter_t l = i; --l;
            z_l = tangent_intersection(*l,*i);
            if((math::isnan)(z_l)){
                std::string str = "(math::isnan)(z_l=%1%) with ";
                str+="l : "; str+=l->as_string();
                str+="i : "; str+=i->as_string();
                format f(str); f%z_l;
                throw std::runtime_error(f.str());
            }
        }
        if(is_not_largest){
                    iter_t r = i; ++r;
                    z_r = tangent_intersection(*i,*r);
            if((math::isnan)(z_l)){
                std::string str = "(math::isnan)(z_r=%1%) with ";
                str+="i : "; str+=i->as_string();
                str+="r : "; str+=r->as_string();
                format f(str); f%z_r;
                throw std::runtime_error(f.str());
            }
        }
        // z[0] = m, z[1],...., z[n-1]=M, (end)
        if(!is_not_lowest){
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
        }catch(std::exception& e){
            std::string str = "adaptive_rejection_sampling::";
            str+= "approximation_impl::";
            str+= "update_impl : ";
            str+= e.what();
            str+= " ";
            str+= this->as_string();
            throw std::runtime_error(str);
        }

    }

    std::string as_string()const{
                typedef utility::container_to_string to_str_t;
                std::string str;
                str+= "[ upper_ = %1%\n";
                str+= " range_unnormalized_cdf_ = %2%\n";
                str+= " {x} = %3%\n";
                str+= " {y} = %4%\n";
                str+= " {dy} = %5%";
                str+= " shift = %6%]";
                format f(str);
                f%to_str_t::get_indexed(upper_);
                f%to_str_t::get_indexed(range_unnormalized_cdf_);
                f%to_str_t::get_indexed(range_x());
                f%to_str_t::get_indexed(range_y());
                f%to_str_t::get_indexed(range_dy());
                f%shift_;
                return f.str();
    }
    protected:
    value_type log_max_value()const{
        typedef math_limit::exp_support<value_type> exp_sup_t;
        return exp_sup_t::highest();
    }
    value_type log_min_value()const{
        typedef math_limit::exp_support<value_type> exp_sup_t;
        return exp_sup_t::lowest();
    }
    value_type highest()const{
            return math::tools::max_value<value_type>();
    }
    value_type lowest()const{
            return (-highest());
    }

    private:

    value_type tangent_impl(const point_t& p0,value_type x)const{
        value_type t = tangent(p0,x);
        if(math::isnan(t)){
            std::string str = "at p0 : %1% isnan(tangent(p0,%2%))";
            format f(str); f%p0.as_string()%x;
            std::runtime_error(f.str());
        }
        return t;
    }
    // Warning: use only in conjunction with unnormalized cdf
    // i.e. not for lower(...), upper(..)
    value_type shift(value_type y)const{
        return (y - shift_);
    }

    // If need arises later : tangent_shift
    //  return (p.y()-shift_)+((x-p.x()) * p.dy()-shift_);

    value_type exp_shift(value_type y)const{
            value_type s = shift(y);
            value_type e = exp(s);
        if(math::isinf(e)){
            std::string str = "isinf(exp_shift(y)), y = %1%, ";
            str += "shift(y) = %2%.";
            format f(str); f%y%s;
            throw std::runtime_error(f.str());
        }else{
            return e;
        }
    }

    void update_unnormalized_cdf(){
        // Since we work with the unnormalized cdf we are free
        // to shift it by an arbitrary amount:
        // s0 := t0 - shift
        // s1 := t1 - shift
        // delta = (exp(s1)-exp(s0))/dy1
        // Property of log-concave distribution :
        // (exp(s1)-exp(s0))/dy1 > 0.0
        //
        // m = min{shift :
        //  exp(s0) < inf (see inverse_cdf)
        //  exp(s1) < inf (see inverse_cdf)
        //  delta < inf
        //  sum{delta[i]:i=1,...,n} < inf
        // }
        //
        // M = max{shift: exp(max(s0,s1)) > 0.0 }
        //
        // We may not always have m<M for all i, so shift>m takes precedence

        bool do_restart = true;
        value_type t0, t1, e0, e1, delta, l1, w0, w1, dy;
        value_type min_shift = lowest();
        shift_ = 0.0;
        value_type max_shift = highest();
        value_type unnormalized_cdf;
        size_type count = 0;
        // (1) incompatible with (2)-(4) (TODO verify), hence at most 3
        size_type max_count = size(points_) * 3;
        try{
            while(do_restart){
                if(++count > max_count){
                    std::string str = " exceed max_count = %1%";
                    format f(str); f%(value_type)(max_count);
                    throw std::runtime_error(str);
                }

                range_unnormalized_cdf_.clear();
                citer_t  i = begin(points_);
                citer_t  e = end(points_);
                cupper_iter_t u0 = begin(upper_);
                cupper_iter_t u1 = u0; ++u1;
                unnormalized_cdf = 0.0;
                while(i < e){

                    // t0[i] = t1[i-1], but probably safer this way :
                    t0 = tangent_impl(*i,*u0);
                    t1 = tangent_impl(*i,*u1);
                    // if i==begin and *u0 = min(),
                    //  t0 < 0 (must) and -inf allowed
                    // if ++i==end and *u1 = max(),
                    //  t1 < 0 (must) and -inf allowed

                    dy = i->dy();
                    delta = (t1 - t0);
                    if(
                        ( (delta>0.0) && (dy<0.0) )
                            ||( (delta<0.0) && (dy>0.0) )
                    )
                    {
                        std::string str = "!((t1-t0)*dy=%1%>0.0)\n";
                        format f(str); f%delta;
                        throw std::runtime_error(f.str());
                    }else{
                        // Allow possibility that delta == 0
                    }
                    //l1 = fabs(dy);
                    //l1 = log(l1);
                    //w0 = t0 - l1;
                    //w1 = t1 - l1;
                    {
                        value_type tmp = ( (t1<t0)?t0:t1 ) - log_min_value();
                        max_shift = (tmp<max_shift)? tmp : max_shift;
                    }
                    if((max_shift > min_shift) && (shift_>max_shift)){
                        shift_ = boost::math::float_prior(max_shift);
                        do_restart  = true; // (1)
                    }else{
                        {
                            value_type tmp = ((t1>t0)? t1 : t0 );
                            tmp -= log_max_value();
                            min_shift = (tmp>min_shift)? tmp : min_shift;
                        }
                        if(shift_<min_shift){
                            shift_ = boost::math::float_next(min_shift);
                            do_restart = true; // (2)
                        }else{
//                        e0 = exp_shift(w0);
//                        e1 = exp_shift(w1);
//                        delta = e1 - e0;
//                        if(dy<0.0){ delta = (-delta); }
                            e0 = exp_shift(t0);
                            e1 = exp_shift(t1);
                            delta = (e1 - e0);

                            value_type c = math_limit::scale_to_max::divides(
                                fabs(delta),
                                fabs(dy)
                            );
                            if(c>1.0){
                                min_shift = shift_ + log(c);
                                shift_ = boost::math::float_next(min_shift);
                                do_restart = true; // (3)
                            }else{
                                delta /= dy;
                                if(math::isnan(delta)){
                                    std::string str;
                                    str = "math::isnan(delta)";
                                    throw std::runtime_error(str);
                                }
                                value_type c
                                    = math_limit::scale_to_max::plus(
                                    unnormalized_cdf,
                                    delta
                                );
                                if(c>1.0){
                                    min_shift = shift_ + log(c);
                                    shift_
                                        = boost::math::float_next(min_shift);
                                    do_restart = true; // (4)
                                }else{
                                    unnormalized_cdf+=delta;
                                    if(math::isinf(unnormalized_cdf)){
                                        std::string str;
                                        str = "math::isinf(unnormalized_cdf)";
                                        throw std::runtime_error(str);
                                    }
                                    range_unnormalized_cdf_.push_back(
                                        unnormalized_cdf);
                                    ++i; ++u0; ++u1;
                                    do_restart = false;
                                }
                            }
                        }
                    }
                    if(do_restart){
                        i = e; //ensures exit
                    }
                }//while
            } //while
            assert_range_unnormalized_cdf_points();
            assert_range_unnormalized_cdf_sorted();
            if(!(range_unnormalized_cdf_.back()>0.0)){
                std::string str = "!(range_unnormalized_cdf_.back()>0.0)";
                throw std::runtime_error(str);
            }
        }catch(std::exception& e){
            std::string str = "";
            str += "update_unnormalized_cdf ";
            str += e.what();
            str += " t0 = %1%, t1 = %2%, e0 = %3%, e1 =%4%, ";
            str += "l1 = %5%, w0 = %6%, w1 = %7%, ";
            str += "delta = %8%, dy = %9%, ";
            str += "unnormalized_cdf = %10%";
            format f(str); f%t0%t1%e0%e1%l1%w0%w1%delta%dy;
            f%unnormalized_cdf;
            str = f.str();
            str+= (this->as_string());
            throw std::runtime_error(str);
        }
    }
            /// --> For testing only
    const upper_t& range_dy()const{
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
    const upper_t& range_x()const{
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
    const upper_t& range_y()const{
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
            value_type                      shift_;
    };

}//adaptive rejection sampling
}//boost


#endif // UPPER_HULL_HPP_INCLUDED
