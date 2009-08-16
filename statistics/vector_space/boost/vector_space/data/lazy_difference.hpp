///////////////////////////////////////////////////////////////////////////////
// vector_space::data::lazy_difference.hpp                                   //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#ifndef BOOST_VECTOR_SPACE_DATA_LAZY_DIFFERENCE_H_ER_2009
#define BOOST_VECTOR_SPACE_DATA_LAZY_DIFFERENCE_H_ER_2009
#include <boost/mpl/not.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/range.hpp>
#include <boost/binary_op/functional/untupler.hpp>
#include <boost/vector_space/data/detail/cref.hpp>

namespace boost{
namespace vector_space{

// A data structure that offers a lazy (i.e. no copying involved) representation 
// of an element by element difference between two ranges.
//
// Usage:
// typedef lazy_difference<X,X1> diff_;
// typedef range_value<diff_> val_;
// diff_ diff(x,x1);
// val d0 = const_begin(diff)
template<typename X,typename X1>
class lazy_difference{
    typedef typename range_value<X>::type value_type;
    typedef std::minus<value_type> minus_;
    typedef binary_op::untupler<minus_> fun_;

    typedef detail::template cref<X> cref_x_;
    typedef detail::template cref<X1> cref_x1_;       
    typedef typename cref_x_::const_iterator it_x_;
    typedef typename cref_x1_::const_iterator it_x1_;
    typedef boost::tuple<it_x_,it_x1_> it_tuple_;
    typedef boost::zip_iterator<it_tuple_> zip_it_;
    
    public:
    lazy_difference(const X& x,const X1& x1)
    :cref_x(x),cref_x1(x1){}
    
    lazy_difference(const lazy_difference& that)
    :cref_x(that.cref_x),cref_x1(that.cref_x1){}

    typedef boost::transform_iterator<fun_,zip_it_> const_iterator;
    typedef const_iterator iterator;

    // TODO size etc.
    const_iterator begin()const{
        return this->get_iterator(
            cref_x.begin(),
            cref_x1.begin()
        );
    }
    const_iterator end()const{
        return this->get_iterator(
            cref_x.end(),
            cref_x1.end()
        );
    }
    
    //private:
    cref_x_ cref_x;
    cref_x1_ cref_x1;

    lazy_difference& operator=(const lazy_difference&);
    const_iterator get_iterator(it_x_ it_x,it_x1_ it_x1)const{
        fun_ f;
        return  make_transform_iterator(
            make_zip_iterator(
                make_tuple(
                    it_x,
                    it_x1
                )
            ),
            f
        );
    }
    
};

}// vector_space
}// boost

#endif