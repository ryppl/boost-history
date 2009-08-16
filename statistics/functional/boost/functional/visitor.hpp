///////////////////////////////////////////////////////////////////////////////
// boost::functional::visitor.hpp                                            //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_FUNCTIONAL_VISITOR_HPP_ER_2009
#define BOOST_FUNCTIONAL_VISITOR_HPP_ER_2009
#include <iterator>
#include <boost/range.hpp>
#include <boost/utility.hpp>

namespace boost{
namespace functional{

    // Models Generator
    //
    // Successively returns elements of a range
    template<typename R>
    class visitor{
        typedef typename range_iterator<R>::type it_t;
        typedef std::size_t size_type;

        public:
        visitor();
        visitor(const R& r);
        visitor(const visitor&);
        visitor& operator=(const visitor&);

        typedef typename range_value<R>::type result_type;

        result_type operator()();

        void reset(size_type n);
        
        private:
        R r_;
        it_t it_;
    };

    // Implementation //

    template<typename R>
    visitor<R>::visitor(){}

    template<typename R>
    void visitor<R>::reset(size_type n){
        this->it_ = boost::next(
            boost::begin(this->r_),
            n
        );
    }
    
    template<typename R>
    visitor<R>::visitor(const R& r)
    :r_(r){ this-> reset(0); }

    template<typename R>
    visitor<R>::visitor(const visitor& that)
    :r_(that.r_){
        this->reset(
            std::distance(
                boost::begin(that.r_),
                that.it_
            )
        );
    }
    
    template<typename R>
    visitor<R>& 
    visitor<R>::operator=(const visitor& that){
        if(&that!=this){
            this->r_ = that.r_;
            this->reset(
                std::distance(
                    boost::begin(that.r_),
                    that.it_
                )
            );
        }
        return *this;
    }

    template<typename R>
    typename visitor<R>::result_type 
    visitor<R>::operator()(){
        typedef typename visitor<R>::result_type x_;
        return *(this->it_++);
    }

}// functional
}// boost

#endif