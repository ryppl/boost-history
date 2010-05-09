//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_TRANSACT_TYPE_SELECTION_HPP
#define BOOST_TRANSACT_TYPE_SELECTION_HPP

#include <boost/assert.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>

namespace boost{
namespace transact{

template<class Sequence>
class type_selection{
public:
    explicit type_selection(unsigned int wh) : which_(wh){
        BOOST_ASSERT(wh < (unsigned int)(mpl::size<Sequence>::value));
    }
    void reset(unsigned int wh){
        BOOST_ASSERT(wh < (unsigned int)(mpl::size<Sequence>::value));
        this->which_=wh;
    }
    template<class Visitor>
    typename Visitor::result_type apply_visitor(Visitor const &v) const{
        return this->apply_visitor<0>(v);
        //detail::type_selection_visitor<Sequence,0,mpl::size<Sequence>::value> visit;
        //visit(v,this->which_);
    }
    unsigned int which() const{ return this->which_; }
private:
    template<std::size_t N,class Visitor>
    typename Visitor::result_type apply_visitor(Visitor const &v) const{
        return this->apply_visitor<N>(v,mpl::bool_<N == std::size_t(mpl::size<Sequence>::value)>());
    }
    template<std::size_t N,class Visitor>
    typename Visitor::result_type apply_visitor(Visitor const &v,mpl::false_) const{
        if(this->which_ == N){
            return v.template operator()<typename mpl::at_c<Sequence,N>::type>();
        }else return this->apply_visitor<N+1>(v);
    }
    template<std::size_t N,class Visitor>
    typename Visitor::result_type apply_visitor(Visitor const &,mpl::true_) const{
        BOOST_ASSERT(false);
    }

    unsigned int which_;
};



}
}


#endif