//////////////////////////////////////////////////////////////////////////////
// standard_distribution::example::location_scale.cpp                       //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <stdexcept>
#include <ostream>
#include <fstream>
#include <boost/typeof/typeof.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/standard_distribution/distributions/chi_squared.hpp>
#include <boost/standard_distribution/distributions/exponential.hpp>
#include <boost/standard_distribution/distributions/gamma.hpp>
#include <boost/standard_distribution/distributions/normal.hpp>
#include <boost/standard_distribution/distributions/students_t.hpp>
#include <boost/standard_distribution/distributions/uniform.hpp>
#include <boost/standard_distribution/distributions/location_scale.hpp>

void example_serialize(std::ostream& out){
    using namespace boost;
    out << " -> example_serialize";
    const char* msg = "example_serialize : ";
    
    typedef std::string                             str_;
    
    typedef boost::archive::text_oarchive           oa_;
    typedef boost::archive::text_iarchive           ia_;
    typedef std::ofstream                           ofs_;
    typedef std::ifstream                           ifs_;

    typedef double val_;

    const char* ar_path = "./ar_dist";

    {
        typedef math::chi_squared_distribution<val_>                dist_;
        typedef math::meta_distribution_primitives< dist_ >::type   prim_;
        dist_ dist(2.0);
        dist_ dist2(4.0);
        BOOST_AUTO(prim,make_distribution_primitives(dist));

        {
            ofs_ ofs(ar_path);
            oa_ oa(ofs);
            oa << prim;        
        }
        {
            ifs_ ifs(ar_path);
            if(ifs.good()){
                ia_ ia(ifs);
                ia >> prim;
            }else{
                throw std::runtime_error( msg );
            }
        }
        BOOST_ASSERT(
            prim != make_distribution_primitives(dist2)
        );
        dist2 = prim; //conversion
        BOOST_ASSERT(
            prim == make_distribution_primitives(dist2)
        );
    }// chi_squared

    {
        typedef math::exponential_distribution<val_> dist_;
        typedef math::meta_distribution_primitives< dist_ >::type prim_;
        dist_ dist;
        dist_ dist2(4.0);
        BOOST_AUTO(prim,make_distribution_primitives(dist));

        {
            ofs_ ofs(ar_path);
            oa_ oa(ofs);
            oa << prim;        
        }
        {
            ifs_ ifs(ar_path);
            if(ifs.good()){
                ia_ ia(ifs);
            }else{
                throw std::runtime_error( msg );
            }
        }
        BOOST_ASSERT(
            prim != make_distribution_primitives(dist2)
        );
        dist2 = prim;
        BOOST_ASSERT(
            prim == make_distribution_primitives(dist2)
        );
    }// exponential

    {
        typedef math::gamma_distribution<val_> dist_;
        typedef math::meta_distribution_primitives< dist_ >::type prim_;
        dist_ dist(2.0);
        dist_ dist2(4.0);
        BOOST_AUTO(prim,make_distribution_primitives(dist));

        {
            ofs_ ofs(ar_path);
            oa_ oa(ofs);
            oa << prim;        
        }
        {
            ifs_ ifs(ar_path);
            if(ifs.good()){
                ia_ ia(ifs);
                ia >> prim;
            }else{
                throw std::runtime_error( msg );
            }
        }
        BOOST_ASSERT(
            prim != make_distribution_primitives(dist2)
        );
        dist2 = prim; //conversion
        BOOST_ASSERT(
            prim == make_distribution_primitives(dist2)
        );
    }// gamma

    {
        typedef math::normal_distribution<val_> dist_;
        typedef math::meta_distribution_primitives< dist_ >::type prim_;
        dist_ dist;
        dist_ dist2(4.0, 4.0);
        BOOST_AUTO(prim,make_distribution_primitives(dist));

        {
            ofs_ ofs(ar_path);
            oa_ oa(ofs);
            oa << prim;        
        }
        {
            ifs_ ifs(ar_path);
            if(ifs.good()){
                ia_ ia(ifs);
                ia >> prim;
            }else{
                throw std::runtime_error( msg );
            }
        }
        BOOST_ASSERT(
            prim != make_distribution_primitives(dist2)
        );
        dist2 = prim; //conversion
        BOOST_ASSERT(
            prim == make_distribution_primitives(dist2)
        );
    }// normal

    {
        typedef math::students_t_distribution<val_> dist_;
        typedef math::meta_distribution_primitives< dist_ >::type prim_;
        dist_ dist(2.0);
        dist_ dist2(4.0);
        BOOST_AUTO(prim,make_distribution_primitives(dist));

        {
            ofs_ ofs(ar_path);
            oa_ oa(ofs);
            oa << prim;        
        }
        {
            ifs_ ifs(ar_path);
            if(ifs.good()){
                ia_ ia(ifs);
                ia >> prim;
            }else{
                throw std::runtime_error( msg );
            }
        }
        BOOST_ASSERT(
            prim != make_distribution_primitives(dist2)
        );
        dist2 = prim; //conversion
        BOOST_ASSERT(
            prim == make_distribution_primitives(dist2)
        );
    }// students_t

    {
        typedef math::uniform_distribution<val_> dist_;
        typedef math::meta_distribution_primitives< dist_ >::type prim_;
        dist_ dist;
        dist_ dist2(-4.0,4.0);
        BOOST_AUTO(prim,make_distribution_primitives(dist));

        {
            ofs_ ofs(ar_path);
            oa_ oa(ofs);
            oa << prim;        
        }
        {
            ifs_ ifs(ar_path);
            if(ifs.good()){
                ia_ ia(ifs);
                ia >> prim;
            }else{
                throw std::runtime_error( msg );
            }
        }
        BOOST_ASSERT(
            prim != make_distribution_primitives(dist2)
        );
        dist2 = prim; //conversion
        BOOST_ASSERT(
            prim == make_distribution_primitives(dist2)
        );
    }// uniform

    {
        typedef math::students_t_distribution<val_> zdist_;
        typedef math::location_scale_distribution<zdist_> dist_;
        typedef math::meta_distribution_primitives< dist_ >::type prim_;
        dist_ dist( zdist_(10.0), 1.0, 2.0 );
        dist_ dist2( zdist_(10.0), -4.0,4.0);
        BOOST_AUTO(prim,make_distribution_primitives(dist));

        {
            ofs_ ofs(ar_path);
            oa_ oa(ofs);
            oa << prim;        
        }
        {
            ifs_ ifs(ar_path);
            if(ifs.good()){
                ia_ ia(ifs);
                ia >> prim;
            }else{
                throw std::runtime_error( msg );
            }
        }
        BOOST_ASSERT(
            prim != make_distribution_primitives(dist2)
        );
        dist2 = prim; //conversion
        BOOST_ASSERT(
            prim == make_distribution_primitives(dist2)
        );
    }// location_scale


    out << "<-";
}