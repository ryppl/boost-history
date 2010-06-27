//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_ARRAY_EXTENSION_HPP
#define BOOST_INTRO_ARRAY_EXTENSION_HPP

#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_pod.hpp>
#include <iterator>
#include <algorithm>
#include <cstring>

namespace boost{
namespace intro{

template<class T>
struct array_extension : mpl::false_{};

template<class T>
struct continuous_values : mpl::false_{};

template<class T>
struct continuous_values<T *> : mpl::true_{};


template<typename Vector>
class vector_back_insert_iterator
    : public std::iterator<std::output_iterator_tag,void,void,void,void>{
public:
    typedef Vector container_type;
    explicit vector_back_insert_iterator(Vector &vec) : vec(vec){}
    vector_back_insert_iterator &operator=(typename Vector::const_reference v){
        this->vec.push_back(v);
        return *this;
    }
    template<class InputIterator,class Size>
    vector_back_insert_iterator &assign(InputIterator it,Size size){
        this->assign(it,size,
            typename array_extension<Vector>::type(),
            mpl::bool_<is_pod<typename Vector::value_type>::type::value
                && continuous_values<InputIterator>::type::value>()
        );
        return *this;
    }
    vector_back_insert_iterator &operator*(){ return *this; }
    vector_back_insert_iterator &operator++(){ return *this; }
    vector_back_insert_iterator operator++(int){ return *this; }
    vector_back_insert_iterator operator+=(std::size_t){ return *this; }
private:
    template<class Iterator,class Size,bool Pod>
    void assign(Iterator it,Size size,mpl::true_ array,mpl::bool_<Pod>){
        this->vec.push_back(it,size);
    }
    template<class Iterator,class Size>
    void assign(Iterator it,Size size,mpl::false_ array,mpl::true_ pod){
        std::size_t const oldsize=this->vec.size();
        this->vec.resize(oldsize + size);
        std::memcpy(&this->vec[oldsize],&*it,size * sizeof(typename Vector::value_type));
    }
    template<class Iterator,class Size>
    void assign(Iterator it,Size size,mpl::false_ array,mpl::false_ pod){
        std::size_t const oldsize=this->vec.size();
        this->vec.resize(oldsize + size);
        for(std::size_t c=0;c<size;++c)
            vec[oldsize+c]=*it++;
    }

    Vector &vec;
};


template<class Vector>
struct array_extension<vector_back_insert_iterator<Vector> > : mpl::true_{};



}
}



#endif
