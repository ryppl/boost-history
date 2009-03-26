////////////////////////////////////////////////////////////////////////////
// approximation.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ADAPTIVE_REJECTION_SAMPLING_APPROXIMATION_HPP_ER_2009
#define BOOST_ADAPTIVE_REJECTION_SAMPLING_APPROXIMATION_HPP_ER_2009
#include <vector>
#include <stdexcept>
#include <boost/range.hpp>
#include <boost/math/tools/precision.hpp>
#include <boost/adaptive_rejection_sampling/approximation_impl.hpp>
namespace boost{
namespace adaptive_rejection_sampling{

    // approximation queries f (of DistFun) as necessary and forwards
    // to approximation_impl.
    // In addition, it recursively adjusts the initial abscissae (x1,x2)
    // until certain necessary conditions are met.
    //
    // DistFun:
    // Associated type:
    //  value_type
    // Valid expressions:
    // Expression                        return type
    // f.unnormalized_pdf(x)             value_type;
    // f.derivative_unnormalized_pdf(x)  value_type
    //
    // The interface is that of approximation_impl, and
    //
    // Constructor:
    // approximation(f,max_recursion) //not initialized
    // approximation(f,x0_init,x1_init,max_recursion)
    //
    // Modifier:
    // initialize(x1,x2)
    // update(x)
    //
    // In case this is used to sample from {Y~(Y|x[i]):i=1,...,n},
    // consider using reset_distribution_function(const dist_f_t& f)
    // rather than creating a new object to save allocation
    template<
        typename DistFun,
        template<typename,typename> class Cont = std::vector,
        template<typename> class Alloc = std::allocator
    >
    class approximation : public approximation_impl<
        typename DistFun::value_type,
        Cont,
        Alloc
    >{
        typedef approximation_impl<
            typename DistFun::value_type,
            Cont,
            Alloc
        >                                                   super_t;
        typedef DistFun                                     dist_f_t;
        typedef std::pair<
            typename super_t::point_t,
            typename super_t::point_t
        >                                                   pair_points_t;
        protected:
        // The search for adequate initial (x1,x2) quits and throws an error
        // after max_recursion
        approximation(
            const dist_f_t& f,
            typename super_t::size_type max_recursion
        ):super_t(f.min(),f.max()),
        dist_f_(f),
        max_recursion_(max_recursion){
                //warning not initialized
        }
        approximation(
            const dist_f_t& f,
            typename super_t::value_type x0_init,
            typename super_t::value_type x1_init,
            typename super_t::size_type max_recursion
        ):super_t(f.min(),f.max()),
        dist_f_(f),
        max_recursion_(max_recursion){
            this->initialize(x0_init,x1_init);
        }
        approximation(const approximation& that)
        :super_t(that),
        dist_f_(that.dist_f_),
        max_recursion_(that.max_recursion_){}

		approximation&
		operator=(const approximation& that){
		    if(&that!=this){
                super_t::operator=(that);
                dist_f_ = that.dist_f_;
                max_recursion_ = that.max_recursion_;
		    }
		    return *this;
		}
        public:
        void reset_distribution_function(const dist_f_t& f){
            dist_f_ = f;
            //next : initialize
        }

        void initialize(
            typename super_t::value_type x1,typename super_t::value_type x2){
            try{
                pair_points_t a
                    = find_valid_starting_points(x1,x2,max_recursion_);
                initialize_impl(a.first,a.second);
            }catch(std::exception& e){
                throw std::runtime_error(e.what());
            }
        }
        // Warning: The client may wish to supply his own update,
        // so we leave this unprotected. By default, however, this is
        // internal.
        void update(typename super_t::value_type x){
            typename super_t::point_t const p = this->get_point(x);
            this->update_impl(p);
        }

        protected:
        typename super_t::point_t
        get_point(typename super_t::value_type x)const{
            typename super_t::value_type y = dist_f_.unnormalized_log_pdf(x);
            typename super_t::value_type dy
                = dist_f_.derivative_unnormalized_log_pdf(x);
            return typename super_t::point_t(x,y,dy);
        }
        typename super_t::value_type
        get_y(typename super_t::value_type x)const{
            typename super_t::value_type y = dist_f_.unnormalized_log_pdf(x);
            return y;
        }
        private:
        pair_points_t
        find_valid_starting_points(
            typename super_t::value_type x1,
            typename super_t::value_type x2,
            typename super_t::size_type max_recursion
        ){
            BOOST_ASSERT(x1<x2);
            typename super_t::size_type recursion = 0;
            typename super_t::value_type new_x1 = x1;
            typename super_t::point_t p1 = get_point(new_x1);
            bool is_ok_1 = is_valid_leftmost_point(p1.dy());

            typename super_t::value_type new_x2 = x2;
            typename super_t::point_t p2 = get_point(new_x2);
            bool is_ok_2 = is_valid_rightmost_point(p2.dy());

            while(
                (!(is_ok_1 && is_ok_2)) && (recursion++<max_recursion)
            ){
                if(!is_ok_1){
                    BOOST_ASSERT(new_x1-new_x2<0.0);
                    new_x1 -=  (new_x2-new_x1);
                    p1 = get_point(new_x1);
                    is_ok_1 = is_valid_leftmost_point(p1.dy());
                }
                if(!is_ok_2){
                    BOOST_ASSERT(new_x2-new_x1>0.0);
                    new_x2  += (new_x2-new_x1);
                    p2      = get_point(new_x2);
                    is_ok_2 = is_valid_rightmost_point(p2.dy());
                }
            }
            if(!is_ok_1){
                std::string str = "adaptive_rejection_sampling::";
                str += "approximation::initialize";
                str += "!is_valid_leftmost_point(dy1=%1%)";
                str += "after %2% recursions, ";
                str += "and x1 = %3%, x2 = %4%, new_x1 = %5%, new_x2 = %6%";
                format f(str);
                f % p1.dy() % recursion % x1 % x2 % new_x1 % new_x2;
                throw std::runtime_error(f.str());
            }
            if(!is_ok_2){
                std::string str = "adaptive_rejection_sampling::";
                str += "approximation::initialize";
                str += "!is_valid_rightmost_point(dy2=%1%)";
                str += "after %2% recursions, ";
                str += "and x1 = %3%, x2 = %4%, new_x1 = %5%, new_x2 = %6%";
                format f(str);
                f % p2.dy() % recursion % x1 % x2 % new_x1 % new_x2;
                throw std::runtime_error(f.str());
            }
            return pair_points_t(p1,p2);
        }

        dist_f_t                    dist_f_;
        typename super_t::size_type max_recursion_;

    };
}//adaptive_rejection_sampling
}//boost

#endif
