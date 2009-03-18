#ifndef BOOST_UTILITY_CONTAINER_TO_STRING_HPP_ER_2009
#define BOOST_UTILITY_CONTAINER_TO_STRING_HPP_ER_2009
#include <string>
#include <boost/range.hpp>
#include <boost/format.hpp>
namespace boost{namespace utility{

    struct container_to_string{
        typedef std::string        result_type;

        // R ForwardRange
        // func(*i)
        template<typename R,typename Functor>
        result_type
        static get(const R& r,const Functor& func){
            format f0("%1% ");
            typedef typename range_iterator<const R>::type citer_t;
            result_type str;
            for(citer_t i = begin(r); i<end(r); i++){
                format f = f0;
                f%func(*i);
                str+= f.str();
            }
            return str;
        }
        template<typename R,typename Functor>
        result_type
        static get_indexed(const R& r,const Functor& func){
            format f0("[%1%]=%2% ");
            typedef typename range_iterator<const R>::type citer_t;
            typedef typename range_difference<const R>::type diff_t;
            result_type str;
            for(citer_t i = begin(r); i<end(r); i++){
                format f = f0;
                diff_t diff = std::distance(begin(r),i);
                f%diff;
                f%func(*i);
                str+= f.str();
            }
            return str;
        }

        template<typename R>
        result_type
        static get_indexed(const R& r){
            identity_func func;
            return get_indexed(r,func);
        }
        template<typename R>
        result_type
        static get(const R& r){
            identity_func func;
            return get(r,func);
        }

        private:

        struct identity_func{
            identity_func(){}
            template<typename T>
            T operator()(T x)const{
                return x;
            }
        };
    };

}//utility
}//boost

#endif // CONTAINER_TO_STRING_HPP_INCLUDED
