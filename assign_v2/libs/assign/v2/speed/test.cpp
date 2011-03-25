//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <cstddef>
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <boost/array.hpp>
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if! BOOST_ASSIGN_V2_ENABLE_CPP0X
#error
#endif
#include <boost/assign/v2/detail/speed.hpp>
#include <boost/config.hpp>
#include <boost/format.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/core.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/numeric/conversion/converter.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/progress.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/static_assert.hpp>
#//include <boost/typeof/typeof.hpp>

#include <libs/assign/v2/speed/tools.h>
#include <libs/assign/v2/speed/test.h>

namespace speed_assign_v2{

struct stat_
{

	typedef boost::numeric::bounds<double> bounds_;
	typedef boost::numeric::converter<double, int> converter_;

	stat_()
        :
        scale_factor(0),
        min( bounds_::highest() ),
        elapsed( 0 ),
        max( 0 ),
        cum_elapsed( 0 ){}

	void put(double elapsed_)
    {
        this->min = this->min > elapsed_ ? elapsed_ : this->min;
        this->max = this->max < elapsed_ ? elapsed_ : this->max;
		this->cum_elapsed += elapsed_;
    }

    static const std::string description;

    int scale_factor;
    std::string method;

	private:
	friend std::ostream& operator<<(std::ostream&, stat_);
	friend struct vec_stat_;
    double min;
    double elapsed;     // not computed internally
    double max;
    double cum_elapsed;

};

const std::string stat_::description = (
    boost::format("{%1%, %|30t|%2%, %|40t|%3%, %|50t|%4%, %|60t|%5%}")
    % "method" % "min" %"average" % "max" % "cumulative"
).str();

std::ostream& operator<<( std::ostream& os, stat_ stat)
{
//%015.8f
//%|20t|%3%
//    boost::format f("{%1%, %|10t|%2$6.3f, %|20t|%3$6.3f, %|30t|%4$6.3f, %|40t|%5$6.1f");
    boost::format f("{%1%, %|30t|%2$6.3f, %|40t|%3$6.3f, %|50t|%4$6.3f, %|60t|%5$6.1f}");
    double scale = stat_::converter_::convert( stat.scale_factor );
    f
        % stat.method
        % ( scale * stat.min )
        % ( scale * stat.elapsed )
        % ( scale * stat.max )
        % ( scale * stat.cum_elapsed );
    ;

	return os << f.str();

/*	'{'
        << stat.min         << ','
        << stat.max         << ','
        << stat.elapsed     << ','
        << stat.cum_elapsed << '}';*/
}

struct vec_stat_
{
    typedef std::size_t size_type;
	typedef std::vector<stat_> storage_;
	typedef boost::numeric::converter<double, size_type> converter_;

	void put(stat_ stat)
    {
        stat.elapsed = stat.cum_elapsed / converter_::convert( k );
    	stats.push_back( stat );
    }

    static const std::string description;

