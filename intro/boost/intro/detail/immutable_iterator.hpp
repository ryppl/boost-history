//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_DETAIL_IMMUTABLE_ITERATOR_HPP
#define BOOST_INTRO_DETAIL_IMMUTABLE_ITERATOR_HPP

#include <iterator>

namespace boost{
namespace intro{
namespace detail{

template<class Iterator>
class immutable_iterator{
public:
    typedef typename std::iterator_traits<Iterator>::value_type value_type;
    typedef typename std::iterator_traits<Iterator>::reference reference;
    typedef typename std::iterator_traits<Iterator>::pointer pointer;
    immutable_iterator(Iterator const &it) : it(it){}
    template<class Other>
    immutable_iterator(immutable_iterator<Other> const &rhs) : it(rhs.it){}
    reference operator*() const{ return *it; }
    Iterator const &operator->() const{ return it; }
    bool operator==(immutable_iterator const &rhs) const{
        return it == rhs.it;
    }
    bool operator!=(immutable_iterator const &rhs) const{
        return it != rhs.it;
    }
private:
    template<class> friend class immutable_iterator;
    Iterator it;
};


}
}
}


#endif
