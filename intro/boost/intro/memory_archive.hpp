//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_MEMORY_ARCHIVE_HPP
#define BOOST_INTRO_MEMORY_ARCHIVE_HPP

#include <boost/intro/detail/auto_buffer.hpp>
#include <boost/intro/char_archive.hpp>
#include <boost/intro/array_extension.hpp>
#include <utility>

namespace boost{
namespace intro{

namespace detail{

class memory_oarchive_base{
protected:
    typedef detail::auto_buffer<char,256> buffer_type;
    buffer_type buffer;
};

}

class memory_oarchive
    : public detail::memory_oarchive_base
    , public char_oarchive<vector_back_insert_iterator<detail::memory_oarchive_base::buffer_type> >{
    typedef vector_back_insert_iterator<buffer_type> iterator_type;
    typedef char_oarchive<iterator_type> archive_type;
public:
    memory_oarchive()
        : archive_type(iterator_type(buffer)){}
    void reserve(std::size_t n){
        buffer.reserve(n);
    }
    std::pair<char const *,char const *> data() const{
        return std::make_pair(buffer.begin(),buffer.end());
    }
};


}
}

#endif
