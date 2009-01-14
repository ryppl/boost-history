#ifndef BOOST_INTERTHREADS_DETAIL_ASYNC_OSTREAM_SINK_HPP
#define BOOST_INTERTHREADS_DETAIL_ASYNC_OSTREAM_SINK_HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
//  copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/iostreams/categories.hpp>   // iostreams::sink_tag

#include <iosfwd>                           // std::streamsize
#include <iostream>                         // std::ostream
#include <boost/shared_ptr.hpp>             // shared_ptr

namespace boost {
namespace interthreads {
    class async_ostream;
    namespace detail {
        
        struct async_ostream_sink {
            typedef char      char_type;
            typedef  boost::iostreams::sink_tag  category;
            
            async_ostream_sink(std::ostream& os);
            
            std::streamsize write(const char* s, std::streamsize n);
            void flush();
            
        private:
            friend class async_ostream_concentrator;
            friend class async_ostream;
            struct impl;
            boost::shared_ptr<impl> impl_;                
        };
    }
}
}

#endif

