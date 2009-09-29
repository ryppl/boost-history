//////////////////////////////////////////////////////////////////////////////
// statistics::goodness_of_fit::data::k_fold_data.hpp                       //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef  BOOST_STATISTICS_GOODNESS_OF_FIT_DATA_K_FOLD_DATA_HPP_ER_2009
#define  BOOST_STATISTICS_GOODNESS_OF_FIT_DATA_K_FOLD_DATA_HPP_ER_2009
#include <stdexcept>
#include <iterator>
#include <vector>
#include <ostream>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/range.hpp>
#include <boost/format.hpp>
#include <boost/utility.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/iterator/iterator_traits.hpp>

//#include <boost/binary_op/meta/include.hpp>
#include <boost/statistics/detail.hpp>

namespace boost{
namespace statistics{
namespace detail{// Added
namespace goodness_of_fit{


        // k = 4
        // train     test
        // [0][1][2] (3)    j = 3
        // -------------
        // [1][2][3] (0)    j = 0
        // [2][3][0] (1)    j = 1
        // [3][0][1] (2)    j = 2
        // [0][1][2] (3)    j = 3

    // Examples:
    // If regression, T == tuple 
    // If marginal,   T == Y
    template<typename T>
    class k_fold_data{
        BOOST_MPL_ASSERT((
            mpl::eval_if<
                binary_op::is_tuple<T>,
                binary_op::tuple_has_no_ref<T>,
                mpl::bool_<true>
            >
        )); 
        BOOST_MPL_ASSERT((
            mpl::eval_if<
                binary_op::is_tuple<T>,
                binary_op::tuple_has_no_const<T>,
                mpl::bool_<true>
            >
        )); 
    
        typedef std::string str_;
        public:
        typedef boost::circular_buffer<T>            train_data_type;
        typedef std::vector<T>                       test_data_type;
        typedef long int                             int_;
        typedef boost::sub_range<const train_data_type>    
                                                    range_train_data_type;

        k_fold_data();
        template<typename It>
        k_fold_data(int_ k, It b,It e); 
        k_fold_data(const k_fold_data&); 
        k_fold_data& operator=(const k_fold_data&);

        template<typename It> 
        void initialize(int_ k, It b,It e); // j = 0
        void initialize(); //restores state to j = 0
        void increment(); // ++j

        // Access
        const train_data_type& train_data()const;
        const test_data_type& test_data()const;

        const int_& n()const;      // size of test data
        const int_& j()const;      // index of current iteration
        const int_& k()const;      // number of iterations

        //private:
        int_ k_;
        int_ j_;
        int_ n_;
        train_data_type train_data_;
        test_data_type test_data_;
    };

    template<typename T>
    std::ostream& operator<<(std::ostream& out,const k_fold_data<T>& that){
        format f("k_fold_data(%1%,%2%,%3%)");
        f % that.n() % that.j() % that.k();
        return (out << f.str());
    }


    // Implementation //

    template<typename T>
    k_fold_data<T>::k_fold_data()
    :k_(0),j_(0),n_(0),train_data_(),test_data_(){}
    
    template<typename T>
        template<typename It>
    k_fold_data<T>::k_fold_data(int_ k, It b,It e)
    {
        this->initialize(k,b,e);
    } 
    
    template<typename T>
    k_fold_data<T>::k_fold_data(const k_fold_data& that)
    :k_(that.k_),j_(that.j_),n_(that.n_),
    train_data_(that.train_data_),
    test_data_(that.test_data_){}
    
    template<typename T>
    k_fold_data<T>& k_fold_data<T>::operator=(const k_fold_data& that)
    {
        this->k_ = that.k_;
        this->j_ = that.j_;
        this->n_ = that.n_;
        this->train_data_ = that.train_data_;
        this->test_data_  = that.test_data_;
    }

    template<typename T>
        template<typename It>
    void k_fold_data<T>::initialize(int_ k,It b,It e)
    {
        BOOST_ASSERT(k>1);
        typedef typename iterator_difference<It>::type  diff_;
        this->k_ = k;
        this->j_ = -1;
        diff_ d = std::distance( b, e);
        if(d % this->k() != 0){
            static const str_ msg 
                = str_("k_fold_estimate : distance(b,e)") 
                    + "%1% not a multiple of k = %2%";
            throw std::runtime_error( ( format(msg) % d % k ).str() );    
        }
        this->n_ = d / this->k(); 

        It i = boost::next( b, this->n() * (k-1) ); 
        this->test_data_.clear();
        this->test_data_.reserve(this->n());
        std::copy(
            i,
            e,
            std::back_inserter(this->test_data_)
        );
        this->train_data_.assign(
            this->n() * (k-1),
            b, 
            i
        );
        this->increment();
    }

    template<typename T>
    void k_fold_data<T>::initialize()
    {
        while(this->j()<this->k()){
            this->increment();
        }
        this->j_ = 0;
    }

    template<typename T>
    void k_fold_data<T>::increment(){
        static test_data_type tmp;
        if( !( this->j()<this->k() ) )
        {
            static const char* msg = "k_fold_data: !j<k=%1%";
            throw std::runtime_error(
                ( format( msg ) % this->k() ).str()
            );
        }
        tmp.clear();
        std::copy(
            boost::begin(this->train_data()),
            next( boost::begin(this->train_data()), this->n() ),
            std::back_inserter(tmp)
        );
        this->train_data_.insert(
            boost::end( this->train_data_ ),
            boost::begin( this->test_data() ),
            boost::end( this->test_data() )
        );
        this->test_data_ = tmp;
        ++this->j_;
    }

    // Access
    template<typename T>
    const typename k_fold_data<T>::train_data_type& 
    k_fold_data<T>::train_data()const
    {
        return this->train_data_;
    }

    template<typename T>
    const typename k_fold_data<T>::test_data_type& 
    k_fold_data<T>::test_data()const
    {
        return this->test_data_;
    }

    template<typename T>
    const typename k_fold_data<T>::int_& 
    k_fold_data<T>::n()const
    {
        return this->n_;
    }

    template<typename T>
    const typename k_fold_data<T>::int_& 
    k_fold_data<T>::j()const
    {
        return this->j_;
    }
    
    template<typename T>
    const typename k_fold_data<T>::int_& 
    k_fold_data<T>::k()const
    {
        return this->k_;
    }
    
}// goodness_of_fit
}// detail
}// statistics
}// boost

#endif