    size_type k;    // outer loops
    size_type n;    // inner loops
    size_type m;    // # arguments
    storage_ stats;

};

const std::string vec_stat_::description = "{{k, n, m}, stats...}";

std::ostream& operator<<( std::ostream& os, vec_stat_ vec_stat)
{
	namespace lambda = boost::lambda;
	boost::format f("{\n{%1%, %2%, %3%}");
	f % vec_stat.k %  vec_stat.n % vec_stat.m;
	os << f.str();
    boost::for_each(
        vec_stat.stats,
        os << lambda::constant( ',' ) << "\n" << lambda::_1
    );
    return os << "\n" << '}' << std::endl;
}

template<typename C, typename ... Args>
void fill_vec_stat(std::size_t scale_factor,
    vec_stat_& vec_stat, std::size_t k, std::size_t n, Args&&... args)
{
    typedef boost::timer timer_;
	C cont;
    vec_stat.k = k;
    vec_stat.n = n;
    vec_stat.m = sizeof...(Args);

    namespace as2 = boost::assign::v2;

#define MACRO(Expr, Method)\
	{\
        stat_ stat;\
        stat.method = Method;\
        stat.scale_factor = scale_factor;\
		for(std::size_t j = 0; j < k; j++)\
    	{\
			double elapsed;\
        	Expr;\
        	stat.put( elapsed );\
        }\
        vec_stat.put( stat );\
    }\
/**/

// Assign Containers

	MACRO( as2::speed_aux::stl_push_back<timer_>(
        n, elapsed, cont, std::forward<Args>( args )... ),
       "stl_push_back"
    )
	MACRO( as2::speed_aux::v1_push_back<timer_>(
        n, elapsed, cont, std::forward<Args>( args )... ),
       "v1_push_back"
    )
	MACRO( as2::speed_aux::v2_put<timer_>(
        n, elapsed, cont, std::forward<Args>( args )... ),
       "v2_put"
    )
	//MACRO( as2::speed_aux::v2_pipe_put<timer_>(
    //    n, elapsed, cont, std::forward<Args>( args )... ),
    //   "v2_pipe_put"
    //)
	MACRO( as2::speed_aux::v2_pipe_csv_put<timer_>(
        n, elapsed, cont, std::forward<Args>( args )... ),
       "v2_pipe_csv_put"
    )

// Generate containers

	MACRO( as2::speed_aux::v1_list_of<timer_>(
        n, elapsed, std::forward<Args>( args )... ),
       "v1_list_of"
    )
	MACRO( as2::speed_aux::v2_deque<timer_>(
        n, elapsed, std::forward<Args>( args )... ),
       "v2_deque"
    )

	MACRO( as2::speed_aux::v2_csv_deque<timer_>(
        n, elapsed, std::forward<Args>( args )... ),
       "v2_csv_deque"
    )
	MACRO( as2::speed_aux::v1_cref_list_of<timer_>(
        n, elapsed, std::forward<Args>( args )... ),
       "v1_cref_list_of"
    )
	MACRO( as2::speed_aux::v2_ref_array<timer_>(
        n, elapsed, std::forward<Args>( args )... ),
       "v2_ref_array"
    )
	MACRO( as2::speed_aux::v2_ref_csv_array<timer_>(
        n, elapsed, std::forward<Args>( args )... ),
       "v2_ref_csv_array"
    )

#undef MACRO

}// fill_vec_stat

void test( std::ostream& os )
{
    namespace lambda = boost::lambda;
	const std::size_t T_size = 1000;
	typedef std::vector<int> T; boost::array<T, 128> args_list;
	typedef std::deque<T> C;
    for(std::size_t i = 0; i < 128; i++)
    {
    	args_list[i] = rand_vec( T_size );
    }

    const std::size_t scale_factor = 1000;
	const std::size_t total_sz = 1280000;
	const std::size_t n = 100;

    BOOST_STATIC_ASSERT( total_sz/(128 * n) > 0); // Invariant : k * n * m = total_sz;

    std::map<std::string, std::string> cols;
    cols["stl_push_back"] =   "cont.push_back( arg1 ); ...; cont.push_back( argm )";
    cols["v1_push_back"] =    "assign::push_back( cont )( arg1 )...( argm )";
    cols["v2_put"] =          "assign::v2::put( cont )( arg1 )...( argm )";
    //cols["v2_pipe_put"] =     "cont | assign::v2::_put( arg1 )...( argm )";
    cols["v2_pipe_csv_put"] = "cont | assign::v2::_csv_put( args... )";
    cols["v1_list_of"] =      "assign::list_of( arg1 )...( argm )";
    cols["v2_deque"] =        "assign::v2::deque<T>( arg1 )...( argm )";
    cols["v2_csv_deque"] =    "assign::v2::csv_deque( args... )";
    cols["v1_cref_list_of"] = "assign::cref_list_of<m>( arg1 )...( argm )";
    cols["v2_ref_array"] =    "assign::v2::ref::array( arg1 )...( argm )";
    cols["v2_ref_csv_array"] = "assign::v2::ref::csv_array( args...)";

    boost::format fn("%1% : %|30t|%2%\n");
    boost::format title("[%|=80|]\n");
    std::string delimiter = ( boost::format("[%|=80|]\n") % " " ).str();

    os << (title % "Machine").str();
    os << BOOST_PLATFORM << std::endl << BOOST_COMPILER << std::endl << std::endl;

    std::map<std::string, std::string> defs;
    boost::format fstat("%1% value of %1% * var_scaled, over k repetitions");
    defs["T"] = "std::vector<int>";
    defs["n"] = "Control a number of loops";
    defs["k"] = "Control a number of loops";
    defs["args..."] = "list of arguments, each of type T and max size ";
    defs["args..."] += ( boost::format("%1%") % T_size ).str();
    defs["var"] = "time to eval method(args...) n times";
    defs["var_scaled"] = ( boost::format("%1% * var") % scale_factor ).str();
    defs["m"] = "size(args...)";
    defs["min"] = ( fstat % "min" ).str();
    defs["average"] = ( fstat % "average" ).str();
    defs["min"] = ( fstat % "max" ).str();
    defs["cumulated"] = ( fstat % "cumulate" ).str();

    std::string notation = ( title % "Notation").str()
        + (fn % "T" % defs["T"]).str()
        + (fn % "n" % defs["n"]).str()
        + (fn % "k" % defs["k"]).str()
        + (fn % "args..." % defs["args..."]).str()
        + (fn % "var" % defs["var"]).str()
        + (fn % "var_scaled" % defs["var_scaled"]).str()
        + (fn % "m" % defs["m"]).str()
        + (fn % "min" % defs["min"]).str()
        + (fn % "average" % defs["average"]).str()
        + (fn % "min" % defs["min"]).str()
        + (fn % "cumulated" % defs["cumulated"]).str();

    os << notation;

    os << std::endl << (title % "Methods").str();
    {
        boost::format f("%1% : %|30t|%2%\n");
        os
            << ( title % "Assign" ).str()
            << ( f %"stl_push_back" % cols["stl_push_back"] ).str()
            << ( f % "v1_push_back"% cols["v1_push_back"] ).str()
            << ( f % "v2_put" % cols["v2_put"] ).str()
            //<< ( f % "v2_pipe_put" % cols["v2_pipe_put"] ).str()
            << ( f % "v2_pipe_csv_put" % cols["v2_pipe_csv_put"] ).str()
            << ( title % "Generate" ).str()
            << ( f % "v1_list_of" % cols["v1_list_of"] ).str()
            << ( f % "v2_deque" % cols["v2_deque"] ).str()
            << ( f % "v2_csv_deque" % cols["v2_csv_deque"] ).str()
            << ( f % "v1_cref_list_of" % cols["v1_cref_list_of"] ).str()
            << ( f % "v2_ref_array" % cols["v2_ref_array"] ).str()
            << ( f % "v2_ref_csv_array" % cols["v2_ref_csv_array"] ).str();
    }
    os << std::endl<< (title % "Results").str();

    boost::format row_format( "{\n{k, n, m},\n%1%...\n}\n" );

    os
        << ( row_format % stat_::description ).str();

#define MACRO1(z, i, data) args_list[i]
#define MACRO2(C_size)\
	{\
		speed_assign_v2::vec_stat_ vec_stat;\
		speed_assign_v2::fill_vec_stat<C>(\
            scale_factor,\
            vec_stat,\
            total_sz/(C_size * n),\
            n,\
            BOOST_PP_ENUM(C_size, MACRO1, ~)\
        );\
		os << vec_stat;\
    }\
/**/

MACRO2(1)
MACRO2(2)
MACRO2(4)
MACRO2(8)
MACRO2(16)
MACRO2(32)

#undef MACRO1
#undef MACRO2

}// test


}// speed_assign_v2
