///////////////////////////////////////////////////////////////////////////////
//  clock.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_GENERATOR_CLOCK_HPP_ER_2009
#define BOOST_GENERATOR_CLOCK_HPP_ER_2009
#include <boost/mpl/void.hpp>
#include <boost/parameter/parameters.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/generator/keyword.hpp>
namespace boost{
namespace generator{

	/// \class clock
	/// \brief Generates \f$ t, t+\delta, t+2\delta ... \f$
    template<typename RealType,typename Id = mpl::void_>
	class clock{
        typedef kwd<Id>                     kwd;
	public:
		typedef RealType					  result_type;

        clock(result_type start,result_type delta)
        :t_(start),delta_(delta){

        }

        template<typename ArgumentPack>
		clock(const ArgumentPack& args)
		:t_(args[kwd::clock_start_time|0.0]),
		delta_(args[kwd::clock_delta_time]){}

        clock(const clock& that)
        :t_(that.t_),delta_(that.delta_){}

        clock& operator=(const clock& that){
            if(&that!=this){
                t_ = that.t_;
                delta_ = that.delta_;
            }
            return *this;
        }

		result_type	operator()(){
			result_type t = t_;
            t_+=delta_;
            return t;
        }

        result_type delta()const{ return delta_; }
        result_type start()const{ return t_; }

	private:
		clock();//dont impl
		RealType t_;
        RealType delta_;
	};

}//generator
}//boost

#endif
