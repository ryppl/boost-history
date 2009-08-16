//////////////////////////////////////////////////////////////////////////////
// scalar_dist::map_pdf::inverse_pdf.hpp                                    //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SCALAR_DIST_MAP_PDF_INVERSE_PDF_HPP_ER_2009
#define BOOST_SCALAR_DIST_MAP_PDF_INVERSE_PDF_HPP_ER_2009
#include <boost/type_traits/is_reference.hpp>
#include <boost/mpl/not.hpp>
#include <boost/utility.hpp>
#include <boost/call_traits.hpp>
#include <boost/scalar_dist/meta/has_pdf.hpp>

namespace boost{
namespace math{

    // Represents the inverse pdf
    //
    // D = T or D = T& or D = T*
    template<typename D>
    class inverse_pdf{
        typedef typename remove_reference<
            typename remove_const<
                D
            >::type
        >::type plain_t;
    public:
        typedef typename plain_t::value_type value_type;

        typedef
            typename call_traits<D>::const_reference
                result_of_distribution_type;

        explicit inverse_pdf(typename call_traits<D>::param_type d):d_(d){}
        inverse_pdf(const inverse_pdf& that):d_(that.d_){}
        inverse_pdf&
        operator=(const inverse_pdf& that){
            if(&that!=this){
                typedef is_reference<D> is_ref_;
                BOOST_MPL_ASSERT((
                    mpl::not<is_ref_>
                ));
                d_ = that.d_;
            }
            return *this;
        }

        result_of_distribution_type
        distribution()const{ return d_; }

    private:
        inverse_pdf();
        typename call_traits<D>::value_type d_;
    };

    template<typename D>
    struct has_pdf< inverse_pdf<D> > : mpl::bool_<true> {};


    template<typename D>
    inverse_pdf<D>
    make_inverse_pdf(const D& d){ return inverse_pdf<D>(d); }

    template<typename D>
    typename inverse_pdf<D>::value_type
    log_unnormalized_pdf(
        const inverse_pdf<D>& dist,
        const typename inverse_pdf<D>::value_type& x
    ){
        return
            ( -log_unnormalized_pdf(dist.distribution(),x) );
    }

}// math
}// boost

#endif 