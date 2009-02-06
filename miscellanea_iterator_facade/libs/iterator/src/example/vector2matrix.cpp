///////////////////////////////////////////////////////////////////////////////
// example/vector2matrix.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <iostream>//TODO remove
#include <boost/assign/std/vector.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/iterator/vector2matrix_iterator.hpp>
#include <libs/iterator/src/example/vector2matrix.h>

void example_vector2matrix()
{
    std::cout << "->example_vector2matrix" << std::endl;
    int stride = 2;
    typedef std::vector<int>                        vec_type;

    {
        typedef vec_type::const_iterator                CVecIter;
        typedef boost::vector2matrix_iterator<CVecIter> Vec2MatIter;
        typedef boost::vector2matrix_iterator<CVecIter> Vec2MatIter;

        vec_type vec;
        vec_type vecc;
        {using namespace boost::assign; vec+=0,1,2,3,4,5,6,7,8;}
        //important to use the const qualifier
        CVecIter base_b = boost::begin(vec);
        CVecIter base_e = boost::end(vec);
        Vec2MatIter     b(base_b,stride); Vec2MatIter i = b;
        Vec2MatIter     e=boost::make_end_vector2matrix_iterator(
            base_b,base_e,stride);
        while(i<e){
            Vec2MatIter::value_type r = *i;
            copy(boost::begin(r),boost::end(r),back_inserter(vecc));
            ++i;
        }
        copy(
            boost::begin(vecc),
            boost::end(vecc),
            std::ostream_iterator<int>(std::cout," ")
        );std::cout<<std::endl;//should return 0,1,...,7
    }

    {
        typedef vec_type::const_iterator                CVecIter;
        typedef boost::vector2matrix_iterator<CVecIter> Vec2MatIter;
        typedef boost::vector2matrix_iterator<CVecIter> Vec2MatIter;

        vec_type vec,vec_a;
        { using namespace boost::assign; vec+=0,1,2,3,4,5,6,7; }
        CVecIter base_b = boost::begin(vec);
        CVecIter base_e = boost::end(vec);
        Vec2MatIter  b(base_b,stride);
        for(CVecIter base_i = base_b; base_i<base_e; base_i++){
            Vec2MatIter e = boost::make_end_vector2matrix_iterator(
                base_b,base_i,stride
            );
            std::cout << "std::distance(b,e)="
                << std::distance(b,e) << std::endl; //TODO problem here
        } std::cout << "exited loop" << std::endl;
    }


//    {
//        vec_type vec,vec_a;
//        { using namespace boost::assign; vec+=0,1,2,3,4,5,6,7; vec_a+=4,5;}
//        boost::nearest_neighbor<vec_type>::result_type res
//           =  boost::make_nearest_neighbor(vec)(vec_a);
//        copy(
//            boost::begin(*res),
//            boost::end(*res),
//            std::ostream_iterator<int>(std::cout," ")
//        );
//    }
/*
    {
        typedef std::list<int>                              list_type;
        typedef list_type::const_iterator   CListIter;
        typedef boost::vector2matrix_iterator<CListIter>    List2MatIter;
        list_type lst;
        list_type lstc;
        {
            lst.push_back(0);
            lst.push_back(1);//1
            lst.push_back(2);
            lst.push_back(3);//2
            lst.push_back(4);
            lst.push_back(5);//3
            lst.push_back(6);
            lst.push_back(7);//4
            lst.push_back(8);
        }

        //important to use the const qualifier
        CListIter base_b = boost::begin(lst);
        CListIter base_e = boost::end(lst);
        List2MatIter     i(base_b,stride);
        List2MatIter     e=boost::make_end_vector2matrix_iterator(
            base_b,base_e,stride);

        while(i<e){
            List2MatIter::value_type r = *i;
            copy(boost::begin(r),boost::end(r),back_inserter(lstc));
            ++i;
        }
        copy(
            boost::begin(lstc),
            boost::end(lstc),
            std::ostream_iterator<int>(std::cout," ")
        );
    }
*/
    std::cout << "<-" << std::endl;
}
