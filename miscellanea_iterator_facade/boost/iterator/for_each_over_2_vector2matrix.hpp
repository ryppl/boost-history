//////////////////////////////////////////////////////////////////////////////
// for_each_over_2_vector2matrix.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ITERATOR_FOR_EACH_ZIP_VECTOR2MATRIX_HPP_ER_2009
#define BOOST_ITERATOR_FOR_EACH_ZIP_VECTOR2MATRIX_HPP_ER_2009
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <boost/type_traits/add_const.hpp>
#include <boost/range.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/iterator/vector2matrix_iterator.hpp>
#include <boost/iterator/zip_iterator.hpp>
namespace boost{

/// Takes two sequences, maps each to matrix, and makes each pair
/// of rows into a tuple, to which is applied a function, T(...).
/// F0, F1: either mpl::identity or add_const
template<
    typename T,
    template<typename> class F0 = mpl::identity,
    template<typename> class F1 = mpl::identity
>
class for_each_over_2_vector2matrix{
    /// Types associated with T:
    typedef typename T::type         obj_type;
    typedef typename T::stride0_type stride0_type; //> instance of int_
    typedef typename T::stride1_type stride1_type; //> instance of int_

    /// Valid expressions:
    ///     x and y two forward ranges,
    ///     T::type t; t(x,y)

    template<typename R,template<typename> class F>
    struct add_ref{
        typedef typename F<R>::type inner_type;
        typedef typename add_reference<inner_type>::type type;
    };

    public:
    static const std::size_t stride0 = stride0_type::value;
    static const std::size_t stride1 = stride1_type::value;

    for_each_over_2_vector2matrix(obj_type& obj):obj_(obj){}
    for_each_over_2_vector2matrix(const for_each_over_2_vector2matrix& that)
    :obj_(that.obj_){}

    template<typename R0, typename R1>
    void operator()(const R0& r0,const R1& r1){ return call_impl(r0,r1); }

    template<typename R0, typename R1>
    void operator()(R0& r0,const R1& r1){
        typedef typename add_ref<R0,F0>::type r0_type;
        return call_impl(static_cast<r0_type>(r0),r1); }

    template<typename R0, typename R1>
    void operator()(const R0& r0,R1& r1){
        typedef typename add_ref<R1,F1>::type r1_type;
        return call_impl(r0,static_cast<r1_type>(r1)); }

    template<typename R0, typename R1>
    void operator()(R0& r0,R1& r1){
        typedef typename add_ref<R0,F0>::type r0_type;
        typedef typename add_ref<R1,F1>::type r1_type;
        return call_impl(static_cast<r0_type>(r0),static_cast<r1_type>(r1)); }

    private:
    for_each_over_2_vector2matrix& operator=(
        const for_each_over_2_vector2matrix& that);
    template<typename R0, typename R1>
    void call_impl(R0& r0, R1& r1){
        typedef zip_fun<R0,R1> zip_fun_type;
        std::for_each(
            make_zip_iterator(
                make_tuple(
                    make_vector2matrix_iterator(begin(r0),stride0),
                    make_vector2matrix_iterator(begin(r1),stride1)
                )
            ),
            make_zip_iterator(
                make_tuple(
                    make_end_vector2matrix_iterator(
                        begin(r0),end(r0),stride0),
                    make_end_vector2matrix_iterator(
                        begin(r1),end(r1),stride1)
                )
            ),
            zip_fun_type(obj_)
        );

    }

        for_each_over_2_vector2matrix();//dont implem

        template<typename R0, typename R1>
        class zip_fun{
            public:

            typedef void                                result_type;

            zip_fun(obj_type& obj):obj_(obj){}
            zip_fun(const zip_fun& that ):obj_(that.obj_){}


// TODO surprisingly ADL suffices i.e. argument_type need not be specified
// Extend to other applications of zip_iterator in other classes?
            template<typename Tuple>
            void operator()(const Tuple& t){
                obj_(t.template get<0>(),t.template get<1>());
            }
            private:
            zip_fun& operator=(const zip_fun& that);
            zip_fun(); //dont implem
            obj_type& obj_;
        };

    obj_type& obj_;
};


}
#endif
